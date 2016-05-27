#ifndef _TEACHING_BOX_C_LINE_EIDT_WITH_TREE_H_
#define _TEACHING_BOX_C_LINE_EIDT_WITH_TREE_H_

#include "CLineEditWithRegExpAndKeyboard.h"
#include "CWidgetWithTree.h"

class CLineEditWithTree:public CLineEditWithRegExpAndKeyboard,public CWidgetWithTree
{
public:
	CLineEditWithTree(QTreeWidgetItem* const parentItem,QTreeWidget* const treeWidget,
		const QString& text="", const QString& regExp=CRegExpManager::STR_REG_ANY_CHAR);
	~CLineEditWithTree();
	
private:
};

#endif