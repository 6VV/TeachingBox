/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CSingleTon.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/12/02
//  Description:	模板类，实现单例模式，当需要一个全局唯一类时，可将该类设为此模板类的友元，
					将该类设为此模板的类型，返回的实例即为唯一类
//  Others:			本类采用了静态指针，而非静态变量，是为了保证每个单例可在需要时被创建，不需要时可随时销毁
//  History:
//    <author>      刘巍 
//    <time>        2015/12/02
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_SINGLE_TON_H_
#define _TEACHING_BOX_C_SINGLE_TON_H_

#include <mutex>

template<typename TYPE>
class CSingleTon
{
public:
	/*获取唯一实例*/
	static TYPE* GetInstance()
	{
		static std::mutex mutex;

		if (!m_instance)
		{
			mutex.lock();

			if (!m_instance)
			{
				m_instance = new TYPE;
			}

			mutex.unlock();
		}
		return m_instance;
	}

	/*销毁实例,可手动调用，或在其它类的析构函数中调用*/
	/*此函数目前仅为预留函数，暂不被其他函数调用*/
	static void Destroy()
	{
		if (m_instance)
		{
			delete m_instance;
			m_instance = NULL;
		}
	}

	
/*************************************************
//  Function: 		IsCreated
//  Description: 	判断是否已经创建实例
//  Calls:		 	
//  Called By: 		线程中需要对界面进行操作的函数
//  Parameter:      
//  Return: 		
//  Others: 		由于界面只能在主线程中建立，而在部分线程中需要对界面数据进行操作，
					故通过此函数来判断界面是否已经建立
*************************************************/
	static bool IsCreated()
	{
		return m_instance!=NULL;
	}

	/*垃圾回收类，用于在程序退出时释放那些之前未被释放掉的内存*/
private:
	class CGarbo
	{
	public:
		CGarbo();
		~CGarbo()
		{
			if (CSingleTon::m_instance)
			{
				delete CSingleTon::m_instance;
				CSingleTon::m_instance = NULL;
			}
		}
	};
	static CGarbo m_garbo;

private:
	static TYPE* m_instance; /*唯一实例*/

	/*禁止构造函数*/
	CSingleTon();
	CSingleTon(const CSingleTon&);
	CSingleTon& operator=(const CSingleTon&);

	/*禁止析构函数*/
	~CSingleTon();
};

template<typename TYPE>
TYPE* CSingleTon<TYPE>::m_instance=NULL;

#endif