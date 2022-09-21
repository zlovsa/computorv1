#include <iostream>
#include "PolySolver.hpp"
#include "Expression.hpp"
#include <map>
#include <iomanip>

int main(int ac, char **av)
{
	PolySolver solver(ac, av);
	if(!solver.getSolved()){
		std::cerr << solver.getMsg() << std::endl;
		return 1;
	}
	if(solver.explane())
		std::cout << "Expression tree:" << std::endl
				  << solver.getTree() << "Getting reduced form by reduction of similar terms:" << std::endl;
	std::cout << "Reduced form: " << solver.reducedForm() << " = 0" << std::endl
			  << "Polynomial degree: " << solver.getDegree() << std::endl;
	if (solver.getDegree() == 2 && solver.explane())
		std::cout << "Discriminant is equal to " << solver.getDiscriminant() << std::endl;
	std::cout << solver.getMsg() << std::endl;
	for (auto x : solver.getRoots())
		std::cout << solver.printPolynom(x, "i") << std::endl;
}
