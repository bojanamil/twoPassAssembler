#include "ProcessorDescriptor.h"
#include <vector>
#include <string>
#include <set>
#include "InstructionDescriptor.h"
#include "RegisterDescriptor.h"

ProcessorDescriptor::ProcessorDescriptor()
{
	initializeInstructions();
	initializeRegisters();
}

void ProcessorDescriptor::initializeInstructions()
{
	std::vector < std::set< InstructionDescriptor::AddressyingType>> paramAddressing;
	std::set < InstructionDescriptor::AddressyingType> perParam;
	std::vector<InstructionDescriptor::RegisterPosition> registerPositions;

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	std::shared_ptr<InstructionDescriptor> instruction = std::make_shared<InstructionDescriptor>("int", InstructionDescriptor::intt, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();


	instruction = std::make_shared<InstructionDescriptor>("ret", InstructionDescriptor::rett, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);


	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jmp", InstructionDescriptor::jmpp, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("call", InstructionDescriptor::calll, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();


	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jz", InstructionDescriptor::jzz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jnz", InstructionDescriptor::jnzz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jgz", InstructionDescriptor::jgzz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
    instruction = std::make_shared<InstructionDescriptor>("jgez", InstructionDescriptor::jgezz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jlz", InstructionDescriptor::jlzz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel); //dodato
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("jlez", InstructionDescriptor::jlezz, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::Immediate);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("loadub", InstructionDescriptor::loadd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::Immediate);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("loadsb", InstructionDescriptor::loadd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::Immediate);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("loaduw", InstructionDescriptor::loadd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::Immediate);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("loadsw", InstructionDescriptor::loadd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::Immediate);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("load", InstructionDescriptor::loadd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("storeb", InstructionDescriptor::storee, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("storew", InstructionDescriptor::storee, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	perParam.insert(InstructionDescriptor::RegDir);
	perParam.insert(InstructionDescriptor::RegInd);
	perParam.insert(InstructionDescriptor::MemDir);
	perParam.insert(InstructionDescriptor::RegIndOffset);
	perParam.insert(InstructionDescriptor::PCRel);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("store", InstructionDescriptor::storee, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("push", InstructionDescriptor::pushh, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	instruction = std::make_shared<InstructionDescriptor>("pop", InstructionDescriptor::popp, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("add", InstructionDescriptor::addd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("sub", InstructionDescriptor::subb, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("mul", InstructionDescriptor::mull, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("div", InstructionDescriptor::divv, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("mod", InstructionDescriptor::modd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("and", InstructionDescriptor::andd, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("or", InstructionDescriptor::orr, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("xor", InstructionDescriptor::xorr, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	instruction = std::make_shared<InstructionDescriptor>("not", InstructionDescriptor::nott, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("asl", InstructionDescriptor::asll, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();

	perParam.insert(InstructionDescriptor::RegDir);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	paramAddressing.push_back(perParam);
	perParam.clear();
	registerPositions.push_back(InstructionDescriptor::zero);
	registerPositions.push_back(InstructionDescriptor::first);
	registerPositions.push_back(InstructionDescriptor::second);
	instruction = std::make_shared<InstructionDescriptor>("asr", InstructionDescriptor::asrr, paramAddressing, registerPositions);
	m_listOfInstructions.push_back(instruction);
	paramAddressing.clear();
	registerPositions.clear();
}


void ProcessorDescriptor::initializeRegisters()
{
	std::shared_ptr<RegisterDescriptor> r0 = std::make_shared<RegisterDescriptor>("r0", RegisterDescriptor::r0);
	m_listOfRegisters.push_back(r0);

	std::shared_ptr<RegisterDescriptor> r1 = std::make_shared<RegisterDescriptor>("r1", RegisterDescriptor::r1);
	m_listOfRegisters.push_back(r1);

	std::shared_ptr<RegisterDescriptor> r2 = std::make_shared<RegisterDescriptor>("r2", RegisterDescriptor::r2);
	m_listOfRegisters.push_back(r2);

	std::shared_ptr<RegisterDescriptor> r3 = std::make_shared<RegisterDescriptor>("r3", RegisterDescriptor::r3);
	m_listOfRegisters.push_back(r3);

	std::shared_ptr<RegisterDescriptor> r4 = std::make_shared<RegisterDescriptor>("r4", RegisterDescriptor::r4);
	m_listOfRegisters.push_back(r4);

	std::shared_ptr<RegisterDescriptor> r5 = std::make_shared<RegisterDescriptor>("r5", RegisterDescriptor::r5);
	m_listOfRegisters.push_back(r5);

	std::shared_ptr<RegisterDescriptor> r6 = std::make_shared<RegisterDescriptor>("r6", RegisterDescriptor::r6);
	m_listOfRegisters.push_back(r6);

	std::shared_ptr<RegisterDescriptor> r7 = std::make_shared<RegisterDescriptor>("r7", RegisterDescriptor::r7);
	m_listOfRegisters.push_back(r7);

	std::shared_ptr<RegisterDescriptor> r8 = std::make_shared<RegisterDescriptor>("r8", RegisterDescriptor::r8);
	m_listOfRegisters.push_back(r8);

	std::shared_ptr<RegisterDescriptor> r9 = std::make_shared<RegisterDescriptor>("r9", RegisterDescriptor::r9);
	m_listOfRegisters.push_back(r9);

	std::shared_ptr<RegisterDescriptor> r10 = std::make_shared<RegisterDescriptor>("r10", RegisterDescriptor::r10);
	m_listOfRegisters.push_back(r10);

	std::shared_ptr<RegisterDescriptor> r11 = std::make_shared<RegisterDescriptor>("r11", RegisterDescriptor::r11);
	m_listOfRegisters.push_back(r11);

	std::shared_ptr<RegisterDescriptor> r12 = std::make_shared<RegisterDescriptor>("r12", RegisterDescriptor::r12);
	m_listOfRegisters.push_back(r12);

	std::shared_ptr<RegisterDescriptor> r13 = std::make_shared<RegisterDescriptor>("r13", RegisterDescriptor::r13);
	m_listOfRegisters.push_back(r13);

	std::shared_ptr<RegisterDescriptor> r14 = std::make_shared<RegisterDescriptor>("r14", RegisterDescriptor::r14);
	m_listOfRegisters.push_back(r14);

	std::shared_ptr<RegisterDescriptor> r15 = std::make_shared<RegisterDescriptor>("r15", RegisterDescriptor::r15);
	m_listOfRegisters.push_back(r15);

	std::shared_ptr<RegisterDescriptor> sp = std::make_shared<RegisterDescriptor>("sp", RegisterDescriptor::sp);
	m_listOfRegisters.push_back(sp);

	std::shared_ptr<RegisterDescriptor> pc = std::make_shared<RegisterDescriptor>("pc", RegisterDescriptor::pc);
	m_listOfRegisters.push_back(pc);

}

bool ProcessorDescriptor::isInTheRegisterList(std::string param) const
{
	for (size_t i = 0; i < m_listOfRegisters.size(); i++) {
		if (m_listOfRegisters[i]->getNameOfRegister() == param) return true;
	}
	return false;
}

std::shared_ptr<RegisterDescriptor> ProcessorDescriptor::getRegister(std::string param) const
{
	bool registerFound = false;;
	std::shared_ptr<RegisterDescriptor> theRegister;
	for (size_t i = 0; i < m_listOfRegisters.size(); i++) {
		if (m_listOfRegisters[i]->getNameOfRegister() == param) {
			theRegister= m_listOfRegisters[i];
			registerFound = true;
			break;
		}
	}
	if (registerFound) return theRegister;
	else throw std::string("There's no register by that name!");

}
