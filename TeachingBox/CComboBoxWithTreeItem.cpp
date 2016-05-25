#include "stdafx.h"
#include "CComboBoxWithTreeItem.h"

CComboBoxWithTreeItem::CComboBoxWithTreeItem(QTreeWidgetItem* treeWidgetItem, QWidget* parent/*=0*/) :QComboBox(parent)
{
	m_treeWidgetItem = treeWidgetItem;
}

CComboBoxWithTreeItem::CComboBoxWithTreeItem(QWidget* parent /*= 0*/) :QComboBox(parent)
{

}

CComboBoxWithTreeItem::~CComboBoxWithTreeItem()
{

}

void CComboBoxWithTreeItem::SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem)
{
	m_treeWidgetItem = treeWidgetItem;
}

void CComboBoxWithTreeItem::SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem, CTreeWidgetWithMonitor* treeWidget)
{
	m_treeWidgetItem = treeWidgetItem;
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotItemChanged()));
	connect(this, SIGNAL(OnChangeTreeItem(QTreeWidgetItem*)), treeWidget, SLOT(SlotItemTextChanged(QTreeWidgetItem*)));
}

QTreeWidgetItem* CComboBoxWithTreeItem::GetTreeWidgetItem()
{
	return m_treeWidgetItem;
}

void CComboBoxWithTreeItem::SlotItemChanged()
{
	emit OnChangeTreeItem(m_treeWidgetItem);
}
