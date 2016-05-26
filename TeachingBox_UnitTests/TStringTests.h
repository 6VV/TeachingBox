#ifndef _TSTRING_TESTS_H_
#define _TSTRING_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "..\TeachingBox\TString.h"

class TStringTests:public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override; 

	void ExpectVariate(QDataStream& dataStream);

	TString* m_variate=nullptr;
};

#endif