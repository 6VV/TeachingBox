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
#include "CVariableSymbol.h"
#include "CMethodSymbol.h"
#include "CParameterManager.h"
#include "TInteger.h"


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
	m_value = CValue::GetInstance();

	InitScope();
}

void CInterpreterAdapter::InitScope()
{
	m_value->m_scopeSystem.SetScopeName(CScope::STR_SCOPE_SYSTEM);	/*设置系统作用域*/

	m_scopeSynergic = new CScope(CScope::STR_SCOPE_SYNERGIC);	/*初始化协作作用域*/
	m_scopeGlobal = new CScope(CScope::STR_SCOPE_GLOBAL);		/*初始化全局作用域*/

	m_value->m_scopeSystem.PushScope(m_scopeSynergic);			/*添加协作作用域*/
	m_scopeSynergic->PushScope(m_scopeGlobal);					/*添加全局作用域*/

	QStringList strListScope;
	strListScope <<CScope::STR_SCOPE_SYSTEM
		<< CScope::STR_SCOPE_SYNERGIC
		<< CScope::STR_SCOPE_GLOBAL;

	/*添加作用域内的符号*/
	for each (auto var in strListScope)
	{
		UpdateValueFromDatabase(var);
	}
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

//////////////////////////////////////////////////////////////////////////
/*过渡变量相关*/
//////////////////////////////////////////////////////////////////////////
void CInterpreterAdapter::GetOverlapListFromEnclosingScope(QStringList &strListPositions, const QString& scope)
{
	std::vector<CScope*> vecScope = GetEnclosingScope(scope.toStdString());
	std::set<std::string> setList;

	for each (auto scope in vecScope)
	{
		for each (auto variable in m_value->m_mapScopeOverlap[scope->GetScopeName().toStdString()])
		{
			setList.insert(variable.first);
		}
	}

	for each (auto variable in setList)
	{
		strListPositions.append(QString::fromStdString(variable));
	}
}


void CInterpreterAdapter::InsertOverlapValue(const QString& scope, CValue::TYPE_PAIR_OVERLAP& pairNewZone)
{
	m_value->m_mapScopeOverlap[scope.toStdString()].insert(pairNewZone);

	m_databaseManager->InsertOverlapValue(scope, QString::fromStdString(pairNewZone.first), pairNewZone.second);

	/*更新符号表*/
	CVariableSymbol* symbol = new CVariableSymbol(scope, QString::fromStdString(pairNewZone.first), CSymbol::TYPE_OVERLAP);
	m_value->m_scopeSystem.FindScopeScrollDown(scope)->DefineSymbol(symbol);
}


void CInterpreterAdapter::UpdateOverlapValue(const QString& scope, const std::string& strOldZone, CValue::TYPE_PAIR_OVERLAP& pairNewZone)
{
	m_value->m_mapScopeOverlap[scope.toStdString()].erase(strOldZone);
	m_value->m_mapScopeOverlap[scope.toStdString()].insert(pairNewZone);

	m_databaseManager->UpdateOverlapValue(scope, QString::fromStdString(strOldZone), QString::fromStdString(pairNewZone.first), pairNewZone.second);

	/*更新符号表*/
	CVariableSymbol* symbol = new CVariableSymbol(scope, QString::fromStdString(pairNewZone.first), CSymbol::TYPE_OVERLAP);
	m_value->m_scopeSystem.FindScopeScrollDown(scope)
		->RenameSymbol(QString::fromStdString(strOldZone), 
		QString::fromStdString(pairNewZone.first), symbol);
}

void CInterpreterAdapter::UpdateOverlapValue(const QString& scope, const std::string& strOldZone, const std::string& strNewZone, CValue::TYPE_OVERLAP& overlap)
{
	m_value->m_mapScopeOverlap[scope.toStdString()].erase(strOldZone);
	m_value->m_mapScopeOverlap[scope.toStdString()][strNewZone]=overlap;

	m_databaseManager->UpdateOverlapValue(scope, QString::fromStdString(strOldZone), QString::fromStdString(strNewZone), overlap);

	/*更新符号表*/
	CVariableSymbol* symbol = new CVariableSymbol(scope, QString::fromStdString(strNewZone), CSymbol::TYPE_OVERLAP);
	m_value->m_scopeSystem.FindScopeScrollDown(scope)
		->RenameSymbol(QString::fromStdString(strOldZone), QString::fromStdString(strNewZone), symbol);
}

void CInterpreterAdapter::DeleteOverlapValue(const QString& strScope, std::string& strName)
{
	/*从符号表中删除*/
	CScope* scope = m_value->m_scopeSystem.FindScopeScrollDown(strScope);
	scope->DeleteSymbol(QString::fromStdString(strName));

	/*从内存中删除*/
	m_value->m_mapScopeOverlap[strScope.toStdString()].erase(strName);

	/*从数据库中删除*/
	m_databaseManager->DeleteOverlapValue(strScope, QString::fromStdString(strName));
}


void CInterpreterAdapter::UpdateOverlapValueFromDatabase(const QString& strScope)
{
	/*添加作用域内位置变量*/
	CValue::TYPE_MAP_OVERLAP map;
	m_value->m_mapScopeOverlap[strScope.toStdString()] = map;
	CValue::TYPE_MAP_OVERLAP& mapFind = m_value->m_mapScopeOverlap[strScope.toStdString()];
	m_databaseManager->SelectOverlapValue(strScope, mapFind);


	/*更新符号表*/
	CScope* scope = m_value->m_scopeSystem.FindScopeScrollDown(strScope);

	for each (auto var in mapFind)
	{
		CVariableSymbol* symbol = new CVariableSymbol(scope->GetScopeName(), QString::fromStdString(var.first), CSymbol::TYPE_OVERLAP);
		scope->DefineSymbol(symbol);
	}
}


bool CInterpreterAdapter::GetOverlapValue(const QString& strScope, const std::string& strName, CValue::TYPE_OVERLAP& value)
{
	auto iter = m_value->m_mapScopeOverlap.at(strScope.toStdString()).find(strName);
	if (iter == m_value->m_mapScopeOverlap.at(strScope.toStdString()).end())
	{
		return false;
	}
	value = m_value->m_mapScopeOverlap[strScope.toStdString()].at(strName);
	return true;
}

bool CInterpreterAdapter::GetOverlapValueFromEnclosingScope(const QString& scope, const std::string& strName, CValue::TYPE_OVERLAP& vecValue)
{
	std::vector<CScope*> vecScope = GetEnclosingScope(scope.toStdString());

	for each (auto var in vecScope)
	{
		if (GetOverlapValue(var->GetScopeName(), strName, vecValue))
		{
			return true;
		}
	}

	return false;
}


bool CInterpreterAdapter::IsOverlapValueExist(const QString& strScope, const std::string& strName)
{
	auto iter1 = m_value->m_mapScopeOverlap.find(strScope.toStdString());

	if (iter1 == m_value->m_mapScopeOverlap.end())
	{
		return false;
	}

	auto iter2 = iter1->second.find(strName);
	if (iter2 == iter1->second.end())
	{
		return false;
	}

	return true;
}


void CInterpreterAdapter::UpdateValueFromDatabase()
{
	//auto t1 = clock();

	/*清空项目内的所有数据*/
	ClearProjectValue();
	
	CScreenProject* project=CScreenProject::GetInstance();

	/*若未打开任何项目*/
	if (!project->IsLoadProject())
	{
		return;
	}

	QStringList strListScope;
	strListScope << project->GetLoadedProjectNameInDatabase();	/*添加项目名*/
	CScope* scopeProject = new CScope(project->GetLoadedProjectNameInDatabase());
	m_scopeGlobal->PushScope(scopeProject);	 /*添加项目作用域*/

	/*添加文件名*/
	for each (auto var in project->GetAllFiles()[project->GetLoadedProjectNameInDatabase()].keys())
	{
		strListScope << var;
		CMethodSymbol* methodSymbol = new CMethodSymbol(scopeProject->GetScopeName(),
			var, CSymbol::TYPE_VOID);
		CScope* methodScope = new CScope(var);
		scopeProject->PushScope(methodScope);	/*添加文件作用域*/
		scopeProject->DefineSymbol(methodSymbol);		/*定义文件名*/
	}


	/*添加作用域内的数据*/
	for each (auto var in strListScope)
	{
		UpdateValueFromDatabase(var);
	}

	strListScope.pop_front();
	TVariateManager::GetInstance()->LoadProjectDataFromDatabase(
		project->GetLoadedProjectNameInDatabase(), strListScope);

	//auto t2 = clock();
	//qDebug() << (t2 - t1) * 1000 / CLOCKS_PER_SEC;
}

void CInterpreterAdapter::UpdateValueFromDatabase(const QString& strScope)
{
	//UpdatePositionValueFromDatabase(strScope);
	//UpdateDynamicValueFromDatabase(strScope);
	//UpdateOverlapValueFromDatabase(strScope);
	//UpdateDoubleValueFromDatabase(strScope);
	//UpdateIntValueFromDatabase(strScope);
	/*UpdateStringValueFromDatabase*/(strScope);
	//UpdateBoolValueFromDatabase(strScope);


	//TVariateManager::GetInstance()->LoadProjectObjectFromDatabase();
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
		UpdateOverlapValue(strScope, strOldName.toStdString(), strNewName.toStdString(), m_value->m_mapScopeOverlap[strScope.toStdString()][strOldName.toStdString()]);
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
