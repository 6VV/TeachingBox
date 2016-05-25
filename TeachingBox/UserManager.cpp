/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    UserManager.cpp
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/02/23
//  Description:	用户管理类，用于记录当前用户信息，并进行用户管理操作，包括更新数据库信息，
					添加、删除用户，查询用户信息等。
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/02/23
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/
#include "stdafx.h"
#include "UserManager.h"
#include "CDatabaseManager.h"
#include "QHostInfo"

CUserManager::CUserManager()
{
	Init();
}

CUserManager::~CUserManager()
{

}

void CUserManager::Init()
{
	GetUserInfoFromDatabase();
	m_currentUser.ClearInformation();
}

const int CUserManager::AUTHORITY_MIN=1;

const int CUserManager::AUTHORITY_MAX=16;

const QString CUserManager::LANGUAGE_CHINESE="Chinese";

const QString CUserManager::LANGUAGE_ENGLISH="English";

const QString CUserManager::IDENTITY_ADMINISTRATOR="Administrator";

const QString CUserManager::IDENTITY_NORMAL="Normal";

const QString CUserManager::USER_ADMINISTRAOR="Administrator";

CUserManager* CUserManager::GetInstance()
{
	return CSingleTon<CUserManager>::GetInstance();
}


void CUserManager::GetUserList(QStringList& strListUser)
{
	for each (auto var in m_mapUsers)
	{
		strListUser.append(var.GetName());
	}
}

CUser::TYPE_MAP_USER& CUserManager::GetUserMap()
{
	return m_mapUsers;
}

void CUserManager::GetUserInfo(const QString& strUserName, CUser& user)
{
	user = m_mapUsers[strUserName];
}

void CUserManager::SetCurrentUser(CUser& user)
{
	m_currentUser = user;
}

void CUserManager::SetControlAuthority(bool controlAuthority)
{
	m_hasControlAuthority = controlAuthority;
}

CUser& CUserManager::GetCurrentUser()
{
	return m_currentUser;
}


bool CUserManager::GetControlAuthority()
{
	return m_hasControlAuthority;
}

void CUserManager::GetUserInfoFromDatabase()
{
	CDatabaseManager::GetInstance()->SelectUserInfo(m_mapUsers);
}

void CUserManager::InsertUser(const CUser& user)
{
	m_mapUsers.insert(user.GetName(),user);
	CDatabaseManager::GetInstance()->InsertUserInfo(user);
}

void CUserManager::UpdateUser(const QString& strUserName, const CUser& user)
{
	auto iterUser = m_mapUsers.find(strUserName);

	/*若不存在*/
	if (iterUser==m_mapUsers.end())
	{
		InsertUser(user);
	}
	else
	{
		m_mapUsers.erase(iterUser);
		m_mapUsers.insert(user.GetName(), user);

		CDatabaseManager::GetInstance()->UpdateUserInfo(strUserName, user);
	}
}


void CUserManager::DeleteUser(const QString& strUserName)
{
	m_mapUsers.remove(strUserName);
	CDatabaseManager::GetInstance()->DeleteUserInfo(strUserName);
}

bool CUserManager::IsUserExist(CUser& user)
{
	/*若未找到*/
	if (m_mapUsers.find(user.GetName())==m_mapUsers.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CUserManager::ClearCurrentUser()
{
	m_currentUser.ClearInformation();	/*清除当前用户信息*/
}

bool CUserManager::IsUserLogin()
{
	return !m_currentUser.GetName().isEmpty();
}

QString CUserManager::GetIpAddress()
{
	/*添加IP地址*/
	QString strAddress;
	QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

	for each (auto var in hostInfo.addresses())
	{
		if (var.protocol() == QAbstractSocket::IPv4Protocol)
		{
			strAddress = var.toString();
			break;
		}
	}
	return strAddress;
}

