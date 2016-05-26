#ifndef _TDOUBLE_TESTS_H_
#define _TDOUBLE_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "..\TeachingBox\TDouble.h"

class TDoubleTests:public testing::Test
{
protected:
	virtual void TearDown() override;
	virtual void SetUp() override;
	
	TDouble* m_double=nullptr;
};

#endif