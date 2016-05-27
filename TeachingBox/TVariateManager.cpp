#include "stdafx.h"
#include "TVariateManager.h"
#include "CWarningManager.h"
#include "QVector"
#include "CVariableSymbol.h"
#include "TVariateFactory.h"


TVariateManager* TVariateManager::GetInstance()
{
	return CSingleTon<TVariateManager>::GetInstance();
}

TVariateManager::TVariateManager()
{
	LoadInitDataFromDatabase();
}

TVariateManager::~TVariateManager()
{
	ClearMap();
	//CDatabaseManager::GetInstance()->DeleteAllVariate();
}

void TVariateManager::Add(TVariate* const object)
{
	AddInMemory(object);
	AddInDatabase(object);
}

void TVariateManager::AddInMemory(TVariate* const object)
{
	AddInMap(object);
	AddInScope(object);
}

void TVariateManager::AddInScope(TVariate* const object)
{
	CVariableSymbol* symbol = new CVariableSymbol(object->GetScope(),object->GetName(), object->GetType());
	CScope* scopeFound = m_scopeRoot.FindScopeScrollDown(object->GetScope());

	if (scopeFound)
	{
		scopeFound->DefineSymbol(object->GetScope(), symbol);
	}
	
}

void TVariateManager::AddInMap(TVariate* const object)
{
	m_objectMap[object->GetScope()][object->GetName()] = object;
}

void TVariateManager::ClearMemory()
{
	//CDatabaseManager::GetInstance()->DeleteAllVariate();
	ClearMap();
	ClearScope();
}

void TVariateManager::ClearProjectData()
{
	ClearMemory();
	LoadInitDataFromDatabase();
}

void TVariateManager::ClearMap()
{
	for (auto iterMap = m_objectMap.begin(); iterMap != m_objectMap.end(); iterMap++)
	{
		for (auto iterVar = iterMap.value().begin(); iterVar != iterMap.value().end(); iterVar++)
		{
			delete iterVar.value();
		}
	}
	m_objectMap.clear();
}

void TVariateManager::AddInDatabase(TVariate* const object)
{
	QByteArray data;
	QDataStream dataStream(&data,QIODevice::ReadWrite);
	object->ReadDataStream(dataStream);
	dataStream.device()->seek(0);
	CDatabaseManager::GetInstance()->InsertVariate(dataStream);
}

void TVariateManager::Delete(const QString& scope, const QString& name)
{
	DeleteFromMap(scope, name);
	DeleteFromDatabase(scope, name);
	DeleteFromScope(scope, name);
}


CScope& TVariateManager::GetRootScope()
{
	return m_scopeRoot;
}

void TVariateManager::DeleteFromMap(const QString& scope, const QString& name)
{
	delete m_objectMap.value(scope).value(name);
	m_objectMap[scope].remove(name);
}

void TVariateManager::DeleteFromDatabase(const QString& scope, const QString& name)
{
	CDatabaseManager::GetInstance()->DeleteVariate(scope, name);
}

void TVariateManager::DeleteFromScope(const QString& scope, const QString& name)
{
	if (CScope* scopeFound = m_scopeRoot.FindScopeScrollDown(scope))
	{
		scopeFound->DeleteSymbol(name);
	}
}

void TVariateManager::LoadInitDataFromDatabase()
{
	CScope* scope = &m_scopeRoot;

	for (auto var : CScope::SCOPE_ORI)
	{
		LoadScopeDataFromDatabase(var);
		CScope* newScope = new CScope(var);
		scope->PushScope(newScope);
		scope = new CScope;
	}
}

void TVariateManager::LoadScopeDataFromDatabase(const QString& scope)
{
	if (m_objectMap.find(scope)!=m_objectMap.end())
	{
		return;
	}

	QVector<QByteArray> variates;
	CDatabaseManager::GetInstance()->SelectVariatesFromScope(variates,scope);
	for (auto var : variates)
	{
		TVariate* v = TVariateFactory::CreateVariate(var);
		AddInMemory(v);
	}
}

