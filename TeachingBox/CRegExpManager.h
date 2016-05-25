#ifndef _TEACHING_BOX_REG_EXP_STRING_H_
#define _TEACHING_BOX_REG_EXP_STRING_H_

class CRegExpManager
{
public:
	CRegExpManager();
	~CRegExpManager();

	/*非负整数（正整数 + 0）*/
	static const char* STR_REG_NONEGATIVE_INT ;

	/*正整数*/
	static const char* STR_REG_POSITIVE_INT ;

	/*非正整数（负整数 + 0）*/
	static const char* STR_REG_NOPOSITIVE_INT;

	/*负整数*/
	static const char* STR_REG_NEGATIVE_INT;

	/*整数*/
	static const char* STR_REG_INT;

	/*非负浮点数（正浮点数 + 0）*/
	static const char* STR_REG_NONEGATIVE_FLOAT;

	/*正浮点数*/
	static const char* STR_REG_POSITIVE_FLOAT;

	/*(非正浮点数（负浮点数 + 0）*/
	static const char* STR_REG_NOPOSITIVE_FLOAT;

	/*负浮点数*/
	static const char* STR_REG_NEGATIVE_FLOAT;

	/*浮点数*/
	static const char* STR_REG_FLOAT;

	/*由26个英文字母组成的字符串*/
	static const char* STR_REG_CHAR;

	/*由26个英文字母的大写组成的字符串*/
	static const char* STR_REG_UPPER_CHAR;

	/*由26个英文字母的小写组成的字符串*/
	static const char* STR_REG_LOWER_CHAR;

	/*由数字和26个英文字母组成的字符串*/
	static const char* STR_REG_NUM_CHAR;

	/*由数字、26个英文字母或者下划线组成的字符串*/
	static const char* STR_REG_NUM_CHAR_UNDERLINE;

	/*变量名，首字母为字母或下划线*/
	static const char* STR_REG_STRING_NAME;
protected:
	
private:
};
#endif