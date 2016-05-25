#ifndef _TEACHING_BOX_C_LINE_EDITOR_H_
#define _TEACHING_BOX_C_LINE_EDITOR_H_

#include "QWidget"
#include "CLabelWithSignal.h"

class CLineEditor:public QWidget
{
	Q_OBJECT

public:
	CLineEditor(const QString& strText,QWidget* parent = 0);
	CLineEditor(QWidget* parent=0);
	~CLineEditor();

	void SetAllText(const QString& strText);
	void SetLabelText(const QString& strText);
	QString GetAllText();
	QString GetLabelText();

	void AddLabel(const QString& strLabel);	/*添加控件*/

	void DeleteCurrentLabel();	/*删除当前选中控件*/

	bool IsOperator(const QString& strOperator);	/*是否是操作符*/

	CLabelWithSignal* GetCurrentLabel();

private:
	void Init();
	
	void InitLabels(const QString& strText);

protected slots:
	void SlotOnLabelClicked();

private:
	const QString STYLE_CLICKED = "background:lightblue";
	const QString STYLE_UNCLICKED = "background:white";

	CLabelWithSignal* m_currentLabel;
	QVector<CLabelWithSignal*> m_vectorLabels;
	QHBoxLayout* m_layoutMain;

};

#endif