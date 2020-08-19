#include "InstructionDescriptor.h"


InstructionDescriptor::InstructionDescriptor(std::string _instructionName, OPCOde _opcode, std::vector<std::set<AddressyingType>> _allowedParameterAddresing, const std::vector < RegisterPosition>& _registerPositions)
	: m_instructionName(_instructionName), m_opcode(_opcode), m_allowedParameterAddresing(_allowedParameterAddresing), m_registerPositions(_registerPositions)
{
}
