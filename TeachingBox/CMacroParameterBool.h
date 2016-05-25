#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_BOOL_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_BOOL_H_

#include "CMacroParameterParent.h"
#include "QComboBox"

class CMacroParameterBool:public CMacroParameterParent
{
	Q_OBJECT

public:
	CMacroParameterBool(QWidget* parent = 0);
	~CMacroParameterBool();

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
	QComboBox* m_cmbParameterValue;
};

#endif