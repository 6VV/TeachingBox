#ifndef _TINTERGER_TESTS_H_
#define _TINTERGER_TESTS_H_

#include "gtest/gtest.h"
#include "../TeachingBox/TInteger.h"
#include "QDataStream"

class TIntergerTests :public testing::Test
{
protected:
	virtual void SetUp();
	virtual void TearDown();

	TInteger* m_interger;
};

#endif