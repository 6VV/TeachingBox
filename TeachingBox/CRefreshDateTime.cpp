#include "stdafx.h"
#include "CRefreshDateTime.h"
#include "CScreenSetting.h"
#include "CScreenState.h"

CRefreshDateTime::CRefreshDateTime()
{
}

CRefreshDateTime::~CRefreshDateTime()
{
}

CRefreshDateTime* CRefreshDateTime::GetInstance()
{
	return CSingleTon<CRefreshDateTime>::GetInstance();
}

bool CRefreshDateTime::IsCreated()
{
	return CSingleTon<CRefreshDateTime>::IsCreated();
}

/*更新显示时间*/
void CRefreshDateTime::SlotRefreshDateTime()
{
	QDateTime dateTime = QDateTime::currentDateTime();
	QString date = dateTime.toString("yyyy-MM-dd");
	QString time = dateTime.toString("hh:mm:ss");
	
	/*若该界面已建立，则更新显示时间*/
	if (CScreenSetting::IsCreated())
	{
		CScreenSetting::GetInstance()->GetDateLabel()->setText(date);
		CScreenSetting::GetInstance()->GetTimeLabel()->setText(time);
	}
	if (CScreenState::IsCreated()){
		CScreenState::GetInstance()->GetTimeLabel()->setText(time);
	}

}
