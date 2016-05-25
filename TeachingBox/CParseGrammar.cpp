#include "stdafx.h"
#include "CParseGrammar.h"
#include "CLexer.h"
#include "CWarningInformation.h"
#include "CExceptionInterpreter.h"
#include "CTokenString.h"


//CParseGrammar::CParseGrammar(std::vector<CToken*>& tokenList)
//{
//	m_tokenList = &tokenList;
//}

CParseGrammar::CParseGrammar(CLexer& lexer,CAstTreeNode* rootNode)
{
	m_tokenList = lexer.GetTokenList();
	m_temporaryTokenList = new std::vector<CToken*>;
	m_rootNode = rootNode;

	Init();
}

CParseGrammar::~CParseGrammar()
{
	/*清除临时词法单元列表*/
	for each (auto var in *m_temporaryTokenList)
	{
		delete(var);
	}

	m_temporaryTokenList->clear();
	delete(m_temporaryTokenList);
}

CAstTreeNode* CParseGrammar::GetAstTreeRootNode()
{
	return m_rootNode;
}

//void CParseGrammar::GetTree(CAstTreeNode* rootNode)
//{
//	m_rootNode = rootNode;
//	m_tokenPointer = 0;
//	while (m_tokenPointer != m_tokenList->size())
//	{
//		///*若为标签
//		// *标签必须放在最外层，故在此解析*/
//		//if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenCommand
//		//	&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::commLabel)
//		//{
//		//	MatchLabelSentence(rootNode);
//		//}
//		//else
//		//{
//		MatchSentece(rootNode);
//		//}
//
//		/*进入下一行*/
//		m_tokenPointer++;
//	}
//	rootNode->AddChildNode(new CAstTreeNode(m_tokenList->at(m_tokenList->size() - 1)));
//	//DisplayTreeNode(rootNode);
//	/*若未到达列表尾*/
//	if (m_tokenPointer < m_tokenList->size())
//	{
//		return;
//	}
//}

void CParseGrammar::MatchSentece(CAstTreeNode* rootNode)
{
	CToken* token = m_tokenList->at(m_tokenPointer);
	switch (token->m_largeCategory)
	{
	/*匹配复制表达式*/
	case CTokenCategory::tokenVariant:
	{
		MatchAssignSentence(rootNode);
	}break;
	/*若为命令*/
	case CTokenCategory::tokenCommand:
	{
		switch (token->m_smallCategory)
		{
		/*若为if*/
		case CTokenCategory::commIf:
		{
			MatchIfSentence(rootNode);
		}break;
		/*若为for*/
		case CTokenCategory::commFor:
		{
			MatchForSentence(rootNode);
		}break;
		/*若为Switch语句*/
		case CTokenCategory::commSwitch:
		{
			MatchSwitchSentence(rootNode);
		}break;
		/*若为break*/
		case CTokenCategory::commBreak:
		{
			MatchBreakSentence(rootNode);
		}break;
		/*若为注释*/
		case CTokenCategory::commRem:
		{
			MatchRemSentence(rootNode);
		}break;
		/*若为Movl语句*/
		case CTokenCategory::commMovl:
		{
			MatchMovlSentence(rootNode);
		}break;
		/*若为Movc语句*/
		case CTokenCategory::commMovc:
		{
			MatchMovcSentence(rootNode);
		}break;
		/*若为Gosub语句*/
		case CTokenCategory::commGosub:
		{
			MatchGosubSentence(rootNode);
		}break;
		/*若为Eof语句*/
		case CTokenCategory::commEof:
		{
			MatchEofSentence(rootNode);
		}break;
		default:
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, token->m_lineNum);
		}break;
		}
	}break;
	case CTokenCategory::tokenSeparator:
	{
		switch (token->m_smallCategory)
		{
		/*若为空行*/
		case CTokenCategory::sepLineBreak:
		{
			return;
		}break;
		/*若为end语句*/
		case CTokenCategory::sepEnd:
		{
			MatchEndSentence(rootNode);
		}break;
		}
	}break;
	/*若不匹配任何语句*/
	default:
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, token->m_lineNum);
	}break;
	}
}

void CParseGrammar::Init()
{
	//MatchAllSentences();
	if (m_tokenList->size()==0)
	{
		throw(CExceptionInterpreter(CWarningInformation::EMPTY));
	}
	while (m_tokenPointer<m_tokenList->size())
	{
		MatchSentece(m_rootNode);
		/*进入下一行*/
		m_tokenPointer++;
	}

	/*若未遇到EOF*/
	if (!m_meetEof)
	{
		throw(CExceptionInterpreter(CWarningInformation::NOT_FIND_EOF, m_tokenList->at(m_tokenList->size()-1)->m_lineNum));
	}
}

