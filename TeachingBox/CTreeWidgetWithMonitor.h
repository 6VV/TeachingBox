#ifndef _TEACHING_BOX_C_TREE_WIDGET_WITH_MONITOR_H_
#define _TEACHING_BOX_C_TREE_WIDGET_WITH_MONITOR_H_

#include "QTreeWidget"

class CTreeWidgetWithMonitor:public QTreeWidget
{
	Q_OBJECT

public:
	CTreeWidgetWithMonitor(QWidget* parent=0);
	~CTreeWidgetWithMonitor();

protected slots:
	virtual void SlotItemTextChanged(QTreeWidgetItem* item);
};

#endif