//
//  cuddFactory.cpp
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//



#include "cuddFactory.hpp"
#include "cuddInt.h"

int cuddFactory::MAXVAR = 100000;

using namespace cudd;

cuddFactory::cuddFactory(double cacheMultiplier) :     mgr(0,0,CUDD_UNIQUE_SLOTS, cacheMultiplier*CUDD_CACHE_SLOTS, 0) {
    SemanticsCudd::setManager(&mgr);
    numVars = 0;
    reorderMap["CUDD_REORDER_NONE"]             = CUDD_REORDER_NONE;
    reorderMap["CUDD_REORDER_SAME"]             = CUDD_REORDER_SAME;
    reorderMap["CUDD_REORDER_RANDOM"]           = CUDD_REORDER_RANDOM;
    reorderMap["CUDD_REORDER_RANDOM_PIVOT"]     = CUDD_REORDER_RANDOM_PIVOT;
    reorderMap["CUDD_REORDER_SIFT"]             = CUDD_REORDER_SIFT;
    reorderMap["CUDD_REORDER_SIFT_CONVERGE"]    = CUDD_REORDER_SIFT_CONVERGE;
    reorderMap["CUDD_REORDER_SYMM_SIFT"]        = CUDD_REORDER_SYMM_SIFT;
    reorderMap["CUDD_REORDER_SYMM_SIFT_CONV"]   = CUDD_REORDER_SYMM_SIFT_CONV;
    reorderMap["CUDD_REORDER_GROUP_SIFT"]       = CUDD_REORDER_GROUP_SIFT;
    reorderMap["CUDD_REORDER_GROUP_SIFT_CONV"]  = CUDD_REORDER_GROUP_SIFT_CONV;
    reorderMap["CUDD_REORDER_WINDOW2_CONV"]     = CUDD_REORDER_WINDOW2_CONV;
    reorderMap["CUDD_REORDER_WINDOW3_CONV"]     = CUDD_REORDER_WINDOW3_CONV;
    reorderMap["CUDD_REORDER_WINDOW4_CONV"]     = CUDD_REORDER_WINDOW4_CONV;
    reorderMap["CUDD_REORDER_ANNEALING"]        = CUDD_REORDER_ANNEALING;
    reorderMap["CUDD_REORDER_GENETIC"]          = CUDD_REORDER_GENETIC;
    reorderMap["CUDD_REORDER_EXACT"]            = CUDD_REORDER_EXACT;
    theBDD = mgr.bddOne();
    reorderMin = std::numeric_limits<int>::max();;
    reorderMax = -1;
    minVar = maxVar = "";
    mgr.AutodynDisable();
    //mgr.DisableGarbageCollection();
}

void    cuddFactory::setValue(std::string s , synExp* e) {
    if (e == synTrue) {
        apply(new synSymbol(s));
        return;
    }
    if (e == synFalse) {
        apply(makeNot(new synSymbol(s)));
        return;
    }
}

std::vector<bool> cuddFactory::generateRandom() {
    std::vector<bool> res;
    //We asume that someone called computeProbabilities already
    DdNode *myNode = theBDD.getNode();
    float randNum;
    while (!cuddIsConstant(Cudd_Regular(myNode))) {
        int indexOrig = mgr.ReadPerm(Cudd_Regular(myNode)->index);
        //std::cerr << "Here, prob " << getProb(myNode) << " index " << mgr.ReadPerm(Cudd_Regular(myNode)->index) << std::endl;
        randNum = (float)random()/(float)RAND_MAX;
        if (randNum < getProb(myNode)) {
            res.push_back(true);
            myNode = cuddT(Cudd_Regular(myNode));
        }
        else {
            res.push_back(false);
            myNode = cuddE(Cudd_Regular(myNode));
        }
        int indexEnd = mgr.ReadPerm(Cudd_Regular(myNode)->index);
        if (cuddIsConstant(Cudd_Regular(myNode)))
            indexEnd = mgr.ReadSize();
        
        //std::cerr << "IndexOrig " << indexOrig << " indexEnd " << indexEnd << std::endl;
        for(int x = indexOrig + 1; x < indexEnd; x++)
            if ((float)random()/(float)RAND_MAX > 0.5)
                res.push_back(true);
            else
                res.push_back(false);
    }
    std::cerr << "Result size " << res.size() << ": ";
    for(int x = 0; x < res.size();x++) std::cerr << res[x] << " ";
    std::cerr << std::endl;
    return res;
}

void   cuddFactory::computeProbabilities() {
    oneCounter = 0;
    //std::cerr << "Starting compute coefficients" << std::endl;
    nodeCounter = 0;
    computeProbRecur(Cudd_Regular(theBDD.getNode()));
    //std::cerr << "Probability of root " << theBDD.getNode() << " is " << getProb(theBDD.getNode()) << std::endl;
    //for(auto m : probabilities)
    //    std::cerr << "Node " << m.first << " probability " << m.second << std::endl;
    unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
    srandom(temp);
}

double  cuddFactory::getProb(DdNode* node) {
    if (Cudd_IsComplement(node)) {
        //std::cerr << " is negated ";
        return 1.0 - probabilities.at(Cudd_Regular(node));
    }
    //std::cerr << "just " << coefficients.at(node) << std::endl;
    return probabilities.at(node);

}

