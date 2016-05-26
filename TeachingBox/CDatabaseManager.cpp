#include "stdafx.h"
#include "CDatabaseManager.h"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QFile"

CDatabaseManager::CDatabaseManager()
{
	/*是否存在数据库*/
	bool isExists = QFile::exists(DATABASE_NAME);

	if (m_db.contains(CONNECTION_NAME))
	{
		m_db = QSqlDatabase::database(CONNECTION_NAME);
	}
	else
	{
		m_db = QSqlDatabase::addDatabase(DATABASE_TYPE, CONNECTION_NAME);
		m_db.setHostName(HOST_NAME);
		m_db.setDatabaseName(DATABASE_NAME);
		m_db.setUserName(USER_NAME);
		m_db.setPassword(PASSWORD);
	}
	if (!m_db.isOpen())
	{
		m_db.open();
	}

	/*若之前不存在数据库，则创建表*/
	if (!isExists)
	{
		CreateTable();
	}
}

CDatabaseManager::~CDatabaseManager()
{
	if (m_db.isOpen())
	{
		m_db.close();
	}
	m_db.removeDatabase(CONNECTION_NAME);
}

void CDatabaseManager::CreateVariateTable()
{
	QSqlQuery query(m_db);
	QString oper = "CREATE TABLE " + VARIATE_TABLE_NAME + " ("
		+ VARIATE_COLUMN_SCOPE + " varchar NOT NULL,"	/*作用域*/
		+ VARIATE_COLUMN_NAME + " varchar NOT NULL,"	/*变量名*/
		+ VARIATE_COLUMN_VALUE + " varchar NOT NULL)"	/*值*/;
	query.prepare(oper);

	if (!query.exec())
	{
		throw std::exception(("create variate table failed:\n" + oper.toStdString()).c_str());
	}
}

void CDatabaseManager::InsertVariate(QDataStream& dataStream)
{
	dataStream.device()->seek(0);
	QSqlQuery query(m_db);

	query.prepare("insert into " + VARIATE_TABLE_NAME + " values(?,?,?)");

	QString scope;
	QString name;
	QByteArray value;

	dataStream >> scope;
	dataStream >> name;
	dataStream.device()->seek(0);
	value=dataStream.device()->readAll();

	query.bindValue(0, scope);
	query.bindValue(1, name);
	query.bindValue(2, value);

	if (!query.exec())
	{
		throw std::exception("insert into database failed");
	}
}

void CDatabaseManager::SelectAllVariate(QVector<QByteArray>& variates)
{
	QSqlQuery query(m_db);

	query.prepare("select * from " + VARIATE_TABLE_NAME);

	if (!query.exec())
	{
		throw std::exception("select variates from database failed");
	}

	while (query.next())
	{
		variates.append(query.value(2).toByteArray());
	}
}

void CDatabaseManager::SelectVariatesFromScope(QVector<QByteArray>& variates, const QString& scope)
{
	QSqlQuery query(m_db);

	query.prepare(QString("select * from %1 where %2 = '%3'")
		.arg(VARIATE_TABLE_NAME)
		.arg(VARIATE_COLUMN_SCOPE)
		.arg(scope));

	if (!query.exec())
	{
		throw std::exception("select variates from database failed");
	}

	while (query.next())
	{
		variates.append(query.value(2).toByteArray());
	}

}

void CDatabaseManager::UpdateVariate(const QString& scope, const QString& name, QDataStream& dataStream)
{
	QSqlQuery query(m_db);
	query.prepare("update " + VARIATE_TABLE_NAME + " set "
		+VARIATE_COLUMN_SCOPE+"=?,"
		+VARIATE_COLUMN_NAME+"=?,"
		+VARIATE_COLUMN_VALUE+"=?"
		+" where "
		+VARIATE_COLUMN_SCOPE+"=?"
		+" and "
		+VARIATE_COLUMN_NAME+"=?");

	dataStream.device()->seek(0);
	QString newScope;
	QString newName;
	QByteArray newValue;

	dataStream >> newScope;
	dataStream >> newName;
	dataStream.device()->seek(0);
	newValue = dataStream.device()->readAll();

	query.bindValue(0, newScope);
	query.bindValue(1, newName);
	query.bindValue(2, newValue);
	query.bindValue(3, scope);
	query.bindValue(4, name);

	if (!query.exec())
	{
		throw std::exception("update variate in database failed");
	}
}

