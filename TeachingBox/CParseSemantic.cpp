#include "stdafx.h"
#include "CParseSemantic.h"
#include "CTokenString.h"
#include "CExceptionInterpreter.h"
#include "CWarningInformation.h"
#include "TVariateManager.h"



//CParseSemantic::CParseSemantic(CAstTreeNode* rootNode)
//{
//	m_rootNode = rootNode;
//	m_value = CValue::GetInstance();
//}

CParseSemantic::CParseSemantic(CParseGrammar& parseGrammar)
{
	m_value = CValue::GetInstance();

	m_rootNode = parseGrammar.GetAstTreeRootNode();
	m_scopeName = static_cast<CTokenString*>(m_rootNode->m_token)->m_strName;
	m_scope = TVariateManager::GetInstance()->GetRootScope().FindScopeScrollDown(QString::fromStdString(m_scopeName));

	ParseAll();
}

CParseSemantic::~CParseSemantic()
{

}

CAstTreeNode* CParseSemantic::GetAstTreeRootNode()
{
	return m_rootNode;
}

void CParseSemantic::ParseAll()
{
	CAstTreeNode* currentNode = m_rootNode->m_firstChildNode;
	while (currentNode != NULL)
	{
		/*若该语句语义正确，解析下一语句*/
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}

	///*函数是否正确*/
	//while (currentNode != NULL)
	//{
	//	/*若为主函数*/
	//	if (currentNode->m_token->m_smallCategory==CTokenCategory::commMain)
	//	{
	//		/*若已存在主函数*/
	//		if (m_isFindMainFunction)
	//		{
	//			throw(CExceptionInterpreter(CWarningInformation::TOO_MANY_MAIN_PROGRAM, currentNode->m_token->m_lineNum));
	//		}
	//		else
	//		{
	//			m_isFindMainFunction = true;
	//		}
	//	}
	//	else
	//	{
	//		CTokenString* tokenFunction = static_cast<CTokenString*>(currentNode->m_firstChildNode->m_token);

	//		/*若之前不存在该函数名*/
	//		if (m_setFuntion.find(tokenFunction->m_strName)==m_setFuntion.end())
	//		{
	//			m_setFuntion.insert(tokenFunction->m_strName);
	//		}
	//		/*已存在该函数*/
	//		else
	//		{
	//			throw(CExceptionInterpreter(CWarningInformation::EXIST_SAME_PROGRAM, currentNode->m_token->m_lineNum));
	//		}
	//	}
	//	currentNode = currentNode->m_nextSiblingNode;
	//}

	///*若未找到主函数*/
	//if (!m_isFindMainFunction)
	//{
	//	throw(CExceptionInterpreter(CWarningInformation::NOT_FIND_MAIN_PROGRAM, 0));
	//}

	//currentNode = m_rootNode->m_firstChildNode;
	//while (currentNode != NULL)
	//{
	//	ParseFunction(currentNode);
	//	currentNode = currentNode->m_nextSiblingNode;

	//}
}

//void CParseSemantic::ParseFunction(CAstTreeNode* rootNode)
//{
//	CAstTreeNode* currentNode;
//	if (rootNode->m_token->m_smallCategory==CTokenCategory::commMain)
//	{
//		currentNode = rootNode->m_firstChildNode;
//	}
//	else
//	{
//		currentNode = rootNode->m_firstChildNode->m_nextSiblingNode;
//	}
//
//	while (currentNode != NULL)
//	{
//		/*若该语句语义正确，解析下一语句*/
//		ParseSentence(currentNode);
//		currentNode = currentNode->m_nextSiblingNode;
//	}
//}

void CParseSemantic::ParseSentenceGosub(CAstTreeNode* rootNode)
{
	/*获取项目名*/
	QString strName = QString::fromStdString(m_scopeName);
	QStringList strList=strName.split(".");
	std::string strProject = strList.at(0).toStdString();

	/*真实函数名*/
	std::string programName = strProject + "." + static_cast<CTokenString*>(rootNode->m_firstChildNode->m_token)->m_strName;
	
	/*若未找到目标函数*/
	if (m_scope->FindSymbolScrollUp(QString::fromStdString(programName))==NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::NOT_EXIST_THIS_PROGRAM, rootNode->m_token->m_lineNum));
	}
}

