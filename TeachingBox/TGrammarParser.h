#ifndef _TEACHING_BOX_T_GRAMMAR_H_
#define _TEACHING_BOX_T_GRAMMAR_H_

#include "TToken.h"
#include "TInterpreterException.h"

class TLexer;
class TAstNode;

class TGrammarParser
{
public:
	static const std::shared_ptr<TAstNode> GetRootNode(TLexer* const lexer);
	static const std::shared_ptr<TAstNode> GetOneNode(TLexer* const lexer);

};

#endif