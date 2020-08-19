#include "FirstPass.h"
#include <string>
#include "Utils.h"
#include <memory>
#include "AbstractSymbol.h"
#include "Symbol.h"
#include "Context.h"
#include <iostream>
#include "Segment.h"
#include "InstructionDescriptor.h"
#include "ProcessorDescriptor.h"
#include "RegisterDescriptor.h"
#include "AbstractInstructionParameter.h"
#include "AbstractSymbol.h"



FirstPass::FirstPass(Context &_context, std::istream &_inpputFile)
	: context(_context), inputFile(_inpputFile), m_orgHappened(false), m_orgValue(0), m_moreUnknownExpressionsLeft(true), m_newExpressionCalculated(true)
{
}


FirstPass::~FirstPass()
{
}

std::string FirstPass::removeComment(const std::string& input)
{
	size_t comentStart = input.find_first_of(';');
	if (comentStart == std::string::npos) return input;
	return input.substr(0, comentStart);
}


void FirstPass::handleLabel( std::string& line) const
{
	size_t endOfLabel = line.find_first_of(':');
	std::string name;
	while (endOfLabel != std::string::npos) {
		std::string name = trim(line.substr(0, endOfLabel));

		if (context.getSymbols().find(name) != context.getSymbols().end()) {
			std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[name]);
			if (symbol->isDefined() )
				throw std::string("Label can't be defined two times");
		}

		else {
			context.getSymbols().insert(make_pair(name, std::make_shared<Symbol>(name)));
			context.getSymbols()[name]->setIsLocal(true);
		}

		std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[name]);

		symbol->setValue(context.getCurrentAdres());
		symbol->setType(AbstractExpression::Relative);
		symbol->setSegment(context.getCurrentSegment());
		symbol->setIsDefined(true);


		line = trim(line.substr(endOfLabel + 1, line.length()));

		endOfLabel = line.find_first_of(':');
	}
	line = trim(line);
	
}

bool FirstPass::handleDEF(std::string& line)
{
	line = trim(line);
	if (line.find("DEF") == std::string::npos) return false;

	size_t startOfDEF = line.find("DEF");

	std::string beforeDEF = line.substr(0, startOfDEF);
	std::string symbolName = trim(beforeDEF);

	std::string afterDEF = line.substr(startOfDEF + 3, line.size() - startOfDEF - 3);
	 afterDEF = trim(afterDEF);
	std::shared_ptr<AbstractExpression> expression = AbstractExpression::parseExpretion(afterDEF, context);

	if (context.getSymbols().find(symbolName) != context.getSymbols().end()) {
		std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[symbolName]);
		if (symbol->isDefined() == true) throw std::string("Symbol can't be defined two times");
		symbol->setIsDefined(true);
		symbol->setIsLocal(false);
		symbol->setExpression(expression);
		symbol->setType(expression->getType());
		if (expression->getType() == AbstractExpression::Absolute) symbol->setValue(expression->getValue());
	}
	else {
		context.getSymbols().insert(make_pair(symbolName, std::make_shared<Symbol>(symbolName)));
		std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[symbolName]);
		symbol->setExpression(expression);
		symbol->setIsLocal(true);
		symbol->setType(expression->getType());
		symbol->setName(symbolName);
		symbol->setSegment(context.getCurrentSegment());
		symbol->setIsDefined(true);
		if (expression->getType() == ExpressionWithOperator::Absolute) symbol->setValue(expression->getValue());
	}

	return true;
}


bool FirstPass::handleGLOBAL(std::string& line) const
{
	line = trim(line);
	if (line.find(".global") == 0 || line.find(".GLOBAL") == 0) {
		std::string restOfLine = trim(line.substr(7, line.size() - 7));
		size_t placeOfComma = restOfLine.find_first_of(',');
		std::string contentBetweenCommas;

		while (placeOfComma != std::string::npos) {
			contentBetweenCommas = trim(restOfLine.substr(0, placeOfComma));
			handleContentBetweenCommasForGLOBAL(contentBetweenCommas);
			restOfLine = trim(restOfLine.substr(placeOfComma + 1, restOfLine.size()-placeOfComma-1));
			placeOfComma = restOfLine.find_first_of(',');
		}
		contentBetweenCommas = restOfLine;
		handleContentBetweenCommasForGLOBAL(contentBetweenCommas);
		return true;
	}
	return false;
}