mpz_class  cuddFactory::computeeCoef(DdNode* node) {
    int index;
    try {
    if (cuddIsConstant(Cudd_Regular(node)))
        index = mgr.ReadSize();
    else
        index = Cudd_Regular(node)->index;

    if (Cudd_IsComplement(node)) {
        temp = pow(2, mgr.ReadSize()  - index) - coefficients.at(Cudd_Regular(node));
        return    temp;
    }
    temp = coefficients.at(node);
    }
    catch(std::exception e) {
        std::cerr << "Error looking for key  " << std::setw(50) << node << " R "
                << Cudd_Regular(node) << std::endl;
        exit(-1);
    }
    return temp;
}

void   cuddFactory::computeProbRecur(DdNode* node) {
    mark[node] = !mark[node];
//    if (cuddIsConstant(node)) {
//        probabilities[node] = 1.0;
//        if (Cudd_IsComplement(node)) {
//            std::cerr << "Negated node in computeProbRecur" << std::endl;
//            exit(-1);
//        }
//        return;
//    }
//    
//    DdNode* TNode = cuddT(node);
//    if (mark[node] != mark[TNode])
//        computeProbRecur(TNode);
//    
//    DdNode* ENode = cuddE(node);
//    if (mark[node] != mark[Cudd_Regular(ENode)])
//        computeProbRecur(Cudd_Regular(ENode));
//    
//    probabilities[node] = (getProb(TNode) + getProb(ENode)) / 2.0;
//    std::cerr   << "node " << node << "  probability is avg of " << getProb(TNode)
//                << " (" << TNode << ") and " << getProb(ENode) << " (" << ENode << ")"
//                <<  " = " << probabilities.at(node) << std::endl;
//    return;
//    std::cerr << "Compute " << node << " " << nodeCounter++ << std::endl;

    if (cuddIsConstant(node)) {
        coefficients[node]  = 1;
        probabilities[node] = 1.0;
        //std::cerr << "true node" << std::endl;
        return;
    }
    DdNode* TNode = cuddT(node);
    int Eindex, Tindex, index;

    if (cuddIsConstant(TNode))
        Tindex = mgr.ReadSize();
    else
        Tindex = mgr.ReadPerm(TNode->index);
    
    if (mark[node] != mark[TNode]) {
        computeProbRecur(TNode);
    }
    DdNode* ENode = cuddE(node);
    
    if (cuddIsConstant(Cudd_Regular(ENode))) {
        Eindex = mgr.ReadSize();
    }
    else {
        Eindex = mgr.ReadPerm(Cudd_Regular(ENode)->index);
    }
    index = mgr.ReadPerm(node->index);
    
    if (Tindex > mgr.ReadSize() + 1 || Eindex > mgr.ReadSize() + 1) {
        std::cerr << "ReadSize " << mgr.ReadSize() << " Tindex " << Tindex << " Eindex " << Eindex << std::endl;
        if (Cudd_IsComplement(ENode)) std::cerr << "E is negated" << std::endl;
        exit(-1);
    }
    if (Tindex < index || Eindex < index) {
        std::cerr << "node->index " << node->index << " Tindex " << Tindex << " Eindex " << Eindex << std::endl;
        if (Cudd_IsComplement(ENode)) std::cerr << "E is negated" << std::endl;
        //std::cerr << "With permutations... ";
        //std::cerr << "node->index " << mgr.ReadPerm(node->index) << " Tindex " << mgr.ReadPerm(Tindex) << " Eindex " << mgr.ReadPerm(Eindex)  << std::endl;

        exit(-1);
    }
    if (mark[node] != mark[Cudd_Regular(ENode)])
        computeProbRecur(Cudd_Regular(ENode));
    
    //std::cerr << "index " << index << " Tindex " << Tindex << " Eindex " << Eindex << std::endl;

    tCoef = coefficients.at(TNode);
    oneTemp = 1.0;
    if (Cudd_IsComplement(ENode)) {
        mpz_mul_2exp(eCoef.get_mpz_t(), oneTemp.get_mpz_t(), mgr.ReadSize()  - Eindex);
        eCoef = eCoef - coefficients.at(Cudd_Regular(ENode));
        //eCoef = (1 << (mgr.ReadSize()  - Eindex)) - coefficients.at(Cudd_Regular(ENode));
    }
    else eCoef = coefficients.at(ENode);
    
    mpz_class tempATcoef, tempAEcoef;
    mpz_mul_2exp(tempATcoef.get_mpz_t(), tCoef.get_mpz_t(), Tindex - index  - 1);
    mpz_mul_2exp(tempAEcoef.get_mpz_t(), eCoef.get_mpz_t(), Eindex - index  - 1);

    mpf_set_z(atCoef.get_mpf_t(), tempATcoef.get_mpz_t());
    mpf_set_z(aeCoef.get_mpf_t(), tempAEcoef.get_mpz_t());

    //atCoef = tCoef * (1 << (Tindex - node->index  - 1));
    //aeCoef = eCoef * (1 << (Eindex - node->index  - 1));
    if (aeCoef < 0) {
        std::cerr << " ecoef " << eCoef << " aeCoef " << aeCoef << std::endl;
        std::cerr << "aeCoef is negative" << std::endl;
        exit(-1);
    }
    //std::cerr << "eCoef " << eCoef << " aeCoef " << aeCoef << std::endl;
    coefficients[node] = atCoef + aeCoef;
    ptemp = atCoef / (atCoef + aeCoef);
    probabilities[node] = mpf_get_d(ptemp.get_mpf_t());
    //std::cerr << " probability atCoef "  << atCoef << " aeCoef " << aeCoef << " " << probabilities[node] << std::endl;
    return;

}


    std::vector<long>    cuddFactory::computeDistribution() {
    mapInfo.clear();
    traverseRecur(theBDD.getNode());
    distrecur(theBDD.getNode());
    std::vector<long> res = *(mapInfo[theBDD.getNode()].dist);
    delete mapInfo[theBDD.getNode()].dist;
    return res;
}

