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
	typedef tAxesAllPositions TYPE_POSITION;	/*位置类型*/
	typedef tDynamicConstraint TYPE_DYNAMIC;	/*动态类型*/
	typedef tOverlapConstraint TYPE_OVERLAP;	/*过度类型*/

	typedef tTransitionMode TYPE_TRANSITION_MODE;

	typedef std::unordered_map<std::string, TYPE_OVERLAP> TYPE_MAP_OVERLAP;
	typedef std::unordered_map<std::string, std::unordered_map<std::string, TYPE_OVERLAP>> TYPE_MAP_SCOPE_OVERLAP;
	typedef  std::pair < std::string, TYPE_OVERLAP> TYPE_PAIR_OVERLAP;	/*过渡参数*/
public:
	std::unordered_map<std::string, int> m_mapCommandCode;	/*命令表*/
	std::unordered_map<std::string, int> m_mapLogicalCode;	/*逻辑表*/
	std::unordered_map<std::string, int> m_mapFunctionCode;	/*函数表*/

	//TYPE_MAP_SYMBOL m_mapSymbol;	/*符号表*/
	CScope m_scopeSystem;			/*系统作用域（根作用域）*/

	TYPE_MAP_SCOPE_OVERLAP m_mapScopeOverlap;	/*数据库中过渡变量Map*/

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
