#include <iostream>
#include "exprnode.hpp"

exprnode::exprnode(exprnode *left, char opcode, exprnode *right) : left(left),
																   right(right),
																   opcode(opcode),
																   value(0) {}

exprnode::exprnode(exprnode *left, char opcode, exprnode *right,
				   VALUE_TYPE value, std::string varname) : left(left),
															right(right),
															opcode(opcode),
															value(value),
															varname(varname) {}

exprnode::exprnode(VALUE_TYPE value) : left(NULL),
									   right(NULL),
									   opcode('c'),
									   value(value) {}

exprnode::exprnode(char opcode, const std::string &varname) : left(NULL),
															  right(NULL),
															  opcode(opcode),
															  value(0),
															  varname(varname) {}

bool exprnode::operator==(const VALUE_TYPE val)
{
	return (opcode == 'c' && value == val);
}

bool exprnode::operator!=(const VALUE_TYPE val)
{
	return !(*this == val);
}

exprnode::~exprnode()
{
	delete left;
	delete right;
}

exprnode *exprnode::clone() const
{
	return new exprnode(
		left == NULL ? left : left->clone(),
		opcode,
		right == NULL ? right : right->clone(),
		value,
		varname);
}
