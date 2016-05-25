#include "stdafx.h"
#include "CModelMacro.h"
#include "CModelCategory.h"
#include "CGrammarManagerFactory.h"

CModelMacro::CModelMacro()
{
	UpdateData();
}

CModelMacro::~CModelMacro()
{

}

CModelMacro* CModelMacro::GetInstance()
{
	return CSingleTon<CModelMacro>::GetInstance();
}

void CModelMacro::UpdateData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	UpdateRootData();

	endResetModel();
}

void CModelMacro::UpdateRootData()
{
	if (m_rootData.isEmpty())
	{
		m_rootData << QCoreApplication::translate("CModelMacro", "Macro");
	}

	if (rootItem == NULL)
	{
		rootItem = new CTreeItem(m_rootData);
	}
	else
	{
		rootItem->SetItemData(m_rootData);
	}
}

void CModelMacro::ChangeToData(QString category)
{
	CModelCategory* modelCategory = CModelCategory::GetInstance();

	if (category == modelCategory->STR_MOVEMENT)
	{
		ChangeToMovementData();
	}
	else if (category == modelCategory->STR_SYSTEM_MATH)
	{
		ChangeToSystemMathData();
	}
	else if (category == modelCategory->STR_SYSTEM_STRUCTURE)
	{
		ChangeToSystemStructureData();
	}
	else if (category == modelCategory->STR_SYSTEM_PROGRAM)
	{
		ChangeToSystemPragramData();
	}
	else if (category == modelCategory->STR_SYSTEM_FUNCTION)
	{
		ChangeToSystemFunctionData();
	}
	else if (category == modelCategory->STR_SYSTEM_OTHERS)
	{
		ChangeToSystemOthersData();
	}
	else if (category == modelCategory->STR_IO_DIGITAL)
	{
		ChangeToIODigitalData();
	}
	else if (category == modelCategory->STR_IO_ANALOG)
	{
		ChangeToIOAnalogData();
	}
	else if (category == modelCategory->STR_WELD_ARC)
	{
		ChangeToWeldArcData();
	}
	else if (category == modelCategory->STR_WELD_SPOT)
	{
		ChangeToWeldSpotData();
	}
	else
	{
		return;
	}

	m_rootData.clear();
	m_rootData << category;
	UpdateRootData();
}

void CModelMacro::GetChildItem(CTreeItem* parentItem, QString childName)
{
	QList<QVariant> childData;
	childData << childName;
	CTreeItem* childItem = new CTreeItem(childData, parentItem);
	parentItem->appendChild(childItem);
}

void CModelMacro::ChangeToMovementData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVL);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVJ);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVJX);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVC);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVX);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVY);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_MOVZ);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ROTX);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ROTY);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ROTZ);

	endResetModel();
}

void CModelMacro::ChangeToSystemMathData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SIN);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_COS);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_TAN);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SQRT);

	endResetModel();
}

void CModelMacro::ChangeToSystemStructureData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_LABEL);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_GOTO);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_GOSUB);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_FOR_NEXT);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ELSEIF);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_IF_ENDIF);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SWITCH_DEFAULT_ENDSWITCH);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_CASE);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_DEFAULT);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_BREAK);

	endResetModel();
}

void CModelMacro::ChangeToSystemPragramData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ABORT);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_END);

	endResetModel();
}

void CModelMacro::ChangeToSystemFunctionData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_WAIT);

	endResetModel();
}

void CModelMacro::ChangeToSystemOthersData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SET_TOOLFRAME);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SET_USERFRAME);

	endResetModel();
}

void CModelMacro::ChangeToIODigitalData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_DIN);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_DOUT);

	endResetModel();
}

void CModelMacro::ChangeToIOAnalogData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_AIN);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_AOUT);

	endResetModel();
}

void CModelMacro::ChangeToWeldArcData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ARC);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_ARC_OFF);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SET_ARC);

	endResetModel();
}

void CModelMacro::ChangeToWeldSpotData()
{
	beginResetModel();

	if (rootItem)
	{
		rootItem->ClearChildren();
	}

	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SPOT_ON);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SPOT_OFF);
	GetChildItem(rootItem, CGrammarManagerFactory::STR_MACRO_SET_SPOT);

	endResetModel();
}

