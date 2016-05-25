//#include "TVariateManagerTests.h"
//#include "..\TeachingBox\TInteger.h"
//#include "..\TeachingBox\TVariateManager.h"
//#include <exception>
//#include "QSet"
//
//TEST_F(TVariateManagerTests, Add_AddVariate_NoException)
//{
//	TInteger* interger=new TInteger("a", "b", 3);
//	try
//	{
//		TVariateManager::GetInstance()->Add(interger);
//		auto var=TVariateManager::GetInstance()->GetVariate(interger->GetScope(), interger->GetName());
//		EXPECT_STREQ(var->GetScope().toStdString().c_str(), "a");
//		EXPECT_STREQ(var->GetName().toStdString().c_str(), "b");
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//TEST_F(TVariateManagerTests, Delete_DeleteVariate_NoException)
//{
//	TInteger* interger = new TInteger("a", "b", 3);
//	try
//	{
//		TVariateManager::GetInstance()->Add(interger);
//		TVariateManager::GetInstance()->Delete(interger->GetScope(), interger->GetName());
//		auto varNew = TVariateManager::GetInstance()->GetVariate("a", "b");
//		EXPECT_EQ(varNew, nullptr);
//	}
//	catch (std::exception& e)
//	{
//		FAIL() << e.what();
//	}
//}
//
//void TVariateManagerTests::TearDown()
//{
//	TVariateManager::GetInstance()->ClearProjectData();
//	CDatabaseManager::GetInstance()->DeleteAllVariate();
//}
//
//TEST_F(TVariateManagerTests, ReadCollection_Read_Equal)
//{
//	TVariateManager::GetInstance()->Add(new TInteger("a", "b", 1));
//	TVariateManager::GetInstance()->Add(new TInteger("a", "c", 2));
//	TVariateManager::GetInstance()->Add(new TInteger("c", "d", 3));
//	TVariateManager::GetInstance()->LoadProjectDataFromDatabase("a", QStringList{ "c" });
//
//	QSet<TVariate*> collection;
//	TVariateManager::GetInstance()->ReadCollection(collection, "a",CSymbol::TYPE_INTERGER);
//	EXPECT_EQ(collection.size(), 2);
//	collection.clear();
//	TVariateManager::GetInstance()->ReadCollection(collection, "c", CSymbol::TYPE_INTERGER);
//	EXPECT_EQ(collection.size(), 1);
//}
//
//TEST_F(TVariateManagerTests, Update_UpdateNewVariate_Equal)
//{
//	TVariateManager::GetInstance()->Add(new TInteger("a", "b", 1));
//	TVariateManager::GetInstance()->Update("a", "b", TInteger("a", "c", 2));
//	TVariateManager::GetInstance()->LoadProjectDataFromDatabase("a",QStringList());
//	TVariate* variate = TVariateManager::GetInstance()->GetVariate("a", "c");
//	if (variate==nullptr)
//	{
//		FAIL() << "nullptr";
//	}
//	else
//	{
//		EXPECT_EQ(static_cast<TInteger*>(variate)->GetValue(), 2);
//	}
//}
//
//TEST_F(TVariateManagerTests, LoadScopeDataFromDatabase_Load_Equal)
//{
//	TVariateManager::GetInstance()->Add(new TInteger("a", "a", 1));
//	TVariateManager::GetInstance()->Add(new TInteger("a.1", "b", 2));
//	TVariateManager::GetInstance()->Add(new TInteger("a.2", "c", 3));
//
//	TVariateManager::GetInstance()->LoadProjectDataFromDatabase("a", QStringList{ "a.1", "a.2" });
//	TVariate* variate=TVariateManager::GetInstance()->GetVariate("a", "a");
//
//	if (variate == nullptr)
//	{
//		FAIL() << "nullptr";
//	}
//	else
//	{
//		EXPECT_EQ(static_cast<TInteger*>(variate)->GetValue(), 1);
//	}
//}
//
//TEST_F(TVariateManagerTests, IsExistVariate_ExitOrNot_Match)
//{
//	TVariateManager::GetInstance()->Add(new TInteger("a", "b", 1));
//	TVariateManager::GetInstance()->LoadProjectDataFromDatabase("a", QStringList{ "a.1", "a.2" });
//
//	EXPECT_TRUE(TVariateManager::GetInstance()->IsExistVariateScrollUp("a", "b"));
//	EXPECT_FALSE(TVariateManager::GetInstance()->IsExistVariateScrollUp("a", "c"));
//	EXPECT_FALSE(TVariateManager::GetInstance()->IsExistVariateScrollUp("b", "c"));
//}