void cuddFactory::traverseRecur(DdNode *n) {
    distInfo& info = mapInfo[n];
    info.mark      = !info.mark;
    info.ref++;
    if (cuddIsConstant(n)) {
        if (info.mark != mapInfo[cuddT(n)].mark)
            traverseRecur(cuddT(n));
        if (info.mark != mapInfo[Cudd_Regular(cuddE(n))].mark)
            traverseRecur(Cudd_Regular(cuddE(n)));
    }
}

void  cuddFactory::distrecur(DdNode* n) {
    distInfo& info = mapInfo[n];
    info.mark      = !info.mark;
    info.tempref++;
    DdNode* low  = Cudd_Regular(n->type.kids.T);
    DdNode* high = Cudd_Regular(n->type.kids.E);
    
    distInfo& lowInfo  = mapInfo[low];
    distInfo& highInfo = mapInfo[high];
    
    if (n->index == mgr.bddZero()) {
        info.dist = new std::vector<long>();
        return;
    }
    if (n->index == mgr.bddOne()) {
        info.dist    = new std::vector<long>(1);
        (*info.dist)[0] = 1;
        return;
    }
    if (info.mark != lowInfo.mark)
        traverseRecur(low);
    
    if (info.mark != highInfo.mark)
        traverseRecur(high);
    
    
    // Length of the index jump from one node to the descendant. At least one.
    int diffalse = low->index  - n->index;
    int diftrue  = high->index - n->index;
 
    std::vector<long> lowdif(diffalse+lowInfo.dist->size());
    std::vector<long> highdif(diftrue+highInfo.dist->size());
    
    // Now we compute the distribution of the "else" (low) path.
    for(int a = 0; a < lowdif.size(); a++) {
        long factor   = 1;
        long multiply = diffalse-1;
        long divide   = 1;
        // We fill lowdif one component at a time
        for(int b = 0; b < diffalse && b <= a; b++) {
            // How many different paths (along omitted nodes) are there from
            // the node to the "else" descendant passing through b omitted
            //  "then" arcs? Answer ((diffalse-1) over b)
            // factor is (diffalse-1) over b
            lowdif[a] += factor * (*lowInfo.dist)[a-b];
            factor *= multiply-- / divide++;
        }
    }
    
    
    // Now we compute the distribution of the "then" (high) path. (symmetrical)
    // a starts at 1 because highdif[0] has to be 0.
    for(int a = 0; a < highdif.size(); a++) {
        // We fill highdif one component at a time
        long factor   = 1;
        long multiply = diftrue-1;
        long divide   = 1;
        for(int b = 0; b < diftrue && b <= a; b++) {
            // How many different paths (along omitted nodes) are there from
            // the node to the "then" descendant passing through b omitted
            //  "then" arcs? Answer (diftrue-1) over b
            // factor is (diftrue-1) over b
            
            highdif[a] += factor * (*highInfo.dist)[a-b];
            factor *= multiply-- / divide++;
        }
    }
    // Now we combine both;
    // Size of the distribution is any of the lowdif or highdif
    // it's the same number
    info.dist = new std::vector<long>(lowdif.size());
    
    for(int l = 0; l < info.dist->size(); l++) {
        // We add the false branch distribution
        if (l < lowdif.size())
            (*info.dist)[l] += lowdif[l];
        
        // In the true branch we always add feature n
        if (l-1 < highdif.size())
            // info.dist is highdif shifted to the right 1 position
            (*info.dist)[l] += highdif[l-1];
    }
    
    lowInfo.tempref++;
    highInfo.tempref++;
    
    if (lowInfo.tempref == lowInfo.ref)
        delete lowInfo.dist;
    
    if (highInfo.tempref == highInfo.ref)
        delete highInfo.dist;
}

