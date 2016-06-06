#include "TPositionTests.h"
#include "QVector"



void TPositionTests::SetUp()
{
	tAxesAllPositions vec = tAxesAllPositions{ { 1, 2, 3, 4, 5, 6 } };
	m_variate = new TPosition("a", "b", vec);
}

void TPositionTests::TearDown()
{
	delete m_variate;
}


void TPositionTests::ExpectVariate(QDataStream& dataStream)
{
	TPosition position(dataStream);

	EXPECT_STREQ(position.GetScope().toStdString().c_str(), "a");
	EXPECT_STREQ(position.GetName().toStdString().c_str(), "b");
	EXPECT_EQ(position.GetType(), CSymbol::TYPE_POSITION);

	for (int i = 0; i < AXIS_SIZE;++i)
	{
		EXPECT_EQ(position.GetValue().m_AxisPosition[i], m_variate->GetValue().m_AxisPosition[i]);
	}
}

TEST_F(TPositionTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TPositionTests, UpdateFrom_Update_Equal)
{
	tAxesAllPositions vec = tAxesAllPositions{ { 3, 4, 5, 6, 7, 8 } };
	//TPosition::TYPE_POSITION vec = TPosition::TYPE_POSITION{ 3, 4, 5, 6, 7, 8 };
	m_variate->UpdateFromVariate(TPosition("c", "d", vec));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");
	for (int i = 0; i < AXIS_SIZE; ++i)
	{
		EXPECT_EQ(vec.m_AxisPosition[i], m_variate->GetValue().m_AxisPosition[i]);
	}
}