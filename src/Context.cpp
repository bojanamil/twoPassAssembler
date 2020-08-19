#include "Context.h"



Context::Context()
{
}


Context::~Context()
{
}

std::shared_ptr<AbstractSymbol> Context::operator[](std::string _name)
{
	if (getSymbols().find(_name) == getSymbols().end()) return nullptr;
	return getSymbols()[_name];
}

void Context::resetCurrentAddress()
{
	m_currentAdress = 0;

}