void CParseGrammar::MatchEofSentence(CAstTreeNode* rootNode)
{
	CAstTreeNode* eofNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(eofNode);

	++m_tokenPointer;
	/*若未到达文件尾*/
	if (!IsFileEnd())
	{
		throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
	}

	m_meetEof = true;
}

//void CParseGrammar::MatchAllSentences()
//{
//	m_tokenPointer = 0;
//	while (!IsFileEnd())
//	{
//		SkipBlankLine();
//
//		/*若为函数*/
//
//		if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//			&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commMain)
//		{
//			MatchFunctionMainSentence(m_rootNode);
//		}
//		else if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//			&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commFunction)
//		{
//			MatchFunctionSentence(m_rootNode);
//		}
//		/*不是函数，抛出异常*/
//		else
//		{
//			throw(CExceptionInterpreter(CWarningInformation::NOT_PROGRAM, m_tokenList->at(m_tokenPointer)->m_lineNum));
//		}
//	}
//	//m_rootNode->AddChildNode(new CAstTreeNode(m_tokenList->at(m_tokenList->size() - 1)));
//}

//void CParseGrammar::MatchFunctionMainSentence(CAstTreeNode* rootNode)
//{
//	/*添加子节点*/
//	CAstTreeNode* functionNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
//	rootNode->AddChildNode(functionNode);
//	++m_tokenPointer;
//
//	/*若语句未正常结束*/
//	if (!IsSentenceEnd())
//	{
//		throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
//	}
//	///*若已到达文件尾*/
//	//if (IsFileEnd())
//	//{
//	//	throw(CExceptionInterpreter(CWarningInformation::NOT_MEET_END, m_tokenList->at(m_tokenPointer)->m_lineNum));
//	//}
//	++m_tokenPointer;
//
//	/*若未遇到end或未到达文件尾*/
//	while (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commEnd)
//		&& !IsFileEnd())
//	{
//		MatchSentece(functionNode);
//
//		/*进入下一行*/
//		m_tokenPointer++;
//	}
//
//	/*若遇到end*/
//	if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commEnd)
//	{
//		functionNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
//		++m_tokenPointer;
//		/*若语句未结束*/
//		if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
//			&& (m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepEnd
//			|| m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepLineBreak)))
//		{
//			throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
//		}
//		SkipBlankLine();
//	}
//	else
//	{
//		throw(CExceptionInterpreter(CWarningInformation::NOT_MEET_END, m_tokenList->at(m_tokenPointer)->m_lineNum));
//	}
//
//}
//
//void CParseGrammar::MatchFunctionSentence(CAstTreeNode* rootNode)
//{
//	/*添加函数子节点*/
//	CAstTreeNode* functionNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
//	rootNode->AddChildNode(functionNode);
//	++m_tokenPointer;
//
//	/*若为正常变量名*/
//	if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant
//		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::varUniversal)
//	{
//		/*添加函数名节点*/
//		functionNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
//		++m_tokenPointer;
//
//		/*若语句未正常结束*/
//		if (!IsSentenceEnd())
//		{
//			throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
//		}
//		///*若已到达文件尾*/
//		//if (IsFileEnd())
//		//{
//		//	throw(CExceptionInterpreter(CWarningInformation::NOT_MEET_END, m_tokenList->at(m_tokenPointer)->m_lineNum));
//		//}
//		else
//		{
//			++m_tokenPointer;
//
//			/*若未遇到end且未到达文件尾*/
//			while (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//				&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commEnd)
//				&& !IsFileEnd())
//			{
//				MatchSentece(functionNode);
//
//				/*进入下一行*/
//				m_tokenPointer++;
//			}
//
//			/*若遇到end*/
//			if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
//				&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commEnd)
//			{
//				functionNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
//				++m_tokenPointer;
//				/*若语句未结束*/
//				if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenSeparator
//					&& (m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepEnd
//					|| m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepLineBreak)))
//				{
//					throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
//				}
//				SkipBlankLine();
//			}
//			else
//			{
//				throw(CExceptionInterpreter(CWarningInformation::NOT_MEET_END, m_tokenList->at(m_tokenPointer)->m_lineNum));
//			}
//		}
//	}
//	else
//	{
//		throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
//	}
//
//}

