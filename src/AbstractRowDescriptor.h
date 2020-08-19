#ifndef _ABSTRACT_ROW_DESCRIPTOR_H
#define _ABSTRACT_ROW_DESCRIPTOR_H
#include "InstructionDescriptor.h"

class InstructionDescriptor;
class AbstractExpression;
#include <memory>
#include <vector>
class Segment;

class AbstractInstructionParameter;

class AbstractRowDescriptor {

public:
	virtual ~AbstractRowDescriptor() {};
	virtual unsigned getRowSize()const =0;
};

class InstructionRowDescriptor : public AbstractRowDescriptor {
	const std::shared_ptr<InstructionDescriptor>& m_instruction;
	const std::vector<std::shared_ptr<AbstractInstructionParameter>> m_parameters;

public:
	InstructionRowDescriptor(const std::shared_ptr<InstructionDescriptor>& _instruction, const std::vector<std::shared_ptr<AbstractInstructionParameter>> _parameters) 
		: m_instruction(_instruction), m_parameters (_parameters) { };

	const std::shared_ptr<InstructionDescriptor>& getInstruction( ) const  { return m_instruction; };
	unsigned getRowSize() const override;
	const std::vector<std::shared_ptr<AbstractInstructionParameter>> getParameters() const { return m_parameters; };
	void outputReallocations(int _currentAddress, std::shared_ptr<Segment> _currentSegment, std::ostream& outputStream) const;
	std::vector<std::string> makeContent(std::shared_ptr<Segment> _currentSegment, int _currentAddress)const ;
	unsigned getOperandType() const;
	InstructionDescriptor::AddressyingType getAddressingMode() const;
};


class DataRowDescriptor : public AbstractRowDescriptor {
	const int m_sizeOfData;
	const  std::vector<std::pair<unsigned, std::shared_ptr<AbstractExpression>>> m_data;
public:
	DataRowDescriptor(int _sizeOfData, std::vector<std::pair<unsigned, std::shared_ptr<AbstractExpression>>> _data) : m_sizeOfData(_sizeOfData), m_data(_data) {};
	unsigned getRowSize() const override;
	void outputReallocations(int _currentAddress, std::ostream& outputStream) const;
	std::vector<std::string> makeContent() const ;
};


#endif