void   cuddFactory::reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod) {
    bool trivaluated = false;
    int start, length = 0;
    if (ss.size() > 1) {
        start = positions[ss.front()].first;
        for(std::string& s : ss) {
            if (positions.at(s).second == 0) {
                trivaluated = true;
                length++;
            }
            else
                length += 2;
        }
        //std::cerr << "num vars " << numVars << std::endl;
        //std::cerr << "Big group: index " << start << " len " << length << std::endl;
        mgr.SetTree(Mtr_InitGroupTree(start, length));
        for(const std::string& s : ss) {
            //std::cerr << "Blocking variable " << std::setw(60) << s
            //<< "pos first " << std::setw(5) << positions[s].first
            //<< "pos second " << std::setw(5) << positions[s].second << std::endl;
            if (positions[s].second !=  0)
                mgr.MakeTreeNodeFix(positions[s].first, 2, MTR_FIXED);
        }
        for(MenuBlock *m : menus) {
            int xmin = std::numeric_limits<int>::max();
            int length = 0;
            for(const std::string& s : m->getContents())
                if (positions.find(s) != positions.end()){
                    length++;
                    if (positions.at(s).first < xmin)
                        xmin = positions.at(s).first;
                }
            std::cerr << "Small group begin " << xmin << " length " << length << std::endl;
            //mgr.MakeTreeNodeFix(xmin, m->getLength(), MTR_DEFAULT);
            mgr.MakeTreeNode(xmin, m->getLength(), MTR_DEFAULT);
        }
        
        mgr.SetSiftMaxVar(ss.size());
        mgr.SetSiftMaxSwap(std::numeric_limits<int>::max());
        Cudd_ReorderingType crt;
        if (rmethod == "")
            crt = CUDD_REORDER_SIFT;
        else {
            if (reorderMap.find(rmethod) != reorderMap.end())
                crt = reorderMap.at(rmethod);
        }
        mgr.ReduceHeap(crt, 0);
    }
};

void    cuddFactory::shuffle(const std::vector<std::string>& extOrder) {
    int intOrder[numVars], cont = 0;
    std::set<int> check;
    //std::cerr << std::endl << "Starting shuffle" << std::endl;
    for(const std::string s : extOrder)
        if (positions[s].second == 0) {
            //std::cerr << std::setw(80) << s << " -> " << positions[s].first << " cont " << cont << std::endl;
            intOrder[cont++] = positions[s].first;
            check.insert(positions[s].first);
        }
        else {
            //std::cerr << std::setw(80) << s << " -> " << positions[s].first << std::endl;
            //std::cerr << std::setw(80) << s << " -> " << positions[s].second << std::endl;
            check.insert(positions[s].first);
            check.insert(positions[s].second);

            intOrder[cont++] = positions[s].first;
            intOrder[cont++] = positions[s].second;
        }
    
    if (cont != numVars || numVars != check.size()) {
        std::cerr << "Not enough variables for shuffling: cont"
                  << cont << " numVars " << numVars << " check size " << check.size() <<  std::endl;
        exit(-1);
    }
    //std::cerr << std::endl << "Calling shuffle... ";
    mgr.ShuffleHeap(intOrder);
    //std::cerr << "done" << std::flush << std::endl;
};

void cuddFactory::changeUp(int pos, int len, int ind, int lenhigh, int* perm) {
    for(int l = lenhigh-1; l >= 0; l--)
        perm[pos-lenhigh+len+l] = perm[pos-lenhigh+l];
    
    for(int l = 0; l < len; l++)
        perm[pos-lenhigh+l] = ind+l;
}

void cuddFactory::changeDown(int pos, int len, int ind, int lenlow, int* perm) {
    for(int l = 0; l < lenlow; l++)
        perm[pos+l] = perm[pos+len+l];
    
    for(int l = 0; l < len; l++)
        perm[pos+lenlow+l] = ind+l;
}

