#ifndef _TEACHING_BOX_C_DATABASE_MANAGER_H_
#define _TEACHING_BOX_C_DATABASE_MANAGER_H_

#include "QString"
#include "QSqlDatabase"
#include "CSingleTon.h"
#include <map>
#include <set>
#include "Init.h"
#include <string>
#include "CValue.h"
#include "User.h"
#include "DataStruct.h"
#include "QDataStream"
#include "QSqlQuery"
#include "QVector"
class CDatabaseManager
{
	friend class CSingleTon<CDatabaseManager>;

private:
	CDatabaseManager();
	CDatabaseManager(const CDatabaseManager&);
	CDatabaseManager& operator=(const CDatabaseManager&);
	~CDatabaseManager();
	
public:
	const QString VARIATE_TABLE_NAME = "TableVariate";
	const QString VARIATE_COLUMN_SCOPE = "Scope";
	const QString VARIATE_COLUMN_NAME = "Name";
	const QString VARIATE_COLUMN_VALUE = "Value";

	void DeleteVariate(const QString& scope, const QString& name);
	void DeleteAllVariate();
	void InsertVariate(QDataStream& dataStream);
	void SelectAllVariate(QVector<QByteArray>& variates);
	void SelectVariatesFromScope(QVector<QByteArray>& variates, const QString& scope);
	void UpdateVariate(const QString& oldScope, const QString& oldName, QDataStream& dataStream);

private:
	void CreateVariateTable();

	/*自定义公有函数*/
public:
	static CDatabaseManager* GetInstance();

	void DeleteScopeData(const QString& strScope);

	/*用户表相关*/
	void InsertUserInfo(const CUser& user);	/*插入新用户*/
	void SelectUserInfo(CUser::TYPE_MAP_USER& mapUser);	/*查询所有用户信息*/

	void UpdateUserInfo(const QString& strUserName, const CUser& user);	/*更新用户信息*/
	void DeleteUserInfo(const QString& strUserName);	/*删除用户信息*/

	bool IsExistVariable(const QString& strScope, const QString& strVariableName);	/*作用域是否存在某变量*/
	bool IsExistVariable(const QString& strTableName, const QString& strScope, const QString& strVaribaleName);		/*某表内、某作用域内是否存在某变量*/

	/*过渡变量相关*/
	void InsertOverlapValue(const QString& strScope, const QString& strOverlap, const tOverlapConstraint& overlapValue);	/*插入过渡变量*/

	void UpdateOverlapValue(const QString& strScope, const QString& strOldOverlapName, const QString& strNewOverlapName, const tOverlapConstraint& overlapValue);	/*更新过渡变量（包括变量名）*/
	void UpdateOverlapValue(const QString& strScope, const QString& strOverlapName, const tOverlapConstraint& overlapValue);	/*更新过渡变量（不包括变量名）*/

	void SelectOverlapValue(const QString& strScope, CValue::TYPE_MAP_OVERLAP& mapOverlap);

	void DeleteOverlapValue(const QString& strScope, const QString& strOverlap);	/*删除特定过渡变量*/

	/*自定义私有函数*/
private:
	void CreateTable();	/*创建所有表*/
	void CreateUserTable();	/*创建用户表*/
	void CreatePositionTable();	/*创建位置表*/
	void CreateDynamicTable();	/*创建速度表*/
	void CreateOverlapTable();		/*创建过渡表*/
	void CreateDoubleTable();	/*创建浮点数表*/
	void CreateIntTable();		/*创建整数表*/
	void CreateBoolTable();		/*创建布尔数表*/
	void CreateStringTable();	/*创建字符串表*/

	//////////////////////////////////////////////////////////////////////////
	/*存储单个值*/

	void DeleteValue(const QString& strTableName, const QString& strColumnVariable, const QString& strScope, const QString& strVariable);	/*删除特定变量*/
	void DeleteValue(const QString& strTableName, const QString& strScope);		/*删除作用内的变量*/


	//////////////////////////////////////////////////////////////////////////

	/*自定义私有变量*/
private:
	const QString DATABASE_TYPE = "QSQLITE";	/*数据库类型*/
	const QString HOST_NAME = "localhost";		/*数据库主机名*/
	const QString DATABASE_NAME = "teaching_box.db"; /*数据库名*/
	const QString USER_NAME = "Administrator";	/*用户名*/
	const QString PASSWORD = "123456";			/*密码*/

	const QString CONNECTION_NAME = "teachingbox";	/*连接名*/
	QSqlDatabase m_db;	/*数据库*/

	//////////////////////////////////////////////////////////////////////////
	/*用户表相关*/
	//////////////////////////////////////////////////////////////////////////
	const int TABLE_USER_NAME_MAX_LENGTH = USER_NAME_MAX_LENGTH;	/*用户名最大长度*/
	const int TABLE_USER_PASSWORD_MAX_LENGTH = USER_PASSWORD_MAX_LENGTH;	/*用户密码最大长度*/

