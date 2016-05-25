#include "TStringTests.h"



void TStringTests::SetUp()
{
	m_variate = new TString("a", "b", "text");
}

void TStringTests::TearDown()
{
	delete m_variate;
}

void TStringTests::ExpectVariate(QDataStream& dataStream)
{
	QString scope;
	QString name;
	int type;
	QString value;
	dataStream >> scope >> name >> type >> value;

	EXPECT_STREQ(scope.toStdString().c_str(), "a");
	EXPECT_STREQ(name.toStdString().c_str(), "b");
	EXPECT_EQ(type, CSymbol::TYPE_STRING);
	EXPECT_STREQ(value.toStdString().c_str(), "text");
}

TEST_F(TStringTests, ReadDataStream_ReadRightData_ReturnTrue)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TStringTests, ReadDataStream_WriteToByteArray_ReturnTrue)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	QByteArray dataW = dataStream.device()->readAll();
	QDataStream dataStreamW(&dataW, QIODevice::ReadWrite);

	ExpectVariate(dataStreamW);
}

TEST_F(TStringTests, TBool_Create_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);

	dataStream.device()->seek(0);
	TString newVariate(dataStream);
	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), newVariate.GetScope().toStdString().c_str());
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), newVariate.GetName().toStdString().c_str());
	EXPECT_EQ(m_variate->GetType(), newVariate.GetType());
	EXPECT_STREQ(m_variate->GetValue().toStdString().c_str(), newVariate.GetValue().toStdString().c_str());
}

TEST_F(TStringTests, UpdateFrom_Update_Equal)
{
	m_variate->UpdateFromVariate(TString("c", "d", "info"));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");
	EXPECT_STREQ(m_variate->GetValue().toStdString().c_str(), "info");
	EXPECT_NE(m_variate->GetValue().toStdString().c_str(), "text");
}