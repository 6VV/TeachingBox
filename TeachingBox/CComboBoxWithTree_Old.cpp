#include "stdafx.h"
#include "CComboBoxWithTree_Old.h"

CComboBoxWithTree_Old::CComboBoxWithTree_Old(QTreeWidgetItem* treeWidgetItem, QWidget* parent/*=0*/) 
	:QComboBox(parent)
{
	m_parentItem = treeWidgetItem;
}

CComboBoxWithTree_Old::CComboBoxWithTree_Old(QWidget* parent /*= 0*/) :QComboBox(parent)
{

}

CComboBoxWithTree_Old::~CComboBoxWithTree_Old()
{

}

void CComboBoxWithTree_Old::SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem)
{
	m_parentItem = treeWidgetItem;
}

void CComboBoxWithTree_Old::SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem, QTreeWidget* treeWidget)
{
	m_parentItem = treeWidgetItem;
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotItemChanged()));
	connect(this, SIGNAL(OnChangeTreeItem(QTreeWidgetItem*)), treeWidget, SLOT(SlotItemTextChanged(QTreeWidgetItem*)));
}


QTreeWidgetItem* CComboBoxWithTree_Old::GetTreeWidgetItem()
{
	return m_parentItem;
}

void CComboBoxWithTree_Old::SlotItemChanged()
{
	emit OnChangeTreeItem(m_parentItem);
}