void cuddFactory::reduce(const std::string& slow, const std::string& shigh) {
    int order[numVars+1], lenlow, lenhigh, lenother, inhigh, inlow, poslow, poshigh, oldnodes, nodes;
    bool keeplow, keephigh;
    keeplow = keephigh = true;
    for(int x = 0; x < numVars; x++) {
        order[x] = mgr.ReadInvPerm(x);
    }
    oldnodes        = theBDD.nodeCount();
    inlow           = positions.at(slow).first;
    inhigh          = positions.at(shigh).first;
    std::cerr << slow << " index " << inlow << " " << shigh << " index " << inhigh << std::endl;
    if (positions[slow].second == 0)
        lenlow = 1;
    else
        lenlow = 2;
    
    if (positions[shigh].second == 0)
        lenhigh = 1;
    else
        lenhigh = 2;
    
    poslow  = mgr.ReadPerm(inlow);
    poshigh = mgr.ReadPerm(inhigh);
  
    while (poslow+1 < poshigh && (keeplow || keephigh)) {
           if (keeplow) {
            try {
            if (positions.at(inVars[mgr.ReadInvPerm(poslow+lenlow)]).second == 0)
               lenother = 1;
            else
                lenother = 2;
            changeDown(poslow, lenlow, inlow, lenother, order);
            }
            catch(std::exception e) {
                std::cerr << "Error looking for key  " << std::setw(50) << inVars[mgr.ReadInvPerm(poslow+lenlow)] << std::endl;
                exit(-1);
            }

            poslow += lenother;
            mgr.ShuffleHeap(order);
            nodes    = theBDD.nodeCount();
            if (nodes > oldnodes) {
                keeplow = false;
                     changeUp(poslow, lenlow, inlow, lenother, order);
                mgr.ShuffleHeap(order);
               poslow -= lenother;
            }
        }
        oldnodes = nodes;
        if (keephigh && poslow+1 < poshigh) {
             if (positions.find(inVars[mgr.ReadInvPerm(poshigh-1)]) != positions.end())
                lenother = 1;
            else
                lenother = 2;
            
            changeUp(poshigh, lenhigh, inhigh, lenother, order);
            poshigh -= lenother;
            mgr.ShuffleHeap(order);
            nodes    = theBDD.nodeCount();
            if (nodes > oldnodes) {
                keephigh = false;
                changeDown(poshigh, lenhigh, inhigh, lenother, order);
                mgr.ShuffleHeap(order);
                poshigh += lenother;
            }
        }
        oldnodes = nodes;
       

    }

    return;
}
int     cuddFactory::getNumComponents() const { return numComponents; }
void    cuddFactory::computeLevelNodes()  {
//    numComponents = 0;
//    startComponent.clear();
//    startComponent.resize(numVars);
    levelnodes.clear();
    levelnodes.resize(numVars, 0);
//    std::cerr << "numVars is " << numVars << std::endl;
    giveOrder(); // To have int2extLevel correct
//    for(int x = 0; x < numVars; x++) {
//        int temp = theBDD.getLevelNodes(x);
//        //astd::cerr << "level = " << std::setw(5) << x << " nodes " << theBDD.getLevelNodes(x) << std::endl;
//        levelnodes[int2extLevel[x]] += temp;
//        if (temp == 1) {
//            numComponents++;
//            startComponent[int2extLevel[x]] = true;
//        }
//    }
//        
//    return;
    int totalNodes = 1;
    std::vector<DdNode *> stack;
    std::set<DdNode *>    processed;
    levelnodes.clear();
    levelnodes.resize(numVars);
    stack.push_back(Cudd_Regular(theBDD.getNode()));
    while (not stack.empty()) {
        DdNode* x = stack.back();
        stack.pop_back();
        if ((x->index != CUDD_CONST_INDEX) && (processed.find(x) == processed.end())) {
            totalNodes++;
            processed.insert(x);
            int l = mgr.ReadPerm(x->index);
 //           std::cerr << "x " << x << " index " << x->index
 //                   << "internal level " << l << " external level " << int2extLevel[l]
 //           << " levelnodes at internal level ";
 //           std::cerr << levelnodes.at(int2extLevel[l]) << std::endl;
            levelnodes.at(int2extLevel[l])++;
            DdNode *T, *E;
            T = Cudd_Regular(x->type.kids.T);
            E = Cudd_Regular(x->type.kids.E);
            if (T != NULL) {
                stack.push_back(T);
            }
            if (E != NULL) {
                stack.push_back(E);
            }
        }
    }
    if (Cudd_DagSize(theBDD.getNode()) != totalNodes) {
        std::cerr << "dagsize " << Cudd_DagSize(theBDD.getNode()) << " total nodes "
                << totalNodes << std::endl;
        exit(-1);
    }
//    for(int i = 0; i < levelnodes.size(); i++)
//        if (levelnodes[i] > 0)
//            std::cerr << "Level " << std::setw(4) << i << " levenodes "
//            << std::setw(9) << levelnodes[i] << std::endl;

}

int    cuddFactory::getLevelNodes(int level) const {
    //return levelnodes[level];
    //std::cerr << "level " << level << " maps to " << int2extLevel[level] << std::endl;
    //std::cerr << "levelnodes " << theBDD.getLevelNodes(int2extLevel[level]) << std::endl;
    //return theBDD.getLevelNodes(int2extLevel[level]);
    if (level != int2extLevel[level]) {
        std::cerr << "Anomaly: level " << level << " int2extLevel[" << level << "] " << int2extLevel[level] << std::endl;
        exit(-1);
    }
    return levelnodes[int2extLevel[level]];
	
}

void   cuddFactory::setUpVariable(const std::string& var) {
        int pos = mgr.ReadPerm(positions[var].first);
        moduleImplies(var);
        if (pos < reorderMin) {
            reorderMin = pos;
            minVar     = var;
        }
        if (pos > reorderMax) {
            reorderMax = pos;
            maxVar     = var;
        }
 }

void   cuddFactory::printBDD() {
};

void cuddFactory::printBDD(std::string bddName) {
	std::cout << storage[bddName] << std::endl;
}


//void cuddFactory::init(bool verbose) {
//    Cudd_SetSiftMaxSwap(mgr.getManager(), std::numeric_limits<int>::max());
    //mgr.SetMaxGrowth(1000000);
//}

void cuddFactory::init(bool verbose, int nodenum, int cachesize) {
	if (nodenum != 0 && cachesize != 0) {
		Cudd_Init(0, 0, nodenum, cachesize, 0);
	}
    Cudd_SetSiftMaxSwap(mgr.getManager(), std::numeric_limits<int>::max());
    //mgr.SetMaxGrowth(1000000);
}

