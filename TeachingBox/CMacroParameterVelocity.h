#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_VELOCITY_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_VELOCITY_H_

#include "CMacroParameterParent.h"

class CMacroParameterVelocity:public CMacroParameterParent
{
	Q_OBJECT

public:
	CMacroParameterVelocity(QWidget* widget=0);
	~CMacroParameterVelocity();

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
	const char* CLASS_NAME = "CMacroParameterVelocity";

	QLabel* m_lbParameterName;
	QLabel* m_lbParameterValue;
};

#endif