#ifndef _C_TEACHING_BOX_C_VARIABLE_TREE_ITEM_MANAGER_H_
#define _C_TEACHING_BOX_C_VARIABLE_TREE_ITEM_MANAGER_H_

#include "QTreeWidget"
#include "CValue.h"
#include "CTreeWidgetWithMonitor.h"

class CVariableTreeItemManager
{
public:
	CVariableTreeItemManager();
	~CVariableTreeItemManager();
	
	/*任意类型*/
	void GetVariableTreeWidgetItem(const QString& strName, const QString& strType, const QString& strScope,
		QTreeWidgetItem* item,CTreeWidgetWithMonitor* treeWidget);

protected:
	
private:
};

#endif