void CParseGrammar::MatchGosubSentence(CAstTreeNode* rootNode)
{
	/*添加gosub节点*/
	CAstTreeNode* gosubNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(gosubNode);
	++m_tokenPointer;

	/*若是变量名*/
	if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenVariant
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::varUniversal)
	{
		gosubNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
		++m_tokenPointer;

		/*若未到达文件尾*/
		if (!IsSentenceEnd())
		{
			throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
		}
	}
	else
	{
		throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum));
	}

}

/*匹配赋值语句*/
void CParseGrammar::MatchAssignSentence(CAstTreeNode* rootNode)
{
	/*若匹配*/
	MatchAssignExp(rootNode);
	/*到达语句尾部*/
	if (!IsSentenceEnd())
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchIfSentence(CAstTreeNode* rootNode)
{
	/*添加If节点*/
	CAstTreeNode* ifNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(ifNode);
	m_tokenPointer++;

	/*匹配表达式*/
	MatchExp(ifNode);
	{
		/*若到行尾*/
		if (IsSentenceEnd())
		{
			CToken* thenToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commThen, ifNode->m_token->m_lineNum);
			m_temporaryTokenList->push_back(thenToken);
			CAstTreeNode* thenNode = new CAstTreeNode(new CToken(thenToken));
			ifNode->AddChildNode(thenNode);

			/*若文件未结束*/
			while (!IsFileEnd())
			{
				m_tokenPointer++;
				switch (m_tokenList->at(m_tokenPointer)->m_largeCategory)
				{
					/*若为命令*/
				case CTokenCategory::tokenCommand:
				{
					switch (m_tokenList->at(m_tokenPointer)->m_smallCategory)
					{
						/*若为endif语句*/
					case CTokenCategory::commEndif:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						thenNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchEndIfSentence(ifNode);
						return;
					}break;
					/*若匹配else语句*/
					case CTokenCategory::commElse:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						thenNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchElseSentence(ifNode);
						return;
					}break;
					/*若匹配elseif语句*/
					case CTokenCategory::commElseIf:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						thenNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchElseIfSentence(ifNode);
						return;
					}break;
					/*若为其它语句*/
					default:
					{
						MatchSentece(thenNode);
					}break;
					}
				}break;
				/*若为其它语句*/
				default:
				{
					MatchSentece(thenNode);
				}break;
				}
			}
			/*未找到endif语句*/
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
		/*若该行未结束*/
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
}

void CParseGrammar::MatchElseIfSentence(CAstTreeNode* rootNode)
{
	/*添加elseif节点*/
	CAstTreeNode* elseIfNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(elseIfNode);

	m_tokenPointer++;

	/*若为表达式*/
	MatchExp(elseIfNode);
	{
		/*若到达语句尾*/
		if (IsSentenceEnd())
		{
			/*若未到达文件尾*/
			while (!IsFileEnd())
			{
				m_tokenPointer++;
				if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand)
				{
					switch (m_tokenList->at(m_tokenPointer)->m_smallCategory)
					{
					case CTokenCategory::commElse:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						elseIfNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchElseSentence(rootNode);
						return;
					}break;
					case CTokenCategory::commElseIf:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						elseIfNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchElseIfSentence(rootNode);
						return;
					}break;
					case CTokenCategory::commEndif:
					{
						CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum-1);
						m_temporaryTokenList->push_back(endifToken);
						elseIfNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

						MatchEndIfSentence(rootNode);
						return;
					}break;
					default:
					{
						MatchSentece(elseIfNode);
					}break;
					}
				}
				/*若为其它语句*/
				else
				{
					MatchSentece(elseIfNode);
				}
			}
		}
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}

}

