#include "TAstNodeIfSentenceTests.h"
#include "QString"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TAstNodeIfSentence.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TVariateManager.h"
#include "..\TeachingBox\TInteger.h"

TEST_F(TAstNodeIfSentenceTests, GetAstNode_CheckEndIf_Equal)
{
	try
	{
		QString text("IF a>1\nb=1\nENDIF");
		TLexer lexer(text);
		auto node = TAstNodeIfSentence::GetAstNode(&lexer);

		auto child = node->GetFirstChild();
		EXPECT_EQ(TToken::OPERATOR_GREATE_THAN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STURCTURE_END_IF, child->GetToken()->GetType());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
	
}

TEST_F(TAstNodeIfSentenceTests, GetAstNode_CheckEndIfAddElse_Equal)
{
	try
	{
		QString text("IF a>1\n"
			"b=1\n"
			"ELSE\n"
			"b=2\n"
			"ENDIF");
		TLexer lexer(text);
		auto node = TAstNodeIfSentence::GetAstNode(&lexer);

		auto child = node->GetFirstChild();
		EXPECT_EQ(TToken::OPERATOR_GREATE_THAN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STURCTURE_ELSE, child->GetToken()->GetType());

		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetFirstChild()->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STURCTURE_END_IF, child->GetToken()->GetType());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}

}

TEST_F(TAstNodeIfSentenceTests, GetAstNode_CheckEndIf_Else_ElseIf_Equal)
{
	try
	{
		QString text("IF a>10\n"
			"b=1\n"
			"ELSEIF a>5\n"
			"b=2\n"
			"ELSEIF a<1\n"
			"b=3\n"
			"ELSE\n"
			"b=5\n"
			"ENDIF");
		TLexer lexer(text);
		auto node = TAstNodeIfSentence::GetAstNode(&lexer);

		auto child = node->GetFirstChild();
		EXPECT_EQ(TToken::OPERATOR_GREATE_THAN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STRUCTURE_ELSE_IF, child->GetToken()->GetType());
		EXPECT_EQ(TToken::OPERATOR_GREATE_THAN, child->GetFirstChild()->GetToken()->GetType());
		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetFirstChild()->GetSibling()->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STRUCTURE_ELSE_IF, child->GetToken()->GetType());
		EXPECT_EQ(TToken::OPERATOR_LESS_THAN, child->GetFirstChild()->GetToken()->GetType());
		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetFirstChild()->GetSibling()->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STURCTURE_ELSE, child->GetToken()->GetType());

		EXPECT_EQ(TToken::OPERATOR_ASSIGN, child->GetFirstChild()->GetToken()->GetType());

		child = child->GetSibling();
		EXPECT_EQ(TToken::STURCTURE_END_IF, child->GetToken()->GetType());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}

}

TEST_F(TAstNodeIfSentenceTests, ParseSemantic_CheckEndIf_Else_ElseIf_NoException)
{
	try
	{
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "a", 1));
		TVariateManager::GetInstance()->Add(new TInteger("SYSTEM", "b", 0));

		QString text("IF a>10\n"
			"b=1\n"
			"ELSEIF a>5\n"
			"b=2\n"
			"ELSEIF a<1\n"
			"b=3\n"
			"ELSE\n"
			"b=5\n"
			"ENDIF");
		TLexer lexer(text);
		auto node = TAstNodeIfSentence::GetAstNode(&lexer);
		node->ParseSemantic();

		TVariateManager::GetInstance()->Delete("SYSTEM", "a");
		TVariateManager::GetInstance()->Delete("SYSTEM", "b");
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}

}