void CParseSemantic::ParseSentence(CAstTreeNode* rootNode)
{
	switch (rootNode->m_token->m_largeCategory)
	{
		/*若为操作符*/
	case CTokenCategory::tokenOperator:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
			/*若为等号*/
		case CTokenCategory::operAssignment:
		{
			ParseSentenceAssignExp(rootNode);
		}break;
		default:
			break;
		}
	}break;
	case CTokenCategory::tokenCommand:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
		case CTokenCategory::commLabel:
		{
			ParseSentenceLabel(rootNode);
		}break;
		case CTokenCategory::commIf:
		{
			ParseSentenceIf(rootNode);
		}break;
		case CTokenCategory::commThen:
		{
			ParseSentenceThen(rootNode);
		}break;
		case CTokenCategory::commElseIf:
		{
			ParseSentenceElseIf(rootNode);
		}break;
		case CTokenCategory::commFor:
		{
			ParseSentenceFor(rootNode);
		}break;
		case CTokenCategory::commSwitch:
		{
			ParseSentenceSwitch(rootNode);
		}break;
		case CTokenCategory::commCase:
		{
			ParseSentenceCase(rootNode);
		}break;
		case CTokenCategory::commDefault:
		{
			ParseSentenceDefault(rootNode);
		}break;
		case CTokenCategory::commMovl:
		{
			ParseSentenceMovl(rootNode);
		}break;
		case CTokenCategory::commMovc:
		{
			ParseSentenceMovc(rootNode);
		}break;
		case CTokenCategory::commGosub:
		{
			ParseSentenceGosub(rootNode);
		}break;
		}
	}break;
	default:
		break;
	}
}

void CParseSemantic::ParseSentenceAssignExp(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
	//CLexer::DisplayToken(firstChildNode->m_token);
	CTokenString* firstToken = static_cast<CTokenString*>(firstChildNode->m_token);
	auto iterFirst = m_scope->FindSymbolScrollUp(QString::fromStdString(firstToken->m_strName));
	/*若不存在该符号*/
	if (iterFirst == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, firstToken->m_lineNum, firstToken->m_strName));
	}

	int firstVarType = iterFirst->GetType();	/*等号左边变量类型*/
	firstToken->m_type = firstVarType;

	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
	/*获取表达式类型*/
	int expType = GetExpType(secondChildNode);
	//DisplayType(expType);

	/*类型匹配*/
	switch (firstVarType)
	{
		/*若左右均为布尔类型*/
	case CSymbol::TYPE_BOOL:
	{
		if (expType != CSymbol::TYPE_BOOL)
		{
			throw(CExceptionInterpreter(CWarningInformation::UNMATCH_TYPE, firstToken->m_lineNum));
		}
	}break;
	/*若左右为数值类型*/
	case CSymbol::TYPE_INTERGER:case CSymbol::TYPE_DOUBLE:
	{
		if (expType != CSymbol::TYPE_DOUBLE && expType != CSymbol::TYPE_INTERGER)
		{
			throw(CExceptionInterpreter(CWarningInformation::UNMATCH_TYPE, firstToken->m_lineNum));
		}
	}break;
	default:
		break;
	}
}