	const QString TABLE_USER = "UserTable";	/*用户表名*/
	const QString TABLE_COLUMN_NAME = "name";	/*用户名列名*/
	const QString TABLE_COLUMN_USER_PASSWORD = "password";	/*用户密码列名*/
	const QString TABLE_COLUMN_USER_AUTHORITY = "authority";	/*用户权限列名*/
	const QString TABLE_COLUMN_USER_LANGUAGE = "language";	/*用户语言*/
	const QString TABLE_COLUMN_USER_IDENTITY = "indetity";	/*用户身份*/

	//////////////////////////////////////////////////////////////////////////
	/*解释器表相关*/
	//////////////////////////////////////////////////////////////////////////
	const int TABLE_COLUMN_PROJECT_MAX_LENGTH =VAR_NAME_MAX_LENGTH;	/*列名最大长度*/
	const int TABLE_COLUMN_STRING_LENGTH = STRING_MAX_LENGTH;	/*字符串最大长度*/

	const QString TABLE_COLUMN_SCOPE = "scope";		/*作用域列*/
	const QString TABLE_COLUMN_PROJECT = "project";	/*项目名列*/
	const QString TABLE_COLUMN_PROGRAM = "Program";	/*程序名列*/

	const QString STR_SCOPE_GLOBAL = "global";		/*全局*/
	const QString STR_SCOPE_LOCAL = "local";		/*局部*/

	/*符号表相关*/
	const QString TABLE_SYMBOL = "SymbolTable";	/*符号表相关*/

	//const QString TABLE_COLUMN_NAME = "name";	/*符号名*/
	const QString TABLE_COLUMN_SYMBOL_TYPE = "type";	/*符号类型*/

	/*PositionTable相关*/
	const QString TABLE_POSITION = "PositionTable";	/*位置表名*/

	//const QString TABLE_COLUMN_NAME = "position";	/*位置表，位置名列*/
	const QString TABLE_COLUMN_POSITION_AXIS1 = "axis1";	
	const QString TABLE_COLUMN_POSITION_AXIS2 = "axis2";
	const QString TABLE_COLUMN_POSITION_AXIS3 = "axis3";
	const QString TABLE_COLUMN_POSITION_AXIS4 = "axis4";
	const QString TABLE_COLUMN_POSITION_AXIS5 = "axis5";
	const QString TABLE_COLUMN_POSITION_AXIS6 = "axis6";

	/*DynamicTable相关*/
	const QString TABLE_DYNAMIC = "DynamicTable";	/*速度表名*/

	//const QString TABLE_COLUMN_NAME = "dynamic";	/*速度表，速度名列*/
	const QString TABLE_COLUMN_DYNAMIC_VELOCITY = "velocity";	/*速度表，速度值列*/
	const QString TABLE_COLUMN_DYNAMIC_ACCELERATION = "acceleration";	/*加速度值列*/
	const QString TABLE_COLUMN_DYNAMIC_DECELERATION = "deceleration";	/*减速度值列*/
	const QString TABLE_COLUMN_DYNAMIC_POSTURE_VELOCITY = "PostureVelocity";	/*姿态速度*/
	const QString TABLE_COLUMN_DYNAMIC_POSTURE_ACCELERATION = "PostureAcceleration";	/*姿态加速度*/
	const QString TABLE_COLUMN_DYNAMIC_POSTURE_DECELERATION = "PostureDeceleration";	/*姿态减速度*/

	/*OverlapTable相关*/
	const QString TABLE_OVERLAP = "OverlapTable";	/*速度表名*/

	//const QString TABLE_COLUMN_NAME = "overlap";	/*过渡表，过渡名列*/
	const QString TABLE_COLUMN_OVERLAP_MODE = "mode";		/*过渡表，模式*/
	const QString TABLE_COLUMN_OVERLAP_VALUE = "value";		/*过渡表，过渡值*/

	/*doubleTable相关*/
	const QString TABLE_DOUBLE = "DoubleTable";	/*浮点数表名*/

	//const QString TABLE_COLUMN_NAME = "double";	/*浮点数表，浮点数名*/
	const QString TABLE_COLUMN_DOUBLE_VALUE = "value";	/*浮点数表，浮点数值*/

	/*labelTable相关*/
	const QString TABLE_LABEL = "LabelTable";	/*标签表名*/

	//const QString TABLE_COLUMN_NAME = "label";	/*标签表，标签列*/

	/*intTable相关*/
	const QString TABLE_INT = "IntTable";	/*整数表名*/

	//const QString TABLE_COLUMN_NAME = "int";	/*整数表，整数名*/
	const QString TABLE_COLUMN_INT_VALUE = "value";	/*整数表，整数值*/

	/*boolTable相关*/
	const QString TABLE_BOOL = "BoolTable";	/*布尔表名*/

	//const QString TABLE_COLUMN_NAME = "bool";	/*布尔表，布尔名*/
	const QString TABLE_COLUMN_BOOL_VALUE = "value";	/*布尔表，布尔值*/

	/*stringTable相关*/
	const QString TABLE_STRING = "StringTable";	/*字符串表名*/

	//const QString TABLE_COLUMN_NAME = "string";	/*字符串表，字符串名*/
	const QString TABLE_COLUMN_STRING_VALUE = "value";	/*字符串表，字符串值*/
};


#endif
