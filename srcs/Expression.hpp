#pragma once
#include <iostream>
#include <set>
#include "exprnode.hpp"

class Expression
{
public:
	typedef exprnode::VALUE_TYPE VALUE_TYPE;
	class IncorrectExpression : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	Expression();
	Expression(const std::string& s);
	Expression(const Expression &other);
	Expression &operator=(const Expression &other);
	~Expression();
	std::string print() const;
	const exprnode *getRoot() const;
	const std::set<std::string> getVars() const;
	void Reduce();
	static std::string fixedout(Expression::VALUE_TYPE val, int precision = 6);

private:
	exprnode *readExpression();
	exprnode *readAddition();
	exprnode *readFactor();
	exprnode *readPower();
	exprnode *readConst();
	void collectVars();
	void collectVars(exprnode *root);

	exprnode *root;
	std::set<std::string> vars;

	char getNextChar();
	char getChar();
	std::string str;
	std::string::size_type i;
};
