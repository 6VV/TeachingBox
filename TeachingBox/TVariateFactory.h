#ifndef _TVARIATE_FACTORY_H_
#define _TVARIATE_FACTORY_H_

#include "TVariate.h"

class TVariateFactory
{
public:
	static TVariate* CreateVariate(QByteArray& dataBytes);
};

#endif