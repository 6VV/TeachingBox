#include "stdafx.h"
#include "TInterpreter.h"
#include "TLexer.h"
#include "TContext.h"
#include "TGrammarParser.h"
#include "TAstNode.h"



void TInterpreter::Interpret(const QString& text)
{
	TLexer lexer(text);
	auto rootNode = TGrammarParser::GetRootNode(&lexer);
	rootNode->ParseSemantic();
	TContext::SetRootNode(rootNode);
	TContext::SetNextNode(rootNode.get());

	Interpret();
}

void TInterpreter::Interpret()
{
	TAstNode* nextNode=nullptr;
	while ((nextNode = TContext::GetNextNode()) != nullptr)
	{
		nextNode->Execute();
	}
}