void   cuddFactory::reorder(std::string reorderMethod) {
	if ((reorderMethod.compare("") == 0) || (reorderMethod.compare("none") == 0)) {
        reorderMethod = "CUDD_REORDER_NONE";
	} else if (reorderMethod.compare("sift") == 0) {
    	reorderMethod = "CUDD_REORDER_GROUP_SIFT";
	} else if (reorderMethod.compare("window2") == 0) {
        reorderMethod = "CUDD_REORDER_WINDOW2_CONV";
	} else if (reorderMethod.compare("window3") == 0) {
        reorderMethod = "CUDD_REORDER_WINDOW3_CONV";
	} else if (reorderMethod.compare("random") == 0) {
        reorderMethod = "CUDD_REORDER_RANDOM";
	}

    reorderMin = 0;
    reorderMax = numVars - 1;
    if (reorderMin != reorderMax) {
        initblocks();
        //std::cerr << "AddMenuBlock " << minVar << " " << maxVar << " "
        //          << reorderMin << " and " << reorderMax << " dif " << reorderMax - reorderMin << std::endl;
        int x = reorderMin;
        int y;
        while (x <= reorderMax) {
            y = mgr.ReadInvPerm(x);
			if (y < inVars.size()) {
	            addblock(inVars.at(y));
	            if (positions[inVars.at(y)].second == 0) x++;
    	        else x += 2;
			} else {
				x++;
			}
        }
    }
    mgr.SetSiftMaxVar(numVars);
    //mgr.SetTimeLimit(10000);
    if (reorderMethod == "") {
        mgr.ReduceHeap(CUDD_REORDER_GROUP_SIFT, 0);
    }
    else
        if (reorderMap.find(reorderMethod) != reorderMap.end()) {
            mgr.ReduceHeap(reorderMap[reorderMethod], 0);
        }
        else {
            std::cerr << "Unknown reorder method for CUDD: " << reorderMethod << std::endl;
            exit(-1);
        }
 
    reorderMin = std::numeric_limits<int>::max();
    reorderMax = -1;
    minVar = maxVar = "";
}


void         cuddFactory::initblocks() {
    mgr.SetTree(Mtr_InitGroupTree(mgr.ReadInvPerm(0), numVars - 1));
    return;
}

std::pair<int, int> cuddFactory::findSmallestBlock(int pos, int pos2) {
    int min = -1;
    std::pair<int, int> block(0,0);
    for(std::set<std::pair<int, int> >::iterator its = currentBlocks.begin();
        its != currentBlocks.end(); its++)
        if (pos >= its->first && pos2 <= its->second &&
            presentBlocks.find(*its) == presentBlocks.end() &&
            its->second - its->first < min) {
            min = its->second - its->first;
            block = *its;
        }
    return block;
}


int   cuddFactory::addblock(std::string var) {
    int start = positions[var].first;

    
    if (positions[var].second ==  0) {
//        if (mgr.MakeTreeNode(start, 1, MTR_DEFAULT) == NULL) {
//            std::cerr   << "There was an error setting block for boolean variable "
//                        << var << std::endl;
//            exit(-1);
//        }
    }
    else {
        if (mgr.MakeTreeNode(start, 2, MTR_FIXED) == NULL) {
            std::cerr   << "There was an error setting block for trivalued variable "
                        << var << " with index " << start << " and pos " << mgr.ReadPerm(start) << std::endl;
            Mtr_PrintGroups(mgr.ReadTree(), 0);
            exit(-1);
        }
    }
     return 0;
}

int   cuddFactory::addMenublock(MenuBlock *m)                    {
    std::string first = m->getFirst();
    std::string last  = m->getLast();
    int start = positions[first].first;
    int finish;
    int type;
    if (m->getType() == "choice")
        type = MTR_FIXED;
    else
        type = MTR_DEFAULT;
    
    if (positions[last].second ==  0) {
        finish = mgr.ReadPerm(positions[last].first);
        if (mgr.MakeTreeNode(start,
                                finish -  mgr.ReadPerm(positions[first].first) + 1,
                             type) == NULL) {
            std::cerr   << "1 There was an error setting a menublock between "
                        << first << " and " << last
                        << " position first "     << mgr.ReadPerm(positions[first].first)
                        << " position last "      << finish
                        << " description '"        << m->getDescription() << "'"
                        << " and length "   << finish - start + 1
                        << std::endl;
            Mtr_PrintGroups(mgr.ReadTree(), 0);
            exit(-1);
        }
    }
    else {
        finish =  mgr.ReadPerm(positions[last].second);
        if (mgr.MakeTreeNode(start,
                            finish -  mgr.ReadPerm(positions[first].first) + 1,
                             type) == NULL) {
            std::cerr   << "2 There was an error setting a menublock between "
                        << first << " and " << last
                        << " position "     << start
                        << " and length "   << finish - start + 1
                        << std::endl;
            exit(-1);
        }
    }
    return 0;
}


void cuddFactory::printVars() {
    //for(const std::pair<std::string, std::pair<cudd::BDD, cudd::BDD> >& p : vars)
    //    std::cerr   << std::setw(50) << p.first << " "
    //                << std::setw(6) << p.second.first << " " << p.second.second << std::endl;
}
std::vector<std::string> cuddFactory::giveOrder() {
    std::vector<std::string> res;
    int2extLevel.clear();
    int x = 0;
    int z = 0;
       while (x < numVars) {
        int y = mgr.ReadInvPerm(x);
        std::string var = inVars.at(y);
        res.push_back(var);
           //std::cerr << "x " << x << " invperm " << mgr.ReadInvPerm(y)
           //         << " inVars " << var << " z " << z << std::endl;
        int2extLevel.push_back(z);
           try {
               if (positions.at(var).second !=  0) {
                   x += 2;
                   int2extLevel.push_back(z);
               }
               else x++;
           }
           catch(std::exception e) {
               std::cerr << "Error in reordering. x = " << std::setw(5) << x
                        << std::setw(5) << y << " var " << std::setw(50) << var
                        << " " << mgr.ReadInvPerm(x+1) << std::setw(50) << inVars.at(mgr.ReadInvPerm(x+1))
                        << std::endl;
                 Mtr_PrintGroups(mgr.ReadTree(), 0);
               for(int x = 0; x < numVars; x++) {
                   std::cerr << "x " << x << " invperm " << mgr.ReadInvPerm(x)
                   << " inVars " << inVars.at(mgr.ReadInvPerm(x)) << std::endl;
               }
                   
                 exit(-1);
           }
           z++;
    }

    return res;
}


