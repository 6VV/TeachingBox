#ifndef _INTERPRETER_C_VALUE_H_
#define _INTERPRETER_C_VALUE_H_

#include <map>
#include <vector>
#include <set>
#include <unordered_map>
#include "DataStruct.h"
#include "CScope.h"
class CValue
{
public:
	static const int MAX_STRING_LENGTH = 64;	/*变量最大长度*/

    enum
	{
		SCOPE_LOCAL,
		SCOPE_GLOBAL,
	};
public:
	static CValue* GetInstance()
	{
		return m_instance;
	}

public:
	//typedef struct
	//{
	//	int scope;
	//	int type;
	//}TYPE_SYMBOL;

	typedef tAxesAllPositions TYPE_POSITION;	/*位置类型*/
	typedef tDynamicConstraint TYPE_DYNAMIC;	/*动态类型*/
	typedef tOverlapConstraint TYPE_OVERLAP;	/*过度类型*/

	typedef tTransitionMode TYPE_TRANSITION_MODE;

	//typedef std::pair<std::string, TYPE_SYMBOL> TYPE_PAIR_SYMBOL;	/*符号表*/
	//typedef std::unordered_map<std::string, TYPE_SYMBOL> TYPE_MAP_SYMBOL;

	typedef std::unordered_map<std::string, TYPE_POSITION> TYPE_MAP_POSITION;
	typedef std::unordered_map<std::string,std::unordered_map<std::string, TYPE_POSITION>> TYPE_MAP_SCOPE_POSITION;		/*位置类型*/
	typedef std::pair<std::string, TYPE_POSITION> TYPE_PAIR_POSITION;

	typedef std::unordered_map<std::string, double> TYPE_MAP_DOUBLE;
	typedef std::pair<std::string, double> TYPE_PAIR_DOUBLE;

	typedef std::unordered_map<std::string, int> TYPE_MAP_INT;
	//typedef std::unordered_map<std::string, std::unordered_map<std::string, int>> TYPE_MAP_SCOPE_INT;	/*整数类型*/

	//typedef std::set<std::string> TYPE_LIST_LABEL;	/*标签类型*/

	typedef std::unordered_map<std::string, std::string> TYPE_MAP_STRING;
	typedef std::unordered_map<std::string, std::unordered_map<std::string, std::string>> TYPE_MAP_SCOPE_STRING;	/*字符串类型*/
	typedef std::pair<std::string, std::string> TYPE_PAIR_STRING;

	typedef std::unordered_map<std::string, TYPE_DYNAMIC> TYPE_MAP_DYNAMIC;
	typedef std::unordered_map<std::string, std::unordered_map<std::string, TYPE_DYNAMIC>> TYPE_MAP_SCOPE_DYNAMIC;
	typedef std::pair<std::string, TYPE_DYNAMIC> TYPE_PAIR_DYNAMIC;	/*动态参数*/

	typedef std::unordered_map<std::string, TYPE_OVERLAP> TYPE_MAP_OVERLAP;
	typedef std::unordered_map<std::string, std::unordered_map<std::string, TYPE_OVERLAP>> TYPE_MAP_SCOPE_OVERLAP;
	typedef  std::pair < std::string, TYPE_OVERLAP> TYPE_PAIR_OVERLAP;	/*过渡参数*/
public:
	std::unordered_map<std::string, int> m_mapCommandCode;	/*命令表*/
	std::unordered_map<std::string, int> m_mapLogicalCode;	/*逻辑表*/
	std::unordered_map<std::string, int> m_mapFunctionCode;	/*函数表*/

	//TYPE_MAP_SYMBOL m_mapSymbol;	/*符号表*/
	CScope m_scopeSystem;			/*系统作用域（根作用域）*/

	TYPE_MAP_SCOPE_POSITION m_mapScopePosition;	/*数据库中位置变量Map*/
	TYPE_MAP_SCOPE_DYNAMIC m_mapScopeDynamic;	/*数据库中速度变量Map*/
	TYPE_MAP_SCOPE_OVERLAP m_mapScopeOverlap;	/*数据库中过渡变量Map*/
	TYPE_MAP_SCOPE_STRING m_mapScopeString;	/*数据库中String类型*/

private:
	CValue();
	CValue(const CValue&);
	CValue& operator=(const CValue&);
	~CValue();

	void Init();

private:
	static CValue* m_instance;

	/*释放内存*/
	class CGarbo
	{
	public:
		CGarbo();
		~CGarbo()
		{
			if (CValue::m_instance)
			{
				delete CValue::m_instance;
				CValue::m_instance = NULL;
			}
		}
	};
	static CGarbo m_garbo;
};

#endif
