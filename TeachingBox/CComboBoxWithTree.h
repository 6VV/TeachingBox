#ifndef _TEACHING_BOX_C_COMBOBOX_WITH_TREE_H_
#define _TEACHING_BOX_C_COMBOBOX_WITH_TREE_H_

#include "QComboBox"
#include "CWidgetWithTree.h"

class CComboBoxWithTree:public QComboBox,public CWidgetWithTree
{
	Q_OBJECT

public:
	CComboBoxWithTree(QTreeWidgetItem* const parentItem, QTreeWidget* const treeWidget);
};

#endif