bool FirstPass::handleORG(std::string& line) 
{	
	line = trim(line);
	if (line.find("org") == 0 || line.find("ORG") == 0) {
		std::string afterORG = trim(line.substr(3, line.size() - 3));
		std::shared_ptr<AbstractExpression> expression = AbstractExpression::parseExpretion(afterORG, context);
		if (expression->getType() != ExpressionWithOperator::Absolute) throw std::string("Expression must be absolute in ORG directive");
		int orgValue = expression->getValue();
		setORGValue(orgValue);
		setORGHappened(true);
		return true;
	}
	return false;
}

bool FirstPass::handleInstruction(std::string& line)
{
	line = trim(line);
	std::string stringLowerInstruction = toLowerFromUpper(line);
	for (size_t i = 0; i < context.GetProcessorDescriptor().getInstructions().size();i++) {
		const auto& instruction = context.GetProcessorDescriptor().getInstructions()[i];
		std::string instructionName = instruction->getName();

		if (stringLowerInstruction.find(instructionName)==0)
		{
			size_t instructionNameLength = instructionName.size();
			line = line.substr(instructionNameLength, line.size()- instructionNameLength);
			line = trim(line);

			size_t numberOfParameters = instruction->getNumberOfParameters();
			std::vector < std::set <InstructionDescriptor::AddressyingType>> allowedParameterAddressing = instruction->getAllowedParameterAddresing();
			std::vector < InstructionDescriptor::RegisterPosition> registerPositions = instruction->getRegisterPositions();

			std::vector<std::shared_ptr<AbstractInstructionParameter>> parameters;

			for(size_t j=0;j<numberOfParameters;j++) {

				auto commaPosition = line.find_first_of(',');
				std::string param = trim(line.substr(0, commaPosition));
				std::string stringLowerParam = toLowerFromUpper(param);
				line = line.substr(commaPosition + 1, line.size() - commaPosition -1);
				line = trim(line);

				std::set <InstructionDescriptor::AddressyingType> addressingTypes = allowedParameterAddressing[j];

				if (checkIfRegDir(addressingTypes, stringLowerParam) ==true) {
					std::shared_ptr<RegisterDescriptor> theRegister = context.GetProcessorDescriptor().getRegister(stringLowerParam);
					std::shared_ptr<RegisterInstructionParameter> par = std::make_shared<RegisterInstructionParameter>(theRegister, InstructionDescriptor::RegDir, registerPositions[j]);
					parameters.push_back(par);
					continue;
				}

				if (checkIfRegIndir(addressingTypes, stringLowerParam)==true) {
					stringLowerParam = stringLowerParam.substr(1, stringLowerParam.size() - 1);
					stringLowerParam = stringLowerParam.substr(0, stringLowerParam.size() - 1);
					stringLowerParam = trim(stringLowerParam);
					std::shared_ptr<RegisterDescriptor> theRegister = context.GetProcessorDescriptor().getRegister(stringLowerParam);
					std::shared_ptr<RegisterInstructionParameter> par = std::make_shared<RegisterInstructionParameter>(theRegister,InstructionDescriptor::RegInd, registerPositions[j]);
					parameters.push_back(par);
					continue;
				}

				if (checkIfRegIndOffset(addressingTypes, stringLowerParam) == true) {
					param = param.substr(1, param.size() - 1);
					param = param.substr(0, param.size() - 1);
					param = trim(param);
					size_t operatorPosition = param.find_first_of("+-"); //bilo je param.find('+') a to nije dobro
					std::string registerName = toLowerFromUpper(trim(param.substr(0, operatorPosition)));
					size_t registerLenght = registerName.size();
					std::shared_ptr<RegisterDescriptor> theRegister = context.GetProcessorDescriptor().getRegister(registerName);

					if (registerLenght == 3) {
						param[0] = '0';
						param[1] = '0';
						param[2] = '0';
					}
					else {
						param[0] = '0';
						param[1] = '0';
					}

					std::string offset = param;
					offset = trim(offset);
					std::shared_ptr<AbstractExpression> offsetExpression= AbstractExpression::parseExpretion(offset, context);

					std::shared_ptr<RegisterInstructionParameterWithOffset> par = std::make_shared<RegisterInstructionParameterWithOffset>(theRegister,InstructionDescriptor::RegIndOffset, offsetExpression, registerPositions[j]);
					parameters.push_back(par);
					continue;	
				}

				if (checkIfImmediate(addressingTypes, param) == true) {
					param = trim(param);
					param = param.substr(1, param.size()-1);
					param = trim(param);
					std::shared_ptr<AbstractExpression> paramExpression = AbstractExpression::parseExpretion(param, context);
					std::shared_ptr<ExpressionParameter> par = std::make_shared<ExpressionParameter>(InstructionDescriptor::Immediate, paramExpression);
					parameters.push_back(par);
					continue;
				}
				if (checkIfPCRel(addressingTypes, param) == true) {
					std::shared_ptr<RegisterDescriptor> theRegister = context.GetProcessorDescriptor().getRegister("pc");
					std::string offset = trim(param.substr(1, param.size() - 1));
					std::shared_ptr<AbstractExpression> offsetExpression = AbstractExpression::parseExpretion(offset, context);

					std::shared_ptr<ExpressionWithOperator> tryParse = std::dynamic_pointer_cast<ExpressionWithOperator>(offsetExpression);
					if (tryParse != nullptr) throw std::string("For PC Relative addressing, content after $ must be either a constant or a Symbol");

					std::shared_ptr<RegisterInstructionParameterWithOffset> par = std::make_shared<RegisterInstructionParameterWithOffset>(theRegister, InstructionDescriptor::PCRel, offsetExpression, registerPositions[j]);
					parameters.push_back(par);
					continue;
				}
				if (checkIfMemDir(addressingTypes, param) == true) {
					param = trim(param);
					std::shared_ptr<AbstractExpression> paramExpression = AbstractExpression::parseExpretion(param, context);
					std::shared_ptr<ExpressionParameter> par = std::make_shared<ExpressionParameter>(InstructionDescriptor::MemDir, paramExpression);
					parameters.push_back(par);
					continue;
				}
				throw std::string("Invalid type of addressing for parameter");
			}
			std::shared_ptr<InstructionRowDescriptor> instructionRowDescriptor = std::make_shared<InstructionRowDescriptor>(instruction, parameters);
			context.getCurrentSegment()->getDescriptors().push_back(instructionRowDescriptor);
			context.setCurrentAdres(context.getCurrentAdres() + instructionRowDescriptor->getRowSize());
			return true;
		}
	}
	return false;
}

