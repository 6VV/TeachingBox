#ifndef _TEACHING_BOX_TOBJECT_H_
#define _TEACHING_BOX_TOBJECT_H_

#include <unordered_map>
#include <QMap>
#include "QString"
#include "QDataStream"
#include "CSymbol.h"
#include "QTreeWidget"
#include "QObject"
#include "QSet"

class TVariate:public QObject
{
	Q_OBJECT

public:
	typedef QSet<TVariate*> SET;

public:
	TVariate(const QString& scope, const QString& name,CSymbol::SymbolType type);
	TVariate(QDataStream& dataStream);
	virtual ~TVariate(){};

public:
	QString GetScope() const;
	QString GetName() const;
	CSymbol::SymbolType GetType() const;

	void SetName(const QString& name);

	virtual void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget)=0;
	void ReadTreeWidgetItem(QTreeWidgetItem* parentItem, QTreeWidget* treeWidget, 
		const CSymbol::SymbolType type);

	void ReadDataStream(QDataStream& dataStream);
	void ReadCollection(SET& collection,const CSymbol::SymbolType type);

	void UpdateFromVariate(TVariate& variate);
	void UpdateRamAndDatabaseFrom(TVariate& variate) const;

protected:
	virtual void ReadValueStream(QDataStream& dataStream)=0;
	virtual void UpdateFromValue(TVariate& variate) =0;

protected:
	CSymbol m_symbol;
};

#endif