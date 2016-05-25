#ifndef _TEACHING_BOX_C_DIALOG_FORBIDDEN_H_
#define _TEACHING_BOX_C_DIALOG_FORBIDDEN_H_

#include "QDialog"
#include "QLabel"

class CDialogLockScreen:public QDialog
{
	Q_OBJECT

public:
	CDialogLockScreen(int lockTime,QWidget* parent=0);
	~CDialogLockScreen();

protected:
	void timerEvent(QTimerEvent *event)	Q_DECL_OVERRIDE;

private:
	void Init();

private:
	int m_timerId;	/*定时器id*/
	int m_lockTime;	/*锁屏时间（s）*/

	QLabel* m_lbRemainingTime;
};

#endif