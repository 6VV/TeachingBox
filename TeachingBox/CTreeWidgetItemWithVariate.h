#ifndef _C_TREE_WIDGET_ITEM_WITH_VARIATE_H_
#define _C_TREE_WIDGET_ITEM_WITH_VARIATE_H_

#include "QTreeWidget"
#include "TVariate.h"

class CTreeWidgetItemWithVariate:public QTreeWidgetItem
{
public:
	CTreeWidgetItemWithVariate(QTreeWidgetItem* parentItem,TVariate* variate);
	~CTreeWidgetItemWithVariate();
	
private:
	TVariate* m_variate;
};

#endif