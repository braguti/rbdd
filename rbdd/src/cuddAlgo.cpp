//
//  cuddAlgo.cpp
//  myKconf
//
//  Created by david on 16/05/2018.
//  Copyright Â© 2018 david. All rights reserved.
//

#include "cuddAlgo.hpp"


std::vector<int> levelNodes;
std::set<int> levelJump;
std::vector<mpz_class> powersof2;
std::vector<std::vector<mpz_class>> combinations;
int totalNodes  = 0;
int divider     = 0;
int counter     = 1;
int distcounter = 1;

int _combLevelNodes(int plevel, int tlevel, int elevel,
                                 const int& tr,
                                 const int& er,
                                 Rcpp::Function functToApply) {
    levelNodes[plevel]++;
    return 0;
}

std::vector<int> getLevelNodes(cuddAdapter* a) {
    levelNodes.resize(a->getNumVars());
    for(int i = 0; i < a->getNumVars(); i++)
        levelNodes[i] = 0;
    traverse(false, a, 0, 0, _combLevelNodes);
    return levelNodes;
}

// We return the biggest jump.
int  _compLevelJump(int plevel, int tlevel, int elevel,
                    const int& tr,
                    const int& er,
                    Rcpp::Function functToApply) {
    int max = -1;
    if (tr > max) max = tr;
    if (er > max) max = er;

    if (tr != -1) {
	levelJump.insert(tlevel - plevel - 1);
        if (tlevel - plevel - 1 > max) {
	    max = tlevel - plevel - 1;
        }
    }
    if (er != -1) {
        levelJump.insert(elevel - plevel - 1);
        if (elevel - plevel - 1 > max) {
	    max = elevel - plevel - 1;
       }
    }
    if (max == -1) max = 0;
    return max;
}

void makePowersof2(int n ) {
    powersof2.resize(n+1);
    levelJump.insert(0);
    for(int i : levelJump)
        mpz_ui_pow_ui(powersof2[i].get_mpz_t(), 2, i);
}

mpz_class& getPowerof2(int n) {
    return powersof2[n];
}

ucr _upProducts(int plevel, int tlevel, int elevel, const ucr& tr, const ucr& er, Rcpp::Function functToApply) {
    ucr res, resT, resE;
    mpz_class thenPart, elsePart;

    if (divider >= 1 && counter % divider == 0)
        if (1+100*(counter)/totalNodes <= 100) {
            Rcpp::Rcout << 1+100*(counter)/totalNodes << "%" << std::endl;
        }
    counter++;
    if (tr.counter != 0) {
        mpz_class& tempThen = getPowerof2(tlevel - plevel - 1);
        thenPart = tr.counter*tempThen;
        mpz_class halfThen = thenPart/2;
	    resT.counter = thenPart;
        resT.products.push_back(thenPart);

        // For reduced nodes in to the then child
        for(int i = plevel+1; i < tlevel; i++)
            resT.products.push_back(halfThen);

        // temp = 2 ^ (tlevel - plevel -1)
        for(const mpz_class& t : tr.products) {
            resT.products.push_back(t*tempThen);
        }
    }
    
    if (er.counter != 0) {
        mpz_class& tempElse = getPowerof2(elevel - plevel - 1);
        elsePart = er.counter*tempElse;
        mpz_class halfElse = elsePart/2;
	    resE.counter = elsePart;
        resE.products.push_back(0);

        for(int i = plevel+1; i < elevel; i++)
            resE.products.push_back(halfElse);

        for(const mpz_class& e : er.products) {
            resE.products.push_back(e*tempElse);
        }
    }

    if (thenPart != 0 && elsePart != 0) {
        res.counter = thenPart + elsePart;
        for(int i = 0; static_cast<unsigned int>(i) < resT.products.size(); i++) {
            res.products.push_back(resT.products[i] + resE.products[i]);
        }

       return res;
    }
    if (thenPart == 0) {
	    return resE;
    }

    return resT;
}

std::vector<mpf_class> computeCommonalitySingle(bool xverbose, cuddAdapter *a) {
    std::vector<mpf_class> res;
    counter = 1;
    levelJump.clear();
    powersof2.clear();

    ucr rZero, rOne, recRes;
    rZero.counter = 0;
    rOne.counter  = 1;
    rZero.products.push_back(0);
    rOne.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(false, a, -1, 0, _compLevelJump);

    makePowersof2(maxJump);
    totalNodes = a->nodecount(a->getTraverseBDD());
    divider    = totalNodes/100;
    recRes = traverse(xverbose, a, rZero, rOne, _upProducts);
    // Last component would be commonality of 1 so we do not include it
    for(int i = 0; static_cast<unsigned int>(i) < recRes.products.size()-1; i++) {
        mpf_class current = ((mpf_class) recRes.products[i] /recRes.counter);
        res.push_back(current);
    }

    return res;
}