void CDatabaseManager::DeleteVariate(const QString& scope, const QString& name)
{
	QSqlQuery query(m_db);
	QString oper = QString("delete from %1 where %2='%3' and %4='%5'")
		.arg(VARIATE_TABLE_NAME)
		.arg(VARIATE_COLUMN_SCOPE).arg(scope)
		.arg(VARIATE_COLUMN_NAME).arg(name);
	query.prepare(oper);

	if (!query.exec())
	{
		throw std::exception(("delete variate from database failed:\n"+oper.toStdString()).c_str());
	}
}

void CDatabaseManager::DeleteAllVariate()
{
	QSqlQuery query(m_db);
	query.prepare(QString("delete from %1").arg(VARIATE_TABLE_NAME));

	if (!query.exec())
	{
		throw std::exception("delete all variates from database failed");
	}
}

CDatabaseManager* CDatabaseManager::GetInstance()
{
	return CSingleTon<CDatabaseManager>::GetInstance();
}


void CDatabaseManager::DeleteScopeData(const QString& strScope)
{
	QStringList strType;
	strType << TABLE_INT << TABLE_DOUBLE << TABLE_BOOL << TABLE_STRING << TABLE_POSITION << TABLE_DYNAMIC << TABLE_OVERLAP;

	for each (auto var in strType)
	{
		DeleteValue(var, strScope);
	}
}

