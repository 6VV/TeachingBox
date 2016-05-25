#include "stdafx.h"
#include "CLexer.h"
#include "CTokenCategory.h"
#include <string>
#include "CValue.h"
#include "CInterpreterManager.h"
#include "CTokenConstantNum.h"
#include "CTokenString.h"
#include "CExceptionInterpreter.h"
#include "CWarningInformation.h"


CLexer::CLexer(const std::string& strContext)
{
	m_pStrContext = &strContext;
	//m_pStrContext->push_back('\0');

	Init();

	//for each (auto var in *m_pListToken)
	//{
	//	DisplayToken(var);
	//}
}

CLexer::~CLexer()
{
	/*释放词法单元*/
	for each (auto var in *m_pListToken)
	{
		delete(var);
	}

	m_pListToken->clear();
	delete(m_pListToken);
}


std::vector<CToken*>* CLexer::GetTokenList()
{
	return m_pListToken;
}

/*************************************************
//  Function: 		GetTokens
//  Description: 	获取文本中的词法单元
//  Calls:		 	GetNextToken()
//  Called By: 		CInterpreterBuilder::Lexer(std::string&)
//  Parameter:      std::list<CToken*>&
//  Return: 		int
//  Others: 		若未出现错误，返回零；若出现错误，返回错误代码行数以及错误字符串
*************************************************/
//void CLexer::GetTokens(std::vector<CToken*>& tokenList)
//{
//	m_pListToken = &tokenList;
//
//	m_currentLine = 1;
//	m_currentPointer = 0;
//	m_currentChar = (*m_pStrContext)[m_currentPointer];
//	int contextLength = m_pStrContext->size();
//	CToken* currentToken;
//	/*获取词法单元并存储*/
//	while (GetNextToken())
//	{
//	}
//
//	///*若未到达文本尾，则提示错误*/
//	//if (m_currentPointer < contextLength - 1)
//	//{
//	//	
//	//}
//	//for each (auto var in *m_pListToken)
//	//{
//	//	DisplayToken(var);
//	//}
//}

