#include "Expression.hpp"
#include <sstream>
#include <iomanip>

const char *Expression::IncorrectExpression::what() const throw()
{
	return "Incorrect expression";
}

Expression::Expression() : root(NULL) {}

Expression::Expression(const std::string &s) : root(NULL)
{
	str = s;
	i = -1;
	root = readExpression();
	if (!root)
		throw IncorrectExpression();
	char c = getChar();
	if (c == '=')
	{
		exprnode *rhs = readExpression();
		if (!rhs)
		{
			delete root;
			root = NULL;
			throw IncorrectExpression();
		}
		exprnode *nexpr = new exprnode(root, c, rhs);
		root = nexpr;
	}
	if(i<str.length())
	{
		delete root;
		root = NULL;
		throw IncorrectExpression();
	}
	collectVars();
}

void Expression::Reduce()
{
	if (root->opcode == '=' && *root->right != 0)
	{
		root->opcode = '-';
		exprnode *nexpr = new exprnode(root, '=', new exprnode(0));
		root = nexpr;
	}
}

const std::set<std::string> Expression::getVars() const
{
	return vars;
}

Expression::Expression(const Expression &other)
{
	if (this != &other)
		*this = other;
}

Expression &Expression::operator=(const Expression &other)
{
	if(this==&other)
		return (*this);
	delete root;
	if(other.root)
		root = other.root->clone();
	else
		root = NULL;
	return (*this);
}

Expression::~Expression()
{
	delete root;
}

const exprnode *Expression::getRoot() const
{
	return root;
}

void recprint(exprnode *root, int indent, std::stringstream &ss)
{
	if(!root)
		return;
	std::string str_indent(indent * 2, ' ');
	if (root->opcode == 'c')
		ss << str_indent << root->value << std::endl;
	else if (root->opcode == 'v')
		ss << str_indent << root->varname << std::endl;
	else
	{
		recprint(root->right, indent + 1, ss);
		ss << str_indent << root->opcode << std::endl;
		recprint(root->left, indent + 1, ss);
	}
}

std::string Expression::print() const
{
	std::stringstream ss;
	recprint(root, 0, ss);
	return ss.str();
}

char Expression::getNextChar()
{
	i++;
	return getChar();
}

char Expression::getChar()
{
	while (i < str.length() && std::isspace(str[i]))
		i++;
	if(i<str.length())
		return str[i];
	return -1;
}

exprnode *Expression::readExpression()
{
	exprnode *expr = readAddition();
	if (!expr)
		return NULL;
	char c = getChar();
	while (c == '+' || c == '-')
	{
		exprnode *rhs = readAddition();
		if (!rhs)
		{
			delete expr;
			return NULL;
		}
		exprnode *nexpr = new exprnode(expr, c, rhs);
		c = getChar();
		expr = nexpr;
	}
	return expr;
}

exprnode *Expression::readAddition()
{
	exprnode *expr = readFactor();
	if (!expr)
		return NULL;
	char c = getChar();
	while (c == '*' || c == '/' || std::isalpha(c) || c == '(')
	{
		if (std::isalpha(c) || c == '(')
		{
			i--;
			c = '*';
		}
		exprnode *rhs = readFactor();
		if (!rhs)
		{
			delete expr;
			return NULL;
		}
		exprnode *nexpr = new exprnode(expr, c, rhs);
		c = getChar();
		expr = nexpr;
	}
	return expr;
}

exprnode *Expression::readFactor()
{
	exprnode *expr = readPower();
	if (!expr)
		return NULL;
	char c = getChar();
	if (c == '^')
	{
		exprnode *rhs = readFactor();
		if (!rhs)
		{
			delete expr;
			return NULL;
		}
		exprnode *nexpr = new exprnode(expr, c, rhs);
		c = getChar();
		expr = nexpr;
	}
	return expr;
}

std::string Expression::fixedout(Expression::VALUE_TYPE val, int precision)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(precision) << (val == 0 ? 0 : val);
	std::string s = ss.str();
	size_t dot = s.find_first_of('.'), last = s.find_last_not_of(".0");
	if (dot != std::string::npos)
		return s.substr(0, dot > last + 1 ? dot : (last + 1));
	else
		return s;
}

exprnode *Expression::readConst()
{
	VALUE_TYPE val = 0;
	exprnode *expr = NULL;
	char *end;
	val = std::strtold(str.c_str() + i, &end);
	if(errno==ERANGE)
		return expr;
	std::string s(str.c_str() + i, end - str.c_str() - i);
	size_t dot = s.find_first_of('.');
	int precision = 1000;
	if (dot != std::string::npos)
		precision = s.length() - dot - 1;
	i = end - str.c_str();
	std::string strval = fixedout(val, precision);
	if (s != strval)
		return expr;
	expr = new exprnode(val);
	return expr;
}

void Expression::collectVars()
{
	vars.clear();
	collectVars(root);
}

void Expression::collectVars(exprnode *root)
{
	if(!root)
		return;
	if(root->opcode=='v')
		vars.insert(root->varname);
	collectVars(root->left);
	collectVars(root->right);
}

exprnode *Expression::readPower()
{
	exprnode *expr = NULL;
	char c = getNextChar();
	if (std::isalpha(c))
	{
		std::string vname;
		vname += c;
		expr = new exprnode('v', vname);
		getNextChar();
	}
	else if (c == '(')
	{
		expr = readExpression();
		c = getChar();
		if (c != ')')
		{
			delete expr;
			return NULL;
		}
		getNextChar();
	}
	else if (std::isdigit(c) || c == '.')
		expr = readConst();
	else if (c == '-'||c=='+')
	{
		char c1 = getNextChar();
		i--;
		if(std::isdigit(c1)||c1=='.')
			expr = readConst();
		else
		{
			exprnode *nexpr = readFactor();
			if (!nexpr)
				return NULL;
			expr = new exprnode(new exprnode(-1), '*', nexpr);
		}
	}
	return expr;
}
