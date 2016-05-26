#include "stdafx.h"
#include "CTeachingBox.h"
#include "CScreenRight.h"
#include "CScreenBottom.h"
#include "CScreenState.h"
#include "CScreenWarning.h"
#include "CScreen.h"
#include "CLabelPixmap.h"
#include <QtWidgets/QApplication>
#include "CTreeModel.h"
#include "CScreenProject.h"
#include "QTranslator"
#include "CKeyBoard.h"
#include "CCodeEditor.h"
#include "CGrammarManagerFactory.h"
#include "CScreenEditMacro.h"
#include "CMacroParameterPosition.h"
#include "CWidgetTest.h"
#include "QIcon"
#include "CTestWidget.h"
#include "CTestMessagebox.h"
#include "CScreenUser.h"
#include "CScreenService.h"
#include "CScreenUserManager.h"
#include "CDialogEditUser.h"
#include "CDialogLockScreen.h"
#include "CScreenNetwork.h"
#include "CLabelWithSignal.h"
#include "CScreenLineEditor.h"
#include "TVariate.h"

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);

	CTeachingBox* w = CTeachingBox::GetInstance();

	w->move((QApplication::desktop()->width() - w->width()) / 2, (QApplication::desktop()->height() - w->height()) / 2);
	w->show();


	return a.exec();
}
