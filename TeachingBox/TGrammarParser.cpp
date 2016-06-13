#include "stdafx.h"
#include "TGrammarParser.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNode.h"
#include "TAstNodeOperator.h"
#include "TAstNodeAssignSentence.h"
#include "TAstNodeForSentence.h"
#include "TAstNodeRoot.h"

const std::shared_ptr<TAstNode> TGrammarParser::GetRootNode(TLexer* const lexer)
{
	std::shared_ptr<TAstNode> result(new TAstNodeRoot);
	std::shared_ptr<TAstNode> node{};
	while (node = GetOneNode(lexer))
	{
		result->AddChild(node);
	}

	if (lexer->PeekToken()->GetType()!=TToken::SEPARATOR_EOF)
	{
		throw TInterpreterException(TInterpreterException::WRONG_GRAMMAR, lexer->PeekToken()->GetLineNumber());
	}

	return result;
}

const std::shared_ptr<TAstNode> TGrammarParser::GetOneNode(TLexer* const lexer)
{
	auto token = lexer->PeekToken();

	switch (token->GetType())
	{
	case TToken::ID:
	{
		return TAstNodeAssignSentence::GetAstNode(lexer);
	}break;
	case TToken::STRUCTURE_FOR:
	{
		return TAstNodeForSentence::GetAstNode(lexer);
	}break;
	default:
	{
		return nullptr;
	}break;
	}
}

