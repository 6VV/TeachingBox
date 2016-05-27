#ifndef _C_WIDGET_WITH_VARIATE_H_
#define _C_WIDGET_WITH_VARIATE_H_

#include "TVariate.h"
#include "QTreeWidget"
#include "QObject"

class CWidgetWithTree
{
public:
	CWidgetWithTree(QTreeWidgetItem* parentItem,QTreeWidget* treeWidget);
	~CWidgetWithTree();

	QTreeWidget* const GetTreeWidget() const;
	QTreeWidgetItem* const GetParentItem() const;

protected:
	QTreeWidgetItem* m_parentItem=nullptr;
	QTreeWidget* m_treeWidget=nullptr;
};

#endif