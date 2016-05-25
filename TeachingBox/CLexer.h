#ifndef _INTERPRETER_C_LEXER_H_
#define _INTERPRETER_C_LEXER_H_

#include <map>
#include <set>
#include <vector>
#include "CToken.h"

class CLexer
{
public:
	CLexer(const std::string& strContext);
	~CLexer();

	const static int NUMBER_INT = 1;
	const static int NUMBER_FLOAT = 2;

	const static int VARIANT_POSITION = 1;
	const static int VARIANT_VELOCITY = 2;
	const static int VARIANT_ZONE = 3;
	const static int VARIANT_UNIVERSAL = 4;

	//void GetTokens(std::vector<CToken*>& tokenList);		//获取文本中的词法单元
	std::vector<CToken*>* GetTokenList();
	bool GetNextToken();

	void SkipBlank();	/*跳过空白字符*/
	inline void Consume();	/*跳过当前字符*/
	void Backspace();	/*返回前一格*/
	bool IsSeparator(char currentChar);	/*是否为分隔符*/
	bool IsNumber(char currentChar);	/*是否为数字*/
	bool IsNumberOrPoint(char currentChar);	/*是否为数字或小数点*/
	int GetNumber(std::string& str);	/*返回数字类型*/
	bool GetString();	/*获取引号内的字符串*/
	bool GetOtherToken(std::string& str);	/*获取其它类型*/
	int GetVariant(std::string& str);	/*返回变量名*/

	static void Test(std::vector<CToken*>& listToken);	/*测试用*/
	static void DisplayToken(CToken* token);

private:
	void Init();

private:
	const std::string* m_pStrContext;	/*文本指针*/
	std::vector<CToken*>* m_pListToken;	/*此法单元列表指针*/

	int m_currentPointer;	/*当前文本指针*/
	char m_currentChar;	/*当前文本字符*/
	int m_currentLine;	/*当前行*/
	//std::string m_strUnknown;	/*未知的字符串*/
};

#endif
