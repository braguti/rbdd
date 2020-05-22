//
//  cuddAdapter.cpp
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//



#include "cuddAdapter.hpp"
#include "cuddAlgo.hpp"
#include "cuddInt.h"

int cuddAdapter::MAXVAR = 100000;

using namespace cudd;

DdNode* cuddAdapter::getBDD(std::string bddName) {
    if (bddName == "" || storage.find(bddName) == storage.end()) {
        return theBDD.getNode();
    } else {
        return storage[bddName].getNode();
    }
}

DdNode* cuddAdapter::getZero() {
    return mgr.bddZero().getNode();
}

DdNode* cuddAdapter::getOne() {
    return mgr.bddOne().getNode();
}

int cuddAdapter::getNumComponents() {
    return pcomp->getNumComponents();
}

int cuddAdapter::getMaxComponent() {
    return pcomp->getMaxLength();
}

int cuddAdapter::getComponent(int x) {
    return pcomp->getComponent(x);
}

int cuddAdapter::getCompSize(int x) {
    return pcomp->getCompSize(x);
}

int cuddAdapter::getNumVars() {
    return numVars;
}

void cuddAdapter::checkComponents() {
    std::vector<int>  levelNodes = getLevelNodes(this);
    
    for(std::pair<int, int> lcomp : pcomp->listComponents()) {
        if (lcomp.second > 1) {
            int tnodes = 0;
            int headNodes = levelNodes[lcomp.first];
            
            for(int i = lcomp.first; i < lcomp.first+lcomp.second; i++)
                tnodes += levelNodes[i];
            
            if (headNodes != 0 && headNodes != 1) {
                Rcpp::Rcerr << "Component start with " << headNodes << " nodes in component with start "
                << lcomp.first << " length " << lcomp.second << " and " << tnodes
                << " nodes " << std::endl;
                Rcpp::Rcerr << "Component members: ";
                for(int x = lcomp.first; x < lcomp.first+lcomp.second; x++) {
                    int  pos = mgr.ReadPerm(mgr.ReadInvPerm(x));
                    Rcpp::Rcerr << inVars[mgr.ReadInvPerm(x)] << "(" << mgr.ReadInvPerm(x) << ") pos "
                             << pos << " ";
                }
                Rcpp::Rcerr << std::endl;
                for(int i = 0; i < lcomp.first+lcomp.second; i++)
                    if (levelNodes[i] > 0) {
                    Rcpp::Rcerr << i << " " << std::setw(40) << inVars[mgr.ReadInvPerm(i)] << "("
                    << mgr.ReadInvPerm(i) << "): " << levelNodes[i] << std::endl;
                    }
              
                Rcpp::Rcerr << "Components:" << std::endl;
                for(std::pair<int, int> qq : pcomp->listComponents())
                    if (qq.second > 1){
                        Rcpp::Rcerr << "start " << qq.first << " length " << qq.second << std::endl;
                        Rcpp::stop("start %d length %d", qq.first, qq.second);//exit(-1);
                    }
            }
        }
    }
    //    Rcpp::Rcerr << std::endl;
    //    for(int i = 0; i < numVars; i++)
    //        if (levelNodes[i] > 0)
    //            Rcpp::Rcerr << i << " " << std::setw(40) << inVars[mgr.ReadInvPerm(i)] << "("
    //            << mgr.ReadInvPerm(i) << "): " << levelNodes[i] << std::endl;
}

void cuddAdapter::reorder() {
    std::set<std::pair<int, int> > ts;
    ts = pcomp->listComponents();
    Rcpp::Rcout << "Reordering. There are " << ts.size() << " components that need reordering" << std::endl;
    checkComponents();
    levelnodes = getLevelNodes(this);
    for(std::pair<int, int> lcomp : ts) {
        if (lcomp.second > 1) {
            int tnodes = 0;
            for(int i = lcomp.first; i < lcomp.first+lcomp.second; i++)
                tnodes += levelnodes[i];
            int headNodes = levelnodes[lcomp.first];
            if (headNodes != 0 && headNodes != 1) {
                Rcpp::Rcerr << "Component start with " << headNodes << " nodes in component with start "
                << lcomp.first << " length " << lcomp.second << " and " << tnodes
                << " nodes " << std::endl;
                //exit(-1);
                Rcpp::stop("Component start with %d nodes in component with start %d length %d and %d nodes ", headNodes, lcomp.first, lcomp.second, tnodes);
            }
            
            //if (tnodes > 5) {
            if (true)
                Rcpp::Rcout  << "Reordering component start "
                << std::setw(4) << lcomp.first
                << " size "  << std::setw(4) << lcomp.second
                << " nodes " << std::setw(9) << tnodes
                << "..." << std::flush;
                
                std::vector<std::string> compVars;
                int c = lcomp.first;
                for(int w = 0; w < lcomp.second; w++) {
                    compVars.push_back(inVars[mgr.ReadInvPerm(c++)]);
		}
                
                std::vector<MenuBlock*> theBlocks;
            
                reorderComponent(compVars, theBlocks, "");
                Rcpp::Rcout << "done" << std::endl;
                // This is only necessary for the first component
                pcomp->setUnchanged(mgr.ReadInvPerm(lcomp.first));
            }
        }


    pcomp->changeOrder(pos2var());
    pcomp->sync();
}

