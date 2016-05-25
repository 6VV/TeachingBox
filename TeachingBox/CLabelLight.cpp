#include "stdafx.h"
#include "CLabelLight.h"

CLabelLight::CLabelLight(QWidget* parent /*= 0*/) :CLabelPixmap(parent)
{
	this->UpdateImage(LIGHT_BLACK_SOURCE);
}

CLabelLight::~CLabelLight()
{

}

void CLabelLight::ChangeToRed()
{
	UpdateImage(LIGHT_RED_SOURCE);
}

void CLabelLight::ChangeToGreen()
{
	UpdateImage(LIGHT_GREEN_SOURCE);
}

void CLabelLight::ChangeToBlack()
{
	UpdateImage(LIGHT_BLACK_SOURCE);
}


