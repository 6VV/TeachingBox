#include "TAstNodeOperatorTests.h"
#include "..\TeachingBox\TAstNodeOperator.h"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\CSymbol.h"

TEST_F(TAstNodeOperatorTests, GetAstNode_Equal)
{
	try
	{
		//a=1+2*3-4/5|6^7&8*(-9+10)>1==2<3

		QStringList textList{ "1+2*3-4/(5|6)^7&8*(-9+10)>1==2<3", "1+2*3-4/5|6^7&8*(-9+10)>1==2<3",
			"1+2*3-4/(5|6)" };
		int id[3]{TToken::OPERATOR_AND, TToken::OPERATOR_OR, TToken::OPERATOR_MINUS};

		for (int i = 0; i < 3;++i)
		{
			TLexer lexer(textList.at(i));
			auto node = TAstNodeOperator::GetAstNode(&lexer);
			EXPECT_EQ(id[i], node->GetToken()->GetType());
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeOperatorTests, GetSymbolType_Equal)
{
	try
	{
		//a=1+2*3-4/5|6^7&8*(-9+10)>1==2<3

		QStringList textList{ "1+2*3-4/5<6^7&2>8*(-9+10)", "1+2*3-4/5>1|6^7>2-8/(2*9)",
			"1+2*3-4/(5)", "1.0-2*3" };
		int id[4]{CSymbol::TYPE_BOOL, CSymbol::TYPE_BOOL, CSymbol::TYPE_INTERGER, CSymbol::TYPE_DOUBLE};

		for (int i = 0; i < 4; ++i)
		{
			TLexer lexer(textList.at(i));
			auto node = TAstNodeOperator::GetAstNode(&lexer);
			EXPECT_EQ(id[i], TAstNodeOperator::GetSymbolType(node));
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}

TEST_F(TAstNodeOperatorTests, Execute_Equal)
{
	try
	{
		//a=1+2*3-4/5|6^7&8*(-9+10)>1==2<3

		QStringList textList{ "1+2*3-4/5", "2-8%(2*9)",
			"1+2*3-4/5>1|6^7>2-8/(2*9)", "1.0-2*3" };
		TAstNodeOperator::ValueReturned id[4]{{1 + 2 * 3 - 4 / 5, CSymbol::TYPE_INTERGER}, { 2 - 8 % (2 * 9), CSymbol::TYPE_INTERGER },
		{ 1 + 2 * 3 - 4 / 5>1 || 6 ^ 7>2 - 8 / (2 * 9), CSymbol::TYPE_BOOL }, { 1.0 - 2 * 3, CSymbol::TYPE_DOUBLE }};

		for (int i = 0; i < 4; ++i)
		{
			TLexer lexer(textList.at(i));
			auto node = TAstNodeOperator::GetAstNode(&lexer);
			TAstNodeOperator::GetSymbolType(node);

			EXPECT_EQ(id[i].value, node->Execute().value);
			EXPECT_EQ(id[i].type, node->Execute().type);
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString().c_str();
	}
}
