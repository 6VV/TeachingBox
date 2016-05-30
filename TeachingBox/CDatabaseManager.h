#ifndef _TEACHING_BOX_C_DATABASE_MANAGER_H_
#define _TEACHING_BOX_C_DATABASE_MANAGER_H_

#include "CSingleTon.h"
#include "User.h"
#include "Init.h"

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

	/*自定义公有函数*/
public:
	static CDatabaseManager* GetInstance();

	/*用户表相关*/
	void InsertUserInfo(const CUser& user);	/*插入新用户*/
	void SelectUserInfo(CUser::TYPE_MAP_USER& mapUser);	/*查询所有用户信息*/

	void UpdateUserInfo(const QString& strUserName, const CUser& user);	/*更新用户信息*/
	void DeleteUserInfo(const QString& strUserName);	/*删除用户信息*/

	/*变量相关*/
	void DeleteAllVariate();
	void DeleteScope(const QString& scope);
	void DeleteVariate(const QString& scope, const QString& name);

	void InsertVariate(QDataStream& dataStream);

	void SelectAllVariate(QVector<QByteArray>& variates);
	void SelectVariatesFromScope(QVector<QByteArray>& variates, const QString& scope);

	void UpdateVariate(const QString& oldScope, const QString& oldName, QDataStream& dataStream);

	/*自定义私有函数*/
private:
	void CreateTable();	/*创建所有表*/
	void CreateUserTable();	/*创建用户表*/
	void CreateVariateTable();

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

};


#endif
