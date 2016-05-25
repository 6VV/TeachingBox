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
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const;	/*�����������Ķ���*/

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;	/*��ȡsection��������*/
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;		/*��ȡ�����¶�Ӧ���е�����*/
	QModelIndex parent(const QModelIndex &index) const;		/*��ȡ��������*/
	int rowCount(const QModelIndex &parent = QModelIndex()) const;	/*��ȡ�����ڵ����ӽڵ�ĸ���*/
	int columnCount(const QModelIndex &parent = QModelIndex()) const;	/*��ȡ�����ڵ㴦������*/


protected:
	CTreeItem *rootItem=NULL; // ��㶥���ڵ�(һ����Ч��QModelIndex)  
};

#endif