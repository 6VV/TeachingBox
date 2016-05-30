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

void CDatabaseManager::DeleteScope(const QString& scope)
{
	QSqlQuery query(m_db);
	query.prepare(QString("delete from %1 where %2='%3'")
		.arg(VARIATE_TABLE_NAME)
		.arg(VARIATE_COLUMN_SCOPE)
		.arg(scope));

	if (!query.exec())
	{
		throw std::exception("delete scope variates from database failed");
	}
}

CDatabaseManager* CDatabaseManager::GetInstance()
{
	return CSingleTon<CDatabaseManager>::GetInstance();
}


void CDatabaseManager::CreateTable()
{
	m_db.transaction();
	CreateVariateTable();
	CreateUserTable();	/*创建用户表*/

	m_db.commit();
}

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
