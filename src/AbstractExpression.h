#ifndef _ABSTRACT_EXPRESSION_H_
#define _ABSTRACT_EXPRESSION_H_

#include <memory>
#include <string>
#include <stack>
#include <vector>

class Context;
class AbstractSymbol;

class AbstractExpression
{
	bool m_isInBrackets;
public:
	enum Type { Absolute, Relative, Unknown };

	AbstractExpression();
	virtual ~AbstractExpression();
	

	virtual int getValue() const =0;
	virtual Type getType() const =0;


	static std::shared_ptr<AbstractExpression> parseExpretion(const std::string& _expression, Context& _context);

	static std::shared_ptr<AbstractExpression> parseOperand(const std::string& _operandString, Context& _context);

	bool isInBrackets() const { return  m_isInBrackets; }

	void setIsInBrackets(bool _isInBrackets) { m_isInBrackets = _isInBrackets; }

	virtual std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const=0;
	
};

class SimpleExpression: public AbstractExpression
{
	int m_value;
public:
	explicit SimpleExpression(int value)
		: m_value(value)
	{
	}

	int getValue() const override { return m_value; }
	static std::shared_ptr<SimpleExpression> tryParseWithBase(const std::string& _expresion, const unsigned _base, const std::string& _prefix, const std::string& _posibleValues);
	static std::shared_ptr<SimpleExpression> tryParse(const std::string& _expresion);
	Type getType() const override { return Absolute; };
	std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const override { return std::vector<std::shared_ptr< const AbstractSymbol>>(); }
	
};

class SymbolExpression : public AbstractExpression
{
	std::string m_name;
	Context &m_context;
public:


	SymbolExpression(const std::string& cs, Context& context)
		: m_name(cs),
		  m_context(context)
	{
	}

	int getValue() const override;
	Type getType() const override;
	bool isFromSameSegment(const std::shared_ptr<SymbolExpression>& right) const;
	static std::shared_ptr<SymbolExpression> tryParse(const std::string& _expresion, Context& _context);
	std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const override;
};

class ExpressionWithOperator : public AbstractExpression
{
public:
	class Operator
	{
		std::string m_operatorName;
		
	public:
		static const std::shared_ptr<Operator> ADD;
		static const std::shared_ptr<Operator> SUB;
		static const std::shared_ptr<Operator> MUL;
		static const std::shared_ptr<Operator> DIV;
		static const std::shared_ptr<Operator> OPEN_BRACKETS;
		static const std::shared_ptr<Operator> CLOSE_BRACKETS;


		Operator(const std::string& _operatorName)
			: m_operatorName(_operatorName)

		{
		}

		
		int getInfixPriority();
		int getStackPriority();
		int getRankChange();

		static std::shared_ptr<Operator> getOperatorFromString(const std::string& operatorName);

		int calculate(std::shared_ptr<AbstractExpression> _leftExpression, std::shared_ptr<AbstractExpression>  _rightExpession);
		



	};
private:
	std::shared_ptr<AbstractExpression> m_leftExpression;
	std::shared_ptr<AbstractExpression> m_rightExpression;
	std::shared_ptr<Operator> m_operator;
public:

	ExpressionWithOperator(std::shared_ptr<AbstractExpression> _leftExpression
		,	std::shared_ptr<AbstractExpression> _rightExpression
		,	std::shared_ptr<Operator> _operator)
		: m_leftExpression(_leftExpression), m_rightExpression(_rightExpression), m_operator(_operator)
	{}

	int getValue() const override { return m_operator->calculate(m_leftExpression, m_rightExpression); };
	Type getType() const override;
	std::shared_ptr<Operator> getOperator() const { return m_operator; };
	std::shared_ptr<AbstractExpression> getLeftExpression() const { return m_leftExpression; };
	std::shared_ptr<AbstractExpression> getRightExpression() const { return m_rightExpression; };

	static void outputOperator(std::stack<std::shared_ptr<AbstractExpression>>& operandStack, std::stack<std::shared_ptr<Operator>>& operatorStack, int& rank);
	std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const override;


};

#endif