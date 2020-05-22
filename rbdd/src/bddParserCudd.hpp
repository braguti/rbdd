
#ifndef __bddParserCudd__
#define __bddParserCudd__

#include <vector>
#include "RbddUtils.hpp"
#include <Rcpp.h>

class bddParserCudd {

	public:
		// Second output arg:
		//	-1: Bad library
		//	0: Not found
		//	1: Cudd library
		std::pair<SemanticsCudd, int> parseBdd(std::string expression, std::vector<RbddElem> rbddElems);
};

#endif /* defined(__bddParserCudd__) */
