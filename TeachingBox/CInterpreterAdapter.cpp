#include "stdafx.h"
#include "CInterpreterAdapter.h"
#include "CScreenProject.h"
#include "CValue.h"
#include "TVariateManager.h"
#include "QSet"
#include "TDouble.h"
#include "TBool.h"
#include "TString.h"
#include "TPosition.h"
#include "TDynamic.h"
#include "CParameterManager.h"
#include "TInteger.h"
#include "TOverlap.h"


CInterpreterAdapter::CInterpreterAdapter()
{
	Init();
}

CInterpreterAdapter::~CInterpreterAdapter()
{

}

void CInterpreterAdapter::Init()
{
	m_databaseManager = CDatabaseManager::GetInstance();

}

std::vector<CScope*> CInterpreterAdapter::GetEnclosingScope(const std::string& currentScope)
{
	CScope* pScope = CValue::GetInstance()->m_scopeSystem.FindScopeScrollDown(QString::fromStdString(currentScope));
	std::vector<CScope*> vecScope;
	while (pScope != NULL)
	{
		vecScope.push_back(pScope);
		pScope = pScope->GetEnclosingScope();
	}

	return vecScope;
}

CInterpreterAdapter* CInterpreterAdapter::GetInstance()
{
	return CSingleTon<CInterpreterAdapter>::GetInstance();
}

void CInterpreterAdapter::UpdateVariableName(const QString& strOldName, const QString& strNewName, 
	const QString& strScope, const QString& strType)
{
	if (strType == CParameterManager::STR_TYPE_INT)
	{
		/*UpdateIntValue(strScope, strOldName.toStdString(), strNewName.toStdString(),
			m_value->m_mapScopeInt[strScope.toStdString()][strOldName.toStdString()]);*/
		int value = static_cast<TInteger*>(TVariateManager::GetInstance()
			->GetVariate(strScope, strOldName))->GetValue();
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TInteger(strScope, strNewName, value));
	}
	else if (strType == CParameterManager::STR_TYPE_DOUBLE)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TDouble(strScope, strNewName, 
			static_cast<TDouble*>(TVariateManager::GetInstance()->GetVariate(strScope,strOldName))->GetValue()));
		//UpdateDoubleValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeDouble[strScope.toStdString()][strOldName.toStdString()]);
	}
	else if (strType == CParameterManager::STR_TYPE_BOOL)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName, 
			TBool(strScope, strNewName, 
			static_cast<TBool*>(TVariateManager::GetInstance()->GetVariate(strScope, strOldName))->GetValue()));
		//UpdateBoolValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeBool[strScope.toStdString()][strOldName.toStdString()]);
	}
	else if (strType == CParameterManager::STR_TYPE_STRING)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TString(strScope, strNewName,
			static_cast<TString*>(TVariateManager::GetInstance()->GetVariate(strScope, strOldName))->GetValue()));

		//UpdateStringValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeString[strScope.toStdString()][strOldName.toStdString()]);
	}
	else if (strType == CParameterManager::STR_TYPE_POSITION)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TPosition(strScope, strNewName,
			static_cast<TPosition*>(TVariateManager::GetInstance()->GetVariate(strScope, strOldName))->GetValue()));

		//UpdatePositionValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopePosition[strScope.toStdString()][strOldName.toStdString()]);
	}
	else if (strType == CParameterManager::STR_TYPE_DYNAMIC)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TDynamic(strScope, strNewName,
			static_cast<TDynamic*>(TVariateManager::GetInstance()->GetVariate(strScope, strOldName))->GetValue()));

		//UpdateDynamicValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeDynamic[strScope.toStdString()][strOldName.toStdString()]);
	}
	else if (strType == CParameterManager::STR_TYPE_OVERLAP)
	{
		TVariateManager::GetInstance()->Update(strScope, strOldName,
			TOverlap(strScope, strNewName,
			static_cast<TOverlap*>(TVariateManager::GetInstance()->GetVariate(strScope, strOldName))->GetValue()));
		//UpdateOverlapValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeOverlap[strScope.toStdString()][strOldName.toStdString()]);
	}
}



bool CInterpreterAdapter::IsCorrectName(std::string& strName)
{
	qDebug() << QString::fromStdString(strName);
	/*若长度为0或过大*/
	if (strName.size()==0 || strName.size()>CValue::GetInstance()->MAX_STRING_LENGTH)
	{
		return false;
	}
	char c = strName.at(0);
	/*若第一个不是字母*/
	if (!((c>='a'&& c<='z')||(c>='A' && c<='Z')))
	{
		return false;
	}
	for each (auto var in strName)
	{
		/*若不是字母或数字*/
		if (!((var >= 'a'&& var <= 'z') || (var >= 'A' && var <= 'Z') || (var >= '0' && var <= '9')))
		{
			return false;
		}
	}
	return true;
}

//bool CInterpreterAdapter::IsExist(CScope* scope, const std::string& strName)
//{
//	if (scope->FindSymbolScrollUp(QString::fromStdString(strName)) != NULL)
//	{
//		return true;
//	}
//	return false;
//}

//bool CInterpreterAdapter::IsExist(const QString& strScope, const std::string& strName)
//{
//	//CScope* scope = m_value->m_scopeSystem.FindScopeScrollDown(strScope);
//
//	//return IsExist(scope, strName);
//	return TVariateManager::GetInstance()->IsExistVariateScrollUp(strScope, QString::fromStdString(strName));
//}

//bool CInterpreterAdapter::IsSymbolExist(const QString& strScopeName, const std::string& strName)
//{
//	//CScope* scope = m_value->m_scopeSystem.FindScopeScrollDown(strScopeName);
//	//if (scope==NULL)
//	//{
//	//	return false;
//	//}
//	return IsExist(strScopeName, strName);
//}


void CInterpreterAdapter::ClearProjectValue()
{
	TVariateManager::GetInstance()->ClearProjectData();


	///*清空项目作用域符号*/
	//m_scopeGlobal->ClearChildren();

	///*清空位置保存数据*/
	//std::list<std::string> strList;
	//for each (auto var in m_value->m_mapScopePosition)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first!=CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopePosition.erase(var);
	//}


	///*清空动态保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeDynamic)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeDynamic.erase(var);
	//}

	///*清空过度保存数据*/
	///*清空动态保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeOverlap)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeOverlap.erase(var);
	//}

	///*清空Int保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeInt)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeInt.erase(var);
	//}
	

	///*清空Double保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeDouble)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeDouble.erase(var);
	//}

	///*清空Bool保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeBool)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeBool.erase(var);
	//}

	///*清空String保存数据*/
	//strList.clear();
	//for each (auto var in m_value->m_mapScopeString)
	//{
	//	/*若不为全局或协作作用域变量*/
	//	if (var.first != CScope::STR_SCOPE_SYNERGIC.toStdString()
	//		&& var.first != CScope::STR_SCOPE_GLOBAL.toStdString()
	//		&& var.first != CScope::STR_SCOPE_SYSTEM.toStdString())
	//	{
	//		strList.push_back(var.first);
	//	}
	//}
	//for each (auto var in strList)
	//{
	//	m_value->m_mapScopeString.erase(var);
	//}
}

void CInterpreterAdapter::DeleteDatabaseData(const QString& strScope)
{
	m_databaseManager->DeleteScopeData(strScope);
}