bool FirstPass::handleData(std::string& line)
{
	if (checkIfHandlingData(line)) {
		int dataSize;
		std::vector<std::pair<unsigned, std::shared_ptr<AbstractExpression>>> pairs_NumEl_Expression;
		std::pair<unsigned, std::shared_ptr<AbstractExpression>> pairing;
		if (line.find("dd") == 0 || line.find("DD") == 0) dataSize = 4;
		else if (line.find("dw") == 0 || line.find("DW") == 0) dataSize = 2;
			else  dataSize = 1;

		line = line.substr(2,line.size()-2);
		line = trim(line);

		std::size_t commaPosition = line.find_first_of(',');
		std::string contentBetweenCommas;

		while (commaPosition != std::string::npos) {
			contentBetweenCommas = trim(line.substr(0, commaPosition));
			pairs_NumEl_Expression.push_back(handleContentBetweenCommasForData(contentBetweenCommas));

			line = line.substr(commaPosition + 1, line.size() - commaPosition - 1);
			line = trim(line);
			commaPosition = line.find_first_of(',');
		}
		contentBetweenCommas = line;

		pairs_NumEl_Expression.push_back(handleContentBetweenCommasForData(contentBetweenCommas));
		
		auto dataRowDescriptor = std::make_shared<DataRowDescriptor>(dataSize, pairs_NumEl_Expression);
		
		context.getCurrentSegment()->getDescriptors().push_back(dataRowDescriptor);
		context.setCurrentAdres(context.getCurrentAdres() + dataRowDescriptor->getRowSize());
		return true;
	}
	return false;
}