void CParseGrammar::MatchElseSentence(CAstTreeNode* rootNode)
{
	/*获取else节点*/
	CAstTreeNode* elseNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(elseNode);

	m_tokenPointer++;

	/*若到达行尾*/
	if (IsSentenceEnd())
	{
		/*若未到达文件尾*/
		while (!IsFileEnd())
		{
			m_tokenPointer++;
			/*若为endif语句*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
				&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commEndif)
			{
				CToken* endifToken = new CToken(CTokenCategory::tokenCommand, CTokenCategory::commEndif, m_tokenList->at(m_tokenPointer)->m_lineNum - 1);
				m_temporaryTokenList->push_back(endifToken);
				elseNode->AddChildNode(new CAstTreeNode(new CToken(endifToken)));

				MatchEndIfSentence(rootNode);
				return;
			}
			/*若为其它语句*/
			else
			{
				MatchSentece(elseNode);
			}
		}

		/*未找到endif语句*/
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchEndIfSentence(CAstTreeNode* rootNode)
{
	rootNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

	m_tokenPointer++;
	if (IsSentenceEnd())
	{
		return;
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchForSentence(CAstTreeNode* rootNode)
{
	/*循环或开关嵌套层数加一*/
	m_numOfLoopOrSwitch++;	

	/*添加for节点*/
	CAstTreeNode* forNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(forNode);

	m_tokenPointer++;

	/*若之后为赋值表达式*/
	MatchAssignExp(forNode);
	/*若之后为to*/
	if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commTo)
	{
		m_tokenPointer++;

		/*若之后为数值表达式*/
		MatchExp(forNode);

		/*若之后为step*/
		if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
			&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commStep)
		{
			m_tokenPointer++;

			/*若之后为数值表达式*/
			MatchExp(forNode);

			/*若语句结束*/
			if (IsSentenceEnd())
			{
				/*若未到达文件尾*/
				while (!IsFileEnd())
				{
					m_tokenPointer++;

					/*若之后是next*/
					if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand
						&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::commNext)
					{
						/*添加next节点*/
						forNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

						m_tokenPointer++;
						/*是否是语句尾*/
						if (IsSentenceEnd())
						{
							/*循环或开关嵌套层数减一*/
							m_numOfLoopOrSwitch--;
							return;
						}
						else
						{
							throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
						}
					}
					/*若不是next*/
					else
					{
						MatchSentece(forNode);
					}
				}
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}
			/*若语句未结束*/
			else
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}
		}
		/*若之后不是step*/
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
	/*若之后不为to*/
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchSwitchSentence(CAstTreeNode* rootNode)
{
	/*循环或开关嵌套层数加一*/
	m_numOfLoopOrSwitch++;

	/*添加switch节点*/
	CAstTreeNode* switchNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(switchNode);

	m_tokenPointer++;
	/*若为整数或变量*/
	if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenVariant
		|| (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenConstantNumber 
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::constantInt))
	{
		/*添加开关节点*/
		switchNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

		m_tokenPointer++;

		if (IsSentenceEnd())
		{
			m_tokenPointer++;
			CLexer::DisplayToken(m_tokenList->at(m_tokenPointer));

			/*若为命令*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenCommand)
			{
				switch (m_tokenList->at(m_tokenPointer)->m_smallCategory)
				{
				case CTokenCategory::commCase:
				{
					MatchCaseSentence(switchNode);
					return;
				}break;
				case CTokenCategory::commDefault:
				{
					MatchDefaultSentence(switchNode);
					return;
				}break;
				/*若为endswitch*/
				case CTokenCategory::commEndSwitch:
				{
					MatchEndSwitchSentence(switchNode);
					return;
				}break;
				/*若为其它命令*/
				default:
				{
					throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
				}break;
				}
			}
			/*若不为命令*/
			else
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}
		}
		/*若语句未结束*/
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
	/*若不是整数或变量*/
	else
	{
		throw CExceptionInterpreter(CWarningInformation::SWITCH_WITH_NOT_INT, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchEndSwitchSentence(CAstTreeNode* rootNode)
{
	/*添加endswitch节点*/
	rootNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

	m_tokenPointer++;
	if (IsSentenceEnd())
	{
		/*循环或开关嵌套层数减一*/
		m_numOfLoopOrSwitch--;
		return;
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchCaseSentence(CAstTreeNode* rootNode)
{
	/*添加case节点*/
	CAstTreeNode* caseNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(caseNode);
	
	m_tokenPointer++;
	/*若不为整数或变量*/
	if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant
		|| (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenConstantNumber
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::constantInt)))
	{
		throw CExceptionInterpreter(CWarningInformation::CASE_WITH_NOT_INT, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}

	/*添加开关节点*/
	caseNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

	m_tokenPointer++;
	/*若语句结束*/
	if (IsSentenceEnd())
	{
		/*若未到达文件尾*/
		while (!IsFileEnd())
		{
			m_tokenPointer++;
			
			/*若为命令*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenCommand)
			{
				switch (m_tokenList->at(m_tokenPointer)->m_smallCategory)
				{
				case CTokenCategory::commCase:
				{
					MatchCaseSentence(rootNode);
					return;
				}break;
				case CTokenCategory::commDefault:
				{
					MatchDefaultSentence(rootNode);
					return;
				}break;
				case CTokenCategory::commEndSwitch:
				{
					MatchEndSwitchSentence(rootNode);
					return;
				}break;
				}
			}

			/*若为其它语句*/
			MatchSentece(caseNode);
		}
	
		/*若到达文件尾*/
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	/*若语句未结束*/
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchDefaultSentence(CAstTreeNode* rootNode)
{
	/*添加default节点*/
	CAstTreeNode* defaultNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(defaultNode);

	m_tokenPointer++;
	if (IsSentenceEnd())
	{
		/*若未到达文件尾*/
		while (!IsFileEnd())
		{
			m_tokenPointer++;

			/*若为endswitch*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenCommand
				&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::commEndSwitch)
			{
				/*匹配endswitch语句*/
				MatchEndSwitchSentence(rootNode);
				return;
			}
			/*若为其它语句*/
			else
			{
				MatchSentece(defaultNode);
			}
		}

		/*若到达文件尾*/
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	/*若语句未结束*/
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchBreakSentence(CAstTreeNode* rootNode)
{
	/*添加break节点*/
	rootNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));

	m_tokenPointer++;
	/*若语句未结束*/
	if (!IsSentenceEnd())
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	else
	{
		/*若不在循环或开关中*/
		if (m_numOfLoopOrSwitch==0)
		{
			throw CExceptionInterpreter(CWarningInformation::BREAK_NOT_IN_LOOP_OR_SWITCH, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
}

void CParseGrammar::MatchRemSentence(CAstTreeNode* rootNode)
{
	/*添加注释节点*/
	CAstTreeNode* remNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(remNode);

	m_tokenPointer++;
}

void CParseGrammar::MatchEndSentence(CAstTreeNode* rootNode)
{
	rootNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
}

void CParseGrammar::MatchMovlSentence(CAstTreeNode* rootNode)
{
	/*添加Movl节点*/
	CAstTreeNode* movlNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(movlNode);

	m_tokenPointer++;

	/*若第一个参数为变量*/
	if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenVariant)
	{
		movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
		m_tokenPointer++;

		/*若不匹配逗号*/
		if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenSeparator
			&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::sepComma))
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}

		m_tokenPointer++;

		/*若第二个参数为变量*/
		if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant)
		{
			movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
			m_tokenPointer++;

			/*若不匹配逗号*/
			if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
				&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepComma))
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}

			m_tokenPointer++;

			/*若第三个参数为变量*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant)
			{
				movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
				m_tokenPointer++;

				/*若到达语句尾*/
				if (IsSentenceEnd())
				{
					//DisplayTreeNode(rootNode);
					return;
				}
				else
				{
					throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
				}
			}
			else
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}
		}
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

