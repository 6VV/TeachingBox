﻿#ifndef _TEACHING_BOX_TDOUBLE_H_
#define _TEACHING_BOX_TDOUBLE_H_

#include "TVariate.h"

class TDouble:public TVariate
{
	Q_OBJECT

public:
	TDouble(const QString& scope, const QString& name, const double value);
	TDouble(QDataStream& dataStream);

	double GetValue();
	void SetValue(const double value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;
	virtual void UpdateFrom(TVariate& variate) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) override;

protected slots:
	void SlotOnTextChanged(const QString& newValue);


private:
	double m_value;
};

#endif