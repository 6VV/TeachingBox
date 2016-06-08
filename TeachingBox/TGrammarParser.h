#ifndef _TEACHING_BOX_T_GRAMMAR_H_
#define _TEACHING_BOX_T_GRAMMAR_H_

#include "TToken.h"
#include "TInterpreterException.h"

class TLexer;
class TAstNode;

class TGrammarParser
{
public:
	TGrammarParser(TLexer& lexer);

	std::shared_ptr<TAstNode> GetRootNode() const;

private:
	std::shared_ptr<TAstNode> GetNode(const std::shared_ptr<TToken>& token);
	std::shared_ptr<TAstNode> GetExpressionNode();
	std::shared_ptr<TAstNode> GetValue();
	std::shared_ptr<TAstNode> GetShiftOperator(const std::shared_ptr<TAstNode>& leftValue, const std::shared_ptr<TAstNode>& leftOper);

	const bool IsRightExpr(const std::shared_ptr<TAstNode>& leftOper, const std::shared_ptr<TAstNode>& rightOper) const;

	void Parse();
	void ParseAssignSentence();

	std::shared_ptr<TAstNode> PeekOperator();

	void ThrowException(const TInterpreterException::InterpreterExceptionCode exceptionCode, const int lineNubmer);
private:
	std::shared_ptr<TAstNode> m_rootNode;
	TLexer* m_lexer=nullptr;
};

#endif