std::vector<mpf_class> computeCommonalityMP(int num, bool xverbose, cuddAdapter *a, bool fast) {
    std::vector<mpf_class> res;
    counter = 1;
    levelJump.clear();
    powersof2.clear();

    ucr rZero, rOne, recRes;
    rZero.counter = 0;
    rOne.counter  = 1;
    rZero.products.push_back(0);
    rOne.products.push_back(1);
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(false, a, -1, 0, _compLevelJump);
    makePowersof2(maxJump);
    totalNodes = a->nodecount(a->getTraverseBDD());
    divider    = totalNodes/100;
    recRes = traverseMP(xverbose, num, a, rZero, rOne, _upProducts, fast);
    // Last component would be commonality of 1 so we do not include it
    for(int i = 0; static_cast<unsigned int>(i) < recRes.products.size()-1; i++) {
        mpf_class current = ((mpf_class) recRes.products[i] /recRes.counter);
        res.push_back(current);
    }
    
    return res;
}

mpz_class& getCombinations(int a, int b) {
    if (b < a/2+1) {
        return combinations.at(a).at(b);
    }
    else {
        return combinations.at(a).at(a-b);
    }
}

void makeCombinations(int n) {
    std::vector<mpz_class> lastRow, current;
    mpz_class lastNum;
    // 0 over 0
    lastRow.push_back(1);
    combinations.push_back(lastRow);
    lastRow.clear();
    // 1 over 0
    lastRow.push_back(1);
    // 1 over 1
    lastRow.push_back(1);
    combinations.push_back(lastRow);
    
    for(int x = 2; x <= n; x++) {
        if (x % 1000 == 0) {
            Rcpp::Rcout << "combinations: " << x << std::endl;
        }
        current.clear();
        lastNum = 0;
        for(int k = 0; k < x/2+1; k++) {
            mpz_class trav = getCombinations(x-1, k);
            current.push_back(lastNum+trav);
            lastNum = trav;
        }
        combinations.push_back(current);
        lastRow = current;
    }
}

std::vector<mpz_class>  _distCombine(int plevel, int tlevel, int elevel,
                                     const std::vector<mpz_class>& tr,
                                     const std::vector<mpz_class>& er,
                                     Rcpp::Function functToApply) {

    if (divider >= 1 && distcounter % divider == 0) 
       if (1+100*(distcounter)/totalNodes <= 100) {
           Rcpp::Rcout << 1+100*(distcounter)/totalNodes << "%" << std::endl;
       }
    distcounter++;
    std::vector<mpz_class> thenBit(tlevel - plevel + tr.size());
    std::vector<mpz_class> elseBit(elevel - plevel + er.size());
    // Size of the distribution is any of the lowdif or highdif
    // it's the same number
    std::vector<mpz_class> infoDist(elseBit.size());
    // Now we compute the distribution of the "else" (low) path.

    // If the else node is zero, then we don't do anything
    if (er.size() != 1 || er[0] != 0)
        for(int a = 0; static_cast<unsigned int>(a) < elseBit.size(); a++) {
            // We fill theBit one component at a time
            for(int b = 0;  b < elevel - plevel ; b++) {
                // How many different paths (along omitted nodes) are there from
                // the node to the "else" descendant passing through b omitted
                //  "then" arcs? Answer ((elevel - plevel -1) over b)
                // factor is (elevel - plevel -1) over b
                if (a-b >= 0 && static_cast<unsigned int>(a - b) < er.size()) {
                    elseBit[a] += getCombinations(elevel-plevel-1, b) * er[a-b];
                }
            }
        }
    
    // Now we compute the distribution of the "then" (high) path. (almost symmetrical)
    // a starts at 1 because thenBit[0] has to be 0.
    // If the then node is zero, then we don't do anything
    if (tr.size() != 1 || tr[0] != 0)
        for(int a = 1; static_cast<unsigned int>(a) < thenBit.size(); a++) {
            // We fill highdif one component at a time
            for(int b = 0; b < tlevel - plevel; b++) {
                // How many different paths (along omitted nodes) are there from
                // the node to the "then" descendant passing through b omitted
                //  "then" arcs? Answer (tlevel - plevel - 1) over b
                // factor is (tlevel - plevel - 1) over b
                if (a-b >= 0 && static_cast<unsigned int>(a-b-1) < tr.size()) {
                    thenBit[a] += getCombinations(tlevel-plevel-1, b) * tr[a-b-1];
                }
            }
        }
    // Now we combine both;
    for(int l = 0; static_cast<unsigned int>(l) < infoDist.size(); l++) {
        // We add the branches
        infoDist[l] = thenBit[l] + elseBit[l];
    }

    return infoDist;
}

