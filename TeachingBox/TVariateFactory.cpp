#include "stdafx.h"
#include "TVariateFactory.h"
#include "TInteger.h"
#include "TDouble.h"
#include "TBool.h"
#include "TString.h"
#include "TPosition.h"
#include "TDynamic.h"



TVariate* TVariateFactory::CreateVariate(QByteArray& dataBytes)
{
	QDataStream dataStream(&dataBytes, QIODevice::ReadOnly);
	CSymbol symbol(dataStream);
	//TVariate variate(dataStream);
	dataStream.device()->seek(0);
	switch (symbol.GetType())
	{
	case CSymbol::TYPE_INTERGER:
	{
		return new TInteger(dataStream);
	}break;
	case CSymbol::TYPE_DOUBLE:
	{
		return new TDouble(dataStream);
	}break;
	case CSymbol::TYPE_BOOL:
	{
		return new TBool(dataStream);
	}break;
	case CSymbol::TYPE_STRING:
	{
		return new TString(dataStream);
	}break;
	case CSymbol::TYPE_POSITION:
	{
		return new TPosition(dataStream);
	}break;
	case CSymbol::TYPE_DYNAMIC:
	{
		return new TDynamic(dataStream);
	}break;
	default:
	{
		return nullptr;
	}break;
	}
}
