#ifndef _TEACHING_BOX_C_LINE_EDIT_WITH_CLICKED_SIGNAL_H_
#define _TEACHING_BOX_C_LINE_EDIT_WITH_CLICKED_SIGNAL_H_

#include "QLineEdit"
#include "QLabel"
#include "QTableWidget"
#include "CKeyBoardInterface.h"
#include "QTreeWidget"
#include "CTreeWidgetWithMonitor.h"

class CLineEditWithClickedSignal:public QLineEdit,public CKeyBoardInterface
{
	Q_OBJECT

public:
	CLineEditWithClickedSignal(QWidget* parent=0);
	CLineEditWithClickedSignal(const QString& str, QWidget* parent = 0);
	CLineEditWithClickedSignal(const QString& str, const QString& strRegExp, QWidget* parent = 0);
	~CLineEditWithClickedSignal();

	void KeyboardEdit();
	void SetRegExpString(const QString& strText);
	void SetKeyboardShowModel(const int showModel);	/*设置键盘显示方式*/
	void SetTreeWidgetItem(QTreeWidgetItem* item,CTreeWidgetWithMonitor*);

signals:
	void clicked();
	void textChangedWithTreeItem(QTreeWidgetItem*);

protected slots:
	void SlotTextChanged();

protected:
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

public:
	/*显示模式*/
	enum KEYBOARD_SHOW_MODEL
	{
		KEYBOARD_SHOW_TEXT,	/*显示文本*/
		KEYBOARD_NOT_SHOW_TEXT,	/*不显示文本*/
	};

private:
	int m_keyboardShowType = KEYBOARD_SHOW_TEXT;
	QString m_strRegExp;

	QTreeWidgetItem* m_treeWidgetItem;	/*所在树形节点*/
};

#endif