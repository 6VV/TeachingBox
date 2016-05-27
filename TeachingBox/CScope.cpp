#include "stdafx.h"
#include "CScope.h"

const QString CScope::STR_SCOPE_SYSTEM = "SYSTEM";		/*系统作用域*/
const QString CScope::STR_SCOPE_SYNERGIC = "SYNERGIC";	/*协作作用域*/
const QString CScope::STR_SCOPE_GLOBAL = "GLOBAL";		/*全局作用域*/
const QString CScope::STR_SCOPE_PROJECT = "PROJECT";		/*项目作用域*/
const QString CScope::STR_SCOPE_LOCAL = "LOCAL";		/*局部作用域*/

const QStringList CScope::SCOPE_ORI = QStringList{ CScope::STR_SCOPE_SYSTEM, CScope::STR_SCOPE_SYNERGIC, CScope::STR_SCOPE_GLOBAL };

QString& CScope::GetScopeName()
{
	return m_scopeName;
}

void CScope::SetScopeName(const QString& strScopeName)
{
	m_scopeName = strScopeName;
}

CScope* CScope::GetEnclosingScope()
{
	return m_enclosingScope;
}

void CScope::PushScope(CScope* scope)
{
	this->m_listScope.push_back(scope);
	scope->m_enclosingScope = this;
	//scope->SetEnclosingScope(this);
}

void CScope::DefineSymbol(CSymbol* sym)
{
	this->m_mapSymbol[sym->GetName()] = sym;
}

bool CScope::DefineSymbol(const QString& desScope, CSymbol* symbol)
{
	CScope* scopeFound = FindScopeScrollDown(desScope);
	if (!scopeFound)
	{
		return false;
	}
	scopeFound->DefineSymbol(symbol);
	return true;
}

void CScope::DeleteSymbol(const QString& strName)
{
	auto iter = m_mapSymbol.find(strName);

	if (iter!=m_mapSymbol.end())
	{
		delete(iter.value());
		m_mapSymbol.erase(iter);
	}
}

CSymbol* CScope::FindSymbolScrollUp(const QString& name)
{
	auto iter = m_mapSymbol.find(name);

	/*若找到*/
	if (iter!=m_mapSymbol.end())
	{
		return iter.value();
	}
	else
	{
		/*若不存在上层作用域*/
		if (this->m_enclosingScope==NULL)
		{
			return NULL;
		}
		else
		{
			return m_enclosingScope->FindSymbolScrollUp(name);
		}
	}
}

CSymbol* CScope::FindSymbolInSelf(const QString& strName)
{
	auto iter = m_mapSymbol.find(strName);

	/*若找到*/
	if (iter != m_mapSymbol.end())
	{
		return iter.value();
	}
	else
	{
		return NULL;
	}
}

CScope* CScope::FindScopeScrollDown(const QString& strScope)
{
	if (m_scopeName==strScope)
	{
		return this;
	}
	
	/*若不存在子作用域*/
	if (m_listScope.size()==0)
	{
		return nullptr;
	}

	/*遍历子作用域*/
	for each (auto var in m_listScope)
	{
		CScope* scope = var->FindScopeScrollDown(strScope);
		if (scope != nullptr)
		{
			return scope;
		}
	}

	/*未找到*/
	return nullptr;
}

CScope::ScopeSymbol CScope::FindSymbolScopeScrollUp(const QString& strSymbolName)
{
	ScopeSymbol scopySymbol;
	auto iter = m_mapSymbol.find(strSymbolName);

	/*若找到*/
	if (iter != m_mapSymbol.end())
	{
		scopySymbol.scope = this;
		scopySymbol.symbol = iter.value();

		return scopySymbol;
	}
	else
	{
		/*若不存在上层作用域*/
		if (this->m_enclosingScope == NULL)
		{
			return scopySymbol;
		}
		else
		{
			return m_enclosingScope->FindSymbolScopeScrollUp(strSymbolName);
		}
	}

}

//void CScope::SetEnclosingScope(CScope* enclosingScope)
//{
//	this->m_enclosingScope = enclosingScope;
//}


void CScope::RenameSymbol(const QString& oldName, const QString& newName, CSymbol* symbol)
{
	auto iter = m_mapSymbol.find(oldName);

	/*若找到*/
	if (iter != m_mapSymbol.end())
	{
		delete(iter.value());
		m_mapSymbol.erase(iter);
		m_mapSymbol[newName] = symbol;
	}
}

void CScope::ClearChildren()
{
	for each (auto var in m_listScope)
	{
		delete(var);
	}
	m_listScope.clear();

}

void CScope::ClearSelf()
{
	for each (auto var in m_mapSymbol)
	{
		delete(var);
	}
	m_mapSymbol.clear();
}

CScope::CScope(const QString& name)
{
	this->m_scopeName = name;
}

CScope::CScope()
{

}

CScope::~CScope()
{
	ClearChildren();
	ClearSelf();
}
