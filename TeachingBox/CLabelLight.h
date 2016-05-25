#ifndef _C_TEACHING_BOX_C_LABEL_LIGHT_H_
#define _C_TEACHING_BOX_C_LABEL_LIGHT_H_

#include "QString"
#include "CLabelPixmap.h"
#include "QPixmap"

#define LIGHT_BLACK_SOURCE ":/Image/Image/Light_Black.png"
#define LIGHT_RED_SOURCE ":/Image/Image/Light_Red.png"
#define LIGHT_GREEN_SOURCE ":/Image/Image/Light_Green.png"

class CLabelLight :public CLabelPixmap
{
	Q_OBJECT
public:
	CLabelLight(QWidget* parent = 0);
	~CLabelLight();

public:
	void ChangeToRed();
	void ChangeToGreen();
	void ChangeToBlack();
};

#endif