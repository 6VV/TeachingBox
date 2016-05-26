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

void TVariateTests::SetUp()
{
	m_variate = new TInteger("a", "b", 1);
}

void TVariateTests::TearDown()
{
	delete m_variate;
}

TEST_F(TVariateTests, ReadDataStream_ReadRightData_ReaturnTrue)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TVariateTests, UpdateFrom_Update_Equal)
{
	m_variate->UpdateFromVariate(TInteger("c", "d", 2));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");
}