void CParseSemantic::ParseSentenceIf(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;

	/*若if语句之后未跟布尔值*/
	if (GetExpType(firstChildNode) != CSymbol::TYPE_BOOL)
	{
		throw CExceptionInterpreter(CWarningInformation::IF_WITH_NOT_BOOL, firstChildNode->m_token->m_lineNum);
	}

	CAstTreeNode* currentNode = firstChildNode->m_nextSiblingNode;
	while (currentNode!=NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceThen(CAstTreeNode* rootNode)
{
	CAstTreeNode* currentNode = rootNode->m_firstChildNode;
	while (currentNode != NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceElseIf(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;

	/*若elseif语句之后未跟布尔值*/
	if (GetExpType(firstChildNode) != CSymbol::TYPE_BOOL)
	{
		throw CExceptionInterpreter(CWarningInformation::ELSEIF_WITH_NOT_BOOL, firstChildNode->m_token->m_lineNum);
	}

	CAstTreeNode* currentNode = firstChildNode->m_nextSiblingNode;
	while (currentNode != NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceLabel(CAstTreeNode* rootNode)
{
	//CTokenString* tokenLabel = (CTokenString*)(rootNode->m_firstChildNode->m_token);
	//if (m_value->m_setLabel.find(tokenLabel->m_strName)==m_value->m_setLabel.end())
	//{
	//	throw CExceptionInterpreter(CWarningInformation::UNKNOWN_LABEL, tokenLabel->m_lineNum,tokenLabel->m_strName);
	//}
}

void CParseSemantic::ParseSentenceFor(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
	CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
	CAstTreeNode* thirdChildNode = secondChildNode->m_nextSiblingNode;

	int firstType = GetExpType(firstChildNode->m_firstChildNode);
	int secondType = GetExpType(firstChildNode->m_firstChildNode->m_nextSiblingNode);
	int thirdType = GetExpType(secondChildNode);
	int forthType = GetExpType(thirdChildNode);

	if (firstType==CSymbol::TYPE_INTERGER
		&& secondType == CSymbol::TYPE_INTERGER
		&& thirdType == CSymbol::TYPE_INTERGER
		&& forthType == CSymbol::TYPE_INTERGER)
	{
		CAstTreeNode* currentNode = thirdChildNode->m_nextSiblingNode;
		while (currentNode != NULL)
		{
			ParseSentence(currentNode);
			currentNode = currentNode->m_nextSiblingNode;
		}
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum);
	}
}

void CParseSemantic::ParseSentenceSwitch(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChild = rootNode->m_firstChildNode;

	/*若为变量*/
	if (firstChild->m_token->m_largeCategory==CTokenCategory::tokenVariant)
	{
		CTokenString* tokenVar = (CTokenString*)(firstChild->m_token);
		auto iterVar = m_scope->FindSymbolScrollUp(QString::fromStdString(tokenVar->m_strName));

		/*若未找到变量*/
		if (iterVar==NULL)
		{
			throw CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, tokenVar->m_lineNum, tokenVar->m_strName);
		}

		/*若不是整数变量*/
		if (iterVar->GetType() != CSymbol::TYPE_INTERGER)
		{
			throw CExceptionInterpreter(CWarningInformation::SWITCH_WITH_NOT_INT, tokenVar->m_lineNum, tokenVar->m_strName);
		}
	}
	/*若为常数*/
	if (firstChild->m_token->m_largeCategory == CTokenCategory::tokenConstantNumber)
	{
		/*若不是整数*/
		if (firstChild->m_token->m_smallCategory != CTokenCategory::constantInt)
		{
			throw CExceptionInterpreter(CWarningInformation::SWITCH_WITH_NOT_INT, firstChild->m_token->m_lineNum);
		}
	}

	CAstTreeNode* currentNode = firstChild->m_nextSiblingNode;

	while (currentNode!=NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceCase(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChild = rootNode->m_firstChildNode;

	/*若为变量*/
	if (firstChild->m_token->m_largeCategory == CTokenCategory::tokenVariant)
	{
		CTokenString* tokenVar = (CTokenString*)(firstChild->m_token);
		auto iterVar = m_scope->FindSymbolScrollUp(QString::fromStdString(tokenVar->m_strName));

		/*若未找到变量*/
		if (iterVar == NULL)
		{
			throw CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, tokenVar->m_lineNum, tokenVar->m_strName);
		}

		/*若不是整数变量*/
		if (iterVar->GetType() != CSymbol::TYPE_INTERGER)
		{
			throw CExceptionInterpreter(CWarningInformation::SWITCH_WITH_NOT_INT, tokenVar->m_lineNum, tokenVar->m_strName);
		}
	}
	/*若为常数*/
	if (firstChild->m_token->m_largeCategory == CTokenCategory::tokenConstantNumber)
	{
		/*若不是整数*/
		if (firstChild->m_token->m_smallCategory != CTokenCategory::constantInt)
		{
			throw CExceptionInterpreter(CWarningInformation::SWITCH_WITH_NOT_INT, firstChild->m_token->m_lineNum);
		}
	}

	CAstTreeNode* currentNode = firstChild->m_nextSiblingNode;

	while (currentNode != NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceDefault(CAstTreeNode* rootNode)
{
	CAstTreeNode* currentNode = rootNode->m_firstChildNode;

	while (currentNode != NULL)
	{
		ParseSentence(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
}

void CParseSemantic::ParseSentenceMovl(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChild = rootNode->m_firstChildNode;
	CAstTreeNode* secondChild = firstChild->m_nextSiblingNode;
	CAstTreeNode* thirdChild = secondChild->m_nextSiblingNode;

	CTokenString* firstToken = (CTokenString*)(firstChild->m_token);
	CTokenString* sencondToken = (CTokenString*)(secondChild->m_token);
	CTokenString* thirdToken = (CTokenString*)(thirdChild->m_token);

	auto iterFirst = m_scope->FindSymbolScrollUp(QString::fromStdString(firstToken->m_strName));
	auto iterSecond = m_scope->FindSymbolScrollUp(QString::fromStdString(sencondToken->m_strName));
	auto iterThird = m_scope->FindSymbolScrollUp(QString::fromStdString(thirdToken->m_strName));

	/*若第一个参数未定义*/
	if (iterFirst==NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, firstToken->m_strName));
	}

	/*若第二个参数未定义*/
	if (iterSecond == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, sencondToken->m_strName));
	}

	/*若第三个参数未定义*/
	if (iterThird == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, thirdToken->m_strName));
	}

	int firstType = iterFirst->GetType();
	int SecondType = iterSecond->GetType();
	int thirdType = iterThird->GetType();

	if (firstType == CSymbol::TYPE_POSITION && SecondType == CSymbol::TYPE_DYNAMIC && thirdType == CSymbol::TYPE_OVERLAP)
	{
		return;
	}
	else
	{
		throw(CExceptionInterpreter(CWarningInformation::PARAMETER_NOT_MATCH, rootNode->m_token->m_lineNum));
	}
}

void CParseSemantic::ParseSentenceMovc(CAstTreeNode* rootNode)
{
	CAstTreeNode* firstChild = rootNode->m_firstChildNode;
	CAstTreeNode* secondChild = firstChild->m_nextSiblingNode;
	CAstTreeNode* thirdChild = secondChild->m_nextSiblingNode;
	CAstTreeNode* forthChild = thirdChild->m_nextSiblingNode;

	CTokenString* firstToken = (CTokenString*)(firstChild->m_token);
	CTokenString* sencondToken = (CTokenString*)(secondChild->m_token);
	CTokenString* thirdToken = (CTokenString*)(thirdChild->m_token);
	CTokenString* forthToken = (CTokenString*)(forthChild->m_token);


	auto iterFirst = m_scope->FindSymbolScrollUp(QString::fromStdString(firstToken->m_strName));
	auto iterSecond = m_scope->FindSymbolScrollUp(QString::fromStdString(sencondToken->m_strName));
	auto iterThird = m_scope->FindSymbolScrollUp(QString::fromStdString(thirdToken->m_strName));
	auto iterForth = m_scope->FindSymbolScrollUp(QString::fromStdString(forthToken->m_strName));


	/*若第一个参数未定义*/
	if (iterFirst == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, firstToken->m_strName));
	}

	/*若第二个参数未定义*/
	if (iterSecond == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, sencondToken->m_strName));
	}

	/*若第三个参数未定义*/
	if (iterThird == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, thirdToken->m_strName));
	}

	/*若第四个参数未定义*/
	if (iterThird == NULL)
	{
		throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, rootNode->m_token->m_lineNum, forthToken->m_strName));
	}

	int firstType = iterFirst->GetType();
	int SecondType = iterSecond->GetType();
	int thirdType = iterThird->GetType();
	int forthType = iterForth->GetType();

	if (firstType == CSymbol::TYPE_POSITION && SecondType == CSymbol::TYPE_POSITION
		&& thirdType == CSymbol::TYPE_DYNAMIC && forthType == CSymbol::TYPE_OVERLAP)
	{
		return;
	}
	else
	{
		throw(CExceptionInterpreter(CWarningInformation::PARAMETER_NOT_MATCH, rootNode->m_token->m_lineNum));
	}
}