bool CLexer::GetNextToken()
{
	SkipBlank();
	switch (m_currentChar)
	{
		/*分割符*/
	case '\n':
	{
		if (m_pListToken->size()>0)
		{
			CToken* tokenBack = m_pListToken->back();
			if (tokenBack->m_largeCategory != CTokenCategory::tokenSeparator || tokenBack->m_smallCategory != CTokenCategory::sepLineBreak)
			{
				m_pListToken->push_back(new CToken(CTokenCategory::tokenSeparator, CTokenCategory::sepLineBreak, m_currentLine));
			}
		}
		
		m_currentLine++;
		Consume();
		return true;
	}break;
	case ',':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenSeparator, CTokenCategory::sepComma, m_currentLine));
		Consume();
		return true;
	}break;
	case '\0':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenSeparator, CTokenCategory::sepEnd, m_currentLine));
		//Consume();
		return false;
	}break;
	case '\"':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenSeparator, CTokenCategory::sepQuote, m_currentLine));
		Consume();
		return GetString();
	}break;
	/*数学运算符*/
	case '+':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operPlus, m_currentLine));
		Consume();
		return true;
	}break;
	case '-':
	{
		/*判断上一个词法单元是否是数值或变量*/
		CToken* token = m_pListToken->back();
		if (token->m_largeCategory==CTokenCategory::tokenConstantNumber || token->m_largeCategory==CTokenCategory::tokenVariant)
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operMinus, m_currentLine));
		}
		else
		{
			GetNextToken();
			/*若下一个词法单元为数值*/
			if (m_pListToken->back()->m_largeCategory==CTokenCategory::tokenConstantNumber)
			{
				CTokenConstantNum* tokenNum = static_cast<CTokenConstantNum*>(m_pListToken->back());
				if (tokenNum->m_smallCategory==CTokenCategory::constantInt)
				{
					tokenNum->m_value.value_int *= (-1);
				}
				else if (tokenNum->m_smallCategory == CTokenCategory::constantFloat)
				{
					tokenNum->m_value.value_double *= (-1);
				}
				else
				{
					throw CExceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, std::string("-"));
				}
			}
			else
			{
				throw CExceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, std::string("-"));
			}
		}

		Consume();
		return true;
	}break;
	case '*':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operMultiply, m_currentLine));
		Consume();
		return true;
	}break;
	case '/':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operDivide, m_currentLine));
		Consume();
		return true;
	}break;
	case '^':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operPower, m_currentLine));
		Consume();
		return true;
	}break;
	case '=':{
		m_currentPointer++;
		if ((*m_pStrContext)[m_currentPointer] == '=')
		{
			m_currentChar = m_pStrContext->at(m_currentPointer);
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operEqual, m_currentLine));
			Consume();
			return true;
		}
		else
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operAssignment, m_currentLine));
			m_currentChar = (*m_pStrContext)[m_currentPointer];
			return true;
		}
	}break;
		/*比较运算符*/
	case '>':
	{
		m_currentPointer++;
		if ((*m_pStrContext)[m_currentPointer] == '=')
		{
			m_currentChar = m_pStrContext->at(m_currentPointer);
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operGreatEqual, m_currentLine));
			Consume();
			return true;
		}
		else
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operGreatThan, m_currentLine));
			m_currentChar = (*m_pStrContext)[m_currentPointer];
			return true;
		}
	}break;
	case '<':
	{
		m_currentPointer++;
		if (m_pStrContext->at(m_currentPointer) == '=')
		{
			m_currentChar = m_pStrContext->at(m_currentPointer);
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operLessEqual, m_currentLine));
			Consume();
			return true;
		}
		else
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operLessThan, m_currentLine));
			m_currentChar = (*m_pStrContext)[m_currentPointer];
			return true;
		}
	}break;
	case '!':
	{
		Consume();
		switch (m_currentChar)
		{
		case '>':
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operLessEqual, m_currentLine));
			Consume();
			return true;
		}break;
		case '<':
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operGreatEqual, m_currentLine));
			Consume();
			return true;
		}break;
		case '=':
		{
			m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operNotEqual, m_currentLine));
			Consume();
			return true;
		}break;
		default:
		{
			CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, std::string("!"));
			throw(exceptionInterpreter);
			return false;
		}break;
		}
	}break;
	/*逻辑运算符*/
	case '&':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operAnd, m_currentLine));
		Consume();
		return true;
	}break;
	case '|':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operOr, m_currentLine));
		Consume();
		return true;
	}break;
	/*括号*/
	case '(':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operLeftBracket, m_currentLine));
		Consume();
		return true;
	}break;
	case ')':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operRightBracket, m_currentLine));
		Consume();
		return true;
	}break;
	case '{':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operLeftBrace, m_currentLine));
		Consume();
		return true;
	}break;
	case '}':
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, CTokenCategory::operRightBrace, m_currentLine));
		Consume();
		return true;
	}break;
	/*字符串*/
	default:
	{
		std::string str;

		/*获取字符串*/
		do
		{
			str.push_back(m_currentChar);
			Consume();
		} while (!IsSeparator(m_currentChar));

		/*若为数字*/
		if (IsNumber(str.at(0)))
		{
			//do
			//{
			//	str.push_back(m_currentChar);
			//	Consume();
			//} while (IsNumberOrPoint(m_currentChar));

			/*判断是否为数字*/
			int result = GetNumber(str);
			if (result == NUMBER_INT)
			{
				m_pListToken->push_back(new CTokenConstantNum(CTokenCategory::tokenConstantNumber, CTokenCategory::constantInt, m_currentLine, atoi(str.c_str())));
				return true;
			}
			else if (result == NUMBER_FLOAT)
			{
				m_pListToken->push_back(new CTokenConstantNum(CTokenCategory::tokenConstantNumber, CTokenCategory::constantFloat, m_currentLine, atof(str.c_str())));
				return true;
			}
			else
			{
				CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, str);
				throw(exceptionInterpreter);
				return false;
			}

		}

		/*获取字符串*/
		else
		{
			//do
			//{
			//	str.push_back(m_currentChar);
			//	Consume();
			//} while (!IsSeparator(m_currentChar));

			if (str.length() > 0)
			{
				return GetOtherToken(str);
			}
			/*若不属于任何字符*/
			else
			{
				CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, str);
				throw(exceptionInterpreter);
				return false;
			}
		}
	}break;
	}
}

