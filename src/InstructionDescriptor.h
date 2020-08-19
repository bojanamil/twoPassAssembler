#ifndef _INSTRUCTION_DESCRIPTOR_H_
#define _INSTRUCTION_DESCRIPTOR_H_

#include <vector>
#include <memory>
#include <set>


class InstructionDescriptor {
public:
	enum OPCOde { intt = 0x00, rett, jmpp, calll, jzz, jnzz, jgzz, jgezz, jlzz, jlezz, loadd = 0x10, storee, pushh = 0x20, popp = 0x21, addd = 0x30, subb, mull, divv, modd, andd, orr , xorr, nott, asll, asrr };
	enum AddressyingType { RegDir = 0b000, RegInd = 0b010, Immediate = 0b100, MemDir = 0b110, RegIndOffset = 0b111, PCRel};
	enum RegisterPosition { zero = 16, first = 11, second = 6 };
private:
	std::string m_instructionName;
	OPCOde m_opcode;
	std::vector < std::set < AddressyingType>> m_allowedParameterAddresing;
	std::vector < RegisterPosition> m_registerPositions;
public:

	InstructionDescriptor(std::string _instructionName, OPCOde _OPCODE, std::vector < std::set < AddressyingType>> _allowedParameterAddresing, const std::vector < RegisterPosition>& _registerPositions);
	
	std::string getName() const { return m_instructionName; };
	OPCOde getOpcode() const { return m_opcode; };
	std::vector < std::set <AddressyingType>> getAllowedParameterAddresing() const { return m_allowedParameterAddresing; };
	size_t getNumberOfParameters() const { return m_allowedParameterAddresing.size(); };

	const std::vector < RegisterPosition>& getRegisterPositions() const { return m_registerPositions; };
	

};

#endif