void CDatabaseManager::CreateTable()
{
	m_db.transaction();
	CreateVariateTable();
	CreateUserTable();	/*创建用户表*/
	//CreateSymbolTable();	/*创建符号表*/
	//CreatePositionTable();	/*创建位置表*/
	//CreateDynamicTable();	/*创建速度表*/
	CreateOverlapTable();	/*创建过渡表*/
	//CreateDoubleTable();	/*创建浮点数表*/
	////CreateLabelTable();	/*创建label表*/
	//CreateIntTable();	/*创建整数表*/
	//CreateBoolTable();	/*创建布尔数表*/
	//CreateStringTable();	/*创建字符串表*/
	m_db.commit();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*用户表相关*/
//////////////////////////////////////////////////////////////////////////
void CDatabaseManager::CreateUserTable()
{
	QSqlQuery query(m_db);
	QString strUserNameMaxLength = QString::number(TABLE_USER_NAME_MAX_LENGTH);
	QString strUserPasswordMaxLength = QString::number(TABLE_USER_PASSWORD_MAX_LENGTH);

	query.prepare("CREATE TABLE " + TABLE_USER + " ("
		+ TABLE_COLUMN_NAME + " varchar(" + strUserNameMaxLength + ") NOT NULL,"	/*用户名*/
		+ TABLE_COLUMN_USER_PASSWORD + " varchar(" + strUserPasswordMaxLength + ") NOT NULL,"	/*用户密码*/
		+ TABLE_COLUMN_USER_AUTHORITY + " int NOT NULL,"	/*用户权限*/
		+ TABLE_COLUMN_USER_LANGUAGE + " varchar(" + strUserNameMaxLength + ") NOT NULL,"	/*用户语言*/
		+ TABLE_COLUMN_USER_IDENTITY + " varchar(" + strUserNameMaxLength + ") NOT NULL)"	/*用户身份*/
		);

	if (query.exec())
	{
		qDebug() << "create user table successed";
	}
	else
	{
		qDebug() << "create user table failed";
	}
}

void CDatabaseManager::InsertUserInfo(const CUser& user)
{
	QSqlQuery query(m_db);
	query.prepare("insert into " + TABLE_USER + " values(?,?,?,?,?)");

	query.bindValue(0, user.GetName());
	query.bindValue(1, user.GetPassword());
	query.bindValue(2, user.GetAuthority());
	query.bindValue(3, user.GetLanguage());
	query.bindValue(4, user.GetIdentity());

	if (query.exec())
	{
		qDebug() << "insert user table successed";
	}
	else
	{
		qDebug() << "insert user table failed";
	}
}

//void CDatabaseManager::SelectUserInfo(const QString& strUserName, CUser& user)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("select * from %1 where %2='%3'").arg(TABLE_USER).arg(TABLE_COLUMN_USER_NAME).arg(strUserName));
//
//	if (query.exec())
//	{
//		while (query.next())
//		{
//			user.SetName(query.value(0).toString());
//			user.SetPassword(query.value(1).toString());
//			user.SetAuthority(query.value(2).toInt());
//		}
//	}
//	else
//	{
//	}
//	
//}

void CDatabaseManager::SelectUserInfo(CUser::TYPE_MAP_USER& mapUser)
{
	QSqlQuery query(m_db);
	query.prepare(QString("select * from %1").arg(TABLE_USER));

	if (query.exec())
	{
		while (query.next())
		{
			CUser user(query.value(0).toString(), query.value(1).toString(), query.value(2).toInt(),query.value(3).toString(),query.value(4).toString());
			mapUser[query.value(0).toString()] = user;
		}
	}
	
}

/*更新用户信息，包括用户名*/
void CDatabaseManager::UpdateUserInfo(const QString& strUserOldName, const CUser& user)
{
	QSqlQuery query(m_db);
	query.prepare("update " + TABLE_USER + " set " + TABLE_COLUMN_NAME + "=?,"
		+ TABLE_COLUMN_USER_PASSWORD + "=?,"
		+ TABLE_COLUMN_USER_AUTHORITY + "=?,"
		+ TABLE_COLUMN_USER_LANGUAGE + "=?,"
		+ TABLE_COLUMN_USER_IDENTITY + "=?"
		+ " where " + TABLE_COLUMN_NAME + "=?");

	query.bindValue(0, user.GetName());
	query.bindValue(1, user.GetPassword());
	query.bindValue(2, user.GetAuthority());
	query.bindValue(3, user.GetLanguage());
	query.bindValue(4, user.GetIdentity());
	query.bindValue(5, strUserOldName);

	if (query.exec())
	{
		qDebug() << "update user table succeed";
	}
	else
	{
		qDebug() << "update user table failed";
	}
}

void CDatabaseManager::DeleteUserInfo(const QString& strUserName)
{
	QSqlQuery query(m_db);
	query.prepare(QString("delete from %1 where %2='%3'").arg(TABLE_USER).arg(TABLE_COLUMN_NAME).arg(strUserName));

	query.exec();
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*符号表*/
//////////////////////////////////////////////////////////////////////////
//void CDatabaseManager::CreateSymbolTable()
//{
//	QSqlQuery query(m_db);
//	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
//	query.prepare("CREATE TABLE " + TABLE_SYMBOL + " ("
//		+ TABLE_COLUMN_SCOPE + " int NOT NULL,"
//		+ TABLE_COLUMN_PROJECT + " varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_PROGRAM + " varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_SYMBOL_NAME + "  varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_SYMBOL_TYPE + " int NOT NULL)"
//		);
//	if (query.exec())
//	{
//		qDebug() << "create symbol succeed";
//	}
//	else
//	{
//		qDebug() << "create symbol failed";
//	}
//}
//
//void CDatabaseManager::InsertSymbolValue(const int scope, const QString& strProject, const QString& strProgram, const QString& strSymbolName,
//	const int symbolType)
//{
//	QSqlQuery query(m_db);
//	query.prepare("insert into " + TABLE_SYMBOL + " values(?,?,?,?,?)");
//
//	query.bindValue(0, scope);
//	query.bindValue(1, strProject);
//	query.bindValue(2, strProgram);
//	query.bindValue(3, strSymbolName);
//	query.bindValue(4, symbolType);
//
//	if (query.exec())
//	{
//		qDebug() << "insert symbol succeed";
//	}
//	else
//	{
//		qDebug() << "insert symbol failed";
//	}
//}
//
//void CDatabaseManager::InsertSymbolValue(const int scope, const QString& strProject, const QString& strProgram,
//	const std::pair<std::string, int>& symbolValue)
//{
//	InsertSymbolValue(scope, strProject, strProgram, QString::fromStdString(symbolValue.first), symbolValue.second);
//}
//
//
//void CDatabaseManager::UpdateSymbolValue(const int scope, const QString& strProject, const QString& strProgram, const QString& strOldSymbol,
//	const QString& strNewSymbol, const int symbolType)
//{
//	QSqlQuery query(m_db);
//	query.prepare("update " + TABLE_SYMBOL + " set " 
//		+ TABLE_COLUMN_SYMBOL_NAME + "=?,"
//		+ TABLE_COLUMN_SYMBOL_TYPE + "=?"
//		+ " where " + TABLE_COLUMN_SYMBOL_NAME + "=?"
//		+ " and " + TABLE_COLUMN_PROGRAM + "=?"
//		+ " and " + TABLE_COLUMN_PROJECT + "=?"
//		+ " and " + TABLE_COLUMN_SCOPE + "=?");
//
//	query.bindValue(0, strNewSymbol);
//	query.bindValue(1, symbolType);
//	query.bindValue(2, strOldSymbol);
//	query.bindValue(3, strProgram);
//	query.bindValue(4, strProject);
//	query.bindValue(5, scope);
//
//	if (query.exec())
//	{
//		qDebug() << "update symbol succeed";
//	}
//	else
//	{
//		qDebug() << "update symbol failed";
//	}
//}
//
//
//void CDatabaseManager::UpdateSymbolValue(const int scope, const QString& strProject, const QString& strProgram,
//	const QString& strSymbolName, const int symbolType)
//{
//	UpdateSymbolValue(scope, strProject, strProgram, strSymbolName, strSymbolName, symbolType);
//}
//
//void CDatabaseManager::SelectSymbolValue(const QString& strProject, const QString& strProgram, CValue::TYPE_MAP_SYMBOL& mapSymbol)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("select * from %1 where (%2='%3' or %4='%5') and %6='%7'")
//		.arg(TABLE_SYMBOL).arg(TABLE_COLUMN_PROGRAM).arg(strProgram).arg(TABLE_COLUMN_PROGRAM).arg("")
//		.arg(TABLE_COLUMN_PROJECT).arg(strProject));
//
//	if (query.exec())
//	{
//		qDebug() << "select symbol succeed";
//	}
//	else
//	{
//		qDebug() << "select symbol failed";
//	}
//
//	CValue::TYPE_SYMBOL pairSymbol;
//	while (query.next())
//	{
//		pairSymbol.scope = query.value(0).toInt();
//		pairSymbol.type = query.value(4).toInt();
//
//		mapSymbol[query.value(3).toString().toStdString()] = pairSymbol;
//	}
//}
//
//void CDatabaseManager::SelectSymbolValue(const QString& strProject, const QString& strProgram, const QString& strSymbolName, int& symbolType, int& symbolScope)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("select * from %1 where %2='%3' and (%4='%5' or %6='%7') and %8='%9'")
//		.arg(TABLE_SYMBOL).arg(TABLE_COLUMN_SYMBOL_NAME).arg(strSymbolName)
//		.arg(TABLE_COLUMN_PROGRAM).arg(strProgram).arg(TABLE_COLUMN_PROGRAM).arg("")
//		.arg(TABLE_COLUMN_PROJECT).arg(strProject));
//
//	if (query.exec())
//	{
//		qDebug() << "select symbol succeed";
//	}
//	else
//	{
//		qDebug() << "select symbol failed";
//	}
//
//	while (query.next())
//	{
//		symbolScope = query.value(0).toInt();
//		symbolType = query.value(4).toInt();
//	}
//}
//
//void CDatabaseManager::DeleteSymbolValue(const QString& strProject, const QString& strProgram, const QString& strSymbolName)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("delete from %1 where %2='%3' and (%4='%5' or %6='%7') and %8='%9'")
//		.arg(TABLE_SYMBOL).arg(TABLE_COLUMN_SYMBOL_NAME).arg(strSymbolName)
//		.arg(TABLE_COLUMN_PROGRAM).arg(strProgram).arg(TABLE_COLUMN_PROGRAM).arg("")
//		.arg(TABLE_COLUMN_PROJECT).arg(strProject));
//
//	if (query.exec())
//	{
//		qDebug() << "delete symbol succeed";
//	}
//	else
//	{
//		qDebug() << "delete symbol failed";
//	}
//}
//
//void CDatabaseManager::DeleteSymbolValue(const QString& strProject, const QString& strProgram)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("delete from %1 where (%2='%3' or %4='%5') and %6='%7'")
//		.arg(TABLE_SYMBOL).arg(TABLE_COLUMN_PROGRAM).arg(strProgram).arg(TABLE_COLUMN_PROGRAM).arg("")
//		.arg(TABLE_COLUMN_PROJECT).arg(strProject));
//
//	if (query.exec())
//	{
//		qDebug() << "delete symbol succeed";
//	}
//	else
//	{
//		qDebug() << "delete symbol failed";
//	}
//}
//
//void CDatabaseManager::DeleteSymbolValue(const QString& strProject)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("delete from %1 where %2='%3'")
//		.arg(TABLE_SYMBOL).arg(TABLE_COLUMN_PROJECT).arg(strProject));
//
//	if (query.exec())
//	{
//		qDebug() << "delete symbol succeed";
//	}
//	else
//	{
//		qDebug() << "delete symbol failed";
//	}
//}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*单个变量*/
//////////////////////////////////////////////////////////////////////////
//void CDatabaseManager::DeleteValue(const QString& strTableName, const QString& strColumnVariable,
//	const QString& strScope, const QString& strProject, const QString& strProgram, const QString& strVariable)
//{
//	QSqlQuery query(m_db);
//	query.prepare(QString("delete from %1 where %2='%3' and %4='%5' and %6='%7' and %8='%9'").arg(strTableName)
//		.arg(strColumnVariable).arg(strVariable)
//		.arg(TABLE_COLUMN_PROGRAM).arg(strProgram)
//		.arg(TABLE_COLUMN_PROJECT).arg(strProject)
//		.arg(TABLE_COLUMN_SCOPE).arg(strScope));
//
//	if (query.exec())
//	{
//		qDebug() << "delete successed";
//	}
//	else
//	{
//		qDebug() << "delete failed";
//	}
//}

void CDatabaseManager::DeleteValue(const QString& strTableName, const QString& strColumnVariable, const QString& strScope, const QString& strVariable)
{
	QSqlQuery query(m_db);
	query.prepare(QString("delete from %1 where %2='%3' and %4='%5'")
		.arg(strTableName)
		.arg(strColumnVariable).arg(strVariable)
		.arg(TABLE_COLUMN_SCOPE).arg(strScope));

	if (query.exec())
	{
		qDebug() << "delete successed";
	}
	else
	{
		qDebug() << "delete failed";
	}
}

void CDatabaseManager::DeleteValue(const QString& strTableName, const QString& strScope)
{
	QSqlQuery query(m_db);
	query.prepare(QString("delete from %1 where %2='%3'")
		.arg(strTableName).arg(TABLE_COLUMN_SCOPE).arg(strScope));
	query.exec();
}


//////////////////////////////////////////////////////////////////////////
/*创建表*/
//////////////////////////////////////////////////////////////////////////
void CDatabaseManager::CreatePositionTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_POSITION + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + "  varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS1 + " double NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS2 + " double NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS3 + " double NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS4 + " double NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS5 + " double NOT NULL,"
		+ TABLE_COLUMN_POSITION_AXIS6 + " double NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create position table successed";
	}
	else
	{
		qDebug() << "create position table failed";
	}
}