void CParseGrammar::MatchMovcSentence(CAstTreeNode* rootNode)
{
	/*添加Movl节点*/
	CAstTreeNode* movlNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
	rootNode->AddChildNode(movlNode);

	m_tokenPointer++;

	/*若第一个参数为变量*/
	if (m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenVariant)
	{
		movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
		m_tokenPointer++;

		/*若不匹配逗号*/
		if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory==CTokenCategory::tokenSeparator
			&& m_tokenList->at(m_tokenPointer)->m_smallCategory==CTokenCategory::sepComma))
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}

		m_tokenPointer++;

		/*若第二个参数为变量*/
		if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant)
		{
			movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
			m_tokenPointer++;

			/*若不匹配逗号*/
			if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
				&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepComma))
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}

			m_tokenPointer++;

			/*若第三个参数为变量*/
			if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant)
			{
				movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
				m_tokenPointer++;

				/*若不匹配逗号*/
				if (!(m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
					&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepComma))
				{
					throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
				}

				m_tokenPointer++;

				/*若第四个参数为变量*/
				if (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenVariant)
				{
					movlNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
					m_tokenPointer++;

					/*若到达语句尾*/
					if (IsSentenceEnd())
					{
						//DisplayTreeNode(rootNode);
						return;
					}
					else
					{
						throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
					}
				}
				else
				{
					throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
				}
			}
			else
			{
				throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
			}
		}
		else
		{
			throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
		}
	}
	else
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
}

