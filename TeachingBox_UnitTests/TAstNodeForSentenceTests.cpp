#include "TAstNodeForSentenceTests.h"
#include "QString"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TAstNodeForSentence.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TContext.h"
#include "..\TeachingBox\TVariateManager.h"
#include "..\TeachingBox\TInteger.h"
#include "..\TeachingBox\TInterpreter.h"

TEST_F(TAstNodeForSentenceTests, GetAstNode_Equal)
{
	try
	{
		QString text("FOR a=1+2 TO 2+3 STEP 5-4\nb=6+7\nNEXT");
		TLexer lexer(text);
		auto node = TAstNodeForSentence::GetAstNode(&lexer);
		auto child = node->GetFirstChild();

		{
			EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetToken()->GetType());
			auto firstChild = child->GetFirstChild();
			EXPECT_EQ(TToken::ID, firstChild->GetToken()->GetType());
			EXPECT_EQ(TToken::OPERATOR_PLUS, firstChild->GetSibling()->GetToken()->GetType());
		}

		{
			child = child->GetSibling();
			EXPECT_EQ(TToken::OPERATOR_PLUS, child->GetToken()->GetType());
			auto firstChild = child->GetFirstChild();
			EXPECT_EQ(TToken::LITERAL_INTERGER, firstChild->GetToken()->GetType());
			EXPECT_EQ(TToken::LITERAL_INTERGER, firstChild->GetSibling()->GetToken()->GetType());
		}

		{
			child = child->GetSibling();
			EXPECT_EQ(TToken::OPERATOR_MINUS, child->GetToken()->GetType());
			auto firstChild = child->GetFirstChild();
			EXPECT_EQ(TToken::LITERAL_INTERGER, firstChild->GetToken()->GetType());
			EXPECT_EQ(TToken::LITERAL_INTERGER, firstChild->GetSibling()->GetToken()->GetType());
		}

		{
			child = child->GetSibling();
			EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetToken()->GetType());
			auto firstChild = child->GetFirstChild();
			EXPECT_EQ(TToken::ID, firstChild->GetToken()->GetType());
			EXPECT_EQ(TToken::OPERATOR_PLUS, firstChild->GetSibling()->GetToken()->GetType());
		}

		{
			child = child->GetSibling();
			EXPECT_EQ(TToken::STRUCTURE_NEXT, child->GetToken()->GetType());
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeForSentenceTests, ParseSenmatic_NoException)
{
	try
	{
		TContext::SetCurrentScope("SYSTEM");
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "a", 1));
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "b", 1));

		QString text("FOR a=1+2 TO 2+3 STEP 5-4\nb=6+7\nNEXT");
		TLexer lexer(text);
		auto node = TAstNodeForSentence::GetAstNode(&lexer);
		node->ParseSemantic();
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeForSentenceTests, Execute_IntergerPlus_GetRightValue)
{
	try
	{
		TContext::SetCurrentScope("SYSTEM");
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "a", 1));
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "b", 0));

		QString text("FOR a=0 TO 5 STEP 2\nb=b+1\nNEXT");
		TInterpreter::Interpret(text);
		/*TLexer lexer(text);
		auto node = TAstNodeForSentence::GetAstNode(&lexer);
		node->ParseSemantic();
		node->Execute();*/
		EXPECT_EQ(3, static_cast<TInteger*>(TVariateManager::GetInstance()->GetVariateSrollUp("SYSTEM", "b"))->GetValue());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}