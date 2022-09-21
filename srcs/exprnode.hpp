#pragma once
#include <string>

struct exprnode
{
	typedef double VALUE_TYPE;
	struct exprnode *left;
	struct exprnode *right;
	char opcode;
	VALUE_TYPE value;
	std::string varname;
	exprnode(exprnode *left, char opcode, exprnode *right);
	exprnode(exprnode *left, char opcode, exprnode *right,
			 VALUE_TYPE value, std::string varname);
	exprnode(const VALUE_TYPE value);
	exprnode(const char opcode, const std::string &varname);
	~exprnode();
	exprnode *clone() const;

	bool operator==(const VALUE_TYPE val);
	bool operator!=(const VALUE_TYPE val);
};