bool FirstPass::handleEND(std::string & line)
{
	if (line.find(".end") == 0 || line.find(".END")==0) {
		if (context.getCurrentSegment() != NULL)
		{
			context.getCurrentSegment()->setSize(context.getCurrentAdres() - context.getCurrentSegment()->getValue());
			return true;
		}
		else throw std::string("There was no segment before this .end (or .END)");
	}
	else return false;
}

void FirstPass::doPass()
{
	bool ENDhappened=false;
	std::string line;
	while (getline(inputFile, line))
	{
		line = removeComment(line);
		line = trim(line);
		try {
			if (orgHappened() == true && line == "") continue;
			if (orgHappened() == true && (!checkIfSectionStart(line))) throw std::string("Section must come next after ORG");
			handleLabel(line);
			if (handleSectionStart(line)) continue;
			if (handleORG(line)) continue;
			if (handleGLOBAL(line)) continue;
			if (handleDEF(line)) continue;
			if (handleData(line)) continue;
			if (handleInstruction(line)) continue;
			if (handleEND(line)) { ENDhappened = true; break; }
		}
		catch (const std::string & msg) {
			std::cout << msg;
		}
	}
	if (ENDhappened == false)
	try {
		throw std::string("Reached the end of the file, no .end or .END found");
	}
	catch (const std::string & msg) {
			std::cout << msg;
		}
}

bool FirstPass::handleSectionStart(std::string line) 
{
	if (checkIfSectionStart(line))
	{
		if (context.getSymbols().find(line) != context.getSymbols().end()) {
			 throw std::string( "Section can't be defined two times");
		}

		context.getSymbols().insert(make_pair(line, std::make_shared<Segment>(line)));
		std::shared_ptr<Segment> segment = std::static_pointer_cast<Segment>(context.getSymbols()[line]);

		if(context.getCurrentSegment() != NULL)
		{
			context.getCurrentSegment()->setSize(context.getCurrentAdres()-context.getCurrentSegment()->getValue());
			
		}
		context.setCurrentSegment(segment);

		context.getSegments().push_back(segment);

		context.resetCurrentAddress();
		if (orgHappened() == true) {
			context.setCurrentAdres(getORGValue());
			setORGHappened(false);
		}
		segment->setValue(context.getCurrentAdres());
		segment->setIsDefined(true);
		segment->setIsLocal(true);
		segment->setType(AbstractExpression::Relative);

		return true;
	}
	return false;
}