//////////////////////////////////////////////////////////////////////////

/*匹配赋值表达式*/
void CParseGrammar::MatchAssignExp(CAstTreeNode* rootNode)
{
	CToken* tokenVariant = m_tokenList->at(m_tokenPointer);
	/*若等号左边不是变量*/
	if (tokenVariant->m_largeCategory != CTokenCategory::tokenVariant)
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	m_tokenPointer++;
	CToken* tokenAssignment = m_tokenList->at(m_tokenPointer);
	/*若之后不是等号*/
	if (tokenAssignment->m_smallCategory != CTokenCategory::operAssignment)
	{
		throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
	}
	/*添加子节点*/
	CAstTreeNode* nodeAssignment = new CAstTreeNode(new CToken(tokenAssignment));
	rootNode->AddChildNode(nodeAssignment);
	CAstTreeNode* firstChild = new CAstTreeNode(new CToken(tokenVariant));
	nodeAssignment->AddChildNode(firstChild);

	m_tokenPointer++;

	/*若匹配算数表达式*/
	MatchExp(nodeAssignment);
}

/*************************************************
//  Function: 		MatchNumExp
//  Description: 	匹配算数表达式
//  Calls:
//  Called By: 		MatchAssignExp
//  Parameter:      void
//  Return: 		int,计算类型
//  Others:
*************************************************/
void CParseGrammar::MatchExp(CAstTreeNode* rootNode)
{
	/*创建临时节点*/
	CAstTreeNode* temporaryNode = new CAstTreeNode();

	/*获取最后一个子节点*/
	CAstTreeNode* preNode = rootNode->m_firstChildNode;
	if (preNode != NULL)
	{
		while (preNode->m_nextSiblingNode != NULL)
		{
			preNode = preNode->m_nextSiblingNode;
		}
	}

	rootNode->AddChildNode(temporaryNode);

	/*获取表达式节点列表*/
	GetNumList(temporaryNode);
	//DisplayTreeNode(temporaryNode);
	/*若匹配表达式，获取表达式数*/
	std::list<CAstTreeNode*> nodeList;
	CAstTreeNode* currentNode = temporaryNode->m_firstChildNode;
	while (currentNode != NULL)
	{
		nodeList.push_back(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}
	/*清除临时节点*/
	temporaryNode->m_firstChildNode = NULL;
	delete(temporaryNode);

	if (preNode == NULL)
	{
		rootNode->m_firstChildNode = NULL;
		rootNode->m_endChildNode = NULL;
	}
	else
	{
		preNode->m_nextSiblingNode = NULL;
		rootNode->m_endChildNode = preNode;
	}
	/*添加表达式树节点*/
	rootNode->AddChildNode(GetNumTree(nodeList));
}

void CParseGrammar::MatchFunctionExp(CAstTreeNode* rootNode)
{
	switch (m_tokenList->at(m_tokenPointer)->m_largeCategory)
	{
	case CTokenCategory::tokenFunction:
	{
		/*将函数节点添加到堆栈中*/
		CAstTreeNode* newNode = new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer)));
		rootNode->AddChildNode(newNode);

		m_tokenPointer++;
		/*若匹配左括号*/
		if (m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::operLeftBracket)
		{
			m_tokenPointer++;
			/*若匹配算数表达式*/
			MatchExp(newNode);
			{
				/*若匹配右括号*/
				if (m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::operRightBracket)
				{
					m_tokenPointer++;
					return;
				}
			}
		}
	}break;
	default:
		break;
	}

	/*若不匹配*/
	//rootNode->ClearChildren();
	throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
}

void CParseGrammar::MatchBracketExp(CAstTreeNode* rootNode)
{
	CToken* token = m_tokenList->at(m_tokenPointer);
	/*若为左括号*/
	if (token->m_smallCategory == CTokenCategory::operLeftBracket)
	{
		rootNode->AddChildNode(new CAstTreeNode(new CToken(token)));
		m_tokenPointer++;
		/*若匹配表达式*/
		GetNumList(rootNode);
		{
			/*若匹配右括号*/
			if (m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::operRightBracket)
			{
				rootNode->AddChildNode(new CAstTreeNode(new CToken(m_tokenList->at(m_tokenPointer))));
				m_tokenPointer++;
				return;
			}
		}
	}

	/*若不匹配*/
	//rootNode->ClearChildren();
	throw CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, m_tokenList->at(m_tokenPointer)->m_lineNum);
}

