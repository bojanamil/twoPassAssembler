#include "AbstractRowDescriptor.h"
#include "AbstractInstructionParameter.h"
#include "AbstractExpression.h"
#include "AbstractSymbol.h"
#include "Segment.h"
#include "Utils.h"
#include "RegisterDescriptor.h"
#include <iomanip>
#include <iostream>
#include <sstream>


unsigned InstructionRowDescriptor::getRowSize() const
{
	if( m_parameters.size() > 0 )
	{
		return m_parameters[m_parameters.size() - 1]->getAddressyingType() == InstructionDescriptor::RegDir
			|| m_parameters[m_parameters.size() - 1]->getAddressyingType() == InstructionDescriptor::RegInd ? 4 : 8;
	}
	return 4;
		
}

void InstructionRowDescriptor::outputReallocations(int _currentAddress, std::shared_ptr<Segment> _currentSegment, std::ostream& outputStream) const
{
	int currentAddress = _currentAddress;
	std::string reallocations="";
	std::string flag = " A ";

	std::shared_ptr<AbstractInstructionParameter> lastParameter = m_parameters[m_parameters.size()-1];
	std::shared_ptr<ExpressionParameter> expressionParameter = std::dynamic_pointer_cast<ExpressionParameter>(lastParameter);

	if (expressionParameter != nullptr) {
		std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols = expressionParameter->getExpression()->getDependableSymbols();
		if (expressionParameter->getAddressyingType() == InstructionDescriptor::PCRel) {

			if (dependableSymbols.size() > 1) throw std::string("PC Relative addressing, expression can only depend on one symbol");
			if (dependableSymbols.size() == 1 && _currentSegment==dependableSymbols[0]->getSegment()) outputStream << "";
			flag = " R ";
		}

		currentAddress = currentAddress + 4;
		for (size_t i = 0; i < dependableSymbols.size(); i++) {
			int numberFromSymbolTable;
			if (dependableSymbols[i]->isLocal()==false) numberFromSymbolTable = dependableSymbols[i]->getNumberInSymbolTable();
			else numberFromSymbolTable = dependableSymbols[i]->getSegment()->getNumberInSymbolTable();

			outputStream <<"0x"<< std::hex << std::uppercase << currentAddress;
			outputStream << flag <<"0x"<< std::hex << std::uppercase << numberFromSymbolTable << " \n";
		}
		
	}

}

std::vector<std::string> InstructionRowDescriptor::makeContent(std::shared_ptr<Segment> _currentSegment, int _currentAddress) const
{
	std::vector<std::string> content;
	std::string instructionName = getInstruction()->getName();
	unsigned inst = 0;

	unsigned opcode = getInstruction()->getOpcode();
	inst |= (opcode<<24);
	InstructionDescriptor::AddressyingType addrMode = getAddressingMode();
	addrMode = (addrMode == InstructionDescriptor::AddressyingType::PCRel ? InstructionDescriptor::AddressyingType::RegIndOffset : addrMode);
	inst |= (addrMode << 21);

	for (size_t i = 0; i < m_parameters.size(); i++) {
		unsigned instructionType = static_cast<unsigned>(m_parameters[i]->getAddressyingType());
		std::shared_ptr<RegisterInstructionParameter> registerParam = std::dynamic_pointer_cast<RegisterInstructionParameter>(m_parameters[i]);
		if (registerParam != nullptr) {

			RegisterDescriptor::OPCODE registerOpcode = registerParam->getRegisterDescriptor()->getOpcode();
			auto registerPlacedOpcode = registerOpcode << registerParam->getRegisterPosition();
			inst |=  registerPlacedOpcode;
		}
	}

	inst |= (getOperandType() << 3);

	std::vector<std::string> firstDoubleWord = outputBytes(inst, true, 4);

	content.insert(content.end(), firstDoubleWord.begin(), firstDoubleWord.end());

	std::shared_ptr<AbstractInstructionParameter> lastParameter = m_parameters[m_parameters.size() - 1];
	std::shared_ptr<ExpressionParameter> expressionParameter = std::dynamic_pointer_cast<ExpressionParameter>(lastParameter);

	if (expressionParameter != nullptr) {
		std::vector<std::string> secondDoubleWord;
		int valueToOutput;
		std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols = expressionParameter->getExpression()->getDependableSymbols();
		int expressionValue = expressionParameter->getExpression()->getValue();

		if (expressionParameter->getAddressyingType() == InstructionDescriptor::PCRel) {
			if (dependableSymbols.size()> 0 && _currentSegment != dependableSymbols[0]->getSegment()) {
				int valueToSubstract;
				if (dependableSymbols[0]->isLocal()==false) valueToSubstract = dependableSymbols[0]->getValue();
				else valueToSubstract = dependableSymbols[0]->getSegment()->getValue();
				valueToOutput = expressionValue - valueToSubstract - 4;
			}
			else {
				valueToOutput = expressionValue - _currentAddress - 4;
			}
			secondDoubleWord = outputBytes(valueToOutput, false, 4);
			
		}
		else {
			int expressionValue = expressionParameter->getExpression()->getValue();
			for (size_t i = 0; i < dependableSymbols.size(); i++) {
				int valueToSubstract;
				if (dependableSymbols[i]->isLocal()) valueToSubstract = dependableSymbols[i]->getSegment()->getValue();
				else valueToSubstract = dependableSymbols[i]->getValue();
				expressionValue -= valueToSubstract;
			}
			secondDoubleWord = outputBytes(expressionValue, false, 4);
		}
		content.insert(content.end(), secondDoubleWord.begin(), secondDoubleWord.end());
	}
	return content;
}

