#ifndef _TPOSITION_TESTS_H_
#define _TPOSITION_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "..\TeachingBox\TPosition.h"

class TPositionTests:public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

	void ExpectVariate(QDataStream& dataStream);

	TPosition* m_variate = nullptr;
};

#endif