void CParseGrammar::GetNumList(CAstTreeNode* rootNode)
{
	CToken* newToken = m_tokenList->at(m_tokenPointer);
	//CLexer::DisplayToken(newToken);
	/*匹配第一个词法单元*/
	switch (newToken->m_largeCategory)
	{
		/*若为函数*/
	case CTokenCategory::tokenFunction:
	{
		/*匹配函数*/
		MatchFunctionExp(rootNode);
	}break;
	/*若为常数或变量*/
	case CTokenCategory::tokenConstantNumber:case CTokenCategory::tokenVariant:
	{
		rootNode->AddChildNode(new CAstTreeNode(new CToken(newToken)));
		m_tokenPointer++;
	}break;
	/*若为左括号*/
	case  CTokenCategory::tokenOperator:
	{
		MatchBracketExp(rootNode);
	}break;
	/*若不是以上任何一种，返回*/
	default:
	{
		return;
	}break;

	}

	/*匹配第二个词法单元*/
	newToken = m_tokenList->at(m_tokenPointer);
	/*若为操作符*/
	switch (newToken->m_largeCategory)
	{
		/*若为操作符*/
	case CTokenCategory::tokenOperator:
	{
		/*判断操作符类型*/
		switch (newToken->m_smallCategory)
		{
			/*若为{、}、=、==、）*/
		case CTokenCategory::operLeftBrace:case CTokenCategory::operRightBrace:case CTokenCategory::operEqual:case CTokenCategory::operAssignment:
		case CTokenCategory::operRightBracket:
		{
			return;
		}break;
		/*若为其它操作符*/
		default:
		{
			rootNode->AddChildNode(new CAstTreeNode(new CToken(newToken)));
			m_tokenPointer++;
			GetNumList(rootNode);
		}break;
		}
	}
	default:
	{
		return;
	}break;
	}
}

CAstTreeNode* CParseGrammar::GetNumTree(std::list<CAstTreeNode*>& nodeList)
{
	std::list<CAstTreeNode*> listSuffix;	/*后缀表达式栈*/
	std::list<CAstTreeNode*> listOper;		/*操作符栈*/

	for each (auto currentNode in nodeList)
	{
		switch (currentNode->m_token->m_largeCategory)
		{
			/*如果是操作数，直接进入后缀表达式栈*/
		case CTokenCategory::tokenConstantNumber:case CTokenCategory::tokenVariant:case CTokenCategory::tokenFunction:
		{
			listSuffix.push_back(currentNode);
		}break;
		/*如果是操作符，入操作符栈，并判断是否弹出*/
		case CTokenCategory::tokenOperator:
		{
			switch (currentNode->m_token->m_smallCategory)
			{
				/*若为右括号，则弹出操作符，直到遇到左括号*/
			case CTokenCategory::operRightBracket:
			{
				/*清除右括号*/
				delete(currentNode);

				/*入栈*/
				while (listOper.back()->m_token->m_smallCategory != CTokenCategory::operLeftBracket)
				{
					listSuffix.push_back(listOper.back());
					listOper.pop_back();
				}

				/*清除左括号*/
				delete(listOper.back());
				listOper.pop_back();
			}break;

			/*若为其它操作符*/
			default:
			{
				while (listOper.size() > 0 && CTokenCategory::g_operators[currentNode->m_token->m_smallCategory].priority
					<= CTokenCategory::g_operators[listOper.back()->m_token->m_smallCategory].priority)
				{
					/*	CLexer::DisplayToken(listOper.back()->m_token);
						CLexer::DisplayToken(currentNode->m_token);*/
					/*	qDebug() << QString::number(CTokenCategory::g_operators[currentNode->m_token->m_smallCategory].priority);
						qDebug() << QString::number(CTokenCategory::g_operators[listOper.back()->m_token->m_smallCategory].priority);*/

					/*若为左括号*/
					if (currentNode->m_token->m_smallCategory == CTokenCategory::operLeftBracket
						|| listOper.back()->m_token->m_smallCategory == CTokenCategory::operLeftBracket)
					{
						break;
					}
					/*将优先级不小于新入栈操作符的其它操作符弹出*/
					else
					{
						listSuffix.push_back(listOper.back());
						listOper.pop_back();
					}
				}
				listOper.push_back(currentNode);
			}break;
			}
		}break;
		default:
			break;
		}
	}

	while (listOper.size() > 0)
	{
		listSuffix.push_back(listOper.back());
		listOper.pop_back();
	}

	////列出栈内容，该部分仅作测试用
	/*for each (auto var in listSuffix)
	{
	CLexer::DisplayToken(var->m_token);
	}
	*/
	return GetNumTreeFromSuffixList(listSuffix);

	//DesplayTreeNode(rootNode);
}

