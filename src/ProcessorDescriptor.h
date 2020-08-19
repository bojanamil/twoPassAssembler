#ifndef _PROCESSOR_DESCRIPTOR_H
#define _PROCESSOR_DESCRIPTOR_H

#include <vector>
#include <memory>

class RegisterDescriptor;

class InstructionDescriptor;

class ProcessorDescriptor {
	std::vector<std::shared_ptr<InstructionDescriptor>> m_listOfInstructions;
	std::vector<std::shared_ptr<RegisterDescriptor>> m_listOfRegisters;
public:
	ProcessorDescriptor();
	~ProcessorDescriptor(){};
	void initializeInstructions();
	const std::vector<std::shared_ptr<InstructionDescriptor>>& getInstructions() const { return m_listOfInstructions; };
	void initializeRegisters();
	const std::vector<std::shared_ptr<RegisterDescriptor>>& getRegisters() const { return m_listOfRegisters; };
	bool isInTheRegisterList(std::string param) const;
	std::shared_ptr<RegisterDescriptor>  getRegister(std::string param) const;
};

#endif