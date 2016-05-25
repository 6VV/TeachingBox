#ifndef _TEACHING_BOX_C_THREAD_REFRESH_DATE_TIME_H_
#define _TEACHING_BOX_C_THREAD_REFRESH_DATE_TIME_H_

#include "CSingleTon.h"
#include "QThread"
#include "QTimer"

class CRefreshDateTime:public QObject
{
	friend class CSingleTon<CRefreshDateTime>;

	Q_OBJECT

private:
	CRefreshDateTime();
	CRefreshDateTime(const CRefreshDateTime&);
	CRefreshDateTime& operator=(const CRefreshDateTime&);
	~CRefreshDateTime();

	/*自定义公有函数*/
public:
	static CRefreshDateTime* GetInstance();
	static bool IsCreated();

	/*自定义槽函数*/
private slots:
	void SlotRefreshDateTime();
};


#endif