/*从后缀表达式中获取抽象语法书*/
CAstTreeNode* CParseGrammar::GetNumTreeFromSuffixList(std::list<CAstTreeNode*>& nodeList)
{
	/*for each (auto var in nodeList)
	{
	CLexer::DisplayToken(var->m_token);
	}*/

	CAstTreeNode* rootNode = nodeList.back();	/*根节点*/
	nodeList.pop_back();

	//CLexer::DisplayToken(rootNode->m_token);

	/*若根节点为操作数*/
	if (rootNode->m_token->m_largeCategory != CTokenCategory::tokenOperator)
	{
		return rootNode;
	}
	/*若根节点为操作符*/
	else
	{
		CAstTreeNode* firstChildNode;	/*左子节点*/
		CAstTreeNode* secondChildNode;	/*右子节点*/

		/*获取右子节点*/
		secondChildNode = GetNumTreeFromSuffixList(nodeList);

		/*获取左子节点*/
		firstChildNode = GetNumTreeFromSuffixList(nodeList);

		rootNode->AddChildNode(firstChildNode);
		rootNode->AddChildNode(secondChildNode);
	}

	return rootNode;
}




bool CParseGrammar::MatchLabelSentence(CAstTreeNode* rootNode)
{
	CToken* currentToken = m_tokenList->at(m_tokenPointer);
	CAstTreeNode* labelNode;
	/*若为label命令*/
	if (currentToken->m_largeCategory == CTokenCategory::tokenCommand && currentToken->m_smallCategory == CTokenCategory::commLabel)
	{
		labelNode = new CAstTreeNode(new CToken(currentToken));
		rootNode->AddChildNode(labelNode);
	}

	m_tokenPointer++;
	currentToken = m_tokenList->at(m_tokenPointer);

	/*若为下一词法单元为变量*/
	if (currentToken->m_largeCategory == CTokenCategory::tokenVariant)
	{
		/*令其为label*/
		currentToken->m_largeCategory = CTokenCategory::tokenConstantString;
		currentToken->m_smallCategory = CTokenCategory::constantLable;
		labelNode->AddChildNode(new CAstTreeNode(new CToken(currentToken)));

		m_tokenPointer++;

		if (IsSentenceEnd())
		{
			return true;
		}
	}

	throw(CExceptionInterpreter(CWarningInformation::WRONG_GRAMMAR, currentToken->m_lineNum));
	return false;
}

/*语句结束
 *若到达文件尾，则抛出异常*/
bool CParseGrammar::IsSentenceEnd()
{
	switch (m_tokenList->at(m_tokenPointer)->m_largeCategory)
	{
	case CTokenCategory::tokenSeparator:
	{
		switch (m_tokenList->at(m_tokenPointer)->m_smallCategory)
		{
		case CTokenCategory::sepLineBreak:
		{
			return true;
		}break;
		case CTokenCategory::sepEnd:
		{
			throw(CExceptionInterpreter(CWarningInformation::NOT_MEET_END, m_tokenList->at(m_tokenPointer)->m_lineNum));
		}break;
		default:
			break;
		}
	}break;
	default:
		break;
	}
	return false;
}
/*文件结束*/
bool CParseGrammar::IsFileEnd()
{
	if ((m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepEnd))
	{
		return true;
	}
	return false;
}

void CParseGrammar::SkipBlankLine()
{
	/*若为换行符*/
	while (m_tokenList->at(m_tokenPointer)->m_largeCategory == CTokenCategory::tokenSeparator
		&& m_tokenList->at(m_tokenPointer)->m_smallCategory == CTokenCategory::sepLineBreak)
	{
		m_tokenPointer++;
	}
}

void CParseGrammar::DisplayTreeNode(CAstTreeNode* rootNode)
{
	CAstTreeNode* currentNode = rootNode->m_firstChildNode;
	/*列出子节点*/
	while (currentNode != NULL)
	{
		DisplayTreeNode(currentNode);
		currentNode = currentNode->m_nextSiblingNode;
	}

	/*列出本节点*/
	CLexer::DisplayToken(rootNode->m_token);

}
//////////////////////////////////////////////////////////////////////////