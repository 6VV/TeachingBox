#ifndef _TEACHING_BOX_TPOSITION_H_
#define _TEACHING_BOX_TPOSITION_H_

#include "TVariate.h"
#include "QVector"
#include "DataStruct.h"

class TPosition:public TVariate
{
	Q_OBJECT

public:
	TPosition(const QString& scope, const QString& name, const tAxesAllPositions& value);
	TPosition(QDataStream& dataStream);

	const tAxesAllPositions GetValue() const;

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) const override;

	virtual void UpdateFromValue(const TVariate& variate) override;

protected slots:
	void SlotOnTextChanged();

private:
	tAxesAllPositions m_value;
};

#endif