#ifndef _TEACHING_BOX_C_SCREEN_SERVICE_H_
#define _TEACHING_BOX_C_SCREEN_SERVICE_H_

#include "CScreenMainParent.h"
#include "QStackedWidget"
#include "CScreenSetting.h"
#include "CScreenUser.h"
#include "CSingleTon.h"
#include "CScreenUserManager.h"
#include "CScreenNetwork.h"

class CScreenService:public CScreenMainParent
{
	friend CSingleTon<CScreenService>;

	Q_OBJECT

private:
	CScreenService(QWidget* parent=0);
	CScreenService(const CScreenService&);
	CScreenService& operator=(const CScreenService&);
	~CScreenService();

protected slots:
	void SlotOnButtonSettingClicked();
	void SlotOnButtonUserClicked();
	void SlotOnButtonUserAdminClicked();
	void SlotOnButtonNetworkClicked();

	void SlotUserChanged();
public:
	static CScreenService* GetInstance();

	void ChangeToScreenSetting();
	void ChangeToScreenUser();
	void ChangeToScreenUserManager();
	void ChangeToScreenNetwork();

protected:
	void RefreshText() Q_DECL_OVERRIDE;

private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

private:
	QStackedWidget* m_stackedWidget=NULL;

	CScreenSetting* m_screenSetting=NULL;
	CScreenUser* m_screenUser=NULL;
	CScreenUserManager* m_screenUserManager = NULL;
	CScreenNetWork* m_screenNetwork=NULL;

	/*底部按钮*/
	CButton* m_btnSettings;
	CButton* m_btnUserInformation;
	CButton* m_btnUserAdmin;
	CButton* m_btnVersion;
	CButton* m_btnInfo;
	CButton* m_btnNetWork;
};

#endif