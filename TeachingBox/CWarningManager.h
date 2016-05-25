#ifndef _TEACHING_BOX_C_WARNING_MANAGER_H_
#define _TEACHING_BOX_C_WARNING_MANAGER_H_

#include "QWidget"
#include "CSingleTon.h"

class CWarningManager
{
	friend CSingleTon<CWarningManager>;

public:
	enum WarningCode
	{
		NameIsEmpty,		/*名称为空*/
		NameIsTooLong,		/*名称过长*/
		PleaseSelectScope,	/*请选择作用域*/
		PleaseSelectVariable,	/*请选择变量*/
		AlreadyExitVariable,	/*已存在该变量*/
		AlreadyLoadOtherProject,	/*已加载其它项目*/
		ForbidDelete,		/*禁止删除*/
		NotFindVariable,	/*未找到变量*/
		WrongName,	/*错误的变量名*/
	};

public:
	static CWarningManager* GetInstance();

	void Warning(QWidget* widget, QString strText);

	void Warning(QWidget* widget, WarningCode);

private:
	CWarningManager();
	CWarningManager(const CWarningManager&);
	CWarningManager& operator=(const CWarningManager&);
	~CWarningManager();
	
private:

};

#endif