#ifndef _TEACHING_BOX_C_MACRO_INTERFACE_H_
#define _TEACHING_BOX_C_MACRO_INTERFACE_H_

class MacroInterface
{
public:
	virtual void EditMacroInterface(const QString& strText) = 0;
	virtual void RemoveLineEditor() = 0;

};

#endif