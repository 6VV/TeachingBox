#ifndef _TEACHING_BOX_C_USER_H_
#define _TEACHING_BOX_C_USER_H_

#include "QMap"

class CUser
{
public:
	/*此处采用对象而非指针，主要是为了避免因不注意导致的内存泄露，
	*由于用户量较小，且不存在频繁的登录、登出操作，故速度不再考虑范围内*/
	typedef QMap<QString, CUser> TYPE_MAP_USER;

public:
	CUser();
	CUser(const QString& strUserName, const QString& strUserPassword, const int authority,const QString& strUserLanguage,const QString& strUserIdentity);
	CUser(const CUser& user);
	~CUser();
	
	void SetName(QString& strUserName);
	void SetPassword(QString& strUserPassword);
	void SetAuthority(int authority);
	void SetLanguage(QString& strUserLanguage);
	void SetIdentity(QString& strUserIdentity);

	const QString& GetName() const;
	const QString& GetPassword() const;
	const int GetAuthority() const;
	const QString& GetLanguage() const;
	const QString& GetIdentity() const;

	void ClearInformation();

private:
	QString m_strName;		/*用户密码*/
	QString m_strPassword;	/*用户名*/
	int m_authority = 16;	/*用户权限*/
	QString m_strLanguage;	/*用户语言*/
	QString m_strIdentity;	/*用户身份*/
};

#endif