#ifndef _TEACHING_BOX_C_MODEL_MACRO_FROM_XML_H_
#define _TEACHING_BOX_C_MODEL_MACRO_FROM_XML_H_

#include "CTreeModel.h"
#include "CSingleTon.h"

class CModelMacroFromXml:public CTreeModel
{
	friend class CSingleTon<CModelMacroFromXml>;

private:
	CModelMacroFromXml(QWidget* parent=0);
	CModelMacroFromXml(const CModelMacroFromXml&);
	CModelMacroFromXml& operator=(const CModelMacroFromXml&);
	~CModelMacroFromXml();

	/*自定义公有函数*/
public:
	static CModelMacroFromXml* GetInstance();
	
	void UpdateData(QList<QString>& childrenItems);

	/*自定义私有函数*/
private:
	void UpdateRootData();

private:
	QList<QVariant> m_rootData;
};

#endif