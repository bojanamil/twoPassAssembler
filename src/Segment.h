#ifndef _SEGMENT_H_
#define _SEGMENT_H_

#include "AbstractSymbol.h"
#include "AbstractRowDescriptor.h"
#include <vector>

class Segment :
	public AbstractSymbol,
	public std::enable_shared_from_this<Segment>
{
	int m_size;
	std::vector<std::shared_ptr<AbstractRowDescriptor>> m_rowDescriptors;
public:
	Segment(std::string name);
	~Segment();

	int getSize() const { return m_size; }
	void setSize(int _size) { m_size = _size; }
	std::shared_ptr<const Segment> getSegment() const override { return shared_from_this(); }
	std::vector<std::shared_ptr<AbstractRowDescriptor>>& getDescriptors() { return m_rowDescriptors; };
	std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const override;
};

#endif