#ifndef _TDYNAMIC_TESTS_H_
#define _TDYNAMIC_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "..\TeachingBox\TDynamic.h"

class TDynamicTests:public testing::Test
{
public:
	virtual void SetUp() override;
	virtual void TearDown() override;

	void ExpectVariate(QDataStream& dataStream);

	TDynamic* m_variate = nullptr;
};

#endif