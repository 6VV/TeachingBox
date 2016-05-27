#include "stdafx.h"
#include "CLineEditWithTree.h"



CLineEditWithTree::CLineEditWithTree(QTreeWidgetItem* const parentItem, QTreeWidget* const treeWidget, 
	const QString& text/*=""*/, const QString& regExp/*=CRegExpManager::STR_REG_ANY_CHAR*/)
	:CLineEditWithRegExpAndKeyboard(text,regExp,treeWidget)
	, CWidgetWithTree(parentItem,treeWidget)
{
}

CLineEditWithTree::~CLineEditWithTree()
{

}
