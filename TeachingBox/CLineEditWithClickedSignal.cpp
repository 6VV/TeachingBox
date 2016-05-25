#include "stdafx.h"
#include "CLineEditWithClickedSignal.h"
#include "CKeyBoard.h"
#include "CWarningManager.h"

CLineEditWithClickedSignal::CLineEditWithClickedSignal(const QString& str, QWidget* parent /*= 0*/) :QLineEdit(str,parent)
{
}

CLineEditWithClickedSignal::CLineEditWithClickedSignal(QWidget* parent/*=0*/) : QLineEdit(parent)
{

}

CLineEditWithClickedSignal::CLineEditWithClickedSignal(const QString& str, const QString& strRegExp, QWidget* parent /*= 0*/) : QLineEdit(str,parent)
{
	m_strRegExp = strRegExp;
}

CLineEditWithClickedSignal::~CLineEditWithClickedSignal()
{
}

void CLineEditWithClickedSignal::KeyboardEdit()
{
	QString strText = CKeyboard::GetInstance()->GetCurrentText();

	/*若正则表达式为空*/
	if (m_strRegExp.isEmpty())
	{
		this->setText(strText);
		return;
	}

	QRegExp regExp(m_strRegExp);
	if (regExp.exactMatch(strText))
	{
		this->setText(strText);
	}
	else
	{
		CWarningManager::GetInstance()->Warning(this, QString("not match"));
	}

}

void CLineEditWithClickedSignal::SetRegExpString(const QString& strText)
{
	m_strRegExp = strText;
}

void CLineEditWithClickedSignal::SetKeyboardShowModel(const int showModel)
{
	m_keyboardShowType = showModel;
}

void CLineEditWithClickedSignal::SetTreeWidgetItem(QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget)
{
	m_treeWidgetItem=item;
	connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(SlotTextChanged()));
	connect(this, SIGNAL(textChangedWithTreeItem(QTreeWidgetItem*)), treeWidget, SLOT(SlotItemTextChanged(QTreeWidgetItem*)));
}

void CLineEditWithClickedSignal::SetTreeItem(QTreeWidgetItem* const item, QTreeWidget* const treeWidget)
{
	m_treeWidgetItem = item;
	m_treeWidget = treeWidget;
}

QTreeWidgetItem* const CLineEditWithClickedSignal::GetTreeWidgetItem() const
{
	return m_treeWidgetItem;
}

QTreeWidget* const CLineEditWithClickedSignal::GetTreeWidget() const
{
	return m_treeWidget;
}

void CLineEditWithClickedSignal::SlotTextChanged()
{
	emit textChangedWithTreeItem(m_treeWidgetItem);
}

void CLineEditWithClickedSignal::mousePressEvent(QMouseEvent *event)
{
	/*emit clicked();*/
	this->selectAll();
	CKeyboard* keyboard = CKeyboard::GetInstance();
	if (m_keyboardShowType==KEYBOARD_SHOW_TEXT)
	{
		keyboard->SetCurrentText(this->text());
	}
	else if (m_keyboardShowType == KEYBOARD_NOT_SHOW_TEXT)
	{
	}
	keyboard->SetKeyboardInterface(this);
	keyboard->show();
}
