#include "TVariateTests.h"
#include "QDataStream"
#include "..\TeachingBox\TVariate.h"
#include "..\TeachingBox\TInteger.h"

void TVariateTests::ExpectVariate(QDataStream& dataStream)
{
	QString scope;
	QString name;
	dataStream >> scope;
	dataStream >> name;
	EXPECT_STREQ(scope.toStdString().c_str(), "a");
	EXPECT_STREQ(name.toStdString().c_str(), "b");
	EXPECT_STRNE(scope.toStdString().c_str(), "c");
}

TEST_F(TVariateTests, ReadDataStream_ReadRightData_ReaturnTrue)
{
	TInteger var("a", "b", 1);
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	var.ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TVariateTests, UpdateFrom_Update_Equal)
{
	TVariate var("a", "b", CSymbol::TYPE_VOID);
	var.UpdateFrom(TVariate("c", "d", CSymbol::TYPE_BOOL));

	EXPECT_STREQ(var.GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(var.GetName().toStdString().c_str(), "d");
	EXPECT_EQ(var.GetType(),CSymbol::TYPE_BOOL);
}