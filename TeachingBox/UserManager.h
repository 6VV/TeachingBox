#ifndef _TEACHING_BOX_C_USER_MANAGER_H_
#define _TEACHING_BOX_C_USER_MANAGER_H_

#include "User.h"
#include "CSingleTon.h"
#include <unordered_map>

class CUserManager
{
	friend CSingleTon<CUserManager>;

public:
	static const int AUTHORITY_MIN;
	static const int AUTHORITY_MAX;

	static const QString LANGUAGE_CHINESE;
	static const QString LANGUAGE_ENGLISH;

	static const QString IDENTITY_ADMINISTRATOR;
	static const QString IDENTITY_NORMAL;

	static const QString USER_ADMINISTRAOR;
public:
	static CUserManager* GetInstance();	/*获取唯一实例*/

	void GetUserList(QStringList& strListUser);	/*获取用户列表*/
	CUser::TYPE_MAP_USER& GetUserMap();	/*获取用户列表*/
	void GetUserInfo(const QString& strUserName, CUser& user);	/*获取用户信息*/
	CUser& GetCurrentUser();	/*获取当前用户*/
	bool GetControlAuthority();	/*获取控制权限*/

	void SetCurrentUser(CUser& user);	/*设置当前用户*/
	void SetControlAuthority(bool controlAuthority);	/*设置控制权限*/

	void InsertUser(const CUser& user);	/*添加新用户*/
	void UpdateUser(const QString& strUserName, const CUser& user);	/*更新用户信息*/
	void DeleteUser(const QString& strUserName);	/*删除用户*/

	bool IsUserExist(CUser& user);	/*用户是否存在*/
	
	void ClearCurrentUser();	/*删除当前用户*/
	bool IsUserLogin();	/*是否有用户登录*/

	QString GetIpAddress();	/*获取本机IP地址*/
private:
	CUserManager();
	CUserManager(const CUserManager&);
	CUserManager& operator=(const CUserManager&);
	~CUserManager();

	void Init();
	void GetUserInfoFromDatabase();	/*从数据库中获取用户信息*/

private:
	CUser m_currentUser;	/*当前用户*/

	CUser::TYPE_MAP_USER m_mapUsers;	/*用户列表*/

	bool m_hasControlAuthority = true;	/*用户控制权限*/
};

#endif