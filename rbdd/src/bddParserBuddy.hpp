
#include <vector>
#include "RbddUtils.hpp"

class bddParserBuddy {

	public:
		// Second output arg:
		//	-1: Bad library
		//	0: Not found
		//	1: Buddy library
		std::pair<SemanticsBuddy, int> parseBdd(std::string expression, std::vector<RbddElem> rbddElems);
};
