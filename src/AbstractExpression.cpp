#include "AbstractExpression.h"
#include "Context.h"
#include "AbstractSymbol.h"
#include <stack>
#include "Utils.h"
#include "Segment.h"
#include "Symbol.h"

const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::ADD = std::make_shared<Operator>("+");
const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::SUB = std::make_shared<Operator>("-");
const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::MUL = std::make_shared<Operator>("*");
const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::DIV = std::make_shared<Operator>("/");
const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::OPEN_BRACKETS = std::make_shared<Operator>("(");
const std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::CLOSE_BRACKETS = std::make_shared<Operator>(")");


AbstractExpression::AbstractExpression()
	: m_isInBrackets(false)
{
}


AbstractExpression::~AbstractExpression()
{
}

void ExpressionWithOperator::outputOperator(std::stack<std::shared_ptr<AbstractExpression>>& _operandStack, std::stack<std::shared_ptr<Operator>>& _operatorStack, int& _rank)
{
	std::shared_ptr<Operator> oldOperator = _operatorStack.top();
	_operatorStack.pop();
	_rank = _rank + oldOperator->getRankChange();
	if (_rank < 1) throw std::string("Invalid expression");
	std::shared_ptr<AbstractExpression> rightOperand = _operandStack.top();
	_operandStack.pop();
	std::shared_ptr<AbstractExpression> leftOperand = _operandStack.top();
	_operandStack.pop();
	_operandStack.push(std::make_shared<ExpressionWithOperator>(leftOperand, rightOperand, oldOperator));
}


std::shared_ptr<AbstractExpression> AbstractExpression::parseExpretion(const std::string& _expression, Context& _context)
{
	std::stack<std::shared_ptr<AbstractExpression>> operandStack;
	std::stack<std::shared_ptr<ExpressionWithOperator::Operator>> operatorStack;
	int rank = 0;
	std::string restOfExpression = _expression;

	while(!restOfExpression.empty())
	{
		auto operatorPosition= restOfExpression.find_first_of("+-()*/");
		std::string operandString = restOfExpression.substr(0, operatorPosition);
		std::string operatorString;
		if(operatorPosition != std::string::npos)
		{
			operatorString = restOfExpression.substr(operatorPosition, 1);
			restOfExpression = trim(restOfExpression.substr(operatorPosition + 1, restOfExpression.size()));
		}
		else
		{
			restOfExpression = "";
		}
		std::shared_ptr<AbstractExpression> operand = parseOperand(trim(operandString), _context);
		if(operand!=nullptr)
		{
			operandStack.push(operand);
			rank++;
		}
		
		std::shared_ptr<ExpressionWithOperator::Operator> oper = ExpressionWithOperator::Operator::getOperatorFromString(operatorString);
		if(oper!=nullptr)
		{
			while(!operatorStack.empty() && oper->getInfixPriority()<=operatorStack.top()->getStackPriority())
			{
				ExpressionWithOperator::outputOperator(operandStack, operatorStack, rank);
			}
			if (oper != ExpressionWithOperator::Operator::CLOSE_BRACKETS)
			{
				operatorStack.push(oper);
			}
			else
			{
				operatorStack.pop();
				operandStack.top()->setIsInBrackets(true);
			}
		}
		
	}
	while(!operatorStack.empty())
	{
		ExpressionWithOperator::outputOperator(operandStack,operatorStack,rank);

	}
	if (rank != 1) throw std::string("Invalid expression");
	return operandStack.top();
}

std::shared_ptr<AbstractExpression> AbstractExpression::parseOperand(const std::string & _operandString, Context& _context)
{
	if (trim(_operandString).empty())
	{
		return std::shared_ptr<AbstractExpression>();
	}
	std::shared_ptr<SimpleExpression> result = SimpleExpression::tryParse(trim(_operandString));
	if (nullptr != result) return result;

	return SymbolExpression::tryParse(trim(_operandString), _context);
}


std::shared_ptr<SimpleExpression> SimpleExpression::tryParseWithBase(const std::string& _expresion, const unsigned _base, const std::string& _prefix,const std::string& _posibleValues)
{
	if (_expresion.size() >= _prefix.size() + 1
		&& _expresion.find(_prefix) == 0
		&& _expresion.substr(_prefix.size(), _expresion.size()).find_first_not_of(_posibleValues) == std::string::npos)
	{
		long n = strtol(_expresion.substr(_prefix.size(), _expresion.size()).c_str(), nullptr, _base);
		
		return std::make_shared<SimpleExpression>(n);
	}
	return std::shared_ptr<SimpleExpression>();
}


std::shared_ptr<SimpleExpression> SimpleExpression::tryParse(const std::string& _expresion)
{
	std::shared_ptr<SimpleExpression> result = tryParseWithBase(_expresion, 2, "0b", "01");
	if (nullptr != result) return result;
	result = tryParseWithBase(_expresion, 8, "0", "01234567");
	if (nullptr != result) return result;
	result = tryParseWithBase(_expresion, 16, "0x", "0123456789ABCDEF");
	if (nullptr != result) return result;
	return tryParseWithBase(_expresion, 10, "", "0123456789");

}

