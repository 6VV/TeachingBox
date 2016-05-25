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

	/*动态变量*/
	void GetDynamicTreeWidgetItem(const QString& strName, const CValue::TYPE_DYNAMIC& dynamic,
		QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget);	/*获取特定变量节点*/
	void GetDynamicTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item,
		CTreeWidgetWithMonitor* treeWidget);	/*获取某作用域及上层作用域内的最近变量*/
	void GetAllDynamicTreeWidgetItemInExactScope(const QString& strScope,
		QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget);	/*获取某特定作用域内所有变量*/

	/*过渡变量*/
	void GetOverlapTreeWidgetItem(const QString& strName, const CValue::TYPE_OVERLAP& overlap,
		QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget);	/*获取特定变量节点*/
	void GetOverlapTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item,
		CTreeWidgetWithMonitor* treeWidget);	/*获取某作用域及上层作用域内的最近变量*/
	void GetAllOverlapTreeWidgetItemInExactScope(const QString& strScope,
		QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget);	/*获取某特定作用域内所有变量*/


protected:
	
private:
};

#endif