/*跳过空白字符*/
void CLexer::SkipBlank()
{
	while (m_currentChar == ' ' || m_currentChar == '\t' ||m_currentChar=='\r')
	{
		Consume();
	}
}

void CLexer::Consume()
{
	m_currentPointer++;
	m_currentChar = (*m_pStrContext)[m_currentPointer];
}

void CLexer::Backspace()
{
	m_currentPointer--;
	m_currentChar = (*m_pStrContext)[m_currentPointer];
}

bool CLexer::IsSeparator(char currentChar)
{
	return !((currentChar >= '0' && currentChar <= '9') ||
		(currentChar >= 'a' && currentChar <= 'z') ||
		(currentChar >= 'A' && currentChar <= 'Z') ||
		currentChar=='.');
}

bool CLexer::IsNumber(char currentChar)
{
	return (currentChar >= '0' && currentChar <= '9');
}

bool CLexer::IsNumberOrPoint(char currentChar)
{
	return ((currentChar >= '0' && currentChar <= '9') || currentChar == '.');
}

int CLexer::GetNumber(std::string& str)
{
	int pointNum = 0;
	for each (auto var in str)
	{
		if (var == '.')
		{
			pointNum++;
		}
		else if (var<'0' || var>'9')
		{
			return 0;
		}
	}
	if (pointNum == 0)
	{
		return NUMBER_INT;
	}
	else if (pointNum == 1)
	{
		return NUMBER_FLOAT;
	}
	else
	{
		return 0;
	}
}

/*获取引号内的字符串*/
bool CLexer::GetString()
{
	std::string str;
	while (m_currentChar != '\"' && m_currentChar != '\0' && m_currentChar!='\n')
	{
		str.push_back(m_currentChar);
		Consume();
	}
	/*若未匹配*/
	if (m_currentChar !='\"')
	{
		//qDebug() << QString::fromStdString(str);
		CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN,m_currentLine,str);
		throw (exceptionInterpreter);
		return false;
	}
	else
	{
		m_pListToken->push_back(new CTokenString(CTokenCategory::tokenConstantString, CTokenCategory::constantString, m_currentLine, str));
		m_pListToken->push_back(new CToken(CTokenCategory::tokenSeparator, CTokenCategory::sepQuote, m_currentLine));
		Consume();
		return true;
	}
}

