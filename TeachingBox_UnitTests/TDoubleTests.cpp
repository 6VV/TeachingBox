#include "TDoubleTests.h"
#include "QIODevice"



void TDoubleTests::TearDown()
{
	delete m_double;
}

void TDoubleTests::SetUp()
{
	m_double = new TDouble( "scope", "name", 3.3 );
}

TEST_F(TDoubleTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_double->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	QString scope;
	QString name;
	int type;
	double value;
	dataStream >> scope >> name >> type >> value;

	EXPECT_STREQ(scope.toStdString().c_str(), "scope");
	EXPECT_STREQ(name.toStdString().c_str(), "name");
	EXPECT_EQ(type, CSymbol::TYPE_DOUBLE);
	EXPECT_EQ(value, 3.3);
}

TEST_F(TDoubleTests, UpdateFrom_Update_Equal)
{
	m_double->UpdateFromVariate(TDouble("c", "d", 6.6));

	EXPECT_STREQ(m_double->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_double->GetName().toStdString().c_str(), "d");
	EXPECT_EQ(m_double->GetValue(), 6.6);
	EXPECT_NE(m_double->GetValue(), 7);
}