unsigned InstructionRowDescriptor::getOperandType() const
{
	std::string instructionName = getInstruction()->getName();
	unsigned type = 0;
	if (instructionName.find("load") == 0 || instructionName.find("store") == 0) {
		if (instructionName.find("load") == 0) {
			if (instructionName == "load") type = 0b000;
			else {
				std::string suffix = instructionName.substr(4, instructionName.size() - 4);
				if (suffix == "uw") type = 0b001;
				else if (suffix == "sw") type = 0b101;
				else if (suffix == "ub") type = 0b011;
				else type = 0b111;
			}
		}
		else {
			if (instructionName == "store") type = 0b000;
			else {
				std::string suffix = instructionName.substr(5, instructionName.size() - 5);
				if (suffix == "b") type = 0b011; 
				else  type = 0b001;
			}
		}
	}
	
	return type;
}

InstructionDescriptor::AddressyingType InstructionRowDescriptor::getAddressingMode() const
{
	InstructionDescriptor::AddressyingType mode;
	std::string instructionName = getInstruction()->getName();
	if (instructionName == "push" || instructionName == "pop" || instructionName == "add" || instructionName == "sub" 
		|| instructionName == "mul" || instructionName == "div" || instructionName == "mod" || instructionName == "and" 
		|| instructionName == "or" || instructionName == "xor" || instructionName == "not" || instructionName == "asl" 
		|| instructionName == "asr") mode = InstructionDescriptor::AddressyingType::RegDir;
	else {
		std::shared_ptr<AbstractInstructionParameter> lastParameter = getParameters()[getParameters().size()-1];
		mode = lastParameter->getAddressyingType();
	}
	return mode;
}


unsigned DataRowDescriptor::getRowSize() const
{
	unsigned result = 0;
	for(auto& data : m_data)
	{
		result += data.first;
	}
	return result * m_sizeOfData;
}

void DataRowDescriptor::outputReallocations(int _currentAddress, std::ostream& outputStream) const
{
	if (m_sizeOfData < 4) {
		for (auto it = m_data.begin(); it < m_data.end(); it++) {
			if (it->second == nullptr)
			{
				continue;
			}
			if ( it->second->getDependableSymbols().size() != 0) throw std::string("Expression can't have dependable symbols, size of data is less than 4");
		}
	}

	for (auto it = m_data.begin(); it < m_data.end(); it++) {
		if (it->second == nullptr)
		{
			continue;
		}
		auto numTimes = it->first;
		std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols = it->second->getDependableSymbols();
		for (auto i = 0; i < numTimes; i++) {
			for (size_t j = 0; j < dependableSymbols.size(); j++) {
				std::string flag = " A ";
				int placeOfSymbol;
				if (dependableSymbols[j]->isLocal() == false) placeOfSymbol = dependableSymbols[j]->getNumberInSymbolTable();
				else placeOfSymbol = dependableSymbols[j]->getSegment()->getNumberInSymbolTable();
				outputStream << "0x" << std::hex << std::uppercase << _currentAddress;
				outputStream << flag;
				outputStream << "0x" << std::hex << std::uppercase << placeOfSymbol << "\n";
			}
			_currentAddress += 4;
		}
	}

}

std::vector<std::string> DataRowDescriptor::makeContent() const
{
	std::vector<std::string> content;

	for (auto it = m_data.begin(); it < m_data.end(); it++) {
		int numTimes = it->first;
		std::shared_ptr<const AbstractExpression> expression = it->second;
		if(expression==nullptr) {
			for (int i = 0; i < numTimes; i++) {
				std::vector<std::string> hexOutputs = outputBytes(0, false, m_sizeOfData);
				content.insert(content.end(), hexOutputs.begin(), hexOutputs.end());
			}
		}
		else {
			int expressionValue = expression->getValue();
			std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols = it->second->getDependableSymbols();
			for (size_t j = 0; j < dependableSymbols.size(); j++) {
				int valueToSubstract;
				if (dependableSymbols[j]->isLocal()) valueToSubstract = dependableSymbols[j]->getSegment()->getValue();
				else valueToSubstract = dependableSymbols[j]->getValue();
				expressionValue -= valueToSubstract;
			}
			for (int i = 0; i < numTimes; i++) {
				std::vector<std::string> hexOutputs = outputBytes(expressionValue, false, m_sizeOfData);
				content.insert(content.end(), hexOutputs.begin(), hexOutputs.end());
			}
		}
	}

	return content;
}
