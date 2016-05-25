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

	/*位置变量*/
	void GetPositionTreeWidgetItem(const QString& strName, const CValue::TYPE_POSITION& position,
		QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget);	/*获取特定位置变量节点*/
	void GetPositionTreeWidgetItem(const QString& strScope, const QString& strName,QTreeWidgetItem* item,
		CTreeWidgetWithMonitor* treeWidget);	/*获取某作用域及上层作用域内的最近变量*/
	void GetAllPositionTreeWidgetItemInExactScope(const QString& strScope,
		QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget);	/*获取某特定作用域内所有位置变量*/

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

	/*获取布尔变量*/
	void GetBoolTreeWidgetItem(const QString& strName, const int value,
		QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget);	/*获取特定变量节点*/
	void GetBoolTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item,
		CTreeWidgetWithMonitor* treeWidget);	/*获取某作用域及上层作用域内的最近变量*/
	void GetAllBoolTreeWidgetItemInExactScope(const QString& strScope,
		QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget);	/*获取某特定作用域内所有变量*/

	/*获取字符串变量*/
	void GetStringTreeWidgetItem(const QString& strName, const std::string& value,
		QTreeWidgetItem* item, CTreeWidgetWithMonitor* treeWidget);	/*获取特定变量节点*/
	void GetStringTreeWidgetItem(const QString& strScope, const QString& strName, QTreeWidgetItem* item,
		CTreeWidgetWithMonitor* treeWidget);	/*获取某作用域及上层作用域内的最近变量*/
	void GetAllStringTreeWidgetItemInExactScope(const QString& strScope,
		QTreeWidgetItem* parentItem, CTreeWidgetWithMonitor* treeWidget);	/*获取某特定作用域内所有变量*/
protected:
	
private:
};

#endif