void   cuddFactory::namevar(std::string name, int index) {
    mgr.pushVariableName(name);
}

void   cuddFactory::newVar(std::string var, std::string type) {
	countVar++;
    vars[var] = std::pair<BDD, BDD>(mgr.bddVar(), mgr.bddZero());
    positions[var] = std::pair<int, int>(numVars, 0);
    inVars.push_back(var);
    mgr.pushVariableName(var);
    numVars++;
}

void   cuddFactory::newVarExpression(std::string var, std::string type, synExp* mySynExp) {
	countVar++;
    vars[var] = std::pair<BDD, BDD>(process(mySynExp), mgr.bddZero());
    positions[var] = std::pair<int, int>(numVars, 0);
    inVars.push_back(var);
    mgr.pushVariableName(var);
    numVars++;
}

void    cuddFactory::restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive) {}

void cuddFactory::moduleImplies(std::string var) {
     if (setImplies.find(var) == setImplies.end()) {
        setImplies.insert(var);
        if (vars.at(var).second != mgr.bddZero()) {
            // theBDD &= (!vars[var].first | vars[var].second) &
            //(!vars[var].first | vars.at("MODULES").first);
            theBDD &= (!vars[var].first | vars[var].second);
          }
    }
}


void cuddFactory::apply(std::string s, synExp * exp) {
    //if (storage.find(s) != storage.end())
    //    storage[s] = storage[s] & process(exp);
    //else 
	storage[s] = process(exp);
}

void cuddFactory::apply(synExp * s) {
    theBDD = theBDD & process(s);
}

cudd::BDD  cuddFactory::getVar(std::string var) {
    if (vars.find(var) != vars.end())
        return vars.at(var).first;
    if (storage.find(var) != storage.end())
        return storage[var];
        
    std::cerr << "Looking for "<< var << " not a previously created variable or bdd" << std::endl;
    exit(-1);
}

SemanticsCudd cuddFactory::getVarBDD(std::string var) {
    if (storage.find(var) != storage.end()) {
		cudd::BDD foundBDD = storage[var];
        return SemanticsCudd(foundBDD, mgr.bddZero());
	}
        
    std::cerr << "Looking for "<< var << " not a previously created variable or bdd" << std::endl;
    exit(-1);
	
}

SemanticsCudd cuddFactory::getTrueBDD() {
	return SemanticsCudd(mgr.bddOne(), mgr.bddZero());
}

SemanticsCudd cuddFactory::getFalseBDD() {
	return SemanticsCudd(mgr.bddZero(), mgr.bddZero());
}

const cudd::BDD cuddFactory::process(synExp* exp) {
    switch (exp->get_type()) {
        case synExp_Const : {
            if (exp == synTrue)   {
            return mgr.bddOne();
            }
            if (exp == synFalse) {
                return mgr.bddZero();
            }
            if (exp == synModule) {
                return mgr.bddOne();
            }
            std::cerr << "Error in const synExp " << exp << std::endl; break;
        }
        case synExp_Symbol : {
            std::string var = exp->getSymbol();
            return getVar(var);
        }
        case synExp_Compound :
        {
            switch (exp->get_op()) {
                case synNot        : {
                    return !process(exp->first()) ;
                }
                case synAnd        : {
                    return process(exp->first()) & process(exp->second());
                }
                case synOr         : {
                    return process(exp->first()) | process(exp->second());
                }
                case synImplies    : {
                    return  !process(exp->first()) | process(exp->second());
                }
                case synIfThenElse : {
                    cudd::BDD condPart = process(exp->first());
                    cudd::BDD thenPart = process(exp->second());
                    cudd::BDD elsePart = process(exp->third());
                    
                    return condPart.Ite(thenPart, elsePart);
                }
                case synEqual      : {
                    const BDD x  = process(exp->first());
                    const BDD y  = process(exp->second());
                    return ((x & y) | (!x & !y)); }
            }
        }
        case synExp_String : { std::cerr << "What is a string doing here? *" << exp->get_string() << "* "
            << std::endl; return mgr.bddZero(); }
            
    }
    return mgr.bddZero();
}

const int cuddFactory::nodecount(std::string s) {
    return Cudd_DagSize(storage[s].getNode());
}

const int cuddFactory::nodecount() {
    return Cudd_DagSize(theBDD.getNode());
}

