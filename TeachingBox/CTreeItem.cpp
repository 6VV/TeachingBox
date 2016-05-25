#include "stdafx.h"
#include "CTreeItem.h"
#include <QStringList>

CTreeItem::CTreeItem(const QList<QVariant> &data, CTreeItem *parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

CTreeItem::~CTreeItem()
{
	ClearChildren();
}

void CTreeItem::appendChild(CTreeItem *item)
{
	m_childItems.append(item);
}

void CTreeItem::appendChildList(QList<CTreeItem*>& childList)
{
	for each (CTreeItem* item in childList)
	{
		m_childItems.append(item);
	}
}

CTreeItem *CTreeItem::child(int row)
{
	return m_childItems.value(row);
}

int CTreeItem::childCount() const
{
	return m_childItems.count();
}

int CTreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant CTreeItem::data(int column) const
{
	return m_itemData.value(column);
}

CTreeItem *CTreeItem::parentItem()
{
	return m_parentItem;
}

void CTreeItem::SetItemData(const QList<QVariant> &data)
{
	if (m_itemData.size() > 0)
	{
		m_itemData.clear();
	}
	m_itemData = data;
}

void CTreeItem::ClearChildren()
{
	if (m_childItems.size()>0)
	{
		qDeleteAll(m_childItems);
		m_childItems.clear();
	}
	
}

int CTreeItem::row() const
{
	if (m_parentItem)
	{
		return m_parentItem->m_childItems.indexOf(const_cast<CTreeItem*>(this));
	}
	return 0;
}

