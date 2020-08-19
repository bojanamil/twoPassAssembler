#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <memory>
#include <vector>
#include <map>
#include "ProcessorDescriptor.h"
class AbstractSymbol;
class Segment;

class Context
{
private:
	std::map<std::string /*nameOfSymbol*/,std::shared_ptr<AbstractSymbol>> m_symbols;
	std::vector < std::shared_ptr<Segment>> m_segments;
	int m_currentAdress;

	std::shared_ptr<Segment> m_currentSegment;

	const ProcessorDescriptor m_processorDescriptor;
public: 
	Context();
	~Context();

	std::map<std::string /*nameOfSymbol*/, std::shared_ptr<AbstractSymbol>>& getSymbols(){ return m_symbols;	}
	const std::map<std::string /*nameOfSymbol*/, std::shared_ptr<AbstractSymbol>>& getSymbols() const { return m_symbols; }

	std::vector<std::shared_ptr<Segment>>& getSegments() { return m_segments; }
	const std::vector<std::shared_ptr<Segment>>& getSegments() const { return m_segments; }

	std::shared_ptr<AbstractSymbol> operator[](std::string _name);

	int getCurrentAdres() const { return m_currentAdress; };
	void resetCurrentAddress();
	void setCurrentAdres(int _currentAdress) {  m_currentAdress = _currentAdress; };

	const std::shared_ptr<Segment>& getCurrentSegment() const { return m_currentSegment; };
	std::shared_ptr<Segment>& getCurrentSegment() { return m_currentSegment; };
	void setCurrentSegment(const std::shared_ptr<Segment>& _currentSegment) { m_currentSegment = _currentSegment; };

	const ProcessorDescriptor& GetProcessorDescriptor() const  { return m_processorDescriptor; }

};

#endif