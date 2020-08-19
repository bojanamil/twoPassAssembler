#include "AbstractSymbol.h"



AbstractSymbol::AbstractSymbol(std::string _name)
	: m_value(0), m_isLocal(false), m_isDefined(false), m_name(_name), m_numberInSymbolTable(0)
{
}

AbstractSymbol::~AbstractSymbol()
{
}
