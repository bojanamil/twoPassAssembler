#include "Symbol.h"



Symbol::Symbol(std::string name) 
	:AbstractSymbol(name), m_expression(nullptr)
{
}


Symbol::~Symbol()
{
}

std::vector<std::shared_ptr<const AbstractSymbol>> Symbol::getDependableSymbols() const
{
	std::vector<std::shared_ptr<const AbstractSymbol>> result;
	if(getExpression() != nullptr)
	{
		return getExpression()->getDependableSymbols();
	}
	if(getType() == AbstractExpression::Relative)
	{
		result.push_back(shared_from_this());
	}
	return result;
}



