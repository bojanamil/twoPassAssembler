#ifndef _ABSTRACT_SYMBOL_H_
#define _ABSTRACT_SYMBOL_H_

#include "AbstractExpression.h"

class Segment;
class AbstractSymbol
{
private:
	int m_value;
	std::string m_name;
	bool m_isLocal;
	bool m_isDefined;
	AbstractExpression::Type m_type;

	int m_numberInSymbolTable;
public:

	AbstractSymbol(std::string name);
	virtual ~AbstractSymbol();

	int getValue() const { return m_value; }
	void setValue(int _value) { m_value = _value; }

	std::string getName() const { return m_name; }
	void setName(std::string _name) { m_name = _name; }

	bool isLocal() const { return m_isLocal; }
	void setIsLocal(bool _isLocal) { m_isLocal = _isLocal; }

	bool isDefined() const { return m_isDefined; }
	void setIsDefined(bool _isDefined) { m_isDefined = _isDefined; }

	AbstractExpression::Type getType() const {return m_type;}
	void setType(AbstractExpression::Type _type) { m_type = _type; }

	int getNumberInSymbolTable() const { return m_numberInSymbolTable; }
	void setNumberInSymbolTable(int _numberInSymbolTable) { m_numberInSymbolTable = _numberInSymbolTable; }
	
	virtual std::shared_ptr<const Segment> getSegment() const =0;

	virtual std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const =0;
};

#endif