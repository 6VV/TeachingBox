#include "stdafx.h"
#include "CLabelWithSignal.h"


CLabelWithSignal::CLabelWithSignal(QWidget* parent/*=0*/) :QLabel(parent)
{
	Init();
}

CLabelWithSignal::CLabelWithSignal(const QString& strText, QWidget* parent /*= 0*/) : QLabel(strText,parent)
{
	Init();
}

CLabelWithSignal::~CLabelWithSignal()
{

}


void CLabelWithSignal::mousePressEvent(QMouseEvent *ev)
{
	emit(SignalClicked());
}


void CLabelWithSignal::Init()
{
	setStyleSheet("background:white");
}
