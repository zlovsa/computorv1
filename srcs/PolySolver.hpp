#include "Expression.hpp"
#include <string>
#include <vector>
#include <map>

class PolySolver
{
public:
	PolySolver();
	PolySolver(int ac, char **av);
	PolySolver(const PolySolver &other);
	PolySolver &operator=(const PolySolver &other);
	~PolySolver();
	static std::string Help();
	bool getSolved() const;
	std::string getMsg() const;
	std::string reducedForm() const;
	int getDegree() const;
	std::string printPolynom(std::map<double, Expression::VALUE_TYPE> polynom, std::string varname) const;
	const std::vector<std::map<double, Expression::VALUE_TYPE>> &getRoots() const;
	std::string getTree() const;
	bool explane() const;
	Expression::VALUE_TYPE getDiscriminant() const;

private:
	bool processArguments(int ac, char **av);
	bool readEquation(std::string &s);
	bool checkDegree();
	bool error(std::string errmsg);
	void solve();
	void addroot(Expression::VALUE_TYPE re, Expression::VALUE_TYPE im);
	
	Expression *_expr = NULL;
	bool _solved = false;
	bool _explanation = false;
	std::string _errmsg;
	long double _precision = 1e-6l;
	std::map<double, Expression::VALUE_TYPE> polynom;
	std::vector<std::map<double, Expression::VALUE_TYPE>> roots;
	int deg;
	int _digits = 6;
	Expression::VALUE_TYPE a = 0, b = 0, c = 0, d = 0;
};
