#ifndef _TEACHING_BOX_TINTEGER_H_
#define _TEACHING_BOX_TINTEGER_H_

#include "TVariate.h"

class TInteger:public TVariate
{
	Q_OBJECT

public:
	TInteger(const QString& scope, const QString& name,const int value);
	TInteger(QDataStream& dataStream);

public:
	int GetValue();
	void SetValue(const int value);
	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) const override;
	virtual void UpdateFromValue(const TVariate& variate) override;

protected slots:
	void SlotOnTextChanged(const QString& newValue);

private:
	int m_value;
};

#endif