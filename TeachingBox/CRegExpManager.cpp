#include "stdafx.h"
#include "CRegExpManager.h"



const char* CRegExpManager::STR_REG_NONEGATIVE_INT = "^\\d + $";

/*正整数*/
const char* CRegExpManager::STR_REG_POSITIVE_INT = "^[0 - 9] * [1 - 9][0 - 9] * $";

/*非正整数（负整数 + 0）*/
const char* CRegExpManager::STR_REG_NOPOSITIVE_INT = "^ ((-\\d + ) | (0 + ))$";

/*负整数*/
const char* CRegExpManager::STR_REG_NEGATIVE_INT = "^-[0 - 9] * [1 - 9][0 - 9] * $";

/*整数*/
const char* CRegExpManager::STR_REG_INT = "^-?[0-9]\\d*$";

/*非负浮点数（正浮点数 + 0）*/
const char* CRegExpManager::STR_REG_NONEGATIVE_FLOAT = "^\\d + (\\.\\d + ) ? $";

/*正浮点数*/
const char* CRegExpManager::STR_REG_POSITIVE_FLOAT = "^ (([0 - 9] + \\.[0 - 9] * [1 - 9][0 - 9] * ) | ([0 - 9] * [1 - 9][0 - 9] * \\.[0 - 9] + ) | ([0 - 9] * [1 - 9][0 - 9] * ))$";

/*(非正浮点数（负浮点数 + 0）*/
const char* CRegExpManager::STR_REG_NOPOSITIVE_FLOAT = "^ ((-\\d + (\\.\\d + ) ? ) | (0 + (\\.0 + ) ? ))$";

/*负浮点数*/
const char* CRegExpManager::STR_REG_NEGATIVE_FLOAT = "^ (-(([0 - 9] + \\.[0 - 9] * [1 - 9][0 - 9] * ) | ([0 - 9] * [1 - 9][0 - 9] * \\.[0 - 9] + ) | ([0 - 9] * [1 - 9][0 - 9] * )))$";

/*浮点数*/
const char* CRegExpManager::STR_REG_FLOAT = "^(-?\\d+)(\\.\\d+)?$";

/*由26个英文字母组成的字符串*/
const char* CRegExpManager::STR_REG_CHAR = "^[A - Za - z] + $";

/*由26个英文字母的大写组成的字符串*/
const char* CRegExpManager::STR_REG_UPPER_CHAR = "^[A - Z] + $";

/*由26个英文字母的小写组成的字符串*/
const char* CRegExpManager::STR_REG_LOWER_CHAR = "^[a - z] + $";

/*由数字和26个英文字母组成的字符串*/
const char* CRegExpManager::STR_REG_NUM_CHAR = "^[A - Za - z0 - 9] + $";

/*由数字、26个英文字母或者下划线组成的字符串*/
const char* CRegExpManager::STR_REG_NUM_CHAR_UNDERLINE = "[_a-zA-Z0-9]*";

/*变量名，首字符为字母或下划线*/
const char* CRegExpManager::STR_REG_STRING_NAME = "[_a-zA-Z][_a-zA-Z0-9]*";

CRegExpManager::CRegExpManager()
{

}

CRegExpManager::~CRegExpManager()
{

}

