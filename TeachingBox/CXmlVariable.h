#ifndef _TEACHING_BOX_C_XML_VARIABLE_H_
#define _TEACHING_BOX_C_XML_VARIABLE_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CXmlVariable.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/12/14
//  Description:	读写存储变量的xml文档
//  Others:			采用QDomDocument读写，因采用该方法读取时需一次性将整个文档读取到内存中，当文档较大时，对内存空间有一定的要求，
且读写速度小于采用QStreamReader和QStreamWriter方法的速度，故目前并未采用此方法
//  History:
//    <author>      刘巍
//    <time>        2015/12/14
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "qdom.h"

class CXmlVariable
{
public:
	CXmlVariable(const char*);
	CXmlVariable(const QString&);
	~CXmlVariable();

public:
	void Read();
	void AddPositionNode(QString nodeName,double* pointValue);

private:
	void Init();
	void Create();
	void ReadDocument();

private:
	QString m_fileName;	/*文件名*/
	QXmlStreamReader* m_xmlReader;
	QDomDocument* m_doc;
};

#endif