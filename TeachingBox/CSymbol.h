#ifndef _INTERPRETER_C_SYMBOL_H_
#define _INTERPRETER_C_SYMBOL_H_

#include "QString"
#include "QDataStream"
#include "QObject"
#include "QVector"
#include "QHash"

class CSymbol:public QObject
{
	Q_OBJECT
public:
	enum SymbolType
	{
		TYPE_VOID,
		TYPE_INTERGER,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_BOOL,
		TYPE_POSITION,
		TYPE_DYNAMIC,
		TYPE_OVERLAP,
	};

	enum SymbolCategory
	{
		CATEGORY_METHOD,
		CATEGORY_VARIABLE,
	};

	static QHash<SymbolType, QString> Map_Type;

public:
	CSymbol(const QString& scope, const QString& name, const SymbolCategory category, const SymbolType type);
	CSymbol(QDataStream& dataStream);
	CSymbol& operator=(const CSymbol& symbol);

	SymbolType GetType() const;
	SymbolCategory GetCategory() const;
	QString GetName() const;
	QString GetScope() const;

	void SetName(const QString& name);

	void ReadDataStream(QDataStream& dataStream) const;

protected:
	QString m_scope;
	QString m_name;
	int m_type=TYPE_VOID;
	SymbolCategory m_category=CATEGORY_VARIABLE;

private:
	class CInitMap
	{
	public:
		CInitMap(QHash<CSymbol::SymbolType, QString>& mapType)
		{
			mapType.insert(TYPE_VOID, "VOID");
			mapType.insert(TYPE_INTERGER, "INTERGER");
			mapType.insert(TYPE_DOUBLE, "DOUBLE");
			mapType.insert(TYPE_STRING, "STRING");
			mapType.insert(TYPE_BOOL, "BOOL");
			mapType.insert(TYPE_POSITION, "POSITION");
			mapType.insert(TYPE_DYNAMIC, "DYNAMIC");
			mapType.insert(TYPE_OVERLAP, "OVERLAP");
		}
	};
	static CInitMap m_init;


};

#endif