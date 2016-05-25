#include "stdafx.h"
#include "TVariateFactory.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"



TVariate* TVariateFactory::CreateVariate(QByteArray& dataBytes)
{
	TVariate* result = nullptr;
	QDataStream dataStream(&dataBytes, QIODevice::ReadOnly);
	TVariate variate(dataStream);
	dataStream.device()->seek(0);
	switch (variate.GetType())
	{
	case CSymbol::TYPE_INTERGER:
	{
		result = new TInteger(dataStream);
	}break;
	case CSymbol::TYPE_DOUBLE:
	{
		result = new TDouble(dataStream);
	}break;
	case CSymbol::TYPE_BOOL:
	{
		result = new TBool(dataStream);
	}break;
	default:
		break;
	}

	return result;
}