int cuddAdapter::getLevel(DdNode const* node) {
    if (Cudd_IsConstant(Cudd_Regular(node))) {
        return (mgr.ReadSize());
    }
    else {
        return (mgr.ReadPerm(Cudd_Regular(node)->index));
    }
}

int cuddAdapter::getLevel(DdNode* node) {
    if (Cudd_IsConstant(Cudd_Regular(node))) {
        return (mgr.ReadSize());
    }
    else {
        return (mgr.ReadPerm(Cudd_Regular(node)->index));
    }
}

void cuddAdapter::existentialQ(const std::set<std::string>& v) {
    cudd::BDD conjunction = mgr.bddOne();
    for(std::string s : v)
        conjunction = conjunction & getVarBDD(s);
    
    theBDD = theBDD.ExistAbstract(conjunction);
}

void cuddAdapter::destroyInternal(const std::set<std::string>& v) {
    theBDD.destroySubtables(v.size());
    numVars = numVars - v.size();
    std::vector<std::string> auxVars;
    for(unsigned int x = 0; x < numVars; x++)
        auxVars.push_back(inVars[x]);
    
    inVars.clear();
    inVars = auxVars;
}

cuddAdapter::cuddAdapter(double cacheMultiplier) : mgr(0,0,CUDD_UNIQUE_SLOTS, cacheMultiplier*CUDD_CACHE_SLOTS, 0) {
    //countVar = 0;
    //funcTrav = "";
    //bddTrav = "";
    withComponents = false;
    pcomp = new OneComponent();
    init(0, 0, "");
}

cuddAdapter::cuddAdapter() : mgr(0,0,CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0) {
    //countVar = 0;
    //funcTrav = "";
    //bddTrav = "";
    withComponents = false;
    pcomp = new OneComponent();
    init(0, 0, "");
}

void cuddAdapter::useComponents() {
    delete pcomp;
    pcomp = new MultiComponents();
    withComponents = true;
}

void cuddAdapter::useComponents(std::vector<int> var2pos, std::vector<int> pos2var) {
    delete pcomp;
    pcomp = new MultiComponents(var2pos, pos2var);
    withComponents = true;
}

void cuddAdapter::init(int nodenum, int cachesize, std::string bddName = "") {
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
    if (bddName != "") {
        storage[bddName] = mgr.bddOne();
    }
    reorderMin = std::numeric_limits<int>::max();
    reorderMax = -1;
    minVar = maxVar = "";
    mgr.AutodynDisable();
    Cudd_SetSiftMaxSwap(mgr.getManager(), std::numeric_limits<int>::max());
    //mgr.SetMaxGrowth(1000000);
    //mgr.DisableGarbageCollection();
}

void cuddAdapter::setValue(std::string s , synExp* e) {
    if (e == synTrue) {
        apply(new synSymbol(s));
        return;
    }
    if (e == synFalse) {
        apply(makeNot(new synSymbol(s)));
        return;
    }
}

std::set<std::string> cuddAdapter::checkOneSolution() {
    DdNode* n = Cudd_Regular(theBDD.getNode());
    return checkOneSolutionRec(n, 0);
}

std::set<std::string> cuddAdapter::checkOneSolutionRec(DdNode* node, int level) {
    std::set<std::string> res, resT, resE;
    int index;
    DdNode *TNode, *ENode;
    TNode = cuddT(node);
    ENode = Cudd_Regular(cuddE(node));
    mark[node] = !mark[node];
    index = mgr.ReadPerm(node->index);
    // If there is a level jump...
    if (index != level)
        for(int x = level; x < index; x++)
            res.insert(inVars[x]);
    
    if (!cuddIsConstant(TNode) && !cuddIsConstant(ENode))
        res.insert(inVars[index]);
    
    if (mark[node] != mark[TNode] && !cuddIsConstant(TNode))
        resT = checkOneSolutionRec(TNode, index+1);
    if (mark[node] != mark[ENode] && !cuddIsConstant(ENode))
        resE = checkOneSolutionRec(ENode, index+1);
    
    if (cuddIsConstant(TNode) && cuddIsConstant(ENode) && index != mgr.ReadSize()-1)
        for(int x = index+1; x < mgr.ReadSize(); x++)
            res.insert(inVars[x]);
    
    res.insert(resT.begin(), resT.end());
    res.insert(resE.begin(), resE.end());
    
    return res;
}

