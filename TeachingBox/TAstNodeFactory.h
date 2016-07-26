#ifndef _TEACHING_BOX_T_GRAMMAR_H_
#define _TEACHING_BOX_T_GRAMMAR_H_

#include "TToken.h"
#include "TInterpreterException.h"

class TLexer;
class TAstNode;

class TAstNodeFactory
{
public:
	static const std::shared_ptr<TAstNode> CreateAst();

	static const std::shared_ptr<TAstNode> GetOneNode(TLexer* const lexer);

};

#endif