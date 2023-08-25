
#include <vector>
#include "RbddUtils.hpp"

class bddParserCudd {

	public:
		// Second output arg:
		//	-1: Bad library
		//	0: Not found
		//	1: Cudd library
		std::pair<SemanticsCudd, int> parseBdd(std::string expression, std::vector<RbddElem> rbddElems);
};
