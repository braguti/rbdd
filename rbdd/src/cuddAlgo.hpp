//
//  cuddAlgo.hpp
//  myKconf
//
//  Created by david on 16/05/2018.
//  Copyright Â© 2018 david. All rights reserved.
//

#ifndef __cuddAlgo__
#define __cuddAlgo__

#include <stdio.h>
#include <sstream>

#include "cuddInt.h"
#include "cuddAdapter.hpp"
#include "Traverser.hpp"

std::vector<int>                getLevelNodes(cuddAdapter* a);
std::vector<mpf_class>          computeCommonalitySingle(bool xverbose, cuddAdapter *a);
std::vector<mpf_class>          computeCommonalityMP(int num, bool xverbose, cuddAdapter *a, bool fast);
std::vector<mpz_class>          computeDistributionSingle(bool xverbose, cuddAdapter *a);
std::vector<mpz_class>          computeDistributionMP(int num, bool xverbose, cuddAdapter *a, bool fast);
std::pair<std::vector<std::string>, std::vector<std::string>> getCoreAndDeadSingle(bool xverbose, cuddAdapter *a);
std::pair<std::vector<std::string>, std::vector<std::string>> getCoreAndDeadMP(int num, bool xverbose, cuddAdapter *a, bool fast);
std::vector<bool>               uniformRandomSamplingBoolSingle(bool xverbose, cuddAdapter *a);
std::vector<bool>               uniformRandomSamplingBoolMP(int num, bool xverbose, cuddAdapter *a, bool fast);
std::string                     uniformRandomSamplingNameSingle(bool xverbose, cuddAdapter *a);
std::string                     uniformRandomSamplingNameMP(int num, bool xverbose, cuddAdapter *a, bool fast);

class ucr {
public:
    // How many solutions to the BDD rooted in this node
    mpz_class               counter;
    // For each variable from this level to the bottom, how many
    // solutions with the variable set to true
    std::vector<mpz_class>  products;

    
};

#endif /* cuddAlgo_hpp */