void CDatabaseManager::CreateDynamicTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_DYNAMIC + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + "  varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_DYNAMIC_VELOCITY + " double NOT NULL,"
		+TABLE_COLUMN_DYNAMIC_ACCELERATION+" double NOT NULL,"
		+TABLE_COLUMN_DYNAMIC_DECELERATION+" double NOT NULL,"
		+TABLE_COLUMN_DYNAMIC_POSTURE_VELOCITY+" double NOT NULL,"
		+TABLE_COLUMN_DYNAMIC_POSTURE_ACCELERATION+" double NOT NULL,"
		+TABLE_COLUMN_DYNAMIC_POSTURE_DECELERATION+" double NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create velocity table successed";
	}
	else
	{
		qDebug() << "create velocity table failed";
	}
}

void CDatabaseManager::CreateOverlapTable()
{
	QSqlQuery query(m_db);

	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);

	query.prepare("CREATE TABLE " + TABLE_OVERLAP + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + "  varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_OVERLAP_MODE + "  int NOT NULL,"
		+ TABLE_COLUMN_OVERLAP_VALUE + " double NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create zone table successed";
	}
	else
	{
		qDebug() << "create zone table failed";
	}
}

void CDatabaseManager::CreateDoubleTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_DOUBLE + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + "  varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_DOUBLE_VALUE + " double NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create double table successed";
	}
	else
	{
		qDebug() << "create double table failed";
	}
}

