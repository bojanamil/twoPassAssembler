#include "SecondPass.h"
#include  "AbstractSymbol.h"
#include "Symbol.h"
#include "Segment.h"
#include <memory>
#include <map>
#include <ostream>
#include "AbstractRowDescriptor.h"
#include <iomanip>


SecondPass::SecondPass(Context & _context, std::ostream &_outputFile): context(_context), outputFile(_outputFile)
{	
}

void SecondPass::setNumbersInSymbolTable()
{
	std::map<std::string, std::shared_ptr<AbstractSymbol>>::iterator it;
	int numberInSymbolTable = 0;

	for (it = context.getSymbols().begin(); it != context.getSymbols().end(); it++) {
		std::shared_ptr<AbstractSymbol> abstractSymbol = it->second;
		abstractSymbol->setNumberInSymbolTable(numberInSymbolTable);
		numberInSymbolTable++;
	}
}

void SecondPass::outputSymbolTable()
{
	outputFile<<"#SYMBOL TABLE\n";

	std::map<std::string, std::shared_ptr<AbstractSymbol>>::iterator it;

	for (it = context.getSymbols().begin(); it != context.getSymbols().end(); it++) {
		std::shared_ptr<AbstractSymbol> abstractSymbol = it->second;
		std::shared_ptr<Symbol> s = std::dynamic_pointer_cast<Symbol>(abstractSymbol);
		if (s != nullptr) {
			int numberOfSegment;
			std::string symbolScope;

			if (s->getType() == AbstractExpression::Absolute) numberOfSegment = -1;
			else if (s->isLocal() == false && s->isDefined()==false) numberOfSegment = 0;
			else numberOfSegment = s->getSegment()->getNumberInSymbolTable();

			if (s->isLocal()) symbolScope = "L ";
			else symbolScope = "G ";

			outputFile << "SYM " << std::dec << abstractSymbol->getNumberInSymbolTable() << " ";
			outputFile << abstractSymbol->getName() << " ";
			outputFile << std::dec <<numberOfSegment << " " << "0x" << std::hex << s->getValue() << " " << symbolScope << "\n";
		}
		else {
			std::shared_ptr<Segment> seg = std::dynamic_pointer_cast<Segment>(abstractSymbol);
			std::string rwx;
			std::string segmentName = it->first;

			if (segmentName.find(".data") == 0 || segmentName.find(".DATA") == 0) rwx = "RW";
			else if (segmentName.find(".rodata") == 0 || segmentName.find(".RODATA") == 0) rwx = "R";
			else if (segmentName.find(".bss") == 0 || segmentName.find(".BSS") == 0) rwx = "RW";
			else rwx = "RE";

			outputFile << "SEG " << std::dec << abstractSymbol->getNumberInSymbolTable() << " ";
			outputFile << abstractSymbol->getName() << " ";
			outputFile << std::dec << abstractSymbol->getNumberInSymbolTable() << " "; 
			outputFile << "0x" << std::hex << seg->getValue() << " ";
			outputFile << "0x" << std::hex<< seg->getSize() << " " << rwx << "\n";

		}
	}

}

void SecondPass::outputReallocations(std::shared_ptr<Segment> segment)
{
	int currentAddress = segment->getValue();

		for (size_t j = 0; j < segment->getDescriptors().size(); j++) {
			std::shared_ptr<AbstractRowDescriptor> currentRow = segment->getDescriptors()[j];

			std::shared_ptr<InstructionRowDescriptor> instructionRow = std::dynamic_pointer_cast<InstructionRowDescriptor>(currentRow);
			if (instructionRow != nullptr) {
				instructionRow->outputReallocations(currentAddress, segment, outputFile);
			}
			else {
				std::shared_ptr<DataRowDescriptor> dataRow = std::dynamic_pointer_cast<DataRowDescriptor>(currentRow);
				dataRow->outputReallocations(currentAddress, outputFile);
			}
			currentAddress = currentAddress + currentRow->getRowSize();
		}
}

void SecondPass::outputSegmentContent(std::shared_ptr<Segment> currentSegment)
{
	std::vector<std::string> content;
	int currentAddress = currentSegment->getValue();
	for (size_t j = 0; j < currentSegment->getDescriptors().size(); j++) {

		std::shared_ptr<AbstractRowDescriptor> currentRow = currentSegment->getDescriptors()[j];

		std::shared_ptr<InstructionRowDescriptor> instructionRow = std::dynamic_pointer_cast<InstructionRowDescriptor>(currentRow);
		if (instructionRow != nullptr) {
				
			std::vector<std::string> newContent =  instructionRow->makeContent(currentSegment, currentAddress);
			content.insert(content.end(), newContent.begin(), newContent.end());
		}
		else {
			std::shared_ptr<DataRowDescriptor> dataRow = std::dynamic_pointer_cast<DataRowDescriptor>(currentRow);
			std::vector<std::string> newContent = dataRow->makeContent();
			content.insert(content.end(), newContent.begin(), newContent.end());
		}
		currentAddress = currentAddress + currentRow->getRowSize();
	}
	size_t numLoop = content.size() / 16;
	int restOfLoop= content.size() % 16;
	int i = 0, j = 0;
	while (i != numLoop) {
		j = i * 16;
		while (j != 16*(i+1)) {
			outputFile << content[j] << " ";
			j++;
		}
		outputFile << "\n";
		i++;
	}
	for (int i = 0; i < restOfLoop; i++) outputFile << content[j + i] << " ";

	
}

void SecondPass::doPass()
{
	try {
		setNumbersInSymbolTable();
		outputSymbolTable();

		for (size_t i = 0; i < context.getSegments().size(); i++) {
			int currentAddress = 0;

			std::shared_ptr<Segment> currentSegment = context.getSegments()[i];
			currentAddress += currentSegment->getValue();

			outputFile << "#rel" << currentSegment->getName() << "\n";
			outputReallocations(currentSegment);
			outputFile << currentSegment->getName() << "\n";
			outputSegmentContent(currentSegment);
			outputFile << "\n";
		}
		outputFile << "#END";
	}
	catch (const std::string & msg) {
		std::cout << msg;
	}
}





