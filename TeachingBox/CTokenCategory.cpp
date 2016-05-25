#include "stdafx.h"
#include "CTokenCategory.h"

const CTokenCategory::OPERATOR CTokenCategory::g_operators[] = {
	{ operPlus, 7 },      // 加  
	{ operMinus, 7 },       // 减  
	{ operMultiply, 8 },      // 乘  
	{ operDivide, 8 },      // 除  
	{ operPower, 9 },      // 幂  
	//赋值运算符
	{ operAssignment, 2 },		//赋值
	// 关系运算
	{ operLessThan, 6 },			// 小于  
	{ operGreatThan, 6 },		// 大于  
	{ operEqual, 5 },			// 等于  
	{ operNotEqual, 5 },        // 不等于  
	{ operLessEqual, 6 },       // 不大于  
	{ operGreatEqual, 6 },         // 不小于  
	/* 逻辑运算 */
	{ operAnd, 4 },				  // 且  
	{ operOr, 3 },				  // 或  
	/*()*/
	{ operLeftBracket, 15 },		//左括号
	{ operRightBracket, 15 },	//右括号
	/*{}*/
	{ operLeftBrace, 15 },		//左大括号
	{ operRightBrace, 15 },		//右大括号
};

CTokenCategory::CTokenCategory()
{
	
	
}

CTokenCategory::~CTokenCategory()
{

}

