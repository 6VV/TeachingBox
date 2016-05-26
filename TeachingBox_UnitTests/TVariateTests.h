#ifndef _TVARIATE_TESTS_H_
#define _TVARIATE_TESTS_H_

#include "gtest/gtest.h"
#include "QDataStream"
#include "../TeachingBox/TVariate.h"

class TVariateTests:public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

	void ExpectVariate(QDataStream& dataStream);

	TVariate* m_variate=nullptr;
};

#endif