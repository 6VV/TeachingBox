#include "TOverlapTests.h"



void TOverlapTests::SetUp()
{
	m_variate = new TOverlap("a", "b", tOverlapConstraint{ tTransitionMode::kTransitionModeRelative, 123 });
}

void TOverlapTests::TearDown()
{
	delete m_variate;
}


void TOverlapTests::ExpectVariate(QDataStream& dataStream)
{
	TOverlap variate(dataStream);

	EXPECT_STREQ(variate.GetScope().toStdString().c_str(), "a");
	EXPECT_STREQ(variate.GetName().toStdString().c_str(), "b");
	EXPECT_EQ(variate.GetType(), CSymbol::TYPE_OVERLAP);

	EXPECT_EQ(variate.GetValue().m_TransitionMode, m_variate->GetValue().m_TransitionMode);
	EXPECT_EQ(variate.GetValue().m_TransitionParameter, m_variate->GetValue().m_TransitionParameter);
}

TEST_F(TOverlapTests, ReadDataStream_CreateFromData_Equal)
{
	QByteArray data;
	QDataStream dataStream(&data, QIODevice::ReadWrite);
	m_variate->ReadDataStream(dataStream);
	dataStream.device()->seek(0);

	ExpectVariate(dataStream);

}

TEST_F(TOverlapTests, UpdateFrom_Update_Equal)
{
	tOverlapConstraint value{ tTransitionMode::kTransitionModeAbsolute,321 };
	m_variate->UpdateFromVariate(TOverlap("c", "d", value));

	EXPECT_STREQ(m_variate->GetScope().toStdString().c_str(), "c");
	EXPECT_STREQ(m_variate->GetName().toStdString().c_str(), "d");

	EXPECT_EQ(m_variate->GetValue().m_TransitionMode, tTransitionMode::kTransitionModeAbsolute);
	EXPECT_EQ(m_variate->GetValue().m_TransitionParameter, 321);
}