void    cuddFactory::readBDD(const std::string id, const std::string& base, const std::string &suffix) {
    std::string filename;
    if (suffix == "")
        filename = base + ".ddmp";
    else
        filename = base + "-" + suffix + ".ddmp";
    
    // The variables must be created in the manager before we read the file
    // So we scan the header for the info we need and create them in case
    // they don't exist yet.
    
    if (mgr.ReadSize() == 0) {
        int nvars;
        // First we read the header
        std::ifstream dump(filename);
        std::string word;
        for(;;) {
            if (dump >> word) {
                if (word == ".nvars") {
                    dump >> nvars;
                    std::cerr << "There are " << nvars << " variables" << std::endl;
                }
                else if (word == ".orderedvarnames")
                    for(int x = 0; x < nvars; x++) {
                        dump >> word;
                        //std::cerr << "Creating variable " << word << std::endl;
                        newVar(word, "boolean");
                    }
                else if (word == ".nodes")
                    break;
                //else std::cerr << "Ignoring " << word << std::endl;
            }
        }
        dump.close();
    }
    
    Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHPERMIDS;
    //Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHIDS;
    //Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHNAMES;
    FILE *fp = fopen(filename.c_str(), "r");
    //char *pvars[MAXVAR];
    //for(int i = 0; i < MAXVAR; i++) pvars[i] = (char*) malloc(sizeof(char)*1000);
    //int  auxids[MAXVAR];
    //int  composeids[MAXVAR];
    //char **ppvars = pvars;
    int mode = DDDMP_MODE_TEXT;
    storage[id] = cudd::BDD(mgr,
                       Dddmp_cuddBddLoad(mgr.getManager(), varMatchMode, NULL, NULL, NULL, mode, (char*)filename.c_str(), fp));
}

void    cuddFactory::readBDD(const std::string& base, const std::string &suffix) {
    std::string filename;
    if (suffix == "")
        filename = base + ".ddmp";
    else
        filename = base + "-" + suffix + ".ddmp";
    
    // The variables must be created in the manager before we read the file
    // So we scan the header for the info we need and create them in case
    // they don't exist yet.
    
    if (mgr.ReadSize() == 0) {
        int nvars;
        // First we read the header
        std::ifstream dump(filename);
        std::string word;
        for(;;) {
            if (dump >> word) {
                if (word == ".nvars") {
                    dump >> nvars;
                    std::cerr << "There are " << nvars << " variables" << std::endl;
                }
                else if (word == ".orderedvarnames")
                    for(int x = 0; x < nvars; x++) {
                        dump >> word;
                        //std::cerr << "Creating variable " << word << std::endl;
                        newVar(word, "boolean");
                    }
                else if (word == ".nodes")
                    break;
                //else std::cerr << "Ignoring " << word << std::endl;
            }
        }
        dump.close();
    }
    
    Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHPERMIDS;
    //Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHIDS;
    //Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHNAMES;
    FILE *fp = fopen(filename.c_str(), "r");
    //char *pvars[MAXVAR];
    //for(int i = 0; i < MAXVAR; i++) pvars[i] = (char*) malloc(sizeof(char)*1000);
    //int  auxids[MAXVAR];
    //int  composeids[MAXVAR];
    //char **ppvars = pvars;
    int mode = DDDMP_MODE_TEXT;
    theBDD = cudd::BDD(mgr,
                       Dddmp_cuddBddLoad(mgr.getManager(), varMatchMode, NULL, NULL, NULL, mode, (char*)filename.c_str(), fp));
}

void    cuddFactory::saveBDD(const std::string& base, const std::string &suffix)  {
    char *pvars[inVars.size()];
    int  mode = DDDMP_MODE_TEXT;
    Dddmp_VarInfoType varInfoType = DDDMP_VARPERMIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARNAMES;
    char **ppvars = pvars;
    for(const std::string& s : inVars)
        *ppvars++ = (char*)s.c_str();
    *ppvars = NULL;
    std::string filename;
    if (suffix == "")
        filename = base + ".ddmp";
    else
        filename = base + "-" + suffix + ".ddmp";
    std::cerr << "Writing to file " << filename << std::endl;
    FILE *fp = fopen(filename.c_str(), "w");
    Dddmp_cuddBddStore(mgr.getManager(), (char*)base.c_str(), theBDD.getNode(), pvars, NULL, mode, varInfoType, (char*)filename.c_str(), fp);
    return;
}

void    cuddFactory::saveBDD(const std::string id, const std::string& base, const std::string &suffix)          {
    char *pvars[inVars.size()];
    int  mode = DDDMP_MODE_TEXT;
    Dddmp_VarInfoType varInfoType = DDDMP_VARPERMIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARNAMES;
    char **ppvars = pvars;
    for(const std::string& s : inVars)
        *ppvars++ = (char*)s.c_str();
    *ppvars = NULL;
    std::string filename;
    if (suffix == "")
        filename = base + ".ddmp";
    else
        filename = base + "-" + suffix + ".ddmp";
    std::cerr << "Writing to file " << filename << std::endl;
    FILE *fp = fopen(filename.c_str(), "w");
    Dddmp_cuddBddStore(mgr.getManager(), (char*)base.c_str(), storage[id].getNode(), pvars, NULL, mode, varInfoType, (char*)filename.c_str(), fp);
    return;
}

bool     cuddFactory::sameBDD(std::string s1, std::string s2) {
    return storage[s1] == storage[s2];
}

void cuddFactory::done() {
	Cudd_Quit(NULL);
}

void cuddFactory::setCacheRatio(double x) {
	std::cout << "Function not available for this BDD manager" << std::endl;
}

void cuddFactory::setMaxNodeNum(int size) {
	std::cout << "Function not available for this BDD manager" << std::endl;
}

std::vector<int> cuddFactory::traverse() {
    std::vector<int> output;
    return output;
}
