#include "stdafx.h"
#include "CTreeWidgetItemWithVariate.h"

CTreeWidgetItemWithVariate::CTreeWidgetItemWithVariate(QTreeWidgetItem* parentItem, TVariate* variate)
	:QTreeWidgetItem(parentItem), m_variate(variate)
{
	this->setText(0, variate->GetName());
	this->setText(1, CSymbol::Map_Type[variate->GetType()]);
	this->setText(2, variate->GetScope());
}

CTreeWidgetItemWithVariate::~CTreeWidgetItemWithVariate()
{

}
