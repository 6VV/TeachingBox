#ifndef _INTERPRETER_C_SCOPE_H_
#define _INTERPRETER_C_SCOPE_H_

#include "CSymbol.h"
#include <QList>
#include "QHash"

class CScope
{
public:
	typedef QHash<QString, CSymbol*> TYPE_MAP_SYMBOL;
	typedef QList<CScope*> TYPE_LIST_SCOPE;

	static const QString STR_SCOPE_SYSTEM;		/*系统作用域*/
	static const QString STR_SCOPE_SYNERGIC;	/*协作作用域*/
	static const QString STR_SCOPE_GLOBAL;		/*全局作用域*/
	static const QString STR_SCOPE_PROJECT;		/*项目作用域*/
	static const QString STR_SCOPE_LOCAL;		/*局部作用域*/

	struct ScopeSymbol 
	{
		CScope* scope=nullptr;
		CSymbol* symbol = nullptr;
	};
public:
	CScope(const QString& name);
	CScope();
	~CScope();

	void ClearChildren();	/*清除子作用域数据*/
	void ClearSelf();		/*清除自身作用域*/

	void DefineSymbol(CSymbol* sym);		/*定义作用域内符号*/
	bool DefineSymbol(const QString& desScope, CSymbol* symbol);
	void DeleteSymbol(const QString& strName);	/*从本作用域中删除符号*/

	CSymbol* FindSymbolScrollUp(const QString& strName);	/*在自身及上层作用域内查找符号*/
	CSymbol* FindSymbolInSelf(const QString& strName);	/*在自身作用域内查找符号*/
	CScope* FindScopeScrollDown(const QString& scope);	/*在本作用域及以下的所有作用域查找*/
	ScopeSymbol FindSymbolScopeScrollUp(const QString& strSymbolName);	/*查找符号所在作用域*/

	QString& GetScopeName();	/*获取当前作用域的名称*/
	CScope* GetEnclosingScope();	/*获取上层作用域*/

	void PushScope(CScope* scope);		/*添加子作用域*/

	void RenameSymbol(const QString& oldName, const QString& newName, CSymbol* symbol);	/*变量重命名*/

	void SetScopeName(const QString& strScopeName);	/*设置当前作用域名*/

protected:
	TYPE_MAP_SYMBOL m_mapSymbol;	/*本层作用域内符号*/
	TYPE_LIST_SCOPE m_listScope;	/*子作用域*/
	CScope* m_enclosingScope = nullptr;		/*上层作用域*/
	QString m_scopeName;		/*作用域名*/
};

#endif