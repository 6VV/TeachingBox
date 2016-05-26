#include "TDynamicTests.h"



void TDynamicTests::SetUp()
{
	tDynamicConstraint value{ 1, 2, 3, 4, 5, 6 };
	m_variate = new TDynamic("a", "b", value);
}

void TDynamicTests::TearDown()
{
	delete m_variate;
}


void TDynamicTests::ExpectVariate(QDataStream& dataStream)
{
	TDynamic variate(dataStream);

	EXPECT_STREQ(variate.GetScope().toStdString().c_str(), "a");
	EXPECT_STREQ(variate.GetName().toStdString().c_str(), "b");
	EXPECT_EQ(variate.GetType(), CSymbol::TYPE_DYNAMIC);

	EXPECT_EQ(variate.GetValue().m_Velocity, m_variate->GetValue().m_Velocity);
	EXPECT_EQ(variate.GetValue().m_Acceleration, m_variate->GetValue().m_Acceleration);
	EXPECT_EQ(variate.GetValue().m_Deceleration, m_variate->GetValue().m_Deceleration);
	EXPECT_EQ(variate.GetValue().m_PostureVelocity, m_variate->GetValue().m_PostureVelocity);
	EXPECT_EQ(variate.GetValue().m_PostureDeceleration, m_variate->GetValue().m_PostureDeceleration);
	EXPECT_EQ(variate.GetValue().m_PostureDeceleration, m_variate->GetValue().m_PostureDeceleration);
}

TEST_F(TDynamicTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TDynamicTests, UpdateFrom_Update_Equal)
{
	tDynamicConstraint value{ 3, 4, 5, 6, 7, 8 };
	m_variate->UpdateFromVariate(TDynamic("c", "d", value));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");
	
	EXPECT_EQ(m_variate->GetValue().m_Velocity,3);
	EXPECT_EQ(m_variate->GetValue().m_Acceleration, 4);
	EXPECT_EQ(m_variate->GetValue().m_Deceleration,5);
	EXPECT_EQ(m_variate->GetValue().m_PostureVelocity,6);
	EXPECT_EQ(m_variate->GetValue().m_PostureAcceleration,7);
	EXPECT_EQ(m_variate->GetValue().m_PostureDeceleration,8);
}
