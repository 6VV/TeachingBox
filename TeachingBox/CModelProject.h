#ifndef _TEACHING_BOX_C_MODEL_PROJECT_H_
#define _TEACHING_BOX_C_MODEL_PROJECT_H_

#include "CTreeModel.h"
#include "QVariant"
#include "QDir"
#include "CButton.h"
#include "CSingleTon.h"

class CModelProject:public CTreeModel
{
	friend class CSingleTon<CModelProject>;	/*����ģʽ*/

	Q_OBJECT

private:
	CModelProject(QObject* parent=0);
	CModelProject(const CModelProject&);
	CModelProject& operator=(const CModelProject&);
	~CModelProject();

	/*�Զ��幫�к���*/
public:
	static CModelProject* GetInstance();	/*��ȡΨһʵ��*/
	static bool IsCreated();

	void UpdateData();		/*����ģ������ */
	void UpdateRootData();	/*��������*/

	bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
private:
	void GetFileName(const QString & path,CTreeItem* parent);


private:
	const char* CLASS_NAME = "CModelProject";	//���������ڹ��ʻ����Է���
};

#endif