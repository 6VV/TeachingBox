#include "stdafx.h"
#include "CTreeModel.h"
#include "CTreeItem.h"
#include <QStringList>
#include <QtGui>  

CTreeModel::CTreeModel(QObject *parent) :QAbstractItemModel(parent)
{
}

CTreeModel::~CTreeModel()
{
	if (rootItem)
	{
		delete rootItem;
	}
}

QVariant CTreeModel::data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	///*添加图标*/
	//if (role == Qt::DecorationRole&&index.column() == 0)
	//	return QIcon(":/Image/Image/Light_Green.png");

	/*显示节点数据值*/
	if (role == Qt::DisplayRole)
	{
		CTreeItem *item = static_cast<CTreeItem*>(index.internalPointer());
		return item->data(index.column());
	}

	return QVariant();
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal&&role == Qt::DisplayRole)
	{
		return rootItem->data(section);
	}
	return QVariant();
}

QModelIndex CTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	CTreeItem *parentItem;
	if (!parent.isValid())
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<CTreeItem*>(parent.internalPointer());
	}

	CTreeItem *childItem = parentItem->child(row);
	if (childItem)
	{
		return createIndex(row, column, childItem); /*展开树形,为子节点建立索引*/
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex CTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	CTreeItem *childItem = static_cast<CTreeItem*>(index.internalPointer());
	if (!childItem)
	{
		return QModelIndex();
	}

	CTreeItem *parentItem = childItem->parentItem();
	if (!parentItem || parentItem == rootItem)
	{
		return QModelIndex();
	}

	/*为父结点建立索引*/  
	return createIndex(parentItem->row(), 0, parentItem);
}

int CTreeModel::rowCount(const QModelIndex &parent) const
{
	CTreeItem *parentItem;

	if (!parent.isValid())
	{
		parentItem = rootItem;
	}
	else
	{
		parentItem = static_cast<CTreeItem*>(parent.internalPointer());
	}

	return parentItem->childCount(); /*返回父结点下子结点数目*/  
}

int CTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return static_cast<CTreeItem*>(parent.internalPointer())->columnCount();
	}
	else
	{
		return rootItem->columnCount();
	}
}
