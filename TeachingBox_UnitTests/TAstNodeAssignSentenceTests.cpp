#include "TAstNodeAssignSentenceTests.h"
#include "QString"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TAstNodeAssignSentence.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TVariateManager.h"
#include "..\TeachingBox\TInteger.h"
#include "..\TeachingBox\TContext.h"
#include "..\TeachingBox\TInterpreter.h"

TEST_F(TAstNodeAssignSentenceTests, ParseSemantic_NoException)
{
	try
	{
		TContext::SetCurrentScope("SYSTEM");
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "a", 1));
		QString text("a=1+2-3/4");
		TLexer lexer(text);
		auto node = TAstNodeAssignSentence::GetAstNode(&lexer);
		node->ParseSemantic();

		TVariateManager::GetInstance()->Delete("SYSTEM", "a");

	}
	catch (TInterpreterException&e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeAssignSentenceTests, Execute_NoException)
{
	try
	{
		TContext::SetCurrentScope("SYSTEM");
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "b", 1));
		QString text("b=b+2-3/4");
		/*TLexer lexer(text);
		auto node = TAstNodeAssignSentence::GetAstNode(&lexer);
		node->ParseSemantic();
		node->Execute();*/
		TInterpreter interpreter;
		interpreter.Interpret(text);

		EXPECT_EQ(3, static_cast<TInteger*>(TVariateManager::GetInstance()->GetVariateSrollUp("SYSTEM", "b"))->GetValue());

		TVariateManager::GetInstance()->Delete("SYSTEM", "b");
	}
	catch (TInterpreterException&e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}
