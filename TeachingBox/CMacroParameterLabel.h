#ifndef _TEACHING_BOX_C_MACRO_PARAMETER_LABEL_H_
#define _TEACHING_BOX_C_MACRO_PARAMETER_LABEL_H_

#include "CMacroParameterParent.h"

class CMacroParameterLabel:public CMacroParameterParent
{
public:
	CMacroParameterLabel(QWidget* parent=0);
	~CMacroParameterLabel();

	void KeyboardEdit();

protected:
	void OnButtonConfirmClicked() Q_DECL_OVERRIDE;
	void OnButtonEditClicked() Q_DECL_OVERRIDE;

private:
	void Init();
	void InitLayout();

	//bool IsCorrectName(QString& strName);

private:
	const char* CLASS_NAME = "CMacroParameterZone";

	QLabel* m_lbParameterName;
};

#endif