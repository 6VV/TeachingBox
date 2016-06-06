#include "TLexerTests.h"
#include "..\TeachingBox\TLexer.h"
#include "..\TeachingBox\TToken.h"
#include "..\TeachingBox\TInterpreterException.h"
#include "..\TeachingBox\TTokenWithValue.h"

TEST_F(TLexerTests, Parse_ParseDigit_Equal)
{
	try
	{
		QString text("123  231\t1.232");
		TLexer lexer(text);
		double num[3][2]{{TToken::LITERAL_INTERGER, 123},
						{ TToken::LITERAL_INTERGER, 231 }, 
						{ TToken::LITERAL_DOUBLE, 1.232 }};

		auto ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), num[0][0]) << "Not Interger";
		EXPECT_EQ(static_cast<TTokenWithValue<int>*>(ptr.get())->GetValue(), num[0][1]);

		ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), num[1][0]) << "Not Interger";
		EXPECT_EQ(static_cast<TTokenWithValue<int>*>(ptr.get())->GetValue(), num[1][1]);

		ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), num[2][0]) << "Not Double";
		EXPECT_EQ(static_cast<TTokenWithValue<double>*>(ptr.get())->GetValue(), num[2][1]);
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString();
	}
}

TEST_F(TLexerTests, Parse_ParseId_Equal)
{
	try{
		QString text("abc _de d21");
		TLexer lexer(text);
		QStringList strList{ "abc", "_de", "d21" };

		auto ptr = lexer.GetToken();
		int index = 0;
		while (ptr)
		{
			EXPECT_EQ(ptr->GetType(), TToken::ID) << "Not ID";
			EXPECT_STREQ(static_cast<TTokenWithValue<QString>*>(ptr.get())->GetValue().toStdString().c_str(), strList.at(index).toStdString().c_str());
			++index;
			ptr = lexer.GetToken();
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString();
	}
}

TEST_F(TLexerTests, Parse_ParseKeyWord_Equal)
{
	try{
		QString text("TRUE IF NEXT");
		TLexer lexer(text);
		QStringList strList{ "TRUE", "IF", "NEXT" };

		auto ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), TToken::VALUE_TRUE) << "Not TRUE";

		ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), TToken::STRUCTURE_IF) << "Not IF";

		ptr = lexer.GetToken();
		EXPECT_EQ(ptr->GetType(), TToken::STRUCTURE_NEXT) << "Not NEXT";

	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString();
	}
}

TEST_F(TLexerTests, Parse_ParseOtherWord_Equal)
{
	try
	{
		QString text{ "* + - /    ^ % \n &    | !> !< !=    > >= < <=    = == ( )" };
		int test[]{TToken::OPERATOR_MULTIPLY, TToken::OPERATOR_PLUS, TToken::OPERATOR_MINUS,TToken::OPERATOR_DIVIDE,
			TToken::OPERATOR_POWER, TToken::OPERATOR_PERCENT,TToken::SEPARATOR_EOL, TToken::OPERATOR_AND,
			TToken::OPERATOR_OR,TToken::OPERATOR_LESS_EQUAL, TToken::OPERATOR_GREATE_EQUAL, TToken::OPERATOR_NOT_EQUAL,
			TToken::OPERATOR_GREATE_THAN, TToken::OPERATOR_GREATE_EQUAL, TToken::OPERATOR_LESS_THAN,TToken::OPERATOR_LESS_EQUAL, 
			TToken::OPERATOR_ASSIGN, TToken::OPERATOR_EQUAL,TToken::OPERATOR_LEFT_BRACKET, TToken::OPERATOR_RIGHT_BRACKET};

		TLexer lexer(text);
		int index = 0;
		while (auto ptr = lexer.GetToken())
		{
			EXPECT_EQ(ptr->GetType(), test[index]);
			++index;
		}
	}
	catch (TInterpreterException& e)
	{
		FAIL() << e.GetInfo().toStdString();
	}

}