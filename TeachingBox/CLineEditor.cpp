#include "stdafx.h"
#include "CLineEditor.h"
#include "CValue.h"
#include "CTextFormat.h"



CLineEditor::CLineEditor(QWidget* parent/*=0*/) :QWidget(parent)
{
	Init();
}

CLineEditor::CLineEditor(const QString& strText, QWidget* parent /*= 0*/) : CLineEditor(parent)
{

}

CLineEditor::~CLineEditor()
{

}

void CLineEditor::SetAllText(const QString& strText)
{
	InitLabels(strText);
}

void CLineEditor::SetLabelText(const QString& strText)
{
	m_currentLabel->setText(strText);
	SetAllText(GetAllText());
}


QString CLineEditor::GetAllText()
{
	QString strText;
	for (int i = 0;i<m_vectorLabels.size();++i)
	{
		strText.append(m_vectorLabels.at(i)->text());
	}
	return strText;
}

QString CLineEditor::GetLabelText()
{
	return m_currentLabel->text();
}

void CLineEditor::AddLabel(const QString& strLabelText)
{
	int index = m_layoutMain->indexOf(m_currentLabel);

	QString strText;

	if (index == -1)
	{
		strText = GetAllText() + strLabelText;
	}
	else
	{
		m_currentLabel->setText(m_currentLabel->text() + strLabelText);
		strText = GetAllText();
	}

	SetAllText(strText);
}

void CLineEditor::DeleteCurrentLabel()
{
	int index = m_layoutMain->indexOf(m_currentLabel);

	if (index==-1)
	{
		return;
	}

	m_currentLabel->setText("");

	SetAllText(GetAllText());
}

bool CLineEditor::IsOperator(const QString& strOperator)
{
	QStringList strList;
	strList << "OR" << "AND" << "+" << "-" << "*" << "/" << "^" << "&" << "/" << ">=" << "<=" << "==" << "!="<<"<"<<">";

	for each (auto var in strList)
	{
		if (var==strOperator)
		{
			return true;
		}
	}

	return false;
}

CLabelWithSignal* CLineEditor::GetCurrentLabel()
{
	return m_currentLabel;
}

void CLineEditor::Init()
{
	m_layoutMain = new QHBoxLayout(this);
	m_layoutMain->setSpacing(0);
}

void CLineEditor::InitLabels(const QString& strText)
{
	/*清除原有控件*/
	for (int i = 0; i < m_layoutMain->count();++i)
	{
		m_layoutMain->removeWidget(m_layoutMain->itemAt(i)->widget());
		m_layoutMain->itemAt(i)->widget()->deleteLater();
	}
	m_vectorLabels.clear();

	CTextFormat textFormat;
	textFormat.FormatLineText(const_cast<QString&>(strText));

	for (int i = 0; i < strText.size(); ++i)
	{
		switch (strText.at(i).toLatin1())
		{
		case ' ':
		{
			CLabelWithSignal* label = new CLabelWithSignal(strText.at(i));
			m_vectorLabels.append(label);
		}break;
		case '+':case '-':case '*':case '/':case '^':case '&':case '|':case ',':case '(':case ')':
		{
			CLabelWithSignal* label = new CLabelWithSignal(strText.at(i));
			m_vectorLabels.append(label);
		}break;
		case '!':
		{
			switch (strText.at(i + 1).toLatin1())
			{
			case '=':case'>':case'<':
			{
				QString str;
				str.append(strText.at(i)).append(strText.at(i + 1));
				CLabelWithSignal* label = new CLabelWithSignal(str);
				m_vectorLabels.append(label);
				++i;
			}break;
			default:
			{
				CLabelWithSignal* label = new CLabelWithSignal(strText.at(i));
				m_vectorLabels.append(label);
			}break;
			}
		}break;
		case '>':case '<':
		{
			switch (strText.at(i + 1).toLatin1())
			{
			case '=':
			{
				QString str;
				str.append(strText.at(i)).append(strText.at(i + 1));
				CLabelWithSignal* label = new CLabelWithSignal(str);
				m_vectorLabels.append(label);
				++i;
			}break;
			default:
			{
				CLabelWithSignal* label = new CLabelWithSignal(strText.at(i));
				m_vectorLabels.append(label);
			}break;
			}
		}break;
		case '=':
		{
			/*若为==*/
			if (strText.at(i + 1) == '=')
			{
				QString str;
				str.append(strText.at(i)).append(strText.at(i + 1));
				CLabelWithSignal* label = new CLabelWithSignal(str);
				m_vectorLabels.append(label);
				++i;
			}
			else
			{
				CLabelWithSignal* label = new CLabelWithSignal(strText.at(i));
				m_vectorLabels.append(label);
			}
		}break;
		default:
		{
			QString str;
			while ((strText.at(i) >= 'a' && strText.at(i) <= 'z')
				|| (strText.at(i) >= 'A'&& strText.at(i) <= 'Z')
				|| (strText.at(i) >= '0'&&strText.at(i) <= '9')
				|| strText.at(i) == '_')
			{
				str.append(strText.at(i));
				if (++i >= strText.size())
				{
					break;
				}
			}
			CLabelWithSignal* label = new CLabelWithSignal(str);
			m_vectorLabels.append(label);
			--i;
		}break;
		}
	}

	/*添加新控件*/
	for each (auto var in m_vectorLabels)
	{
		/*若为空格*/
		if (var->text()==" " || var->text()=="=")
		{
			var->setEnabled(false);
		}
		else
		{
			/*若为命令*/
			for each (auto commandCode in CValue::GetInstance()->m_mapCommandCode)
			{
				if (var->text().toStdString() == commandCode.first)
				{
					var->setEnabled(false);
				}
			}
		}

		m_layoutMain->addWidget(var);
		connect(var, SIGNAL(SignalClicked()), this, SLOT(SlotOnLabelClicked()));
	}

	CLabelWithSignal* labelNull = new CLabelWithSignal;
	m_layoutMain->addWidget(labelNull);
	m_layoutMain->setStretch(m_layoutMain->count()-1, 1);

	m_currentLabel = m_vectorLabels.at(0);
}

void CLineEditor::SlotOnLabelClicked()
{
	m_currentLabel = static_cast<CLabelWithSignal*>(sender());

	for each (auto var in m_vectorLabels)
	{
		static_cast<CLabelWithSignal*>(var)->setStyleSheet(STYLE_UNCLICKED);
	}
	m_currentLabel->setStyleSheet(STYLE_CLICKED);
}
