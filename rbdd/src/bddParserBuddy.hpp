
#ifndef __bddParserBuddy__
#define __bddParserBuddy__

#include <vector>
#include "RbddUtils.hpp"
#include <Rcpp.h>

class bddParserBuddy {

	public:
		// Second output arg:
		//	-1: Bad library
		//	0: Not found
		//	1: Buddy library
		std::pair<SemanticsBuddy, int> parseBdd(std::string expression, std::vector<RbddElem> rbddElems);
};


#endif /* defined(__bddParserBuddy__) */
