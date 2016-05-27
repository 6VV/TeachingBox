#include "stdafx.h"
#include "CComboBoxWithTree.h"

CComboBoxWithTree::CComboBoxWithTree(QTreeWidgetItem* const parentItem, QTreeWidget* const treeWidget)
	:QComboBox(treeWidget)
	,CWidgetWithTree(parentItem,treeWidget)
{
}
