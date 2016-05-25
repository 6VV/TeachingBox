#include "stdafx.h"
#include "CValue.h"
#include "CTokenCategory.h"

CValue::CValue()
{
	Init();
}

CValue::~CValue()
{
}

void CValue::Init()
{
	/*命令相关*/
	m_mapCommandCode["IF"] = CTokenCategory::commIf;
	m_mapCommandCode["THEN"] = CTokenCategory::commThen;
	m_mapCommandCode["ELSE"] = CTokenCategory::commElse;
	m_mapCommandCode["ELSEIF"] = CTokenCategory::commElseIf;
	m_mapCommandCode["ENDIF"] = CTokenCategory::commEndif;
	m_mapCommandCode["GOTO"] = CTokenCategory::commGoto;
	m_mapCommandCode["FOR"] = CTokenCategory::commFor;
	m_mapCommandCode["NEXT"] = CTokenCategory::commNext;
	m_mapCommandCode["TO"] = CTokenCategory::commTo;
	m_mapCommandCode["GOSUB"] = CTokenCategory::commGosub;
	m_mapCommandCode["RETURN"] = CTokenCategory::commReturn;
	m_mapCommandCode["ABORT"] = CTokenCategory::commAbort;
	m_mapCommandCode["SWITCH"] = CTokenCategory::commSwitch;
	m_mapCommandCode["GOSUB"] = CTokenCategory::commGosub;
	m_mapCommandCode["CASE"] = CTokenCategory::commCase;
	m_mapCommandCode["BREAK"] = CTokenCategory::commBreak;
	m_mapCommandCode["DEFAULT"] = CTokenCategory::commDefault;
	m_mapCommandCode["ENDSWITCH"] = CTokenCategory::commEndSwitch;
	m_mapCommandCode["END"] = CTokenCategory::commEnd;
	m_mapCommandCode["NOT"] = CTokenCategory::commNot;
	m_mapCommandCode["STEP"] = CTokenCategory::commStep;
	m_mapCommandCode["REM"] = CTokenCategory::commRem;
	m_mapCommandCode["EOF"] = CTokenCategory::commEof;
	m_mapCommandCode["MAIN"] = CTokenCategory::commMain;
	m_mapCommandCode["FUNCTION"] = CTokenCategory::commFunction;
	m_mapCommandCode["LABEL"] = CTokenCategory::commLabel;

	m_mapCommandCode["INKEY"] = CTokenCategory::commInkey;
	m_mapCommandCode["PRINT"] = CTokenCategory::commPrint;
	m_mapCommandCode["BPRINT"] = CTokenCategory::commBprint;
	m_mapCommandCode["IPRINT"] = CTokenCategory::commIprint;
	m_mapCommandCode["PPRINT"] = CTokenCategory::commPprint;
	m_mapCommandCode["INPUT"] = CTokenCategory::commInput;
	m_mapCommandCode["MOVL"] = CTokenCategory::commMovl;
	m_mapCommandCode["MOVJ"] = CTokenCategory::commMovj;
	m_mapCommandCode["MOVJX"] = CTokenCategory::commMovjx;
	m_mapCommandCode["MOVC"] = CTokenCategory::commMovc;
	m_mapCommandCode["MOVX"] = CTokenCategory::commMovx;
	m_mapCommandCode["MOVY"] = CTokenCategory::commMovy;
	m_mapCommandCode["MOVZ"] = CTokenCategory::commMovz;
	m_mapCommandCode["ROTX"] = CTokenCategory::commRotx;
	m_mapCommandCode["ROTY"] = CTokenCategory::commRoty;
	m_mapCommandCode["ROTZ"] = CTokenCategory::commRotz;
	m_mapCommandCode["WAIT"] = CTokenCategory::commWait;
	m_mapCommandCode["DWELL"] = CTokenCategory::commDwell;
	m_mapCommandCode["HOME"] = CTokenCategory::commHome;
	m_mapCommandCode["DIIN"] = CTokenCategory::commDin;
	m_mapCommandCode["DOUT"] = CTokenCategory::commDout;
	m_mapCommandCode["AIN"] = CTokenCategory::commAin;
	m_mapCommandCode["AOUT"] = CTokenCategory::commAout;
	m_mapCommandCode["DIN"] = CTokenCategory::commDin;
	m_mapCommandCode["DOUT"] = CTokenCategory::commDout;
	m_mapCommandCode["ARC"] = CTokenCategory::commArc;
	m_mapCommandCode["SPOT"] = CTokenCategory::commSpot;
	m_mapCommandCode["SET"] = CTokenCategory::commSet;
	m_mapCommandCode["ON"] = CTokenCategory::commOn;
	m_mapCommandCode["OFF"] = CTokenCategory::commOff;
	m_mapCommandCode["CLC"] = CTokenCategory::commClc;
	m_mapCommandCode["USERFRAME"] = CTokenCategory::commUserFrame;
	m_mapCommandCode["TOOLFRAME"] = CTokenCategory::commToolFrame;
	m_mapCommandCode["SETGROUPOVERRIDE"] = CTokenCategory::commSetGroupOverride;
	m_mapCommandCode["SETAXISOVERRIDE"] = CTokenCategory::commSetAxisOverride;

	/*逻辑相关*/
	m_mapLogicalCode["OR"] = CTokenCategory::operOr;
	m_mapLogicalCode["AND"] = CTokenCategory::operAnd;
	
	/*函数相关*/
	m_mapFunctionCode["SIN"] = CTokenCategory::funSin;
	m_mapFunctionCode["COS"] = CTokenCategory::funCos;
	m_mapFunctionCode["TAN"] = CTokenCategory::funTan;
	m_mapFunctionCode["SQRT"] = CTokenCategory::funSqrt;

}

CValue* CValue::m_instance = new CValue();
