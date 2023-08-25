
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "configInfo.hpp"
#include "SiftMinScore.hpp"
#include "parserCNF.hpp"
#include "RbddUtils.hpp"
#include "bddParserBuddy.hpp"
#include "bddParserCudd.hpp"
#include "TraverseElem.hpp"

class RbddFunctions {

	public:
		void traverseRec(int node, Rcpp::Function functionToApply);
};