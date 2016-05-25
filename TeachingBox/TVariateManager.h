#ifndef _TEACHING_BOX_TOBJECT_MANAGER_H_
#define _TEACHING_BOX_TOBJECT_MANAGER_H_

#include "TVariate.h"
#include "CDatabaseManager.h"
#include "QObject"
#include "QHash"
#include "QSet"
#include "CSingleTon.h"
#include "CScope.h"
#include "QStringList"

class TVariateManager:public QObject
{
	friend CSingleTon<TVariateManager>;

	Q_OBJECT

private:
	typedef QHash<QString, QHash<QString, TVariate*>> MAP;

public:
	static TVariateManager* GetInstance();

private:
	TVariateManager();
	TVariateManager(const TVariateManager&);
	TVariateManager& operator=(const TVariateManager&);
	~TVariateManager();

public:
	void Add(TVariate* const object);

	void ClearProjectData();

	void Delete(const QString& scope, const QString& name);

	CScope& GetRootScope();
	TVariate* GetVariate(const QString& scope, const QString& name);
	TVariate* GetVariateSrollUp(const QString& scope, const QString& name);

	bool IsExistVariateScrollUp(const QString& scope, const QString& name);

	void LoadProjectDataFromDatabase(const QString& project, const QStringList& programs);

	void ReadCollection(TVariate::SET& collection, const QString& scope,
		CSymbol::SymbolType type);
	void ReadTreeItemCollection(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget,
		const QString scope, const CSymbol::SymbolType type);

	void Update(const QString& scope, const QString& name, TVariate& newVariate);

private:
	void AddInMemory(TVariate* const object);
	void AddInMap(TVariate* const object);
	void AddInScope(TVariate* const object);
	void AddInDatabase(TVariate* const object);

	void ClearMemory();
	void ClearMap();
	void ClearScope();

	void DeleteFromMap(const QString& scope, const QString& name);
	void DeleteFromDatabase(const QString& scope, const QString& name);
	void DeleteFromScope(const QString& scope, const QString& name);

	void InitScope();

	void LoadInitDataFromDatabase();
	void LoadScopeDataFromDatabase(const QString& scope);

	void PushProjectScopes(const QString& project, const QStringList& programs);

	void UpdateInMap(const QString& scope, const QString& name, TVariate& newVariate);
	void UpdateInScope(const QString& scope, const QString& name, TVariate& newVariate);
	void UpdateInDatabase(const QString& scope, const QString& name, TVariate& newVariate);

private:
	MAP m_objectMap;
	CScope m_scopeRoot;
};

#endif