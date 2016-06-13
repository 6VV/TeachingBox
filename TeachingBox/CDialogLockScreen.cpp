#include "stdafx.h"
#include "CDialogLockScreen.h"
#include "QBoxLayout"


CDialogLockScreen::CDialogLockScreen(int lockTime, QWidget* parent/*=0*/) :QDialog(parent)
{
	m_lockTime = lockTime;
	Init();
	m_timerId = startTimer(1000);
}

CDialogLockScreen::~CDialogLockScreen()
{
	if (m_timerId!=0)
	{
		killTimer(m_timerId);
	}
	qDebug() << "delete";
}


void CDialogLockScreen::timerEvent(QTimerEvent *event)
{
	QDialog::timerEvent(event);

	m_lbRemainingTime->setText(QString::number(--m_lockTime));

	if (m_lockTime<=0)
	{
		delete(this);
	}
}

void CDialogLockScreen::Init()
{
	this->setWindowFlags(Qt::CustomizeWindowHint);
	this->setWindowModality(Qt::ApplicationModal);

	m_lbRemainingTime = new QLabel(QString::number(m_lockTime));
	m_lbRemainingTime->setAlignment(Qt::AlignCenter);
	QLabel* lbLockScreen = new QLabel("Lock Screen");
	lbLockScreen->setAlignment(Qt::AlignCenter);

	QVBoxLayout* layoutMain = new QVBoxLayout(this);
	layoutMain->addWidget(lbLockScreen);
	layoutMain->addWidget(m_lbRemainingTime);
}
