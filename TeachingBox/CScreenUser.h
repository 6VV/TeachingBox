#ifndef _TEACHING_BOX_C_SCREEN_USER_H_
#define _TEACHING_BOX_C_SCREEN_USER_H_

#include "QWidget"
#include "QTableWidget"

class CScreenUser:public QWidget
{
	Q_OBJECT

public:
	CScreenUser(QWidget* parent=0);
	~CScreenUser();

protected:
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotControlAuthorityChanged(int state);

private:
	void Init();
	
	void RefreshInformation();

private:
	const char* CLASS_NAME = "CScreenUser";

	QTableWidget* m_tableWidget;
};

#endif