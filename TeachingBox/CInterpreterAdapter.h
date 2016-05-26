#ifndef _TEACHING_BOX_C_PARAMETER_H_
#define _TEACHING_BOX_C_PARAMETER_H_

#include "CSingleTon.h"
#include "QList"
#include "CDatabaseManager.h"
#include "CValue.h"
#include "QThread"
#include "CThreadInterpreterManager.h"
#include "CInterpreterManager.h"

class CInterpreterAdapter
{
	friend class CSingleTon<CInterpreterAdapter>;

public:
	static const int MAX_STRING_LENGTH = CValue::MAX_STRING_LENGTH;	/*变量最大长度*/

private:
	CInterpreterAdapter();
	CInterpreterAdapter(const CInterpreterAdapter&);
	CInterpreterAdapter& operator=(const CInterpreterAdapter&);
	~CInterpreterAdapter();

	void Init();		/*初始化*/
	void InitScope();	/*初始化作用域*/

	std::vector<CScope*> GetEnclosingScope(const std::string& currentScope);
	/*公有函数*/
public:
	static CInterpreterAdapter* GetInstance();
	//void StartInterpreterFromLine(std::string& strContext, int lineNumber);	/*从某一行开始执行*/
	//void StopInterpreter();	/*停止执行*/

	//////////////////////////////////////////////////////////////////////////
	/*增添变量时需要修改的函数*/
	//////////////////////////////////////////////////////////////////////////
	void UpdateValueFromDatabase(const QString& strScope);
	void UpdateVariableName(const QString& strOldName, const QString& strNewName, const QString& strScope,const QString& strType);
	//void DeleteVariable(const QString& strName, const QString& strType, const QString& strScope);
	void ClearProjectValue();	/*清除项目内的所有数据*/
	void DeleteDatabaseData(const QString& strScope);	/*清除数据库数据*/
	//////////////////////////////////////////////////////////////////////////

	void UpdateValueFromDatabase();	/*从数据库中更新数据*/

	bool IsCorrectName(std::string& strName);	/*是否是正确的变量名*/

	/*过渡变量相关*/
	void GetOverlapListFromEnclosingScope(QStringList &strListPositions, const QString& scope);	/*获取位置变量列表*/
	void InsertOverlapValue(const QString& scope, CValue::TYPE_PAIR_OVERLAP& pairNewZone);
	void UpdateOverlapValue(const QString& scope, const std::string& strOldZone, CValue::TYPE_PAIR_OVERLAP& pairNewZone);
	void UpdateOverlapValue(const QString& scope, const std::string& strOldZone, const std::string& strNewZone, CValue::TYPE_OVERLAP& overlap);
	void DeleteOverlapValue(const QString& scope, std::string& strName);
	void UpdateOverlapValueFromDatabase(const QString& strScope);
	bool GetOverlapValue(const QString& strScope, const std::string& strName, CValue::TYPE_OVERLAP& value);
	bool GetOverlapValueFromEnclosingScope(const QString& scope, const std::string& strName, CValue::TYPE_OVERLAP& vecValue);	/*获取位置变量*/
	bool IsOverlapValueExist(const QString& strScope, const std::string& strName);

	/*私有变量*/
private:
	CDatabaseManager* m_databaseManager=NULL;	/*数据库管理器*/
	CValue* m_value;	/*解释器数值*/
	CScope* m_scopeSynergic=NULL;	/*协作作用域*/
	CScope* m_scopeGlobal=NULL;		/*全局作用域*/

};

#endif
