#ifndef _INTERPRETER_C_PARSE_H_
#define _INTERPRETER_C_PARSE_H_

#include "CToken.h"
#include "CAstTreeNode.h"
#include "CLexer.h"

class CParseGrammar
{
public:
	//CParseGrammar(std::vector<CToken*>& tokenList);
	CParseGrammar(CLexer& lexer,CAstTreeNode* rootNode);
	~CParseGrammar();

	//void GetTree(CAstTreeNode* rootNode);
	CAstTreeNode* GetAstTreeRootNode();
	void MatchSentece(CAstTreeNode* rootNode);			/*匹配语句*/

private:
	const int TYPE_INT = 1;		/*整数*/
	const int TYPE_FLOAT = 2;	/*浮点数*/
	const int TYPE_STRING = 3;	/*字符串*/
	const int TYPE_BOOL = 4;	/*布尔变量*/

private:
	void Init();

	//void MatchAllSentences();		/*匹配所有语句*/
	//void MatchFunctionMainSentence(CAstTreeNode* rootNode);	/*匹配主函数*/
	//void MatchFunctionSentence(CAstTreeNode* rootNode);	/*匹配其它函数*/
	void MatchEofSentence(CAstTreeNode* rootNode);		/*匹配Eof语句*/
	void MatchGosubSentence(CAstTreeNode* rootNode);	/*匹配Gosub语句*/

	void MatchAssignSentence(CAstTreeNode* rootNode);	/*匹配赋值语句*/

	void MatchAssignExp(CAstTreeNode* rootNode);		/*匹配赋值表达式*/
	void MatchExp(CAstTreeNode* rootNode);				/*匹配算数表达式*/
	void MatchFunctionExp(CAstTreeNode* rootNode);		/*匹配函数表达式*/
	void MatchBracketExp(CAstTreeNode* rootNode);		/*匹配括号表达式*/

	void MatchIfSentence(CAstTreeNode* rootNode);		/*匹配If语句*/
	void MatchElseIfSentence(CAstTreeNode* rootNode);	/*匹配elseif语句*/
	void MatchElseSentence(CAstTreeNode* rootNode);		/*匹配else语句*/
	void MatchEndIfSentence(CAstTreeNode* rootNode);	/*匹配Endif语句*/

	void MatchForSentence(CAstTreeNode* rootNode);		/*匹配For语句*/

	void MatchSwitchSentence(CAstTreeNode* rootNode);	/*匹配Switch语句*/
	void MatchEndSwitchSentence(CAstTreeNode* rootNode);/*匹配endswitch语句*/
	void MatchCaseSentence(CAstTreeNode* rootNode);		/*匹配case语句*/
	void MatchDefaultSentence(CAstTreeNode* rootNode);	/*匹配default语句*/

	void MatchBreakSentence(CAstTreeNode* rootNode);	/*匹配Break语句*/
	void MatchRemSentence(CAstTreeNode* rootNode);		/*匹配注释语句*/
	void MatchEndSentence(CAstTreeNode* rootNode);		/*匹配end语句*/

	void MatchMovlSentence(CAstTreeNode* rootNode);		/*匹配Movl语句*/
	void MatchMovcSentence(CAstTreeNode* rootNode);		/*匹配Movc语句*/

	void GetNumList(CAstTreeNode* rootNode);			/*获取数值表达式列表*/
	CAstTreeNode* GetNumTree(std::list<CAstTreeNode*>& nodeList);	/*获取数值表达式树*/
	CAstTreeNode* GetNumTreeFromSuffixList(std::list<CAstTreeNode*>& nodeList);	/*根据后缀表达式建立抽象语法树*/

	bool MatchLabelSentence(CAstTreeNode* rootNode);	/*匹配label语句*/

	bool IsSentenceEnd();	/*判断是否到达行尾，若到达文件尾则抛出异常*/
	bool IsFileEnd();		/*判断是否到达文件尾*/

	void SkipBlankLine();	/*跳过空白行*/

	//static bool matchMovl();				//匹配movl语句
	//static bool matchMOVS();				//匹配movj语句
	//static bool matchMovc();				//匹配movc语句
	//static bool matchDin();					//匹配din语句
	//static bool matchDout();				//匹配dout语句
	//static bool matchAin();					//匹配ain语句
	//static bool matchAout();				//匹配aout语句
	//static bool matchDwellSentence();		//匹配dwell语句
	//static bool matchInputSentence();		//匹配input语句
	//static bool matchSet();					//匹配set语句
	//static bool matchSetAxisOverride();		//匹配setAxisOverride语句
	//static bool matchSetGroupOverride();	//匹配setGroupOverride语句
	//static bool matchVecExp();				//匹配vec表达式

	//static bool matchGotoGosubSentence();	//匹配goto、gosub语句
	//static bool matchEndSentence();			//匹配end语句
	//static bool matchSubProgram();			//匹配子程序
	//static bool matchReturnSentence();		//匹配return语句

	static void DisplayTreeNode(CAstTreeNode* rootNode);	/*列出树节点内容*/
private:
	CAstTreeNode* m_rootNode;	/*语法树根节点*/
	std::vector<CToken*>* m_tokenList;	/*词法单元列表*/
	std::vector<CToken*>* m_temporaryTokenList;	/*临时词法单元列表*/

	bool m_meetEof = false;	/*是否遇到eof语句*/

	int m_tokenPointer = 0;	/*当前token指针*/
	int m_lastTokenPointer = 0;	/*前一语法对应指针*/
	int m_numOfLoopOrSwitch = 0;	/*用于统计循环或开关的嵌套层数*/
};

#endif