TVariate* TVariateManager::GetVariate(const QString& scope, const QString& name)
{
	auto iterMap = m_objectMap.find(scope);
	if (iterMap==m_objectMap.end())
	{
		return nullptr;
	}
	auto iterVar = iterMap.value().find(name);
	if (iterVar==iterMap.value().end())
	{
		return nullptr;
	}
	return iterVar.value();
}

TVariate* TVariateManager::GetVariateSrollUp(const QString& scope, const QString& name)
{
	CScope* currentScope = m_scopeRoot.FindScopeScrollDown(scope);
	if (!currentScope)
	{
		return nullptr;
	}
	CSymbol* symbol = currentScope->FindSymbolScrollUp(name);
	if (!symbol)
	{
		return nullptr;
	}
	return GetVariate(symbol->GetScope(), symbol->GetName());
}

bool TVariateManager::IsExistVariateScrollUp(const QString& scope, const QString& name)
{
	if (GetVariateSrollUp(scope, name))
	{
		return true;
	}
	return false;
}

void TVariateManager::LoadProjectDataFromDatabase(const QString& project, const QStringList& programs)
{
	ClearProjectData();

	PushProjectScopes(project, programs);
	LoadScopeDataFromDatabase(project);

	for (auto program:programs)
	{
		LoadScopeDataFromDatabase(program);
	}
}

void TVariateManager::PushProjectScopes(const QString& project, const QStringList& programs)
{
	CScope* projectScope = new CScope(project);
	m_scopeRoot.FindScopeScrollDown(CScope::STR_SCOPE_GLOBAL)->PushScope(projectScope);

	for (auto program : programs)
	{
		projectScope->PushScope(new CScope(program));
	}
}

void TVariateManager::ClearScope()
{
	m_scopeRoot.ClearChildren();
}

void TVariateManager::GetCollection(TVariate::SET& collection, const QString& scope,
	CSymbol::SymbolType type)
{
	auto& varMap = m_objectMap[scope];
	for (auto var : varMap)
	{
		var->ReadCollection(collection, type);
	}
}

void TVariateManager::GetVariateItems(QTreeWidgetItem* const parentItem, QTreeWidget* const treeWidget, const QString& scope)
{
	for (auto type:CSymbol::Map_Type.keys())
	{
		GetVariateItemsByType(parentItem, treeWidget, scope, type);
	}
}

void TVariateManager::GetVariateItemsByType(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget,
	const QString scope, const CSymbol::SymbolType type)
{
	auto& varMap = m_objectMap[scope];
	for (auto var : varMap)
	{
		var->ReadTreeWidgetItem(parentItem, treeWidget, type);
	}
}

void TVariateManager::Update(const QString& scope, const QString& name, TVariate& newVariate)
{
	UpdateInMap(scope, name, newVariate);
	UpdateInDatabase(scope, name, newVariate);
	UpdateInScope(scope, name, newVariate);
}


void TVariateManager::UpdateInMap(const QString& scope, const QString& name, TVariate& newVariate)
{
	auto& iterMap = m_objectMap.find(scope);
	auto& iterVar = iterMap.value().find(name);

	auto ptrVariate = iterVar.value();
	ptrVariate->UpdateFromVariate(newVariate);

	iterMap.value().erase(iterVar);

	m_objectMap[scope][name] = ptrVariate;
}


void TVariateManager::UpdateInDatabase(const QString& scope, const QString& name, TVariate& newVariate)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	newVariate.ReadDataStream(dataStream);
	CDatabaseManager::GetInstance()->UpdateVariate(scope, name, dataStream);
}

void TVariateManager::UpdateInScope(const QString& scope, const QString& name, TVariate& newVariate)
{
	CVariableSymbol* symbol = new CVariableSymbol(scope, newVariate.GetName(), newVariate.GetType());
	CScope* scopeFound = m_scopeRoot.FindScopeScrollDown(scope);
	if (scopeFound)
	{
		scopeFound->RenameSymbol(name, newVariate.GetName(), symbol);
	}
}