void cuddAdapter::reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod) {
    int startPos, length = 0;
    startPos = mgr.ReadPerm(indices[ss.front()].first);
    for(std::string& s : ss) {
        if (indices.at(s).second == 0) {
            length++;
        }
        else {
            length += 2;
        }
    }
    
    int groupStart = indices[ss.front()].first;
    int groupLength = length;
    mgr.SetTree(Mtr_InitGroupTree(groupStart, length));
    for(const std::string& s : ss) {
        if (indices[s].second !=  0)
            mgr.MakeTreeNodeFix(indices[s].first, 2, MTR_FIXED);
    }
    for(MenuBlock *m : theMenuBlocks) {
        int xmin = std::numeric_limits<int>::max();
        int length = 0;
        for(const std::string& s : m->getContents())
            if (indices.find(s) != indices.end()){
                length++;
                if (indices.at(s).first < xmin)
                    xmin = indices.at(s).first;
            }
        if (xmin >= startPos && xmin+length <= startPos + groupLength) {
            if (m->getDescription() == "XOR")
                mgr.MakeTreeNodeFix(xmin, m->getLength(), MTR_FIXED);
            else
                mgr.MakeTreeNodeFix(xmin, m->getLength(), MTR_DEFAULT);
        }
    }
    
    mgr.SetSiftMaxVar(ss.size());
    mgr.SetSiftMaxSwap(std::numeric_limits<int>::max());
    Cudd_ReorderingType crt = CUDD_REORDER_GROUP_SIFT;
    /**if (rmethod == "")
        crt = CUDD_REORDER_GROUP_SIFT;
        //crt = CUDD_REORDER_SIFT;
    else {
        if (reorderMap.find(rmethod) != reorderMap.end())
            crt = reorderMap.at(rmethod);
    }*/
	if (rmethod != "" && reorderMap.find(rmethod) != reorderMap.end()) {
		crt = reorderMap.at(rmethod);
	}
	
    mgr.ReduceHeapMinMax(startPos, startPos+length-1, crt, 0);
}

void    cuddAdapter::shuffle(const std::vector<std::string>& extOrder) {
    // If there are no variables...
    if (extOrder.empty())
        return;
    if (extOrder.size() != numVars) {
        std::ostringstream ost;
        ost << "Shuffle size " << extOrder.size() << " != " << numVars << " numVars" << std::endl;
        if (numVars == 0) ost << "Maybe you are reading a dddmp file in an old format (without variable names)";
        throw std::logic_error(ost.str());
    }
    int intOrder[numVars], cont = 0;
    std::set<int> check;
    for(const std::string& s : extOrder)
        if (indices[s].second == 0) {
            intOrder[cont++] = indices[s].first;
            check.insert(indices[s].first);
        }
        else {
            check.insert(indices[s].first);
            check.insert(indices[s].second);
            
            intOrder[cont++] = indices[s].first;
            intOrder[cont++] = indices[s].second;
        }
    if (static_cast<unsigned int>(cont) != numVars || numVars != check.size()) {
        Rcpp::stop("Not enough variables for shuffling: cont %d numVars %ud check size %uz", cont, numVars, check.size());//exit(-1);
    }
    for(int a = 0; static_cast<unsigned int>(a) < numVars; a++)
        if (check.find(a) == check.end()) {
            Rcpp::stop("%d not found in check set", a);//exit(-1);
        }
    mgr.SetTree(Mtr_InitGroupTree(mgr.ReadInvPerm(0), numVars));
    mgr.ShuffleHeap(intOrder);
}

void cuddAdapter::changeUp(int pos, int len, int ind, int lenhigh, int* perm) {
    for(int l = lenhigh-1; l >= 0; l--)
        perm[pos-lenhigh+len+l] = perm[pos-lenhigh+l];
    
    for(int l = 0; l < len; l++)
        perm[pos-lenhigh+l] = ind+l;
}

void cuddAdapter::changeDown(int pos, int len, int ind, int lenlow, int* perm) {
    for(int l = 0; l < lenlow; l++)
        perm[pos+l] = perm[pos+len+l];
    
    for(int l = 0; l < len; l++)
        perm[pos+lenlow+l] = ind+l;
}

