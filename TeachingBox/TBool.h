#ifndef _TEACHING_BOX_TBOOL_H_
#define _TEACHING_BOX_TBOOL_H_

#include "TVariate.h"
#include "QString"

class TBool:public TVariate
{
	Q_OBJECT

public:
	const static QString STR_VALUE_TRUE;
	const static QString STR_VALUE_FALSE;

public:
	TBool(const QString& scope, const QString& name, const bool value);
	TBool(QDataStream& dataStream);

	const bool GetValue() const;
	void SetValue(const bool value);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget) override;
	virtual void UpdateFrom(TVariate& variate) override;

protected:
	virtual void ReadValueStream(QDataStream& dataStream) override;

protected slots:
	void SlotOnIndexChanged(const QString&);


private:
	bool m_value;
};

#endif