std::vector<mpz_class> computeDistributionSingle(bool xverbose, cuddAdapter* a) {
    std::vector<mpz_class> dZero, dOne;
    distcounter = 1;
    levelJump.clear();
    combinations.clear();

    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    int maxJump = traverse(false, a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount(a->getTraverseBDD());
    divider    = totalNodes/100;
    makeCombinations(maxJump);

    std::vector<mpz_class> combinations = traverse(xverbose, a, dZero, dOne, _distCombine);
    return combinations;
}

std::vector<mpz_class> computeDistributionMP(int num, bool xverbose, cuddAdapter *a, bool fast) {
    std::vector<mpz_class> dZero, dOne;
    distcounter = 1;
    levelJump.clear();
    combinations.clear();

    dZero.resize(1);
    dOne.resize(1);
    dZero[0] = 0;
    dOne[0]  = 1;
    // Jumps to node zero are ignored. But jumps to one are counted.
    int maxJump = traverse(false, a, -1, 0, _compLevelJump);
    totalNodes = a->nodecount(a->getTraverseBDD());
    divider    = totalNodes/100;
    makeCombinations(maxJump);

    std::vector<mpz_class> combinations = traverseMP(xverbose, num, a, dZero, dOne, _distCombine, fast);
    return combinations;
}

std::vector<int> core;
std::vector<int> dead;

bool _updateCoreDead(int plevel, int tlevel, int elevel, const bool& tr, const bool& er, Rcpp::Function functToApply) {
    for (int i = plevel + 1; i < tlevel; i++) {
        core[i] = 0;
        dead[i] = 0;
    }

    for (int i = plevel + 1; i < elevel; i++) {
        core[i] = 0;
        dead[i] = 0;
    }

    if (tr && er) { // Los dos son ciertos
        core[plevel] = 0;
        dead[plevel] = 0;
    }

    if (tr && !er) {
        dead[plevel] = 0;
    }

    if (!tr && er) {
        core[plevel] = 0;
    }

    return true; // Este no es el nodo cero.
}

std::pair<std::vector<std::string>, std::vector<std::string>> getCoreAndDeadSingle(bool xverbose, cuddAdapter *a) {
    std::vector<std::string> vars = a->getVars(false);
    core.clear();
    dead.clear();
    core.resize(vars.size(), 1);
    dead.resize(vars.size(), 1);

    traverse(xverbose, a, false, true, _updateCoreDead);

    std::vector<std::string> coreVars;
    std::vector<std::string> deadVars;

    for (int i = 0; static_cast<unsigned int>(i) < vars.size(); i++) {
        if (core[i]) {
            coreVars.push_back(vars[i]);
        }

        if (dead[i]) {
            deadVars.push_back(vars[i]);
        }
    }

    std::pair<std::vector<std::string>, std::vector<std::string>> coreDead;
    coreDead.first = coreVars;
    coreDead.second = deadVars;

    return coreDead;
}

std::pair<std::vector<std::string>, std::vector<std::string>> getCoreAndDeadMP(int num, bool xverbose, cuddAdapter *a, bool fast) {
    std::vector<std::string> vars = a->getVars(false);
    core.clear();
    dead.clear();
    core.resize(vars.size(), 1);
    dead.resize(vars.size(), 1);

    traverseMP(xverbose, num, a, false, true, _updateCoreDead, fast);

    std::vector<std::string> coreVars;
    std::vector<std::string> deadVars;

    for (int i = 0; static_cast<unsigned int>(i) < vars.size(); i++) {
        if (core[i]) {
            coreVars.push_back(vars[i]);
        }

        if (dead[i]) {
            deadVars.push_back(vars[i]);
        }
    }

    std::pair<std::vector<std::string>, std::vector<std::string>> coreDead;
    coreDead.first = coreVars;
    coreDead.second = deadVars;

    return coreDead;
}

std::map<int, mpf_class> probabilities;

void getProducts(int plevel, int tlevel, int elevel, const mpz_class& tr, const mpz_class& er, mpz_class& thenPart, mpz_class& elsePart) {
    // thenPart = 2 ^ (tlevel-plevel-1) * tr
    mpz_ui_pow_ui(thenPart.get_mpz_t(), 2, tlevel - plevel -1);
    thenPart *= tr;
    
    // elsePart = 2 ^(elevel - plevel -1) * er
    mpz_ui_pow_ui(elsePart.get_mpz_t(), 2, elevel - plevel -1);
    elsePart *= er;
}

mpz_class _probabilities(int plevel, int tlevel, int elevel, const mpz_class& tr, const mpz_class& er, int node, Rcpp::Function functToApply) {
    mpz_class thenPart, elsePart;
    getProducts(plevel, tlevel, elevel, tr, er, thenPart, elsePart);
    mpf_class fthen = thenPart;
    mpf_class felse = elsePart;
    probabilities[node] = fthen / (fthen + felse);
    return (thenPart + elsePart);
}

void compProbabilities(bool xverbose, cuddAdapter* a) {
    //unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    unsigned int temp = (unsigned int) time(NULL) % 1000;
    //cout << "Initializing random seed " << temp << endl;
    srand(temp);
    traverse(xverbose, a, mpz_class(0), mpz_class(1), &_probabilities);
}

void compProbabilitiesMP(bool xverbose, int threads, cuddAdapter* a, bool fast) {
    //unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    unsigned int temp = (unsigned int) time(NULL) % 1000;
    //cout << "Initializing random seed " << temp << endl;
    srand(temp);
    traverseMP(xverbose, threads, a, mpz_class(0), mpz_class(1), &_probabilities, fast);
}

bool fiftyfifty() {
    float randNum = (float)rand()/(float)RAND_MAX;
    return (randNum < 0.5);
}

std::vector<bool> genRandom(cuddAdapter* a) {
    //unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    //cout << "Initializing random seed " << temp << endl;
    //srandom(temp);
    // We assume the probabilities have been computed already
    DdNode *g1, *g0;
    int index;
    std::vector<bool> exemplar(a->getNumVars());
    DdNode *trav = a->getBDD(a->getTraverseBDD());
    DdNode *one  = a->getOne();
    DdNode *zero = a->getZero();
    int    pos   = 0;
    if (trav == zero)
        return exemplar;
    
    index = a->getLevel(trav);
    for(int i = 0; i< index; i++) {
        exemplar[a->varAtPos(pos++)] = fiftyfifty();
    }
    
    while (trav != one) {
        cuddGetBranches(trav, &g1, &g0);
        float randNum = (float)rand()/(float)RAND_MAX;

        int posNode = a->getNodePos(trav);
        if (randNum <= probabilities[posNode]) {
            trav = g1;
            exemplar[a->varAtPos(pos++)] = true;
        } else {
            exemplar[a->varAtPos(pos++)] = false;
            trav = g0;
        }

        for(int i = index+1; i < a->getLevel(trav); i++) {
            exemplar[a->varAtPos(pos++)] = fiftyfifty();
        }
        
        index = a->getLevel(trav);
    }
    return exemplar;
}

std::string nameProduct(cuddAdapter*a, std::vector<bool> v) {
    std::ostringstream ost;
    int var = 0;
    for(bool b : v) {
        std::string nameVar = a->getVarName(var++);
        if (b)
            ost <<  "    " << nameVar << " ";
        else
            ost << "not " << nameVar << " ";
    }
    return ost.str();
}

std::string nameRandom(cuddAdapter *a) {
    std::ostringstream ost;
    return nameProduct(a, genRandom(a));
}

std::vector<bool> uniformRandomSamplingBoolSingle(bool xverbose, cuddAdapter *a) {
    probabilities.clear();
    a->clearNodeList();
    compProbabilities(xverbose, a);

    return genRandom(a);
}

std::vector<bool> uniformRandomSamplingBoolMP(int num, bool xverbose, cuddAdapter *a, bool fast) {
    probabilities.clear();
    a->clearNodeList();
    compProbabilitiesMP(xverbose, num, a, fast);

    return genRandom(a);
}

std::string uniformRandomSamplingNameSingle(bool xverbose, cuddAdapter *a) {
    probabilities.clear();
    a->clearNodeList();
    compProbabilities(xverbose, a);

    return nameRandom(a);
}

std::string uniformRandomSamplingNameMP(int num, bool xverbose, cuddAdapter *a, bool fast) {
    probabilities.clear();
    a->clearNodeList();
    compProbabilitiesMP(xverbose, num, a, fast);

    return nameRandom(a);
}
