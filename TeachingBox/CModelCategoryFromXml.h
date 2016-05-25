#ifndef _TEACHING_BOX_C_MODEL_CATEGORY_FROM_XML_H_
#define _TEACHING_BOX_C_MODEL_CATEGORY_FROM_XML_H_

#include "CTreeModel.h"
#include "CSingleTon.h"
#include "QWidget"

class CModelCategoryFromXml:public CTreeModel
{
	friend class CSingleTon<CModelCategoryFromXml>;

private:
	CModelCategoryFromXml(QWidget* parent=0);
	CModelCategoryFromXml(const CModelCategoryFromXml&);
	CModelCategoryFromXml& operator=(const CModelCategoryFromXml&);
	~CModelCategoryFromXml();

public:
	static CModelCategoryFromXml* GetInstance();

	void UpdateRootData();
	CTreeItem* GetRootItem();
	
private:
	const char* CLASS_NAME = "CModelCategoryFromXml";
};

#endif