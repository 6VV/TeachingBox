#ifndef _TEACHING_BOX_C_MODEL_PROJECT_H_
#define _TEACHING_BOX_C_MODEL_PROJECT_H_

#include "CTreeModel.h"
#include "QVariant"
#include "QDir"
#include "CButton.h"
#include "CSingleTon.h"

class CModelProject:public CTreeModel
{
	friend class CSingleTon<CModelProject>;	/*单例模式*/

	Q_OBJECT

private:
	CModelProject(QObject* parent=0);
	CModelProject(const CModelProject&);
	CModelProject& operator=(const CModelProject&);
	~CModelProject();

	/*自定义公有函数*/
public:
	static CModelProject* GetInstance();	/*获取唯一实例*/
	static bool IsCreated();

	void UpdateData();		/*更新模型数据 */
	void UpdateRootData();	/*更新列名*/

	bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
private:
	void GetFileName(const QString & path,CTreeItem* parent);


private:
	const char* CLASS_NAME = "CModelProject";	//类名，用于国际化语言翻译
};

#endif