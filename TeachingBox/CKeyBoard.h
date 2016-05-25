#ifndef _TEACHING_BOX_C_KEYBOARD_H_
#define _TEACHING_BOX_C_KEYBOARD_H_

#include "CSingleTon.h"
#include "CButtonKeyBoard.h"
#include "QWidget"
#include "Init.h"
#include "CKeyBoardInterface.h"

class CKeyboard:public QWidget
{
	friend class CSingleTon<CKeyboard>;	/*单例模式*/

	Q_OBJECT

private:
	CKeyboard(QWidget* parent=0);
	CKeyboard(const CKeyboard&);
	CKeyboard& operator=(const CKeyboard&);
	~CKeyboard();
	
	/*自定义公有函数*/
public:
	static CKeyboard* GetInstance();

	QLineEdit* GetLineEdit();	
	QString GetCurrentText();	/*获取当前文本*/
	void SetConfirmCommand(int);	/*设置确认按钮对应命令*/
	void SetCurrentWidget(QWidget*);	/*设置调用本键盘的窗口*/
	void SetCurrentText(QString&);	/*设置键盘当前文本*/
	void SetKeyboardInterface(CKeyBoardInterface*);	/*设置Confirm按钮按下时调用的函数*/


	/*回调函数所在类*/
private:
	CKeyBoardInterface* m_keyboardInterface;

	/*自定义私有函数*/
private:
	void Init();
	void InitLayout();
	void InitSignalSlot();

	void OnButtonConfirmClicked();

	void ChangeToLower();	/*切换为小写*/
	void ChangeToUpper();	/*切换为大写*/

	void Clear();	/*清空参数*/

	/*重定义父类函数*/
protected:
	void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
	void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

protected slots:
	void SlotOnButtonClicked();

	/*定义确认按钮按下后所执行的操作指令*/
public:
	const static int CONFIRM_SCREEN_SETTING_LOGIN;			/*登录*/
	const static int CONFIRM_SCREEN_PROGRAM_EDIT_PROGRAM;	/*编辑程序*/
	const static int CONFIRM_SCREEN_PROJECT_NEW_PROJECT;	/*新建项目*/
	const static int CONFIRM_SCREEN_PROJECT_NEW_PROGRAM;	/*新建程序*/
	const static int CONFIRM_MACRO_POSITION_EIDT;			/*编辑位置变量参数*/
	const static int CONFIRM_MACRO_VELOCITY_EDIT;			/*编辑速度变量参数*/
	const static int CONFIRM_MACRO_ZONE_EDIT;				/*编辑过渡变量参数*/
	const static int CONFIRM_MACRO_DOUBLE_EDIT;				/*编辑浮点数变量参数*/
	const static int CONFIRM_MACRO_LABEL_EDIT;				/*编辑标签变量*/
	const static int CONFIRM_NEW_PARAMETER;					/*添加新变量*/
	const static int CONFIRM_MACRO_INT_EDIT;				/*编辑整数变量*/
	const static int CONFIRM_MACRO_BOOL_EDIT;				/*编辑布尔变量*/
	const static int CONFIRM_MACRO_STRING_EDIT;				/*编辑字符串变量*/
	const static int CONFIRM_MACRO_NUMBER_EDIT;				/*编辑数值参数*/

	/*控件*/
private:
	const char* CLASS_NAME = "CKeyBoard";
	int m_confirmCommand = -1;	/*当前确认按钮对应的命令*/
	QWidget* m_currentWidget=NULL;	/*调用本键盘的窗口*/
	QLineEdit* m_lineEditText;	/*显示文本控件*/
	bool m_isLower = true;	/*是否为小写字符*/

	CButtonKeyBoard* m_btnTilde;
	CButtonKeyBoard* m_btnNum1;
	CButtonKeyBoard* m_btnNum2;
	CButtonKeyBoard* m_btnNum3;
	CButtonKeyBoard* m_btnNum4;
	CButtonKeyBoard* m_btnNum5;
	CButtonKeyBoard* m_btnNum6;
	CButtonKeyBoard* m_btnNum7;
	CButtonKeyBoard* m_btnNum8;
	CButtonKeyBoard* m_btnNum9;
	CButtonKeyBoard* m_btnNum0;
	CButtonKeyBoard* m_btnMinus;
	CButtonKeyBoard* m_btnPlus;
	CButtonKeyBoard* m_btnBackspace;

	CButtonKeyBoard* m_btnTab;
	CButtonKeyBoard* m_btnQ;
	CButtonKeyBoard* m_btnW;
	CButtonKeyBoard* m_btnE;
	CButtonKeyBoard* m_btnR;
	CButtonKeyBoard* m_btnT;
	CButtonKeyBoard* m_btnY;
	CButtonKeyBoard* m_btnU;
	CButtonKeyBoard* m_btnI;
	CButtonKeyBoard* m_btnO;
	CButtonKeyBoard* m_btnP;
	CButtonKeyBoard* m_btnLeftBracket;
	CButtonKeyBoard* m_btnRightBracket;
	CButtonKeyBoard* m_btnBackSlash;
	CButtonKeyBoard* m_btnEqual;

	CButtonKeyBoard* m_btnCapsLock;
	CButtonKeyBoard* m_btnA;
	CButtonKeyBoard* m_btnS;
	CButtonKeyBoard* m_btnD;
	CButtonKeyBoard* m_btnF;
	CButtonKeyBoard* m_btnG;
	CButtonKeyBoard* m_btnH;
	CButtonKeyBoard* m_btnJ;
	CButtonKeyBoard* m_btnK;
	CButtonKeyBoard* m_btnL;
	CButtonKeyBoard* m_btnSemicolon;
	CButtonKeyBoard* m_btnQuotation;
	CButtonKeyBoard* m_btnEnter;

	CButtonKeyBoard* m_btnShift;
	CButtonKeyBoard* m_btnZ;
	CButtonKeyBoard* m_btnX;
	CButtonKeyBoard* m_btnC;
	CButtonKeyBoard* m_btnV;
	CButtonKeyBoard* m_btnB;
	CButtonKeyBoard* m_btnN;
	CButtonKeyBoard* m_btnM;
	CButtonKeyBoard* m_btnComma;
	CButtonKeyBoard* m_btnPeriod;
	CButtonKeyBoard* m_btnSlash;
	CButtonKeyBoard* m_btnUp;

	CButtonKeyBoard* m_btnCtrl;
	CButtonKeyBoard* m_btnAlt;
	CButtonKeyBoard* m_btnSpace;
	CButtonKeyBoard* m_btnLf;
	CButtonKeyBoard* m_btnRg;
	CButtonKeyBoard* m_btnBt;

	CButtonKeyBoard* m_btnCancle;
	CButtonKeyBoard* m_btnConfirm;
};

#endif