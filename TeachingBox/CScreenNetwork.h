#ifndef _TEACHING_BOX_C_SCREEN_NETWORK_H_
#define _TEACHING_BOX_C_SCREEN_NETWORK_H_

#include "QWidget"
#include "QGroupBox"
#include "QListWidget"
#include "CButton.h"
#include "QCheckBox"
#include "CKeyBoardInterface.h"

class CScreenNetWork:public QWidget,public CKeyBoardInterface
{
	Q_OBJECT

public:
	CScreenNetWork(QWidget* parent=0);
	~CScreenNetWork();
	
private:
	void Init();

	void InitLayout();
	void InitLayoutHost();
	void InitLayoutNetwork();
	QHBoxLayout* GetLayoutVNCTop();
	QGridLayout* GetLayoutVNCBottom();

	void InitSignalSlot();

	void RefreshText();

protected slots:
	void SlotOnButtonAddClicked();

protected:
	void KeyboardEdit() override;

private:
	const char* CLASS_NAME = "CScreenNetwork";
	QGroupBox* m_groupboxHostManager;	/*主机管理*/
	QGroupBox* m_groupboxNetwork;		/*网络设置*/
	QGroupBox* m_groupboxVNCConnect;	/*VNC连接*/

	/*主机管理界面按钮*/
	QListWidget* m_listWidgetHost;
	CButton* m_btnAddHost;
	CButton* m_btnDeleteHost;
	CButton* m_btnConnectHost;

	/*网络设置界面*/
	QLabel* m_lbLocalIpValue;
	QLabel* m_lbControlerIpValue;

	/*VNC连接界面*/
	QListWidget* m_listWidgetVNC;
	CButton* m_btnAddVNC;
	CButton* m_btnDeleteVNC;
	CButton* m_btnConnectVNC;

	QLabel* m_lbVNCName;
	QLabel* m_lbVisiable;
	QCheckBox* m_checkBoxFullScreen;
	QCheckBox* m_checkBoxContentAdaption;
};

#endif