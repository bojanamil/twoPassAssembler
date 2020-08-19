#include "Segment.h"



Segment::Segment(std::string name)
	:AbstractSymbol(name)
{
}


Segment::~Segment()
{
}

std::vector<std::shared_ptr<const AbstractSymbol>> Segment::getDependableSymbols() const
{
	auto result = std::vector<std::shared_ptr<const AbstractSymbol>>();
	result.push_back(shared_from_this());
	return result;

}
