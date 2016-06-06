#ifndef _INTERPRETER_C_SEMANTIC_PARSE_H_
#define _INTERPRETER_C_SEMANTIC_PARSE_H_

#include "CAstTreeNode.h"
#include "CValue.h"
#include "CParseGrammar.h"
#include <list>

class CParseSemantic
{
public:
	//CParseSemantic(CAstTreeNode* rootNode);
	CParseSemantic(CParseGrammar& parseGrammar);
	~CParseSemantic();

	CAstTreeNode* GetAstTreeRootNode();

private:
	void ParseAll();	/*语义解析*/
	
	//void ParseFunction(CAstTreeNode* rooktNode);	/*解析函数*/
	void ParseSentenceGosub(CAstTreeNode* rootNode);/*解析Gosub语句*/

	void ParseSentence(CAstTreeNode* rootNode);		/*解析语句*/

	void ParseSentenceAssignExp(CAstTreeNode* rootNode);	/*解析赋值语句*/
	void ParseSentenceIf(CAstTreeNode* rootNode);			/*解析If语句*/
	void ParseSentenceThen(CAstTreeNode* rootNode);			/*解析Then语句*/
	void ParseSentenceElseIf(CAstTreeNode* rootNode);		/*解析ElseIf语句*/
	void ParseSentenceFor(CAstTreeNode* rootNode);			/*解析For语句*/
	void ParseSentenceSwitch(CAstTreeNode* rootNode);		/*解析Switch语句*/
	void ParseSentenceCase(CAstTreeNode* rootNode);			/*解析Case语句*/
	void ParseSentenceDefault(CAstTreeNode* rootNode);		/*解析Default语句*/

	void ParseSentenceMovl(CAstTreeNode* rootNode);		/*解析Movl语句*/
	void ParseSentenceMovc(CAstTreeNode* rootNode);		/*解析Movc语句*/

	void ParseSentenceLabel(CAstTreeNode* rootNode);	/*解析label语句*/

	int GetExpType(CAstTreeNode* rootNode);	/*返回表达式类型*/
	
	void DisplayType(int type);

private:
	CAstTreeNode* m_rootNode;
	CValue* m_value;
	std::string m_scopeName;
	CScope* m_scope;

	std::set<std::string> m_setFuntion;
	bool m_isFindMainFunction=false;
};

#endif