//void CDatabaseManager::CreateLabelTable()
//{
//	QSqlQuery query(m_db);
//	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
//	query.prepare("CREATE TABLE " + TABLE_LABEL + " ("
//		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_PROJECT + " varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_PROGRAM + " varchar(" + strColumnLength + ") NOT NULL,"
//		+ TABLE_COLUMN_LABEL_NAME + " varchar(" + strColumnLength + ") NOT NULL)"
//		);
//	if (query.exec())
//	{
//		qDebug() << "create label table successed";
//	}
//	else
//	{
//		qDebug() << "create label table failed";
//	}
//}

void CDatabaseManager::CreateIntTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_INT + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_INT_VALUE + "  int NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create int table successed";
	}
	else
	{
		qDebug() << "create int table failed";
	}
}

void CDatabaseManager::CreateBoolTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_BOOL + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_BOOL_VALUE + "  int NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create bool table successed";
	}
	else
	{
		qDebug() << "create bool table failed";
	}
}

void CDatabaseManager::CreateStringTable()
{
	QSqlQuery query(m_db);
	QString strColumnLength = QString::number(TABLE_COLUMN_PROJECT_MAX_LENGTH);
	QString strTextLength = QString::number(TABLE_COLUMN_STRING_LENGTH);
	query.prepare("CREATE TABLE " + TABLE_STRING + " ("
		+ TABLE_COLUMN_SCOPE + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_NAME + " varchar(" + strColumnLength + ") NOT NULL,"
		+ TABLE_COLUMN_STRING_VALUE + " varchar(" + strTextLength + ") NOT NULL)"
		);
	if (query.exec())
	{
		qDebug() << "create string table successed";
	}
	else
	{
		qDebug() << "create string table failed";
	}
}

