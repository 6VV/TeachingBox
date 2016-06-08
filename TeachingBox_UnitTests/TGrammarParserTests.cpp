#include "TGrammarParserTests.h"
#include "QString"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TAstNode.h"
#include "..\TeachingBox\TGrammarParser.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TTokenWithValue.h"

TEST_F(TGrammarParserTests, ParseAssignSentence_MatchToken_Equal)
{
	try
	{
		//a=1+2*3-4/5|6^7&8*(-9+10)>1==2<3
		QString text("a=1+2*3-4/5|6^7&8*(-9+10)>1==2<3");
		TLexer lexer(text);
		TGrammarParser grammarParser(lexer);

		EXPECT_EQ(1, 1);
		auto node = grammarParser.GetRootNode();

		auto assignSentence = node->GetFirstChild();
		EXPECT_EQ(assignSentence->GetToken()->GetType(), TToken::OPERATOR_ASSIGN);

		auto leftId = assignSentence->GetFirstChild();
		EXPECT_EQ(leftId->GetToken()->GetType(), TToken::ID);
		EXPECT_STREQ(static_cast<TTokenWithValue<QString>*>(leftId->GetToken().get())->GetValue().toStdString().c_str(), "a");
		auto rightOper = leftId->GetSibling();
		EXPECT_EQ(TToken::OPERATOR_OR, rightOper->GetToken()->GetType());
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}

}

