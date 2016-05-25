#ifndef _TEACHING_BOX_C_DIALOG_EDIT_USER_H_
#define _TEACHING_BOX_C_DIALOG_EDIT_USER_H_

#include "qwindowdefs.h"
#include "CLineEditWithClickedSignal.h"
#include "CButton.h"
#include "User.h"
#include "CScreenUserManager.h"

class CDialogEditUser:public QDialog
{
	Q_OBJECT

public:
	CDialogEditUser(CScreenUserManager* screenUserManager);
	CDialogEditUser(CScreenUserManager* screenUserManager,CUser& user);

	~CDialogEditUser();

	void SetUser(const CUser& user);
protected:
	void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonConfirmClicked();
	void SlotOnButtonCancelClicked();

	void SlotOnLineEditConfirmPasswordTextChanged();	/*确认密码*/
	void SlotOnLineEditUserTextChanged();	/*用户名改变*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void RefreshText();
	void DisableWidget();	/*根据用户权限禁用控件*/
	void InitUserInfo();	/*初始化用户信息*/

private:
	const char* CLASS_NAME = "CDialogEditUser";

	CUser m_user;	/*当前编辑的用户*/
	
	CScreenUserManager* m_screenUserManager;

	QLabel* m_lbName;
	QLabel* m_lbPassword;
	QLabel* m_lbConfirmPassword;
	QLabel* m_lbAuthority;
	QLabel* m_lbLanguage;
	QLabel* m_lbIdentity;

	CLineEditWithClickedSignal* m_lineEditName;
	CLineEditWithClickedSignal* m_lineEditPassword;
	CLineEditWithClickedSignal* m_lineEditConfirmPassword;
	QComboBox* m_comboBoxAuthority;
	QComboBox* m_comboBoxLanguage;
	QCheckBox* m_checkBoxIdencity;

	CButton* m_btnConfirm;
	CButton* m_btnCancel;
};

#endif