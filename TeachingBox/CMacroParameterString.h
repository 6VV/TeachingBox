#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_STRING_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_STRING_H_

#include "CMacroParameterParent.h"

class CMacroParameterString:public CMacroParameterParent
{
	Q_OBJECT

public:
	CMacroParameterString(QWidget* parent = 0);
	~CMacroParameterString();

	void KeyboardEdit();

protected:
	void OnButtonConfirmClicked() Q_DECL_OVERRIDE;
	void OnButtonEditClicked() Q_DECL_OVERRIDE;

private:
	void Init();
	void InitLayout();

private:
	const char* CLASS_NAME = "CMacroParameterDouble";

	QLabel* m_lbParameterName;
	QLabel* m_lbParameterValue;
};

#endif