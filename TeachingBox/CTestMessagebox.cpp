#include "stdafx.h"
#include "CTestMessagebox.h"


CTestMessagebox::CTestMessagebox(QWidget* parent/*=0*/) :QMessageBox(parent)
{
	qDebug() << "create";
}

CTestMessagebox::~CTestMessagebox()
{
	qDebug() << "delete";
}
