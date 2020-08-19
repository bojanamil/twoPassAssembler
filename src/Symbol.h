#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "AbstractSymbol.h"
class Symbol :
	public AbstractSymbol,
	public std::enable_shared_from_this<Symbol>
{
	std::shared_ptr<const Segment> m_segment;
	std::shared_ptr<AbstractExpression> m_expression;
public:
	Symbol(std::string name);
	~Symbol();
	std::shared_ptr<const Segment> getSegment() const override { return m_segment; };
	std::shared_ptr<AbstractExpression> getExpression() const { return m_expression; };
	void setExpression(std::shared_ptr<AbstractExpression> _expression) { m_expression = _expression; };
	void setSegment(std::shared_ptr<const Segment> _segment) { m_segment = _segment; }
	
	virtual std::vector<std::shared_ptr<const AbstractSymbol>> getDependableSymbols() const override;
};

#endif