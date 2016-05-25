#ifndef _TEACHING_BOX_C_TREEMODEL_H
#define _TEACHING_BOX_C_TREEMODEL_H

#include <QAbstractItemModel>  
#include "CTreeItem.h"  

class CTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	CTreeModel(QObject *parent = 0);
	~CTreeModel();

public:
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;	/*设置索引处的对象*/

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;	/*获取section处的列名*/
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;		/*获取索引下对应行列的数据*/
	QModelIndex parent(const QModelIndex &index) const;		/*获取父级索引*/
	int rowCount(const QModelIndex &parent = QModelIndex()) const;	/*获取索引节点下子节点的个数*/
	int columnCount(const QModelIndex &parent = QModelIndex()) const;	/*获取索引节点处的列数*/


protected:
	CTreeItem *rootItem=NULL; // 最顶层顶根节点(一个无效的QModelIndex)  
};

#endif