int SymbolExpression::getValue() const
{
	if (m_context[m_name] == nullptr) return 0;
	return m_context[m_name]->getValue();
}

AbstractExpression::Type SymbolExpression::getType() const
{
	if (m_context[m_name] == nullptr) return Unknown;
	return m_context[m_name]->getType();
}

bool SymbolExpression::isFromSameSegment(const std::shared_ptr<SymbolExpression>& right) const
{
	return m_context[m_name] != nullptr && m_context[right->m_name] != nullptr
		&& m_context[m_name]->getSegment() == m_context[right->m_name]->getSegment();
}

std::shared_ptr<SymbolExpression> SymbolExpression::tryParse(const std::string& _expresion, Context& _context)
{
	return std::make_shared<SymbolExpression>(_expresion, _context);
}

std::vector<std::shared_ptr<const AbstractSymbol>> SymbolExpression::getDependableSymbols() const
{
	return m_context.getSymbols()[m_name]->getDependableSymbols();
}


int ExpressionWithOperator::Operator::getInfixPriority()
{
	if (ADD.get() == this || SUB.get() == this) return 2;
	if (MUL.get() == this || DIV.get() == this) return 3;
	if(OPEN_BRACKETS.get() == this) return 6;
	return 1;
}

int ExpressionWithOperator::Operator::getStackPriority()
{
	if (ADD.get() == this || SUB.get() == this) return 2;
	if (MUL.get() == this || DIV.get() == this) return 3;
	if (OPEN_BRACKETS.get() == this) return 0;
	return -1;
}

int ExpressionWithOperator::Operator::getRankChange()
{
	if (ADD.get() == this || SUB.get() == this || MUL.get() == this || DIV.get() == this) return -1;
	return 1;
}

std::shared_ptr<ExpressionWithOperator::Operator> ExpressionWithOperator::Operator::getOperatorFromString(const std::string& operatorName)
{
	if ("+" == operatorName)
		return ADD;
	if ("-" == operatorName)
		return SUB;
	if ("*" == operatorName)
		return MUL;
	if ("/" == operatorName)
		return DIV;
	if ("(" == operatorName)
		return OPEN_BRACKETS;
	if (")" == operatorName)
		return CLOSE_BRACKETS;
	return nullptr;
}

int ExpressionWithOperator::Operator::calculate(std::shared_ptr<AbstractExpression> _leftExpression, std::shared_ptr<AbstractExpression> _rightExpession)
{
	if (_leftExpression != nullptr && _rightExpession != nullptr)
	{
		if (ADD.get() == this)
		 	return  _leftExpression->getValue() + _rightExpession->getValue();
		if (SUB.get() == this)
			return  _leftExpression->getValue() - _rightExpession->getValue();
		if (MUL.get() == this)
			return  _leftExpression->getValue() * _rightExpession->getValue();
		if (DIV.get() == this)
			return  _leftExpression->getValue() / _rightExpession->getValue();
	}
	
	throw std::string("Invalid expression to calculate");
}



AbstractExpression::Type ExpressionWithOperator::getType() const
{
	if (m_leftExpression->getType() == Unknown || m_rightExpression->getType() == Unknown) return Unknown;
	if (m_leftExpression->getType() == Absolute && m_rightExpression->getType() == Absolute) return Absolute;

	if (m_operator == Operator::ADD) return Relative;
	if (m_operator == Operator::SUB)
	{
		if (m_rightExpression->getType() == Absolute) return Relative;
		if (m_leftExpression->getType() != Absolute && isInBrackets())
		{
			std::shared_ptr<SymbolExpression> left = std::dynamic_pointer_cast<SymbolExpression>(m_leftExpression);
			std::shared_ptr<SymbolExpression> right = std::dynamic_pointer_cast<SymbolExpression>(m_rightExpression);
			if (left != nullptr && right != nullptr && left->isFromSameSegment(right)) return Absolute;
		}	
	}
	throw std::string("Illegal expression");
}

std::vector<std::shared_ptr<const AbstractSymbol>> ExpressionWithOperator::getDependableSymbols() const
{
	std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbols;

	if (getType() == Relative)
	{
		std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbolsLeft = m_leftExpression->getDependableSymbols();
		std::vector<std::shared_ptr<const AbstractSymbol>> dependableSymbolsRight = m_rightExpression->getDependableSymbols();
		for (size_t i = 0; i < dependableSymbolsLeft.size(); i++) {
			dependableSymbols.push_back(dependableSymbolsLeft[i]);
		}
		for (size_t i = 0; i < dependableSymbolsRight.size(); i++) {
			dependableSymbols.push_back(dependableSymbolsRight[i]);
		}
	}

	return dependableSymbols;
}


