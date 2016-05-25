#ifndef _TBOOL_TESTS_H_
#define _TBOOL_TESTS_H_

#include <..\packages\gmock.1.7.0\lib\native\include\gtest\gtest.h>
#include "..\TeachingBox\TBool.h"

class TBoolTests:public testing::Test
{
protected:
	virtual void SetUp() override;
	virtual void TearDown() override;

	TBool* m_bool;
};

#endif