//#include "CDatabaseManagerTests.h"
//#include "..\TeachingBox\CDatabaseManager.h"
//#include "..\TeachingBox\TVariate.h"
//#include "QVector"
//#include "QByteArray"
//#include "..\TeachingBox\TInteger.h"
//
//void CDatabaseManagerTests::TearDown()
//{
//	CDatabaseManager::GetInstance()->DeleteAllVariate();
//}
//
//TEST_F(CDatabaseManagerTests, InsertVariate_InsertNewVariate_NoException)
//{
//	TInteger var("a", "b", 1);
//	QByteArray data;
//	QDataStream dataStream(&data, QIODevice::ReadWrite);
//	var.ReadDataStream(dataStream);
//
//	try
//	{
//		CDatabaseManager::GetInstance()->InsertVariate(dataStream);
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//TEST_F(CDatabaseManagerTests, DeleteVariate_DeleteOldVariate_NoException)
//{
//	TInteger var("a", "b", 1);
//	QByteArray data;
//	QDataStream dataStream(&data, QIODevice::ReadWrite);
//	var.ReadDataStream(dataStream);
//
//	try
//	{
//		CDatabaseManager::GetInstance()->InsertVariate(dataStream);
//		CDatabaseManager::GetInstance()->DeleteVariate(var.GetScope(), var.GetName());
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//TEST_F(CDatabaseManagerTests, UpdateVariate_Update_NoException)
//{
//	TInteger var("a", "b", 1);
//	QByteArray data;
//	QDataStream dataStream(&data, QIODevice::ReadWrite);
//	var.ReadDataStream(dataStream);
//
//	try
//	{
//		CDatabaseManager::GetInstance()->InsertVariate(dataStream);
//
//		dataStream.device()->seek(0);
//		data.clear();
//		TInteger var1("a", "b", 1);
//		var1.ReadDataStream(dataStream);
//
//		CDatabaseManager::GetInstance()->UpdateVariate(var.GetScope(), var.GetName(), dataStream);
//
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//TEST_F(CDatabaseManagerTests, SelectAllVariate_Select_RightValue)
//{
//	TInteger var("a", "b", 1);
//	QByteArray data;
//	QDataStream dataStream(&data, QIODevice::ReadWrite);
//	var.ReadDataStream(dataStream);
//
//	try
//	{
//		CDatabaseManager::GetInstance()->InsertVariate(dataStream);
//
//		QVector<QByteArray> values;
//		CDatabaseManager::GetInstance()->SelectAllVariate(values);
//
//		if (values.size() != 1)
//		{
//			throw std::exception("size not equal");
//		}
//		QByteArray data = values.at(0);
//		QDataStream newDataStream(&data, QIODevice::ReadOnly);
//		newDataStream.device()->seek(0);
//		QString scope;
//		QString name;
//		newDataStream >> scope >> name;
//
//		EXPECT_STREQ(scope.toStdString().c_str(), "a");
//		EXPECT_STREQ(name.toStdString().c_str(), "b");
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//TEST_F(CDatabaseManagerTests, SelectVariateFromScope_Select_RightValue)
//{
//	TInteger var("a", "b", 1);
//	QByteArray data;
//	QDataStream dataStream(&data, QIODevice::ReadWrite);
//	var.ReadDataStream(dataStream);
//
//	try
//	{
//		CDatabaseManager::GetInstance()->InsertVariate(dataStream);
//
//		QVector<QByteArray> values;
//		CDatabaseManager::GetInstance()->SelectVariatesFromScope(values,"a");
//
//		if (values.size() != 1)
//		{
//			throw std::exception("size not equal");
//		}
//		QByteArray data = values.at(0);
//		QDataStream newDataStream(&data, QIODevice::ReadOnly);
//		newDataStream.device()->seek(0);
//		QString scope;
//		QString name;
//		newDataStream >> scope >> name;
//
//		EXPECT_STREQ(scope.toStdString().c_str(), "a");
//		EXPECT_STREQ(name.toStdString().c_str(), "b");
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
