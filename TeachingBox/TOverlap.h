#ifndef _TEACHING_BOX_TOVERLAP_H_
#define _TEACHING_BOX_TOVERLAP_H_

#include "TVariate.h"
#include "DataStruct.h"

class TOverlap:public TVariate
{
	Q_OBJECT

public:
	static const QString STR_MODE_ABSOLUTE;
	static const QString STR_MODE_RELATIVE;

public:
	TOverlap(const QString& scope, const QString& name, const tOverlapConstraint& value);
	TOverlap(QDataStream& dataStream);

	const tOverlapConstraint& GetValue() const;
	void SetValue(const tOverlapConstraint& value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) const override;
	virtual void UpdateFromValue(const TVariate& variate) override;

	void WriteValueSteam(QDataStream& dataStream);

protected slots:
	void SlotOnTextChanged();

private:
	tOverlapConstraint m_value;
};

#endif