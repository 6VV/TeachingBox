#include "TBoolTests.h"

void TBoolTests::SetUp()
{
	m_bool = new TBool("a", "b", true);
}

void TBoolTests::TearDown()
{
	delete m_bool;
}

void TBoolTests::ExpectVariate(QDataStream& dataStream)
{
	QString scope;
	QString name;
	int type;
	bool value;
	dataStream >> scope >> name >> type >> value;

	EXPECT_STREQ(scope.toStdString().c_str(), "a");
	EXPECT_STREQ(name.toStdString().c_str(), "b");
	EXPECT_EQ(type, CSymbol::TYPE_BOOL);
	EXPECT_EQ(value, true);
}

TEST_F(TBoolTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_bool->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TBoolTests, UpdateFrom_Update_Equal)
{
	m_bool->UpdateFromVariate(TBool("c", "d", false));

	EXPECT_STREQ(m_bool->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_bool->GetName().toStdString().c_str(), "d");
	EXPECT_EQ(m_bool->GetValue(), false);
	EXPECT_NE(m_bool->GetValue(), true);
}