#ifndef _TEACHING_BOX_C_TREE_ITEM_H
#define _TEACHING_BOX_C_TREE_ITEM_H

#include <QList>
#include <QVariant>


class CTreeItem
{
public:
	explicit CTreeItem(const QList<QVariant> &data, CTreeItem *parentItem = 0);
	~CTreeItem();

	void appendChild(CTreeItem *child);
	void appendChildList(QList<CTreeItem*>& childList);

	CTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	CTreeItem *parentItem();

	/*自定义公有函数*/
public:
	void SetItemData(const QList<QVariant> &data);	/*刷新数据*/

	void ClearChildren();

private:
	QList<CTreeItem*> m_childItems;	/*子节点列表*/
	QList<QVariant> m_itemData;		/*子节点数据*/
	CTreeItem *m_parentItem=NULL;			/*父节点*/
};


#endif 