#ifndef _TEACHING_BOC_C_LABEL_WITH_SIGNAL_H_
#define _TEACHING_BOC_C_LABEL_WITH_SIGNAL_H_

#include "QLabel"
#include "CKeyBoardInterface.h"

class CLabelWithSignal:public QLabel
{
	Q_OBJECT

public:
	CLabelWithSignal(QWidget* parent=0);
	CLabelWithSignal(const QString& strText, QWidget* parent = 0);
	~CLabelWithSignal();

signals:
	void SignalClicked();

protected:
	void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;

private:
	void Init();
};

#endif