bool CDatabaseManager::IsExistVariable(const QString& strTableName, const QString& strScope, const QString& strVaribaleName)
{
	QSqlQuery query(m_db);
	query.prepare(QString("select * from %1 where %2 ='%3' and %4='%5'")
		.arg(strTableName).arg(TABLE_COLUMN_NAME).arg(strVaribaleName).arg(TABLE_COLUMN_SCOPE).arg(strScope));

	query.exec();

	while (query.next())
	{
		return true;
	}
	return false;
}



bool CDatabaseManager::IsExistVariable(const QString& strScope, const QString& strVariableName)
{
	QStringList strListTable;
	strListTable << TABLE_INT << TABLE_DOUBLE << TABLE_BOOL << TABLE_STRING << TABLE_POSITION << TABLE_DYNAMIC << TABLE_OVERLAP;

	for (int i = 0; i < strListTable.size();++i)
	{
		if (IsExistVariable(strListTable.at(i),strScope,strVariableName))
		{
			return true;
		}
	}

	return false;
}



void CDatabaseManager::InsertOverlapValue(const QString& strScope, const QString& strOverlap, const tOverlapConstraint& overlapValue)
{
	QSqlQuery query(m_db);

	query.prepare("insert into " + TABLE_OVERLAP + " values(?,?,?,?)");

	query.bindValue(0, strScope);
	query.bindValue(1, strOverlap);
	query.bindValue(2, overlapValue.m_TransitionMode);
	query.bindValue(3, overlapValue.m_TransitionParameter);

	query.exec();
}

void CDatabaseManager::UpdateOverlapValue(const QString& strScope, const QString& strOldOverlapName, const QString& strNewOverlapName,
	const tOverlapConstraint& overlapValue)
{
	QSqlQuery query(m_db);

	query.prepare("update " + TABLE_OVERLAP + " set "
		+ TABLE_COLUMN_NAME + "=?,"
		+ TABLE_COLUMN_OVERLAP_MODE + "=?,"
		+ TABLE_COLUMN_OVERLAP_VALUE + "=?"
		+ " where " + TABLE_COLUMN_NAME + "=?"
		+ " and " + TABLE_COLUMN_SCOPE + "=?"
		);

	query.bindValue(0, strNewOverlapName);
	query.bindValue(1, overlapValue.m_TransitionMode);
	query.bindValue(2, overlapValue.m_TransitionParameter);
	query.bindValue(3, strOldOverlapName);
	query.bindValue(4, strScope);

	if (query.exec())
	{
		qDebug() << "update succeed";
	}
	else
	{
		qDebug() << "update failed";
	}

}

void CDatabaseManager::UpdateOverlapValue(const QString& strScope, const QString& strOverlapName, const tOverlapConstraint& overlapValue)
{
	UpdateOverlapValue(strScope, strOverlapName, strOverlapName, overlapValue);
}

void CDatabaseManager::SelectOverlapValue(const QString& strScope, CValue::TYPE_MAP_OVERLAP& mapOverlap)
{
	QSqlQuery query(m_db);

	query.prepare("select * from " + TABLE_OVERLAP
		+ " where " + TABLE_COLUMN_SCOPE + "=?");

	query.bindValue(0, strScope);

	query.exec();

	tOverlapConstraint overlap;

	while (query.next())
	{
		overlap.m_TransitionMode = query.value(2).toInt();
		overlap.m_TransitionParameter = query.value(3).toDouble();
		mapOverlap[query.value(1).toString().toStdString()] = overlap;
	}
}

void CDatabaseManager::DeleteOverlapValue(const QString& strScope, const QString& strOverlap)
{
	DeleteValue(TABLE_OVERLAP, TABLE_COLUMN_NAME, strScope, strOverlap);

}
