#ifndef _TOVERLAP_TESTS_H_
#define _TOVERLAP_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "QDataStream"
#include "..\TeachingBox\TOverlap.h"

class TOverlapTests:public testing::Test
{
public:
	virtual void SetUp() override;
	virtual void TearDown() override;

	void ExpectVariate(QDataStream& dataStream);

	TOverlap* m_variate = nullptr;
};

#endif