bool FirstPass::checkIfRegDir(std::set<InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{
	if (addressingTypes.find(InstructionDescriptor::RegDir) != addressingTypes.end() && context.GetProcessorDescriptor().isInTheRegisterList(param) == true) return true;
	return false;
}

bool FirstPass::checkIfRegIndir(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{	
	param = trim(param);
	if (addressingTypes.find(InstructionDescriptor::RegInd) != addressingTypes.end() && param.find('[') == 0 && param.find(']') == (param.size() - 1)) {
		param = param.substr(1, param.size()-1);
		param = param.substr(0, param.size()-1);
		if (context.GetProcessorDescriptor().isInTheRegisterList(param) == true) return true;
	}
	return false;
}

bool FirstPass::checkIfRegIndOffset(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{
	if (addressingTypes.find(InstructionDescriptor::RegIndOffset) != addressingTypes.end() && param.find('[') == 0 && param.find(']') == param.size() - 1) {
		param = param.substr(1, param.size() - 1);
		param = param.substr(0, param.size() - 1);
		param = trim(param);
		for (size_t i = 0; i < context.GetProcessorDescriptor().getRegisters().size(); i++) {
			std::string parameterName = context.GetProcessorDescriptor().getRegisters()[i]->getNameOfRegister();
			if (param.find(parameterName) == 0) return true;
		}
	}
	return false;
}

bool FirstPass::checkIfImmediate(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{
	param = trim(param);
	if (addressingTypes.find(InstructionDescriptor::Immediate) != addressingTypes.end() && param.find('#') == 0) {
		return true;
	}
	return false;
}

bool FirstPass::checkIfMemDir(std::set<InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{
	return addressingTypes.find(InstructionDescriptor::MemDir) != addressingTypes.end();
}

bool FirstPass::checkIfPCRel(std::set<InstructionDescriptor::AddressyingType> addressingTypes, std::string param)
{
	param = trim(param);
	if (addressingTypes.find(InstructionDescriptor::PCRel) != addressingTypes.end() && param.find('$') == 0) return true;
	return false;
}

void FirstPass::calculateSymbolValues()
{
	while (m_moreUnknownExpressionsLeft == true || m_newExpressionCalculated == true) {
		m_moreUnknownExpressionsLeft = false;
		m_newExpressionCalculated = false;

		std::map<std::string, std::shared_ptr<AbstractSymbol>>::iterator it;

		for (it = context.getSymbols().begin(); it != context.getSymbols().end(); it++) {
			std::shared_ptr<AbstractSymbol> symbol = it->second;
			std::shared_ptr<Symbol> s = std::dynamic_pointer_cast<Symbol>(symbol);
			if (s==nullptr) continue;
			if (s->getType() == AbstractExpression::Unknown) {
				std::shared_ptr<AbstractExpression> expression=s->getExpression();
				s->setType(expression->getType());
				if (s->getType() != AbstractExpression::Unknown) {
					s->setValue(expression->getValue());

					std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols=s->getDependableSymbols();
					if(dependableSymbols.size()>1) throw std::string("Symbol can be dependable on only one other symbol");
					if(dependableSymbols.size()==1) s->setSegment(dependableSymbols[0]->getSegment());

					m_newExpressionCalculated = true;
				}
				else {
					m_moreUnknownExpressionsLeft = true;
				}
			}

		}
		
	}
}

std::pair<unsigned, std::shared_ptr<AbstractExpression>> FirstPass::handleContentBetweenCommasForData(std::string content) const
{
	std::pair<unsigned, std::shared_ptr<AbstractExpression>> pairing;

	std::size_t placeOfDup = content.find("DUP");
	if (placeOfDup != std::string::npos) {
		std::string beforeDup = trim(content.substr(0, placeOfDup));
		std::string afterDup = trim(content.substr(placeOfDup + 3, content.size() - placeOfDup - 3));

		std::shared_ptr<AbstractExpression> parsedExpressionBeforeDup = AbstractExpression::parseExpretion(beforeDup, context);
		if (parsedExpressionBeforeDup->getType() != AbstractExpression::Absolute) throw std::string("Handling dd, dw, db: expression before dup must be Absolute");

		if (trim(afterDup) == "?")
		{
			pairing = std::make_pair(parsedExpressionBeforeDup->getValue(), nullptr);
		}
		else
		{
			std::shared_ptr<AbstractExpression> parsedExpressionAfterDup = AbstractExpression::parseExpretion(afterDup, context);
			pairing = std::make_pair(parsedExpressionBeforeDup->getValue(), parsedExpressionAfterDup);
		}
	}
	else {

		if(trim(content) == "?")
		{
			pairing = std::make_pair(1, nullptr);
		}
		else
		{
			std::shared_ptr<AbstractExpression> parsedExpression = AbstractExpression::parseExpretion(content, context);
			pairing = std::make_pair(1, parsedExpression);	
		}

	}

	return pairing;
}

void FirstPass::handleContentBetweenCommasForGLOBAL(std::string &symbolName) const
{
	if (context.getSymbols().find(symbolName) != context.getSymbols().end()) {
		std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[symbolName]);
		symbol->setIsLocal(false);
	}
	else {
		context.getSymbols().insert(make_pair(symbolName, std::make_shared<Symbol>(symbolName)));
		std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(context.getSymbols()[symbolName]);
		symbol->setIsLocal(false);
		symbol->setIsDefined(false);
		symbol->setType(ExpressionWithOperator::Relative);
		symbol->setName(symbolName);
		symbol->setValue(0);
	}
}

bool FirstPass::checkIfSectionStart(std::string & line)
{
	if (line.find(".data") == 0 || line.find(".rodata") == 0 || line.find(".bss") == 0 || line.find(".text") == 0
		|| line.find(".DATA") == 0 || line.find(".RODATA") == 0 || line.find(".BSS") == 0 || line.find(".TEXT") == 0) return true;
	return false;
}

bool FirstPass::checkIfHandlingData(std::string & line)
{
	if (line.find("dd") == 0 || line.find("db") == 0 || line.find("dw") == 0 || line.find("DD") == 0 || line.find("DB") == 0 || line.find("DW") == 0) return true;
	return false;
}


