﻿#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_ZONE_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_ZONE_H_

#include "CMacroParameterParent.h"

class CMacroParameterZone:public CMacroParameterParent
{
	Q_OBJECT

public:
	CMacroParameterZone(QWidget* parent=0);
	~CMacroParameterZone();

	void KeyboardEdit();

protected:
	void OnButtonConfirmClicked() Q_DECL_OVERRIDE;
	void OnButtonEditClicked() Q_DECL_OVERRIDE;

private:
	void Init();
	void InitLayout();

	//bool IsCorrectName(QString& strName);
	bool IsCorrectValue(QString& strValue);

private:
	const char* CLASS_NAME = "CMacroParameterZone";

	QLabel* m_lbParameterName;
	QLabel* m_lbParameterValue;
};

#endif