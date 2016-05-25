#ifndef _TVARIATE_TESTS_H_
#define _TVARIATE_TESTS_H_

#include "gtest/gtest.h"
#include "QDataStream"

class TVariateTests:public testing::Test
{
protected:
	void ExpectVariate(QDataStream& dataStream);

private:
};

#endif