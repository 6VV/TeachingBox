#include "stdafx.h"
#include "CMacroParameterParent.h"

CMacroParameterParent::CMacroParameterParent(QWidget* parent/*=0*/)
{
	Init();
}

CMacroParameterParent::~CMacroParameterParent()
{

}

void CMacroParameterParent::SetOperType(int operType)
{
	m_operType = operType;
}

void CMacroParameterParent::SetOldParameterName(QString strOldParameterName)
{
	m_oldParameterName = strOldParameterName;
}

void CMacroParameterParent::RefreshText()
{
	m_btnConfirm->setText(QCoreApplication::translate(CLASS_NAME, "Confirm"));
	m_btnEdit->setText(QCoreApplication::translate(CLASS_NAME, "Edit"));
	m_btnBack->setText(QCoreApplication::translate(CLASS_NAME, "Back"));

	QStringList strListHeader;
	strListHeader.append(QCoreApplication::translate(CLASS_NAME, "Name"));
	strListHeader.append(QCoreApplication::translate(CLASS_NAME, "Value"));
	m_tableWidget->setHorizontalHeaderLabels(strListHeader);
}

void CMacroParameterParent::resizeEvent(QResizeEvent *event)
{
	m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_tableWidget->setColumnWidth(0, m_tableWidget->width() / 2);
	m_tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void CMacroParameterParent::SlotOnButtonConfirmClicked()
{
	/*获取变量名*/
	QLabel* lbParameterName = static_cast<QLabel*>(m_tableWidget->cellWidget(0, 1));
	m_newParameterName = lbParameterName->text();

	/*若变量名为空*/
	if (m_newParameterName.isEmpty())
	{
		QMessageBox::warning(this,
			QCoreApplication::translate(CLASS_NAME, "Warning"),
			QCoreApplication::translate(CLASS_NAME, "The name is empty"),
			QMessageBox::Cancel);
		return;
	}

	OnButtonConfirmClicked();
}

//void CMacroParameterParent::SlotOnButtonCancleClicked()
//{
//	CScreenMain::GetInstance()->ChangeToScreenEditMacro();
//}

void CMacroParameterParent::SlotOnButtonEditClicked()
{
	OnButtonEditClicked();
}

bool CMacroParameterParent::IsCorrectName(QString& strName)
{
	if (strName.size() > VAR_NAME_MAX_LENGTH)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "The length is too long"));
		return false;
	}
	if (!CInterpreterAdapter::GetInstance()->IsCorrectName(strName.toStdString()))
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Wrong name"));
		return false;
	}
	return true;
}

void CMacroParameterParent::Init()
{
	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CMacroParameterParent::InitLayout()
{
	m_tableWidget = new QTableWidget;

	/*按钮列表*/
	m_btnConfirm = new CButton;
	m_btnEdit = new CButton;
	CButton* btnNull1 = new CButton;
	CButton* btnNull2 = new CButton;
	CButton* btnNull3 = new CButton;
	CButton* btnNull4 = new CButton;
	//m_btnBack = new CButton;

	btnNull1->setEnabled(false);
	btnNull2->setEnabled(false);
	btnNull3->setEnabled(false);
	btnNull4->setEnabled(false);

	m_btnList.append(m_btnConfirm);
	m_btnList.append(m_btnEdit);
	m_btnList.append(btnNull1);	
	m_btnList.append(btnNull2);
	m_btnList.append(btnNull3);
	m_btnList.append(btnNull4);
	m_btnList.append(m_btnBack);

	m_screenBottom->AddButtonList(m_btnList);
	m_layoutCenter->addWidget(m_tableWidget);
}

void CMacroParameterParent::InitSignalSlot()
{
	connect(m_btnConfirm, SIGNAL(clicked()), this, SLOT(SlotOnButtonConfirmClicked()));
	//connect(m_btnBack, SIGNAL(clicked()), this, SLOT(SlotOnButtonCancleClicked()));
	connect(m_btnEdit, SIGNAL(clicked()), this, SLOT(SlotOnButtonEditClicked()));
}

int CMacroParameterParent::m_operType=0;

QString CMacroParameterParent::m_newParameterName="";

QString CMacroParameterParent::m_oldParameterName="";
