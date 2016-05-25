#ifndef _TEACHING_BOX_TPOSITION_H_
#define _TEACHING_BOX_TPOSITION_H_

#include "TVariate.h"
#include "QVector"

class TPosition:public TVariate
{
	Q_OBJECT

public:
	static const int AXIS_SIZE = 6;
	typedef QVector<double> TYPE_POSITION;

public:
	TPosition(const QString& scope, const QString& name, const TYPE_POSITION& value);
	TPosition(QDataStream& dataStream);

	const TYPE_POSITION& GetValue() const;
	void SetValue(const TYPE_POSITION& value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) override;

	virtual void UpdateFromValue(TVariate& variate) override;

protected slots:
	void SlotOnTextChanged();

private:
	TYPE_POSITION m_value;
};

#endif