void cuddAdapter::reduce(const std::string& slow, const std::string& shigh) {
    int order[numVars+1], lenlow, lenhigh, lenother, inhigh, inlow, poslow, poshigh, oldnodes, nodes;
    bool keeplow, keephigh;
    keeplow = keephigh = true;
    for(int x = 0; static_cast<unsigned int>(x) < numVars; x++) {
        order[x] = mgr.ReadInvPerm(x);
    }
    oldnodes        = theBDD.nodeCount();
    inlow           = indices.at(slow).first;
    inhigh          = indices.at(shigh).first;
    Rcpp::Rcerr << slow << " index " << inlow << " " << shigh << " index " << inhigh << std::endl;
    if (indices[slow].second == 0)
        lenlow = 1;
    else
        lenlow = 2;
    
    if (indices[shigh].second == 0)
        lenhigh = 1;
    else
        lenhigh = 2;
    
    poslow  = mgr.ReadPerm(inlow);
    poshigh = mgr.ReadPerm(inhigh);
    
    while (poslow+1 < poshigh && (keeplow || keephigh)) {
        if (keeplow) {
            try {
                if (indices.at(inVars[mgr.ReadInvPerm(poslow+lenlow)]).second == 0)
                    lenother = 1;
                else
                    lenother = 2;
                changeDown(poslow, lenlow, inlow, lenother, order);
            }
            catch(std::exception e) {
                Rcpp::stop("Error looking for key %s", inVars[mgr.ReadInvPerm(poslow+lenlow)]);
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
            if (indices.find(inVars[mgr.ReadInvPerm(poshigh-1)]) != indices.end())
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

int cuddAdapter::getNumComponents() const {
    return numComponents;
}

void cuddAdapter::setUpVariable(const std::string& var) {
    int pos = mgr.ReadPerm(indices[var].first);
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

void cuddAdapter::printBDD() {}

int cuddAdapter::varAtPos(int pos) {
    return mgr.ReadInvPerm(pos);
}

void cuddAdapter::reorder(std::string reorderMethod) {
	if (reorderMethod == "" || reorderMethod == "none") {
        reorderMethod = "CUDD_REORDER_NONE";
	} else if (reorderMethod == "sift") {
    	reorderMethod = "CUDD_REORDER_GROUP_SIFT";
	} else if (reorderMethod == "window2") {
        reorderMethod = "CUDD_REORDER_WINDOW2_CONV";
	} else if (reorderMethod == "window3") {
        reorderMethod = "CUDD_REORDER_WINDOW3_CONV";
	} else if (reorderMethod == "random") {
        reorderMethod = "CUDD_REORDER_RANDOM";
	}

    reorderMin = 0;
    reorderMax = numVars - 1;
    if (reorderMin != reorderMax) {
        initblocks();
        int x = reorderMin;
        int y;
        while (x <= reorderMax) {
            y = mgr.ReadInvPerm(x);
            addblock(inVars.at(y));
            if (indices[inVars.at(y)].second == 0) x++;
            else x += 2;
        }
    }
    mgr.SetSiftMaxVar(numVars);
    if (reorderMethod == "") {
        mgr.ReduceHeap(CUDD_REORDER_GROUP_SIFT,  0);
    }
    else
        if (reorderMap.find(reorderMethod) != reorderMap.end()) {
            mgr.ReduceHeap(reorderMap[reorderMethod],  0);
        }
        else {
            Rcpp::stop("Unknown reorder method for CUDD: %s");
        }
    
    reorderMin = std::numeric_limits<int>::max();
    reorderMax = -1;
    minVar = maxVar = "";
    pcomp->changeOrder(pos2var());
}

void cuddAdapter::changeOrder(std::vector<int> pos2var) {
    pcomp->changeOrder(pos2var);
}

std::vector<int> cuddAdapter::pos2var() {
    std::vector<int> res;
    for(unsigned int x = 0; x < numVars; x++)
        res.push_back(mgr.ReadInvPerm(x));
    return res;
}

std::vector<int> cuddAdapter::var2pos() {
    std::vector<int> res;
    for(unsigned int x = 0; x < numVars; x++)
        res.push_back(mgr.ReadPerm(x));
    return res;
}

void cuddAdapter::initblocks() {
    mgr.SetTree(Mtr_InitGroupTree(mgr.ReadInvPerm(0), numVars));
    return;
}

std::pair<int, int> cuddAdapter::findSmallestBlock(int pos, int pos2) {
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
int cuddAdapter::addblock(std::string var) {
    int start = indices[var].first;
    
    
    if (indices[var].second ==  0) {}
    else {
        if (mgr.MakeTreeNode(start, 2, MTR_FIXED) == NULL) {
            Mtr_PrintGroups(mgr.ReadTree(), 0);
            Rcpp::stop("There was an error setting block for trivalued variable %d with index %d and pos %d", var, start, mgr.ReadPerm(start));
        }
    }
    return 0;
}

int cuddAdapter::addMenublock(MenuBlock *m) {
    std::string first = m->getFirst();
    std::string last  = m->getLast();
    int start = indices[first].first;
    int finish;
    int type;
    if (m->getType() == "choice")
        type = MTR_FIXED;
    else
        type = MTR_DEFAULT;
    
    if (indices[last].second ==  0) {
        finish = mgr.ReadPerm(indices[last].first);
        if (mgr.MakeTreeNode(start,
                             finish -  mgr.ReadPerm(indices[first].first) + 1,
                             type) == NULL) {
            Mtr_PrintGroups(mgr.ReadTree(), 0);
            Rcpp::stop("1 There was an error setting a menublock between %s and %s position first %d position last %d description '%s' and length %d", first, last, mgr.ReadPerm(indices[first].first), finish, m->getDescription(), finish - start + 1);
        }
    }
    else {
        finish =  mgr.ReadPerm(indices[last].second);
        if (mgr.MakeTreeNode(start,
                             finish -  mgr.ReadPerm(indices[first].first) + 1,
                             type) == NULL) {
            Rcpp::stop("2 There was an error setting a menublock between %s and %s position %d and length %d", first, last, start, finish - start + 1);
        }
    }
    return 0;
}

void cuddAdapter::printVars() {
    //for(const std::pair<std::string, std::pair<cudd::BDD, cudd::BDD> >& p : vars)
    //    Rcpp::Rcerr   << std::setw(50) << p.first << " "
    //                << std::setw(6) << p.second.first << " " << p.second.second << std::endl;
}

std::vector<std::string> cuddAdapter::giveOrder() {
    std::vector<std::string> res;
    for(unsigned int x = 0; x < numVars; x++)
        res.push_back(inVars.at(mgr.ReadInvPerm(x)));

    return res;
}

void cuddAdapter::namevar(std::string name, int index) {
    mgr.pushVariableName(name);
}

void cuddAdapter::newVar(std::string var, std::string type) {
	//pcomp->newVariable();
    countVar++;
    vars[var] = std::pair<BDD, BDD>(mgr.bddVar(), mgr.bddZero());
    indices[var] = std::pair<int, int>(numVars, 0);
    inVars.push_back(var);
    mgr.pushVariableName(var);
    numVars++;
    this->hasNewVar = true;
}

std::string cuddAdapter::getVarName(int x) {
    return inVars[x];
}

void cuddAdapter::newVar(std::string var, std::string type, int pos) {
    countVar++;
    vars[var] = std::pair<BDD, BDD>(mgr.bddNewVarAtLevel(pos), mgr.bddZero());
    indices[var] = std::pair<int, int>(numVars, 0);
    inVars.push_back(var);
    mgr.pushVariableName(var);
    numVars++;
}

void cuddAdapter::moduleImplies(std::string var) {
     if (setImplies.find(var) == setImplies.end()) {
        setImplies.insert(var);
        if (vars.at(var).second != mgr.bddZero()) {
            // theBDD &= (!vars[var].first | vars[var].second) &
            //(!vars[var].first | vars.at("MODULES").first);
            theBDD &= ((!vars[var].first) | vars[var].second);
          }
    }
}

bool cuddAdapter::apply(std::string s, synExp * exp) {
    if (storage.find(s) != storage.end())
        storage[s] = storage[s] & process(exp);
    else
	    storage[s] = process(exp);
    return pcomp->join(exp, false);
}

bool cuddAdapter::apply(synExp * s) {
    theBDD = theBDD & process(s);
    bool res = pcomp->join(s, false);
    return res;
}

void cuddAdapter::checkOrder() {
    int min = std::numeric_limits<int>::max();
    int max = -1;
    bool err = false;
    std::set<int> check;
    for(int i : pcomp->getOrder()) {
        check.insert(i);
        if (i < min) min = i;
        if (i > max) max = i;
    }
    if (check.size() != numVars) {
        Rcpp::Rcerr << "checkOrder error, check size " << check.size() << " numVars " << numVars << std::endl;
        err = true;
    }
    if (min != 0 || max != (int) (numVars-1)) {
	    Rcpp::Rcerr << "checkOrder, check size " << check.size() << " numVars " << numVars << std::endl;
    	Rcpp::Rcerr << "min is " << min << " and max is " << max << std::endl;
	    for(int i : pcomp->getOrder()) {
            Rcpp::Rcerr << i << " " << std::endl;
        }
	    err = true;
    }
    if (err) {
        Rcpp::stop("Error");//exit(-1);
    }
}

void cuddAdapter::syncOrder(std::string bdd_name = "") {
    int nn;
    if (bdd_name == "")
        nn = nodecount();
    else
        nn = nodecount(bdd_name);
    if (withComponents) {
        checkOrder();
        pcomp->reorder();

        int theOrder[numVars];
        int c = 0;
        
        std::set<int> check;
        for(int i : pcomp->getOrder()) {
            theOrder[c++] = i;
        }
        checkOrder();
        mgr.SetTree(Mtr_InitGroupTree(mgr.ReadInvPerm(0), numVars));
        mgr.ShuffleHeap(theOrder);
        checkComponents();
    }
    int nnow;
    if (bdd_name == "")
        nnow = nodecount();
    else
        nnow = nodecount(bdd_name);
    if (nnow > nn) {
        Rcpp::stop("Before syncing %d nodes, after syncing %d", nn, nnow);
    }

}

cudd::BDD cuddAdapter::getVarBDD(std::string var) {
    if (vars.find(var) != vars.end())
        return vars.at(var).first;
    if (storage.find(var) != storage.end())
        return storage[var];
        
    throw std::logic_error("Looking for "+var+" not a previously created variable or bdd");
}

SemanticsCudd cuddAdapter::getVar(std::string var) {
    if (storage.find(var) != storage.end()) {
		cudd::BDD foundBDD = storage[var];
        return SemanticsCudd(foundBDD, mgr.bddZero());
	}
    
    Rcpp::stop("Looking for %d not a previously created variable or bdd", var);
}

const cudd::BDD cuddAdapter::process(synExp* exp) {
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
            Rcpp::Rcerr << "Error in const synExp " << exp << std::endl; break;
        }
        case synExp_Symbol : {
            std::string var = exp->getSymbol();
            return getVarBDD(var);
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
                    return  (!process(exp->first())) | process(exp->second());
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
                    return ((x & y) | ((!x) & (!y))); }
            }
        }
        case synExp_String : {
            Rcpp::Rcerr << "What is a string doing here? *" << exp->get_string() << "* "
            << std::endl; return mgr.bddZero();
            return mgr.bddZero();
        }
        case synExp_XOR : {
            std::vector<DdNode*> theVector;
            // USE ONLY VAR VARIABLES or CONSTS
            for(synExp* var : exp->get_parms()) {
                if (var->isSymbol())
                    theVector.push_back(process(var).getNode());
                else
                    if (var == synTrue)
                        theVector.push_back(mgr.bddOne().getNode());
                    else
                        if (var == synFalse)
                            theVector.push_back(mgr.bddZero().getNode());
                        else {
                            std::ostringstream ost;
                            ost << "Error, calling XOR with non-symbol argument" << var << std::endl;
                            throw std::logic_error(ost.str());
                        }
            }
            
            return BDD(mgr, mgr.onlyOne(theVector));
        }
            
    }
    return mgr.bddZero();
}

const int cuddAdapter::nodecount(std::string s) {
    return Cudd_DagSize(storage[s].getNode());
}

const int cuddAdapter::nodecount() {
    return Cudd_DagSize(theBDD.getNode());
}

cudd::BDD cuddAdapter::auxReadBDD(const std::string& base, const std::string &suffix) {
    std::string filename;
    if (suffix == "")
        filename = base + ".dddmp";
    else
        filename = base + "-" + suffix + ".dddmp";
    
    // The variables must be created in the manager before we read the file
    // So we scan the header for the info we need and create them in case
    // they don't exist yet.
    
    if (mgr.ReadSize() == 0) {
        int nvars = 0;
        // First we read the header
        std::ifstream dump(filename);
        if (!dump.good()) {
            throw std::invalid_argument("Couldn't open file "+filename);
        }
        
        std::string word;
        std::vector<std::string> shuf;
        for(;;) {
            if (dump >> word) {
                if (word == ".nvars") {
                    dump >> nvars;
                }
                // Now it's not support variable names, it's ALL variable names
                else if (word == ".varnames")
                    for(int x = 0; x < nvars; x++) {
                        dump >> word;
                        newVar(word, "boolean");
                    }
                else if (word == ".orderedvarnames") {
                          for(int x = 0; x < nvars; x++) {
                               dump >> word;
                              shuf.push_back(word);
                          }
                    shuffle(shuf);
                }
                else if (word == ".nodes")
                    break;
            }
        }
        dump.close();
    }
    
    if (pcomp != NULL) delete pcomp;
    pcomp = new OneComponent(numVars);
    Dddmp_VarMatchType  varMatchMode = DDDMP_VAR_MATCHPERMIDS;
    FILE *fp = fopen(filename.c_str(), "r");
    int mode = DDDMP_MODE_TEXT;
    return cudd::BDD(mgr,
                     Dddmp_cuddBddLoad(mgr.getManager(), varMatchMode, NULL, NULL, NULL, mode, (char*)filename.c_str(), fp));
}

void cuddAdapter::readBDD(const std::string id, const std::string& base, const std::string &suffix) {
    storage[id] = auxReadBDD(base, suffix);

}

void cuddAdapter::readBDD(const std::string& base, const std::string &suffix) {
    theBDD = auxReadBDD(base, suffix);
}

void cuddAdapter::saveBDD(const std::string& base, const std::string &suffix)  {
    char *pvars[inVars.size() + 1];
    int  mode = DDDMP_MODE_TEXT;
    Dddmp_VarInfoType varInfoType = DDDMP_VARPERMIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARIDS;
    //Dddmp_VarInfoType varInfoType = DDDMP_VARNAMES;
    char **ppvars = pvars;
    //int cc = 0;
    for(const std::string& s : inVars)
        *ppvars++ = (char*)s.c_str();
    *ppvars = NULL;
    std::string filename;
    if (suffix == "")
        filename = base + ".dddmp";
    else
        filename = base + "-" + suffix + ".dddmp";
    Rcpp::Rcerr << "Writing to file " << filename << std::endl;
    FILE *fp = fopen(filename.c_str(), "w");
    Dddmp_cuddBddStore(mgr.getManager(), (char*)base.c_str(), theBDD.getNode(), pvars, NULL, mode, varInfoType, (char*)filename.c_str(), fp);
    fclose(fp);
    return;
}

void cuddAdapter::saveBDD(const std::string id, const std::string& base, const std::string &suffix)          {
    char *pvars[inVars.size() + 1];
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
        filename = base + ".dddmp";
    else
        filename = base + "-" + suffix + ".dddmp";
    Rcpp::Rcerr << "Writing to file " << filename << std::endl;
    FILE *fp = fopen(filename.c_str(), "w");
    Dddmp_cuddBddStore(mgr.getManager(), (char*)base.c_str(), storage[id].getNode(), pvars, NULL, mode, varInfoType, (char*)filename.c_str(), fp);
    fclose(fp);
    return;
}

bool cuddAdapter::sameBDD(std::string s1, std::string s2) {
    return storage[s1] == storage[s2];
}

void cuddAdapter::setXORBlocks(synExp* s) {
    if (s->isConst())
        return;
    
    if (s->isSymbol())
        return;
    if (s->isNot()) {
        setXORBlocks(s->first());
        return;
    }
    if (s->isAnd() || s->isOr() || s->isImplies() || s->isEqual()) {
        setXORBlocks(s->first());
        setXORBlocks(s->second());
        return;
    }
    if (s->isIf()) {
        setXORBlocks(s->first());
        setXORBlocks(s->second());
        setXORBlocks(s->third());
        return;
    }
    if (s->isXOR()) {
        auxXOR(s);
        return;
    }
    Rcpp::Rcerr << "s is " << s << std::endl;
}

void cuddAdapter::auxXOR(synExp* exp) {
    // Here we create groups for group sifting
    std::set<int> theindices;
    // USE ONLY VAR VARIABLES or CONSTS
    Rcpp::Rcerr << "process XOR " << std::endl;
    for(synExp* var : exp->get_parms()) {
        if (var->isSymbol()) {
            theindices.insert(indices[var->getSymbol()].first);
        }
    }
    bool first = true;
    std::vector<std::string> vnames;
    int lastPos;
    for(int i : theindices) {
        if (first) {
            vnames.push_back(inVars[i]);
            first = false;
            lastPos = mgr.ReadPerm(i);
        }
        else {
            int p =  mgr.ReadPerm(i);
            if (p == lastPos+1) {
                vnames.push_back(inVars[i]);
                lastPos++;
            }
            else {
                createXORBlock(vnames);
                first = true;
                vnames.push_back(inVars[mgr.ReadInvPerm(p)]);
            }
        }
    }
    createXORBlock(vnames);
}

void cuddAdapter::createXORBlock(std::vector<std::string>& vec) {
    if (!vec.empty()) {
        theMenuBlocks.push_back(new MenuBlock(vec.begin(), vec.end(), "XOR", "XOR"));
        vec.clear();
    }
}

void cuddAdapter::newVarExpression(std::string var, synExp* mySynExp) {
	countVar++;
    vars[var] = std::pair<BDD, BDD>(process(mySynExp), mgr.bddZero());
    indices[var] = std::pair<int, int>(numVars, 0);
    inVars.push_back(var);
    mgr.pushVariableName(var);
    //numVars++;
}

void cuddAdapter::done() {
    Cudd_Quit(mgr.getManager());
}

void cuddAdapter::setCacheRatio(double x) {
	Rcpp::Rcout << "Function not available for this BDD manager" << std::endl;
}

void cuddAdapter::setMaxNodeNum(int size) {
	Rcpp::Rcout << "Function not available for this BDD manager" << std::endl;
}

SemanticsCudd cuddAdapter::getTrueBDD() {
	return SemanticsCudd(mgr.bddOne(), mgr.bddZero());
}

SemanticsCudd cuddAdapter::getFalseBDD() {
	return SemanticsCudd(mgr.bddZero(), mgr.bddZero());
}

std::string cuddAdapter::printBDD(std::string bddName) {
	std::stringstream buffer;
	buffer << storage[bddName];
	std::string str = buffer.str();

    if (!this->hasNewVar) {
        return str;
    }

    std::string result = "";
    std::regex r("x\\d");
    std::vector<std::string> tokens = {std::sregex_token_iterator(str.begin(), str.end(), r, -1), std::sregex_token_iterator()};
    std::smatch match;
    std::vector<std::string> varsCudd;
    int i = 1;
    while (std::regex_search(str, match, r)) {
        varsCudd.push_back(getVarName(std::stoi(match.str(0).substr(1))));
        i++;
        str = match.suffix().str();
    }

    for (unsigned int j = 0; j < tokens.size() - 1; j++) {
        result = result + tokens[j] + varsCudd[j];
    }

    if (tokens.size() > 0) {
        result = result +  tokens[tokens.size() - 1];
    }

    return result;
}

void cuddAdapter::restrictBDD(std::string expression, std::string restriction, std::string variableName, bool isPositive) {
    //countVar++;

    BDD bddExpression = vars[restriction].first;
    
    DdNode* node = getBDD(expression);
    if (!isPositive) {
        node = Cudd_Not(node);
    }
    
    DdNode* restrictNode = Cudd_bddRestrict(mgr.getManager(), node, bddExpression.getNode());
    BDD restrictBdd(mgr, restrictNode);

    storage[variableName] = restrictBdd;
}

Rcpp::Function cuddAdapter::getFunction() {
    return this->funcTrav;
}

void cuddAdapter::setFunction(Rcpp::Function funcTrav) {
    this->funcTrav = funcTrav;
}

std::string cuddAdapter::getTraverseBDD() {
    return this->bddTrav;
}

void cuddAdapter::setTraverseBDD(std::string bddName) {
    this->bddTrav = bddName;
}

void cuddAdapter::saveBDDToStorage(std::string s) {
    this->storage[s] = this->theBDD;
}

std::vector<mpf_class> cuddAdapter::computeCommonality(int num, bool verbose, bool fast) {
    if (num <= 1) {
        return computeCommonalitySingle(verbose, this);
    } else {
        return computeCommonalityMP(num, verbose, this, fast);
    }
}

std::vector<mpz_class> cuddAdapter::computeDistribution(int num, bool verbose, bool fast) {
    if (num <= 1) {
        return computeDistributionSingle(verbose, this);
    } else {
        return computeDistributionMP(num, verbose, this, fast);
    }
}

std::vector<std::string> cuddAdapter::getVars(bool xverbose = true) {
    std::vector<std::string> resultVector;

    int i = 0;
    int result = 0;
    while (result != -1) {
        result = Cudd_ReadInvPerm(mgr.getManager(), i);
        if (result != -1 && result != CUDD_CONST_INDEX) {
            std::string name = getVarName(result);
            resultVector.push_back(name);
            if (xverbose) {
                Rcpp::Rcout << "Variable " << i << " -> " << name << std::endl;
            }
        }
        i++;
    }

    return resultVector;
}

bool cuddAdapter::existVar(std::string name) {
    return vars.find(name) != vars.end();
}

std::pair<std::vector<std::string>, std::vector<std::string>> cuddAdapter::getCoreDead(int num, bool verbose, bool fast) {
    if (num <= 1) {
        return getCoreAndDeadSingle(verbose, this);
    } else {
        return getCoreAndDeadMP(num, verbose, this, fast);
    }
}

std::vector<bool> cuddAdapter::uniformRandomSamplingBool(int num, bool verbose, bool fast) {
    if (num <= 1) {
        return uniformRandomSamplingBoolSingle(verbose, this);
    } else {
        return uniformRandomSamplingBoolMP(num, verbose, this, fast);
    }
}

std::string cuddAdapter::uniformRandomSamplingName(int num, bool verbose, bool fast) {
    if (num <= 1) {
        return uniformRandomSamplingNameSingle(verbose, this);
    } else {
        return uniformRandomSamplingNameMP(num, verbose, this, fast);
    }
}

int cuddAdapter::addNode(DdNode* node) {
    int pos = getNodePos(node);
    if (pos == -1) {
        this->nodeList.push_back(node);
        return this->nodeList.size() - 1;
    } else {
        return -1;
    }
}

DdNode* cuddAdapter::getNode(int node) {
    return this->nodeList[node];
}

int cuddAdapter::getNodePos(DdNode* node) {
    bool found = false;
    int i = 0;

    while (!found && static_cast<unsigned int>(i) < this->nodeList.size()) {
        found = this->nodeList[i] == node;
        if (found) {
            return i;
        }
        i++;
    }

    return -1;
}

void cuddAdapter::clearNodeList() {
    this->nodeList.clear();
}

bool cuddAdapter::isNodeZero(int node) {
    DdNode *zero = getZero();
    return this->nodeList[node] == zero;
}

bool cuddAdapter::isNodeOne(int node) {
    DdNode *one = getOne();
    return this->nodeList[node] == one;
}

std::vector<int> cuddAdapter::getChildrenNodes(int node) {
    DdNode *g1, *g0;
    cuddGetBranches(getNode(node), &g1, &g0);
    std::vector<int> result;
    result.push_back(getNodePos(g1));
    result.push_back(getNodePos(g0));

    return result;
}
