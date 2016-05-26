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

TEST_F(TStringTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TStringTests, UpdateFrom_Update_Equal)
{
	m_variate->UpdateFromVariate(TString("c", "d", "info"));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");
	EXPECT_STREQ(m_variate->GetValue().toStdString().c_str(), "info");
	EXPECT_NE(m_variate->GetValue().toStdString().c_str(), "text");
}