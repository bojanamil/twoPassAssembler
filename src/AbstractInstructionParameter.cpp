#include "AbstractInstructionParameter.h"

AbstractInstructionParameter::AbstractInstructionParameter(InstructionDescriptor::AddressyingType _addressingType)
	:m_addressingType(_addressingType)
{
}

RegisterInstructionParameter::RegisterInstructionParameter(const std::shared_ptr<RegisterDescriptor> _register, InstructionDescriptor::AddressyingType _addressingType, InstructionDescriptor::RegisterPosition _registerPosition)
	: AbstractInstructionParameter(_addressingType), m_register(_register), m_registerPosition( _registerPosition)
{
	if (_addressingType != InstructionDescriptor::RegDir && _addressingType != InstructionDescriptor::RegInd
		&& _addressingType != InstructionDescriptor::RegIndOffset && _addressingType != InstructionDescriptor::PCRel)
		throw std::string("Register instruction parameter can only be of addressing type RegDir or RegInd");

}

RegisterInstructionParameterWithOffset::RegisterInstructionParameterWithOffset(std::shared_ptr<RegisterDescriptor> _register, InstructionDescriptor::AddressyingType _addressingType, std::shared_ptr<AbstractExpression> _offset, InstructionDescriptor::RegisterPosition _registerPosition)
	: AbstractInstructionParameter(_addressingType), RegisterInstructionParameter(_register, _addressingType, _registerPosition), ExpressionParameter(_addressingType, _offset)
{
	if ( _addressingType != InstructionDescriptor::RegIndOffset && _addressingType != InstructionDescriptor::PCRel)
		throw std::string("Register instruction parameter with Offset can only be of addressing type RegIndOffset or PCRel");
}



ExpressionParameter::ExpressionParameter(InstructionDescriptor::AddressyingType _addressingType, std::shared_ptr<AbstractExpression> _expression)
	: AbstractInstructionParameter(_addressingType), m_expression(_expression)
{
	if (_addressingType != InstructionDescriptor::MemDir && _addressingType != InstructionDescriptor::Immediate
		&& _addressingType != InstructionDescriptor::RegIndOffset && _addressingType != InstructionDescriptor::PCRel)
		throw std::string("Register instruction parameter can only be of addressing type RegDir or RegInd");
}
