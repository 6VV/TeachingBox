#include "stdafx.h"
#include "CWidgetWithTree.h"


CWidgetWithTree::CWidgetWithTree(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	m_parentItem = parentItem;
	m_treeWidget = treeWidget;
}

CWidgetWithTree::~CWidgetWithTree()
{

}

QTreeWidget* const CWidgetWithTree::GetTreeWidget() const
{
	return m_treeWidget;
}

QTreeWidgetItem* const CWidgetWithTree::GetParentItem() const
{
	return m_parentItem;
}
