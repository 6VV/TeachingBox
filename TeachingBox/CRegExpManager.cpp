#include "stdafx.h"
#include "CRegExpManager.h"



const char* CRegExpManager::STR_REG_NONEGATIVE_INT = "^\\d + $";

/*������*/
const char* CRegExpManager::STR_REG_POSITIVE_INT = "^[0 - 9] * [1 - 9][0 - 9] * $";

/*���������������� + 0��*/
const char* CRegExpManager::STR_REG_NOPOSITIVE_INT = "^ ((-\\d + ) | (0 + ))$";

/*������*/
const char* CRegExpManager::STR_REG_NEGATIVE_INT = "^-[0 - 9] * [1 - 9][0 - 9] * $";

/*����*/
const char* CRegExpManager::STR_REG_INT = "^-?[0-9]\\d*$";

/*�Ǹ����������������� + 0��*/
const char* CRegExpManager::STR_REG_NONEGATIVE_FLOAT = "^\\d + (\\.\\d + ) ? $";

/*��������*/
const char* CRegExpManager::STR_REG_POSITIVE_FLOAT = "^ (([0 - 9] + \\.[0 - 9] * [1 - 9][0 - 9] * ) | ([0 - 9] * [1 - 9][0 - 9] * \\.[0 - 9] + ) | ([0 - 9] * [1 - 9][0 - 9] * ))$";

/*(�������������������� + 0��*/
const char* CRegExpManager::STR_REG_NOPOSITIVE_FLOAT = "^ ((-\\d + (\\.\\d + ) ? ) | (0 + (\\.0 + ) ? ))$";

/*��������*/
const char* CRegExpManager::STR_REG_NEGATIVE_FLOAT = "^ (-(([0 - 9] + \\.[0 - 9] * [1 - 9][0 - 9] * ) | ([0 - 9] * [1 - 9][0 - 9] * \\.[0 - 9] + ) | ([0 - 9] * [1 - 9][0 - 9] * )))$";

/*������*/
const char* CRegExpManager::STR_REG_FLOAT = "^(-?\\d+)(\\.\\d+)?$";

/*��26��Ӣ����ĸ��ɵ��ַ���*/
const char* CRegExpManager::STR_REG_CHAR = "^[A - Za - z] + $";

/*��26��Ӣ����ĸ�Ĵ�д��ɵ��ַ���*/
const char* CRegExpManager::STR_REG_UPPER_CHAR = "^[A - Z] + $";

/*��26��Ӣ����ĸ��Сд��ɵ��ַ���*/
const char* CRegExpManager::STR_REG_LOWER_CHAR = "^[a - z] + $";

/*�����ֺ�26��Ӣ����ĸ��ɵ��ַ���*/
const char* CRegExpManager::STR_REG_NUM_CHAR = "^[A - Za - z0 - 9] + $";

/*�����֡�26��Ӣ����ĸ�����»�����ɵ��ַ���*/
const char* CRegExpManager::STR_REG_NUM_CHAR_UNDERLINE = "[_a-zA-Z0-9]*";

/*�����������ַ�Ϊ��ĸ���»���*/
const char* CRegExpManager::STR_REG_STRING_NAME = "[_a-zA-Z][_a-zA-Z0-9]*";

CRegExpManager::CRegExpManager()
{

}

CRegExpManager::~CRegExpManager()
{

}

