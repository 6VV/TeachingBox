#ifndef _C_TEACHING_BOX_C_SCREEN_USER_MANAGER_H_
#define _C_TEACHING_BOX_C_SCREEN_USER_MANAGER_H_

#include "QWidget"
#include "QTableWidget"
#include "CButton.h"

class CScreenUserManager:public QWidget
{
	Q_OBJECT

public:
	CScreenUserManager(QWidget* parent=0);
	~CScreenUserManager();

	void RefreshUserTable();

protected:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonAddUserClicked();
	void SlotOnButtonEditUserClicked();
	void SlotOnButtonDeleteUserClicked();
	void SlotTabelWidgetSelectionChanged(QTableWidgetItem*);

private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void RefreshText();

private:
	const char* CLASS_NAME = "CScreenUserManager";

	QTableWidget* m_tableWidget;
	CButton* m_btnEdit;
	CButton* m_btnNew;
	CButton* m_btnDelete;
};

#endif