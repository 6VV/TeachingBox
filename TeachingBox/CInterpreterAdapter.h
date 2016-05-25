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
	//bool IsExist(const QString& strScope, const  std::string& strName);	/*查找变量是否存在*/
	//bool IsExist(CScope* scope, const std::string& strName);	/*在作用域内查找字符是否存在*/
	//bool IsSymbolExist(const QString& strScopeName, const  std::string& strName);

	/*位置变量相关*/
	void GetPositionListFromEnclosingScope(QStringList &strListPositions,const QString& scope);	/*获取位置变量列表*/
	void InsertPositionValue(const QString& scope, CValue::TYPE_PAIR_POSITION &pairNewPosition);		/*插入位置变量*/
	void UpdatePositionValue(const QString& scope, std::string &strOldPosition, CValue::TYPE_PAIR_POSITION &pairNewPosition);	/*更新位置变量*/
	void UpdatePositionValue(const QString& scope, std::string &strOldPosition, std::string &strNewPosition,CValue::TYPE_POSITION &pairNewPosition);	/*更新位置变量*/
	void DeletePositionValue(const QString& strScope, std::string& strName);
	void UpdatePositionValueFromDatabase(const QString& strScope);	/*获取作用域的位置变量*/
	bool GetPositionValue(const QString& scope, const std::string& strPositionName, CValue::TYPE_POSITION& vecPositionValue);	/*获取位置变量*/
	bool GetPositionValueFromEnclosingScope(const QString& scope, const std::string& strPositionName, CValue::TYPE_POSITION& vecPositionValue);	/*获取位置变量*/
	bool IsPositionValueExist(const QString& strScope, const std::string &strPositionName);	/*位置变量是否存在*/

	/*速度变量相关*/
	void GetDynamicListFromEnclosingScope(QStringList &strListPositions, const QString& scope);	/*获取位置变量列表*/
	void InsertDynamicValue(const QString& scope, CValue::TYPE_PAIR_DYNAMIC& pairNewVelocity);
	void UpdateDynamicValue(const QString& scope, std::string& strOldVelocity, CValue::TYPE_PAIR_DYNAMIC& pairNewVelocity);
	void UpdateDynamicValue(const QString& scope, std::string& strOldVelocity, std::string& strNewVelocity,CValue::TYPE_DYNAMIC& dynamic);
	void DeleteDynamicValue(const QString& scope, std::string& strName);
	void UpdateDynamicValueFromDatabase(const QString& strScope);	/*获取作用域的动态变量*/
	bool GetDynamicValue(const QString& strScope, const std::string& strName, CValue::TYPE_DYNAMIC& velocityValue);
	bool GetDynamicValueFromEnclosingScope(const QString& scope, const std::string& strName, CValue::TYPE_DYNAMIC& vecValue);	/*获取位置变量*/
	bool IsDynamicValueExist(const QString& strScope, const std::string& strName);

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


	/*布尔变量相关*/
	void GetBoolListFromEnclosingScope(QStringList &strListPositions, const QString& scope);	/*获取位置变量列表*/
	void InsertBoolValue(const QString& scope, CValue::TYPE_PAIR_BOOL& pairNewBool);
	void UpdateBoolValue(const QString& scope, const std::string& strOldName, CValue::TYPE_PAIR_BOOL& pairNew);
	void UpdateBoolValue(const QString& scope, const std::string& strOldName, const std::string& strNewName, int value);
	void DeleteBoolValue(const QString& strScope, std::string& strName);
	void UpdateBoolValueFromDatabase(const QString& strScope);

	bool GetBoolValue(const QString& strScope, const std::string& strName, int& value);
	bool GetBoolValueFromEnclosingScope(const QString& scope, const std::string& strName, int& value);	/*获取位置变量*/
	bool IsBoolValueExist(const QString& strScope, const std::string& strName);

	/*字符串变量相关*/
	void GetStringListFromEnclosingScope(QStringList &strListPositions, const QString& scope);	/*获取位置变量列表*/

	void InsertStringValue(const QString& scope, CValue::TYPE_PAIR_STRING& pairNewString);
	void UpdateStringValue(const QString& scope, const std::string& strOldSName, CValue::TYPE_PAIR_STRING& pairNew);
	void UpdateStringValue(const QString& scope, const std::string& strOldName, const std::string& strNewName,std::string& value );
	void DeleteStringValue(const QString& strScope, std::string& strName);
	void UpdateStringValueFromDatabase(const QString& strScope);
	bool GetStringValue(const QString& strScope, const std::string& strName, std::string& value);
	bool GetStringValueFromEnclosingScope(const QString& scope, const std::string& strName, std::string& value);	/*获取位置变量*/
	bool IsStringValueExist(const QString& strScope, const std::string& strStringName);
	/*私有变量*/
private:
	CDatabaseManager* m_databaseManager=NULL;	/*数据库管理器*/
	CValue* m_value;	/*解释器数值*/
	CScope* m_scopeSynergic=NULL;	/*协作作用域*/
	CScope* m_scopeGlobal=NULL;		/*全局作用域*/

};

#endif