bool CLexer::GetOtherToken(std::string& str)
{
	CValue* value = CValue::GetInstance();

	/*std::string strLower;
	for (int i = 0; i < str.length(); i++)
	{
	strLower.push_back(tolower(str[i]));
	}*/
	

	/*判断是否为命令*/
	auto iter = value->m_mapCommandCode.find(str);
	if (iter != value->m_mapCommandCode.end())
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenCommand, iter->second, m_currentLine));

		/*若为注释*/
		if (iter->second == CTokenCategory::commRem)
		{
			do
			{
				Consume();
			} while (m_currentChar != '\n' && m_currentChar != '\0');
		}

		return true;
	}

	/*判断是否为函数*/
	iter = value->m_mapFunctionCode.find(str);
	if (iter != value->m_mapFunctionCode.end())
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenFunction, iter->second, m_currentLine));
		return true;
	}

	/*判断是否为逻辑操作符*/
	iter = value->m_mapLogicalCode.find(str);
	if (iter != value->m_mapLogicalCode.end())
	{
		m_pListToken->push_back(new CToken(CTokenCategory::tokenOperator, iter->second, m_currentLine));
		return true;
	}

	/*判断是否为True或False*/
	if (str == "TRUE")
	{
		m_pListToken->push_back(new CTokenConstantNum(CTokenCategory::tokenConstantNumber,CTokenCategory::constantBool,m_currentLine,true));
		return true;
	}
	else if (str == "FALSE")
	{
		m_pListToken->push_back(new CTokenConstantNum(CTokenCategory::tokenConstantNumber, CTokenCategory::constantBool, m_currentLine, false));
		return true;
	}

	///*是否区分大小写*/
	//if (CInterpreterManager::m_matchState == CInterpreterManager::MATCH_NOCASE)
	//{
	//	str = str;
	//}

	switch (GetVariant(str))
	{
	case VARIANT_POSITION:
	{
		m_pListToken->push_back(new CTokenString(CTokenCategory::tokenVariant, CTokenCategory::varPosition, m_currentLine, str));
		return true;
	}break;
	case VARIANT_VELOCITY:
	{
		m_pListToken->push_back(new CTokenString(CTokenCategory::tokenVariant, CTokenCategory::varV, m_currentLine, str));
		return true;
	}break;
	case VARIANT_ZONE:
	{
		m_pListToken->push_back(new CTokenString(CTokenCategory::tokenVariant, CTokenCategory::varZ, m_currentLine, str));
		return true;
	}break;
	case VARIANT_UNIVERSAL:{
		m_pListToken->push_back(new CTokenString(CTokenCategory::tokenVariant, CTokenCategory::varUniversal, m_currentLine, str));
		return true;
	}break;
	default:
	{
		CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, str);
		throw(exceptionInterpreter);
		return false;
		break;
	}
	}
}

int CLexer::GetVariant(std::string& str)
{
	if (str.length() > CValue::MAX_STRING_LENGTH)
	{
		return 0;
	}
	else
	{
		for each (auto var in str)
		{
			if (!((var>='a' && var<='z')|| (var>='A' && var<='Z') || (var>='0' && var<='9')|| var=='_'))
			{
				CExceptionInterpreter exceptionInterpreter(CWarningInformation::UNKNOWN_TOKEN, m_currentLine, str);
				throw(exceptionInterpreter);
			}
		}

		return VARIANT_UNIVERSAL;
	}
	//char var = str[0];
	//std::string strNum;
	///*获取之后的数值*/
	//for (int i = 1; i < str.length();i++)
	//{
	//	strNum.push_back(str[i]);
	//}
	//int result = GetNumber(strNum);
	///*若是整数*/
	//if (result==NUMBER_INT)
	//{
	//	int num=atoi(strNum.c_str());
	//	/*若超限*/
	//	if (num<0 || num>1024)
	//	{
	//		return 0;
	//	}
	//}
	///*若不是整数*/
	//else
	//{
	//	return 0;
	//}

	//switch (var)
	//{
	//case 'p':case 'P':
	//{
	//	return VARIANT_POSITION;
	//}break;
	//case 'v':case 'V':
	//{
	//	return VARIANT_VELOCITY;
	//}break;
	//case 'z':case 'Z':
	//{
	//	return VARIANT_ZONE;
	//}break;
	//default:
	//	return 0;
	//	break;
	//}
}

void CLexer::Test(std::vector<CToken*>& listToken)
{
	for each (auto var in listToken)
	{
		DisplayToken(var);
	}
}

