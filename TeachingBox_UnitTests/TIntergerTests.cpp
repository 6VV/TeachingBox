#include "TIntergerTests.h"
#include <gtest\gtest.h>
#include "..\TeachingBox\TInteger.h"
#include "..\TeachingBox\CSymbol.h"


TEST_F(TIntergerTests, ReadDataStream_ReadRightData_ReturnTrue)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_interger->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	QString scope;
	QString name;
	int type;
	int value;
	dataStream >> scope >> name >> type >> value;

	EXPECT_STREQ(scope.toStdString().c_str(), "a");
	EXPECT_STREQ(name.toStdString().c_str(), "b");
	EXPECT_EQ(type, CSymbol::TYPE_INTERGER);
	EXPECT_EQ(value, 3);
}

TEST_F(TIntergerTests, ReadDataStream_WriteToByteArray_ReturnTrue)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_interger->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	QByteArray dataW=dataStream.device()->readAll();
	QDataStream dataStreamW(&dataW, QIODevice::ReadWrite);
	
	QString scope;
	QString name;
	int type;
	int value;
	dataStreamW >> scope >> name >> type >> value;

	EXPECT_STREQ(scope.toStdString().c_str(), "a");
	EXPECT_STREQ(name.toStdString().c_str(), "b");
	EXPECT_EQ(type, CSymbol::TYPE_INTERGER);
	EXPECT_EQ(value, 3);
}

TEST_F(TIntergerTests, TInteger_Create_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_interger->ReadDataStream(dataStream);

	dataStream.device()->seek(0);
	TInteger newInterger(dataStream);
	EXPECT_STREQ(m_interger->GetScope().toStdString().c_str(), newInterger.GetScope().toStdString().c_str());
	EXPECT_STREQ(m_interger->GetName().toStdString().c_str(), newInterger.GetName().toStdString().c_str());
	EXPECT_EQ(m_interger->GetType(), newInterger.GetType());
}

TEST_F(TIntergerTests, UpdateFrom_Update_Equal)
{
	m_interger->UpdateFromVariate(TInteger("c", "d", 6));

	EXPECT_STREQ(m_interger->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_interger->GetName().toStdString().c_str(), "d");
	EXPECT_EQ(m_interger->GetValue(), 6);
}

void TIntergerTests::SetUp()
{
	m_interger = new TInteger("a", "b", 3);
}

void TIntergerTests::TearDown()
{
	delete m_interger;
}
