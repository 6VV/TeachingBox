#ifndef _TEACHING_BOX_C_COMBO_BOX_WITH_TREE_ITEM_H_
#define _TEACHING_BOX_C_COMBO_BOX_WITH_TREE_ITEM_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CComboBoxWithTreeItem.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2016/03/10
//  Description:	该类记录包含其的QTreeWidgetItem,用于防止内容改变时，没有正确的触发QTreeWigetItem改变事件
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2016/03/10
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#include "QComboBox"
#include "QTreeWidget"
#include "CTreeWidgetWithMonitor.h"
#include "CWidgetWithTree.h"

class CComboBoxWithTree_Old:public QComboBox
{
	Q_OBJECT

public:
	CComboBoxWithTree_Old(QWidget* parent = 0);
	CComboBoxWithTree_Old(QTreeWidgetItem* treeWidgetItem,QWidget* parent=0);
	~CComboBoxWithTree_Old();

	void SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem);
	void SetTreeWidgetItem(QTreeWidgetItem* treeWidgetItem, CTreeWidgetWithMonitor*);
	QTreeWidgetItem* GetTreeWidgetItem();

signals:
	void OnChangeTreeItem(QTreeWidgetItem*);

protected slots:
	void SlotItemChanged();

private:
	QTreeWidgetItem* m_parentItem;
};
#endif