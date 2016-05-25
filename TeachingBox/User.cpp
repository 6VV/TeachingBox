/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    User.cpp
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/02/23
//  Description:	用户类，用于记录用户相关信息
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/02/23
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/
#include "stdafx.h"
#include "User.h"

CUser::CUser()
{
	m_strName = "";
	m_strPassword = "";
	m_authority = 0;
	m_strLanguage = "";
	m_strIdentity = "";
}

CUser::CUser(const QString& strUserName, const QString& strUserPassword, const int authority, const QString& strUserLanguage, const QString& strUserIdentity)
{
	m_strName = strUserName;
	m_strPassword = strUserPassword;
	m_authority = authority;
	m_strLanguage = strUserLanguage;
	m_strIdentity = strUserIdentity;
}

CUser::CUser(const CUser& user)
{
	this->m_strName = user.GetName();
	this->m_strPassword = user.GetPassword();
	this->m_authority = user.GetAuthority();
	this->m_strLanguage = user.GetLanguage();
	this->m_strIdentity = user.GetIdentity();
}

CUser::~CUser()
{

}

void CUser::SetName(QString& strUserName)
{
	m_strName = strUserName;
}

void CUser::SetPassword(QString& strUserPassword)
{
	m_strPassword = strUserPassword;
}

void CUser::SetAuthority(int authority)
{
	m_authority = authority;
}

void CUser::SetLanguage(QString& strUserLanguage)
{
	m_strLanguage = strUserLanguage;
}

void CUser::SetIdentity(QString& strUserIdentity)
{
	m_strIdentity = strUserIdentity;
}

const QString& CUser::GetName() const
{
	return m_strName;
}

const QString& CUser::GetPassword() const
{
	return m_strPassword;
}

const int CUser::GetAuthority() const
{
	return m_authority;
}

const QString& CUser::GetLanguage() const
{
	return m_strLanguage;
}

const QString& CUser::GetIdentity() const
{
	return m_strIdentity;
}

void CUser::ClearInformation()
{
	this->m_strName = "";
	this->m_strPassword = "";
	this->m_authority = 0;
	this->m_strLanguage = "";
	this->m_strIdentity = "";
}
