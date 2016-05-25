#ifndef _INTERPRETER_C_TOKEN_CATEGORY_H_
#define _INTERPRETER_C_TOKEN_CATEGORY_H_

#include <string>

class CTokenCategory
{

public:
	/*常量*/
	typedef enum{
		constantInt,	//整型常量
		constantFloat,		//浮点型常量
		constantString,		//字符串常量
		constantBool,		//布尔常量
		constantLable,		//标签
		constantIO,			//IO
	}CATEGORY_CONSTANT;

	typedef union{
        int conInt;
		double conFloat;
		bool conBool;
	}VALUE_CONSTANT_NUMBER;

	/*变量*/
	typedef enum{
		varInt,		//整形变量
		varFloat,		//浮点型变量
		varChar,		//字节变量
		varPosition,	//位置变量
		varUniversal,	//通用变量
		varBool,		//布尔变量
		varV,			//速度变量
		varZ,			//过渡变量
	}CATEGORY_VARIANT;

	/*运算符*/
	typedef enum{
		//算数运算符
		operPlus,        // 加  
		operMinus,         // 减  
		operMultiply,      // 乘  
		operDivide,        // 除  
		operPower,         // 幂  
		//赋值运算符
		operAssignment ,		//赋值
		// 关系运算
		operLessThan,			// 小于  
		operGreatThan,			// 大于  
		operEqual,				// 等于  
		operNotEqual,           // 不等于  
		operLessEqual ,          // 不大于  
		operGreatEqual,         // 不小于  
		/* 逻辑运算 */
		operAnd,				  // 且  
		operOr,					  // 或  
		/*()*/
		operLeftBracket,		//左括号
		operRightBracket ,		//右括号
		/*{}*/
		operLeftBrace,			//左大括号
		operRightBrace,			//右大括号
	}CATEGORY_OPERATOR;

	//结合性
	typedef enum{
		leftToRight,	//自左向右结合
		rightToleft		//自右向左结合
	}ASSOCIATIVITY;

	typedef struct{
		//String name;
		CATEGORY_OPERATOR type;
		//	associativity ass;
		int priority;
	}OPERATOR;

	const static OPERATOR g_operators[];
		
	/*分隔符*/
	typedef enum{
		sepSpace,		//空格
		sepLineBreak,	//换行
		sepComma,		//逗号
		sepEnd,			//文本结束
		sepQuote,	//左引号
	}CATEGORY_SEPARATOR;

	/*命令*/
	typedef enum{
		/*流程控制*/
		commFor,		//有限循环语句，对变量赋初始值		for 变量=表达式1 to 表达式2 step 表达式3  语句  next
		commTo,			//有限循环的终止值			
		commStep,		//有限循环中变量增量
		commNext,		//结束有限循环
		commIf,			//与then配合，构建条件转移语句		if  逻辑表达式	then	语句
		commThen,		//与if配合，构建条件转移语句
		commElse,
		commElseIf,
		commEndif,
		commSwitch,
		commCase,
		commBreak,
		commDefault,
		commEndSwitch,
		commNot,
		commInkey,
		/*程序控制*/
		commGoto,		//与标签配合，构建无条件转移语句	goto 标签
		commGosub,		//与标签配合，调用子程序语句，遇return返回		gosub 标签
		commReturn,		//子程序结束并返回
		commAbort,		//无条件终止整个程序运行
		commHome,
		commEnd,		//程序结束
		commEof,		//文档结束
		commMain,		//主程序开始
		commFunction,	//子程序开始
		/*运动规划*/
		commMovl,		//直线运动		
		commMovj,		//关节运动
		commMovc,		//圆弧运动
		commMovs,		//样条运动
		commMovjx,
		commMovx,
		commMovy,
		commMovz,
		commRotx,
		commRoty,
		commRotz,
		/*输入输出*/
		commInput,		//终端输入
		commPrint,		//终端输出
		commBprint,
		commIprint,
		commPprint,
		commDin,		//数字量输入
		commDout,		//数字量输出
		commAin,		//模拟量输入
		commAout,		//模拟量输出
		/*其它*/
		commDwell,		//延时
		commWait,		//同步信号
		/*弧焊作业*/
		commArc,		//起弧或停弧	acr 表达式1,表达式2	或	arc off
		commSpot,		//启动或停止电焊
		commOn,			//与spot配合，启动电焊	spot once
		commOff,		//与arc或spot配合，停弧或停止电焊

		/*参数设置*/
		commSet,		//与arc、spot、userframe、toolframe配合使用
		commUserFrame,
		commToolFrame,
		commSetGroupOverride,
		commSetAxisOverride,

		commLabel,		//标签
		commRem,		//注释

		commClc,			//清空控制台，测试用命令
	}CATEGORY_COMMAND;

	/*函数*/
	typedef enum{
		funSin,
		funCos,
		funTan,
		funSqrt
	}CATEGORY_FUNCTION;

	//词法单元
	typedef enum{
		tokenRoot,
		tokenProgram,
		tokenConstantNumber,	//常数
		tokenConstantString,	//常字符串
		tokenVariant,			//变量
		tokenOperator,			//运算符
		tokenSeparator,		//分隔符
		tokenCommand,			//命令
		tokenFunction ,			//函数
		tokenNote,				//注释
	}CATEGORY_TOKEN;


private:
	CTokenCategory();
	CTokenCategory(const CTokenCategory&);
	CTokenCategory& operator=(const CTokenCategory&);
	~CTokenCategory();
};

#endif