int CParseSemantic::GetExpType(CAstTreeNode* rootNode)
{
	switch (rootNode->m_token->m_largeCategory)
	{
	/*若为常数*/
	case CTokenCategory::tokenConstantNumber:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
			/*布尔值*/
		case CTokenCategory::constantBool:
		{
			return CSymbol::TYPE_BOOL;
		}break;
		/*浮点数*/
		case CTokenCategory::constantFloat:
		{
			return CSymbol::TYPE_DOUBLE;
		}break;
		/*常数*/
		case CTokenCategory::constantInt:
		{
			return CSymbol::TYPE_INTERGER;
		}break;
		default:
			break;
		}
	}break;
	/*若为变量*/
	case CTokenCategory::tokenVariant:
	{
		CTokenString* token = (CTokenString*)(rootNode->m_token);
		CSymbol* result = m_scope->FindSymbolScrollUp(QString::fromStdString(token->m_strName));

		/*若变量不存在*/
		if (result==NULL)
		{
			throw(CExceptionInterpreter(CWarningInformation::UNDEFINED_VARIANT, token->m_lineNum, token->m_strName));
		}
		/*若变量存在*/
		else
		{
			token->m_type = result->GetType();
			switch (result->GetType())
			{
			case CSymbol::TYPE_DOUBLE:
			{
				return CSymbol::TYPE_DOUBLE;
			}break;
			case CSymbol::TYPE_INTERGER:
			{
				return CSymbol::TYPE_INTERGER;
			}break;
			case  CSymbol::TYPE_BOOL:
			{
				return CSymbol::TYPE_BOOL;
			}break;
			/*若为其它类型变量*/
			default:
			{
				throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum));
			}break;
			}
		}
		
	}break;
	/*若为函数*/
	case  CTokenCategory::tokenFunction:
	{
		/*若参数是浮点数（浮点数及整数统一采用浮点数表示）*/
		if (GetExpType(rootNode->m_firstChildNode)==CSymbol::TYPE_DOUBLE
			|| GetExpType(rootNode->m_firstChildNode) == CSymbol::TYPE_INTERGER)
		{
			return CSymbol::TYPE_DOUBLE;
		}
		else
		{
			throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum));
		}
	}break;
	/*若为操作符*/
	case  CTokenCategory::tokenOperator:
	{
		switch (rootNode->m_token->m_smallCategory)
		{
		/*若为数值运算符*/
		case CTokenCategory::operPlus:case  CTokenCategory::operMinus:case CTokenCategory::operMultiply:
		case CTokenCategory::operDivide:case CTokenCategory::operPower:
		{
			CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
			CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
			/*若左右节点均为浮点数*/
			if (GetExpType(firstChildNode) == CSymbol::TYPE_DOUBLE && GetExpType(secondChildNode) == CSymbol::TYPE_DOUBLE)
			{
				return CSymbol::TYPE_DOUBLE;
			}
			else if (GetExpType(firstChildNode) == CSymbol::TYPE_DOUBLE && GetExpType(secondChildNode) == CSymbol::TYPE_INTERGER)
			{
				return  CSymbol::TYPE_DOUBLE;
			}
			else if (GetExpType(firstChildNode) == CSymbol::TYPE_INTERGER && GetExpType(secondChildNode) == CSymbol::TYPE_DOUBLE)
			{
				return  CSymbol::TYPE_DOUBLE;
			}
			else if (GetExpType(firstChildNode) == CSymbol::TYPE_INTERGER && GetExpType(secondChildNode) == CSymbol::TYPE_INTERGER)
			{
				return  CSymbol::TYPE_INTERGER;
			}
			else
			{
				throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum));
			}
		}break;
		/*若为比较运算符*/
		case CTokenCategory::operGreatEqual:case CTokenCategory::operGreatThan:case CTokenCategory::operLessEqual:
		case CTokenCategory::operLessThan:case CTokenCategory::operEqual:case CTokenCategory::operNotEqual:
		{
			CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
			CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
			/*若左右节点均为数值*/
			if ((GetExpType(firstChildNode) == CSymbol::TYPE_DOUBLE || GetExpType(firstChildNode) == CSymbol::TYPE_INTERGER)
				&& (GetExpType(secondChildNode) == CSymbol::TYPE_DOUBLE || GetExpType(secondChildNode) == CSymbol::TYPE_INTERGER))
			{
				return CSymbol::TYPE_BOOL;
			}
			else
			{
				throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum));
			}
		}break;
		/*若为逻辑操作符*/
		case  CTokenCategory::operAnd:case CTokenCategory::operOr:
		{
			CAstTreeNode* firstChildNode = rootNode->m_firstChildNode;
			CAstTreeNode* secondChildNode = firstChildNode->m_nextSiblingNode;
			/*若左右节点均为浮点数*/
			if (GetExpType(firstChildNode) == CSymbol::TYPE_BOOL && GetExpType(secondChildNode) == CSymbol::TYPE_BOOL)
			{
				return CSymbol::TYPE_BOOL;
			}
			else
			{
				throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, rootNode->m_token->m_lineNum));
			}
		}break;
		}
	}break;
	default:
		break;
	}

	return -1;
}


void CParseSemantic::DisplayType(int type)
{
	switch (type)
	{
	case CSymbol::TYPE_BOOL:
	{
		qDebug() << "bool";
	}break;
	case CSymbol::TYPE_DOUBLE:
	{
		qDebug() << "double";
	}break;
	case -1:
	{
		qDebug() << "error";
	}break;
	default:
		break;
	}
}