void CLexer::DisplayToken(CToken* token)
{
	if (token==NULL)
	{
		return;
	}
	switch (token->m_largeCategory)
	{
	case CTokenCategory::tokenCommand:{
		switch (token->m_smallCategory)
		{
		case  CTokenCategory::commMovc:
		{
			qDebug() << QString("command:") + "movc";
		}break;
		case  CTokenCategory::commMovl:
		{
			qDebug() << QString("command:") + "movl";
		}break;
		case CTokenCategory::commAbort:
		{
			qDebug() << QString("command:") + "abort";
		}break;
		case CTokenCategory::commRem:
		{
			qDebug() << QString("command:") + "rem";
		}break;
		case CTokenCategory::commFor:
		{
			qDebug() << QString("command:") + "for";
		}break;
		case CTokenCategory::commElse:
		{
			qDebug() << QString("command:") + "else";
		}break;
		case CTokenCategory::commIf:
		{
			qDebug() << QString("command:") + "if";
		}break;
		case CTokenCategory::commElseIf:
		{
			qDebug() << QString("command:") + "else if";
		}break;
		case CTokenCategory::commEndif:
		{
			qDebug() << QString("command:") + "endif";
		}break;
		case CTokenCategory::commThen:
		{
			qDebug() << QString("command:") + "then";
		}break;
		case CTokenCategory::commEof:
		{
			qDebug() << QString("command;") + "eof";
		}break;
		case CTokenCategory::commFunction:
		{
			qDebug() << QString("command") + "function";
		}break;
		}
	}break;
	case CTokenCategory::tokenConstantNumber:
	{
		switch (token->m_smallCategory)
		{
		case CTokenCategory::constantFloat:
		{
			qDebug() << QString("constant:") + "float " + QString::number(((CTokenConstantNum*)token)->m_value.value_double);
		}break;
		case CTokenCategory::constantInt:
		{
			qDebug() << QString("constant:") + "int " + QString::number(((CTokenConstantNum*)token)->m_value.value_int);
		}break;
		}
	}break;
	case CTokenCategory::tokenConstantString:
	{
		qDebug() << QString("constant:") + "string:" + QString::fromStdString(((CTokenString*)token)->m_strName);
	}break;
	case CTokenCategory::tokenFunction:
	{
		switch (token->m_smallCategory)
		{
		case CTokenCategory::funSin:
		{
			qDebug() << QString("function:") + "sin";
		}break;
		case CTokenCategory::funCos:
		{
			qDebug() << QString("function:") + "cos";
		}break;
		case CTokenCategory::funTan:
		{
			qDebug() << QString("function:") + "tan";
		}break;
		case CTokenCategory::funSqrt:
		{
			qDebug() << QString("function:") + "sqrt";
		}break;
		}

	}break;
	case CTokenCategory::tokenOperator:
	{
		switch (token->m_smallCategory)
		{
		case CTokenCategory::operAnd:
		{
			qDebug() << QString("operator:") + "&";
		}break;
		case  CTokenCategory::operEqual:
		{
			qDebug() << QString("operator:") + "==";
		}break;
		case CTokenCategory::operAssignment:
		{
			qDebug() << QString("operator:") + "=";
		}break;
		case CTokenCategory::operPlus:
		{
			qDebug() << QString("operator:") + "+";
		}break;
		case CTokenCategory::operMinus:
		{
			qDebug() << QString("operator:") + "-";
		}break;
		case CTokenCategory::operMultiply:
		{
			qDebug() << QString("operator:") + "*";
		}break;
		case CTokenCategory::operDivide:
		{
			qDebug() << QString("operator:") + "/";
		}break;
		case CTokenCategory::operPower:
		{
			qDebug() << QString("operator:") + "^";
		}break;
		}break;
	case CTokenCategory::tokenVariant:
	{
		switch (token->m_smallCategory)
		{
		case CTokenCategory::varUniversal:
		{
			qDebug() << QString("variant:") + "universal," + QString::fromStdString(((CTokenString*)token)->m_strName);
		}break;
		default:
			break;
		}
	}break;
	case CTokenCategory::tokenSeparator:
	{
		switch (token->m_smallCategory)
		{
		case CTokenCategory::sepLineBreak:
		{
			qDebug() << "\\n";
		}break;
		case CTokenCategory::sepEnd:
		{
			qDebug() << "\\0";
		}break;
		}
	}break;
	}
	}
}

void CLexer::Init()
{
	m_pListToken = new std::vector<CToken*>;

	m_currentLine = 1;
	m_currentPointer = 0;
	m_currentChar = (*m_pStrContext)[m_currentPointer];
	int contextLength = m_pStrContext->size();
	CToken* currentToken;
	/*获取词法单元并存储*/
	while (GetNextToken())
	{
	}
}
