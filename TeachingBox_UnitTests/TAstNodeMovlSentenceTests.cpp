#include "TAstNodeMovlSentenceTests.h"
#include "QString"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TAstNodeMovlSentence.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TContext.h"
#include "..\TeachingBox\TVariateManager.h"
#include "..\TeachingBox\TPosition.h"
#include "..\TeachingBox\TDynamic.h"
#include "..\TeachingBox\TOverlap.h"


TEST_F(TAstNodeMovlSentenceTests, GetAstNode_Equal)
{
	try
	{
		QString text("MOVL p,d,ov");
		TLexer lexer(text);

		auto node = TAstNodeMovlSentence::GetAstNode(&lexer);
		EXPECT_EQ(TToken::MACRO_MOVL, node->GetToken()->GetType());

		node = node->GetFirstChild();
		EXPECT_EQ(TToken::ID, node->GetToken()->GetType());

		node = node->GetSibling();
		EXPECT_EQ(TToken::ID, node->GetToken()->GetType());

		node = node->GetSibling();
		EXPECT_EQ(TToken::ID, node->GetToken()->GetType());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeMovlSentenceTests, ParseSemantic_NoException)
{
	try
	{
		TContext::SetCurrentScope("SYSTEM");
		TVariateManager::GetInstance()->Add(new TPosition("SYSTEM", "p", tAxesAllPositions{}));
		TVariateManager::GetInstance()->Add(new TDynamic("SYSTEM", "d", tDynamicConstraint{}));
		TVariateManager::GetInstance()->Add(new TOverlap("SYSTEM", "ov", tOverlapConstraint{}));

		QString text("MOVL p,d,ov");
		TLexer lexer(text);
		TAstNodeMovlSentence::GetAstNode(&lexer)->ParseSemantic();

		TVariateManager::GetInstance()->Delete("SYSTEM", "p");
		TVariateManager::GetInstance()->Delete("SYSTEM", "d");
		TVariateManager::GetInstance()->Delete("SYSTEM", "ov");
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}