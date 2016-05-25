﻿#ifndef _TEACHING_BOX_TSTRING_H_
#define _TEACHING_BOX_TSTRING_H_

#include "TVariate.h"

class TString:public TVariate
{
public:
	TString(const QString& scope, const QString& name, const QString& value);
	TString(QDataStream& dataStream);

	const QString& GetValue() const;
	void SetValue(const QString& value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;

	virtual void UpdateFrom(TVariate& variate) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) override;

protected slots:
	void SlotOnTextChanged(const QString& text);

private:
	QString m_string;
};

#endif