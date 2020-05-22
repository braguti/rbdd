
#ifndef __RbddFunctions__
#define __RbddFunctions__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "configInfo.hpp"
#include "SiftMinScore.hpp"
#include "ParserCNF.hpp"
#include "RbddUtils.hpp"
#include "bddParserBuddy.hpp"
#include "bddParserCudd.hpp"
#include "Traverser.hpp"
#include "splotScanner.hpp"
#include "Transformation.hpp"
#include "cuddAlgo.hpp"
//#include "gmp.h"
//#include "gmpxx.h"

using namespace std;
using namespace Rcpp;

class RbddFunction {
	public:
		static Rcpp::List traverseBdd(int plevel, int tlevel, int elevel, const Rcpp::List& tr, const Rcpp::List& er, Rcpp::Function functToApply);
		static Rcpp::List traverseBddNode(int plevel, int tlevel, int elevel, const Rcpp::List& tr, const Rcpp::List& er, int node, Rcpp::Function functToApply);
};

#endif /* defined(__RbddFunctions__) */
