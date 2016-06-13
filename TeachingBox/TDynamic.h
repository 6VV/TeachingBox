#ifndef _TEACHING_BOX_TDYNAMIC_H_
#define _TEACHING_BOX_TDYNAMIC_H_

#include "TVariate.h"
#include "DataStruct.h"

class TDynamic:public TVariate
{
	Q_OBJECT
public:
	TDynamic(const QString& scope, const QString& name, const tDynamicConstraint& value);
	TDynamic(QDataStream& dataStream);

	const tDynamicConstraint& GetValue() const;
	void SetValue(const tDynamicConstraint& value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) const override;
	virtual void UpdateFromValue(const TVariate& variate) override;

	void WriteValueStream(QDataStream& dataStream);

protected slots:
	void SlotOnTextChanged();

private:
	tDynamicConstraint m_value;
};

#endif