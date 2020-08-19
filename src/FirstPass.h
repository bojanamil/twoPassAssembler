#ifndef _FIRST_PASS_H_
#define _FIRST_PASS_H_

#include <istream>
#include <set>
#include "InstructionDescriptor.h"
class Context;
class AbstractExpression;

class FirstPass
{

	Context &context;
	std::istream &inputFile;

	bool m_orgHappened;
	int m_orgValue;

	bool m_newExpressionCalculated;
	bool m_moreUnknownExpressionsLeft;
	
public:
	FirstPass(Context &_context, std::istream &_inpputFile);
	~FirstPass();

	void setORGHappened(bool _orgHappened) { m_orgHappened = _orgHappened; };
	void setORGValue(int _orgValue) { m_orgValue = _orgValue; };

	bool orgHappened() const { return m_orgHappened; };
	int getORGValue() const { return m_orgValue; };

	static std::string removeComment(const std::string& input);

	void handleLabel(std::string& line) const;
	bool handleDEF(std::string& line);
	bool handleGLOBAL(std::string& line) const;
	bool handleORG(std::string& line);
	bool handleInstruction(std::string& line);
	bool handleData(std::string& line);
	bool handleEND(std::string& line);	
	bool handleSectionStart(std::string line);

	void doPass();

	bool checkIfRegDir(std::set<InstructionDescriptor::AddressyingType> addressingTypes, std::string param);
	bool checkIfRegIndir(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param);
	bool checkIfRegIndOffset(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param);
	bool checkIfImmediate(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param);
	bool checkIfMemDir(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param);
	bool checkIfPCRel(std::set <InstructionDescriptor::AddressyingType> addressingTypes, std::string param);

	void calculateSymbolValues();

	std::pair<unsigned, std::shared_ptr<AbstractExpression>> handleContentBetweenCommasForData(std::string content) const;
	void handleContentBetweenCommasForGLOBAL(std::string &symbolName) const;

	bool checkIfSectionStart(std::string &line);
	bool checkIfHandlingData(std::string &line);

};

#endif