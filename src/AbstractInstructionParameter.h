#ifndef _ABSTRACT_INSTRUCTION_PARAMETER_H
#define _ABSTRACT_INSTRUCTION_PARAMETER_H

class RegisterDescriptor;
class AbstractExpression;
#include <memory>
#include "InstructionDescriptor.h"

class AbstractInstructionParameter {

	InstructionDescriptor::AddressyingType m_addressingType;
public:
	AbstractInstructionParameter(InstructionDescriptor::AddressyingType _addressingType);

	virtual ~AbstractInstructionParameter() {};

	virtual InstructionDescriptor::AddressyingType getAddressyingType() { return m_addressingType; };
};

class RegisterInstructionParameter : public virtual AbstractInstructionParameter {
public:
	
private:
	const std::shared_ptr<RegisterDescriptor> m_register;
	const InstructionDescriptor::RegisterPosition m_registerPosition;
public:
	RegisterInstructionParameter(const std::shared_ptr<RegisterDescriptor> _register, InstructionDescriptor::AddressyingType _addressingType, InstructionDescriptor::RegisterPosition _registerPosition);
	InstructionDescriptor::RegisterPosition getRegisterPosition() const { return m_registerPosition; }
	const std::shared_ptr<RegisterDescriptor>& getRegisterDescriptor() const { return m_register; }
};

class ExpressionParameter : public virtual AbstractInstructionParameter {
	std::shared_ptr<AbstractExpression> m_expression;
public:
	ExpressionParameter(InstructionDescriptor::AddressyingType _addressingType, std::shared_ptr<AbstractExpression> _expression);
	std::shared_ptr<AbstractExpression> getExpression() const { return m_expression; }

};

class RegisterInstructionParameterWithOffset : public RegisterInstructionParameter, public ExpressionParameter {

public:
	RegisterInstructionParameterWithOffset(std::shared_ptr<RegisterDescriptor> _register, InstructionDescriptor::AddressyingType _addressingType, std::shared_ptr<AbstractExpression> _offset, InstructionDescriptor::RegisterPosition _registerPosition);
};




#endif