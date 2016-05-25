#ifndef _TEACHING_BOX_C_KEY_BOARD_INTERFACE_H_
#define _TEACHING_BOX_C_KEY_BOARD_INTERFACE_H_

class CKeyBoardInterface
{
public:
	CKeyBoardInterface();
	~CKeyBoardInterface();

	virtual void KeyboardEdit() = 0;

protected:
	
private:
};

#endif