#include "stdafx.h"
#include "TOverlap.h"
#include "CTreeWidgetItemWithVariate.h"
#include "CRegExpManager.h"
#include "CComboBoxWithTree.h"
#include "CLineEditWithTree.h"
#include "DataStruct.h"

const QString TOverlap::STR_MODE_ABSOLUTE = "Absolute";
const QString TOverlap::STR_MODE_RELATIVE = "Relative";

TOverlap::TOverlap(const QString& scope, const QString& name, const tOverlapConstraint& value)
	:TVariate(scope, name, CSymbol::TYPE_OVERLAP), m_value(value)
{
}

TOverlap::TOverlap(QDataStream& dataStream) : TVariate(dataStream)
{
	WriteValueSteam(dataStream);
}

const tOverlapConstraint& TOverlap::GetValue() const
{
	return m_value;
}

void TOverlap::SetValue(const tOverlapConstraint& value)
{
	m_value = value;
}

void TOverlap::ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)
{
	CTreeWidgetItemWithVariate* variateItem = new CTreeWidgetItemWithVariate(parentItem, this);

	CComboBoxWithTree* modeBox = new CComboBoxWithTree(variateItem,treeWidget);
	modeBox->addItem(STR_MODE_ABSOLUTE);
	modeBox->addItem(STR_MODE_RELATIVE);

	switch (m_value.m_TransitionMode)
	{
	case tTransitionMode::kTransitionModeAbsolute:
	{
		modeBox->setCurrentText(STR_MODE_ABSOLUTE);
	}break;
	case tTransitionMode::kTransitionModeRelative:
	{
		modeBox->setCurrentText(STR_MODE_RELATIVE);
	}break;
	default:
		break;
	}

	QTreeWidgetItem* modeItem = new QTreeWidgetItem(variateItem, QStringList{ "Mode" });
	treeWidget->setItemWidget(modeItem, 1, modeBox);
	connect(modeBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(SlotOnTextChanged()));

	CLineEditWithTree* paraEdit = new CLineEditWithTree(variateItem, treeWidget, QString::number(m_value.m_TransitionParameter), CRegExpManager::STR_REG_FLOAT);
	QTreeWidgetItem* parameterItem = new QTreeWidgetItem(variateItem, QStringList{ "Value" });
	treeWidget->setItemWidget(parameterItem, 1, paraEdit);
	connect(paraEdit, SIGNAL(textChanged(const QString&)), this, SLOT(SlotOnTextChanged()));
}

void TOverlap::ReadValueStream(QDataStream& dataStream)
{
	dataStream << m_value.m_TransitionMode;
	dataStream << m_value.m_TransitionParameter;
}

void TOverlap::UpdateFromValue(TVariate& variate)
{
	m_value = static_cast<TOverlap&>(variate).m_value;
}

void TOverlap::WriteValueSteam(QDataStream& dataStream)
{
	dataStream >> m_value.m_TransitionMode;
	dataStream >> m_value.m_TransitionParameter;
}

void TOverlap::SlotOnTextChanged()
{
	CWidgetWithTree* widget = dynamic_cast<CWidgetWithTree*>(sender());
	QTreeWidgetItem* variateItem = widget->GetParentItem();
	QTreeWidget* treeWidget = widget->GetTreeWidget();

	QString mode = static_cast<QComboBox*>(treeWidget->itemWidget(variateItem->child(0), 1))->currentText();
	if (mode==STR_MODE_ABSOLUTE)
	{
		m_value.m_TransitionMode = tTransitionMode::kTransitionModeAbsolute;
	}
	else
	{
		m_value.m_TransitionMode = tTransitionMode::kTransitionModeRelative;
	}

	m_value.m_TransitionParameter = static_cast<QLineEdit*>(treeWidget->itemWidget(variateItem->child(1), 1))->text().toDouble();

	UpdateRamAndDatabaseFrom(*this);
}