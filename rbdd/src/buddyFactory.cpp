//
//  buddyFactory.cpp
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "buddyFactory.hpp"

using namespace buddy;


buddyFactory::buddyFactory() {  theBDD = bddtrue;
                                unsigned int temp = (unsigned int) get_cpu_time() % 1000000;
                                //cout << "Initializing random seed " << temp << endl;
                                srandom(temp);  }

void    buddyFactory::setValue(std::string s , synExp* e) {
        if (e == synTrue) {
            apply(new synSymbol(s));
            return;
        }
        if (e == synFalse) {
            apply(makeNot(new synSymbol(s)));
            return;
        }
}
								
void    buddyFactory::reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod) {};
void    buddyFactory::shuffle(const std::vector<std::string>& order) {};

void    buddyFactory::printBDD()  {
   //         std::cout << theBDD << std::endl;
         };

void buddyFactory::printBDD(std::string bddName) {
	std::cout << storage[bddName] << std::endl;
}

void   buddyFactory::setUpVariable(const std::string& var) {
    int max = -1;
    int min = std::numeric_limits<int>::max();
    std::string minVar, maxVar;
        int pos = bdd_var2level(firstMenuBlocks[var]);
        moduleImplies(var);
        //std::cerr << "Addblock " << var << " position " << pos << std::endl;
        addblock(var);
        if (pos < min) {
            min    = pos;
            minVar = var;
        }
        if (pos > max) {
            max    = pos;
            maxVar = var;
        }
    //std::cerr << "MinVar " << minVar << " min " << min << " max " << max << " maxVar " << maxVar << std::endl;
    //if (minVar != maxVar) {
        //std::cerr << "AddMenuBlock " << minVar << " " << maxVar << std::endl;
    //    addMenublock(minVar, maxVar);
    //}
}

int                 buddyFactory::getNumComponents() const { return numComponents; }
void                buddyFactory::reduce(const std::string& slow, const std::string& shigh) {
    return;
}
int      buddyFactory::getNodes() { return totalNodes; }

void     buddyFactory::computeLevelNodes() {
    numComponents = 0;
    startComponent.clear();
    startComponent.resize(index);
    totalNodes = 0;
    std::vector<bdd> stack;
    std::set<int>    processed;
    levelnodes.clear();
    levelnodes.resize(index);
    stack.push_back(theBDD);
    while (not stack.empty()) {
        bdd x = stack.back();
        stack.pop_back();
        if (x != bddtrue && x != bddfalse && (processed.find(x.giveInt()) == processed.end())) {
            totalNodes++;
            processed.insert(x.giveInt());
            int l = bdd_var2level(bdd_var(x));
            levelnodes[int2extLevel[l]]++;
            stack.push_back(bdd_high(x));
            stack.push_back(bdd_low(x));
        }
    }
   
    for(int x = 0; x < index; x++) {
        int temp = levelnodes[x];
        if (temp == 1) {
            numComponents++;
            startComponent[int2extLevel[x]] = true;
        }
    }

    //std::cerr << "stack empty" << std::endl;
}
int     buddyFactory::getLevelNodes(int level) const {
    return levelnodes[level];
}
void  buddyFactory::initblocks() {
//    int myfirst, mylast;
    //bdd_clrvarblocks();
//    // We clear all the blocks and then reapply only the menublocks
//    for(std::set<std::pair<int, int> >::iterator its = currentBlocks.begin();
//        its != currentBlocks.end(); its++) {
//        std::cout << "Level: " << its->first << " and " << its->second << std::endl;
//        myfirst = bdd_level2var(its->first);
//        mylast = bdd_level2var(its->second);
//        if (myfirst <= mylast) {
//            std::cout << "1 Adding clrblocks " << myfirst
//                << " " << mylast << std::endl;
//                bdd_intaddvarblock(myfirst,
//                         mylast,
//                           BDD_REORDER_FREE);
//        }
//        else {
//            std::cout << "2 Adding clrblocks " << mylast
//            << " " << myfirst << std::endl;
//            bdd_intaddvarblock(mylast,
//                               myfirst,
//                               BDD_REORDER_FREE);
//        }
//    }
}

//void buddyFactory::init(bool verbose)    {
    //buddy:bdd_init(3E8, 5E7);
//	if (buddy::bdd_isrunning()) {
//		bdd_done();
//	}
//	bdd_init(1E4, 1E4);
//	bdd_setmaxincrease(1E5);
//    bdd_setcacheratio(1);
    //bdd_setvarnum(1);
//    bdd_reorder_verbose(0);
    //bdd_autoreorder(BDD_REORDER_SIFT);
//    if (verbose)
//        bdd_reorder_verbose(2);
//    else
//        bdd_reorder_verbose(0);
//}

void buddyFactory::init(bool verbose, int nodenum, int cachesize)    {
    //buddy:bdd_init(3E8, 5E7);
	if (buddy::bdd_isrunning()) {
		bdd_done();
	}
	if (nodenum != 0 && cachesize != 0) {
		bdd_init(nodenum, cachesize);
	} else {
		bdd_init(1E4, 1E4);
	}
	bdd_setmaxincrease(1E5);
    bdd_setcacheratio(1);
    //bdd_setvarnum(1);
    bdd_reorder_verbose(0);
    //bdd_autoreorder(BDD_REORDER_SIFT);
    if (verbose)
        bdd_reorder_verbose(2);
    else
        bdd_reorder_verbose(0);
}

void   buddyFactory::reorder(std::string reorderMethod) {
	bdd_reorder_verbose(0);
	if ((reorderMethod.compare("") == 0) || (reorderMethod.compare("none") == 0)) {
		bdd_reorder(BDD_REORDER_NONE);
	} else if (reorderMethod.compare("sift") == 0) {
    	bdd_reorder(BDD_REORDER_SIFTITE);
	} else if (reorderMethod.compare("window2") == 0) {
    	bdd_reorder(BDD_REORDER_WIN2ITE);
	} else if (reorderMethod.compare("window3") == 0) {
    	bdd_reorder(BDD_REORDER_WIN3ITE);
	} else if (reorderMethod.compare("random") == 0) {
    	bdd_reorder(BDD_REORDER_RANDOM);
	}
}


int   buddyFactory::addblock(std::string var)                      {
    //std::cerr << "addblock " << var << " " << vars[var].first << " " << vars[var].second << std::endl;
    if (vars[var].second == bddfalse)
        return bdd_addvarblock(vars[var].first, BDD_REORDER_FREE);
    else
        return bdd_addvarblock(vars[var].first & vars[var].second,
                               BDD_REORDER_FIXED);
}
int    buddyFactory::addMenublock(MenuBlock *m) {
    std::string first = m->getFirst();
    std::string last  = m->getLast();
    int type;
    if (m->getType() == "choice")
        type = BDD_REORDER_FIXED;
    else
        type = BDD_REORDER_FREE;
            
    //std::cerr << "menublock " << first << " " << last << " " << firstMenuBlocks[first] << " "
    //          << lastMenuBlocks[last] <<std::endl;
    
    return bdd_intaddvarblock(firstMenuBlocks[first],
                                lastMenuBlocks[last],
                              type);
    
}


std::vector<std::string> buddyFactory::giveOrder() {
    int2extLevel.clear();
    //std::cerr << "In giveOrder" << std::endl;
    std::vector<std::string> res;
    int z = 0;
    int x = 0;
    while (x < bdd_varnum()) {
        int y = bdd_level2var(x);
        std::string var = inVars.at(y);
        int2extLevel.push_back(z);
        //std::cerr << "x=" << std::setw(5) << x << std::setw(5) << y << " var " << var << std::endl;
        res.push_back(var);
        if (vars.at(var).second !=  bddfalse) {
            int2extLevel.push_back(z);
            x += 2;
        }
        else x++;
        z++;
    }
    
    return res;
}
void    buddyFactory::newVar(std::string var, std::string type) {
    if (type == "boolean") {
        firstMenuBlocks[var] = index;
        lastMenuBlocks[var]  = index;

        bdd_setvarnum(index+1);
        vars[var] = std::pair<bdd, bdd>(bdd_ithvar(index), bddfalse);
        inVars.push_back(var);
		
bdd_addvarblock(bdd_ithvar(index), 1);

        index++;


    }
    else
        if (type == "tristate") {
            bdd_setvarnum(index+2);
            firstMenuBlocks[var] = index;
            lastMenuBlocks[var]  = index+1;
            inVars.push_back(var);
            inVars.push_back("NO_VARIABLE");
            vars[var] = std::pair<bdd, bdd>(bdd_ithvar(index), bdd_ithvar(index+1));
bdd_addvarblock(bdd_ithvar(index), 1);            

index += 2;
			
        }
}

void    buddyFactory::newVarExpression(std::string var, std::string type, synExp* mySynExp) {
    firstMenuBlocks[var] = index;
    lastMenuBlocks[var]  = index;

    bdd_setvarnum(index+1);

	SemanticsBuddy sm = process(mySynExp);

    vars[var] = std::pair<bdd, bdd>(sm.main(), bddfalse);
    inVars.push_back(var);

bdd_addvarblock(bdd_ithvar(index), 1);
    index++;
}

void    buddyFactory::restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive) {
    firstMenuBlocks[variableName] = index;
    lastMenuBlocks[variableName]  = index;

	bdd_setvarnum(index+1);
	bdd bddExpression = vars[expression].first;
	bdd bddVar = vars[varToRestrict].first;
	
	if (isPositive) {
    	vars[variableName] = std::pair<bdd, bdd>(bdd_restrict(bddExpression, bddVar), bddfalse);
	} else {
		vars[variableName] = std::pair<bdd, bdd>(bdd_restrict(bddExpression, !bddVar), bddfalse);
	}
    inVars.push_back(variableName);
    index++;
}

void  buddyFactory::moduleImplies(std::string var) {
      if (vars[var].second != bddfalse)
        theBDD &=   (!vars[var].first | vars[var].second) &
                    (!vars[var].first | vars.at("MODULES").first);
        //theBDD &= (!(vars[var].first & vars["MODULES"].first) | vars[var].second);
}

SemanticsBuddy    buddyFactory::getVar(std::string var) {
    if (vars.find(var) != vars.end()) {
        return SemanticsBuddy(vars[var].first, vars[var].second);
	}
    
    if (storage.find(var) != storage.end()) {
        return SemanticsBuddy(storage[var], bddfalse);
	}
}

SemanticsBuddy buddyFactory::getVarBDD(std::string var) {
    if (storage.find(var) != storage.end()) {
        return SemanticsBuddy(storage[var], bddfalse);
	}
}


SemanticsBuddy buddyFactory::getTrueBDD() {
	return SemanticsBuddy(bddtrue, bddfalse);
}

SemanticsBuddy buddyFactory::getFalseBDD() {
	return SemanticsBuddy(bddfalse, bddfalse);
}

void   buddyFactory::namevar(std::string name, int index) {
    
}

void buddyFactory::apply(std::string s, synExp * exp) {
    SemanticsBuddy sm = process(exp);
    //if (storage.find(s) != storage.end())
    //    storage[s] &= sm.main();
    //else
        storage[s] = sm.main();
}

void buddyFactory::apply(synExp *s) {
    SemanticsBuddy sm = process(s);
     theBDD &= sm.main();
}

SemanticsBuddy buddyFactory::process(synExp* exp) {
    //std::cerr << "process " << exp << std::endl;
    switch (exp->get_type()) {
        case synExp_Const : {
            if (exp == synTrue)   {
                return  SemanticsBuddy(bddtrue, bddfalse);
            }
            if (exp == synFalse) {
                return SemanticsBuddy(bddfalse, bddfalse);
            }
            if (exp == synModule) {
                return SemanticsBuddy(bddtrue,  bddtrue);
            }
            std::cerr << "Error in const synExp " << exp << std::endl; break;
        }
        case synExp_Symbol : {
            std::string var = exp->getSymbol();
            std::string t   = symbolTable::getSymbol(var)->getVartype();
            //if (t == "boolean" || t == "tristate") {
                return  SemanticsBuddy(getVar(var));
            //}
            //else {
            //    std::cerr << "Variable of type " << t << std::endl;
            //    return SemanticsBuddy(bddtrue, bddfalse);
            //}
        }
        case synExp_Compound :
        {
            switch (exp->get_op()) {
                case synNot        : {
                    SemanticsBuddy c = process(exp->first());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    return  SemanticsBuddy(!x | xm, xm);
                    }
                case synAnd        : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return  SemanticsBuddy(x & y, x & bdd_ite(y, xm | ym, bddfalse));
                }
                case synOr         : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return SemanticsBuddy(x | y, (xm & !y) | (ym & !x) | xm & ym);
                }
                case synImplies    : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return  SemanticsBuddy(bdd_ite(y,
                                                    bdd_ite(ym, !x | xm, bddtrue),
                                                    !x & !xm),
                                            bddfalse);
                }
                case synIfThenElse : {
                    SemanticsBuddy condPart = process(exp->first());
                    SemanticsBuddy thenPart = process(exp->second());
                    SemanticsBuddy elsePart = process(exp->third());
                    
                    return SemanticsBuddy(bdd_ite(condPart.main(), thenPart.main(), elsePart.main()),
                                           bdd_ite(condPart.main(), thenPart.module(), elsePart.module()));
                }
                case synEqual      : {
                    SemanticsBuddy c = process(exp->first());
                    SemanticsBuddy d = process(exp->second());
                    const bdd x  = c.main();
                    const bdd xm = c.module();
                    const bdd y  = d.main();
                    const bdd ym = d.module();
                    return SemanticsBuddy( ((x & y) | (!x & !y)) & ((xm & ym) | (!xm & !ym)),
                                           bddfalse);
                }
            }
        }
        case synExp_String : {
            std::cerr   << "What is a string doing here? *" << exp->get_string() << "* "
                        << std::endl;
            return SemanticsBuddy();
        }
            
    }
    return SemanticsBuddy(bddfalse, bddfalse);
}

std::vector<bool> buddyFactory::generateRandom() {
    std::vector<bool> result(bdd_varnum());
    BDD node = theBDD.id();
    int nextHigh, nextLow;
    float randNum;
    while (node != 1) {
        randNum = (float)random()/(float)RAND_MAX;
        if (randNum > Problow[node]/Pr1[node]) {
            result[bdd_var(node)] = true;
            if (bdd_high(node) != 0 && bdd_high(node) != 1)
                nextHigh = bdd_var(bdd_high(node));
            else
                nextHigh = bdd_varnum() + 1;
            for (int i = bdd_var(node)+1; i < nextHigh; i++) {
                randNum = (float)random()/(float)RAND_MAX;
                if (randNum > 0.5)
                    result[i] = true;
                else
                    result[i] = false;
            }
            node = bdd_high(node);
        }
        else {
            result[bdd_var(node)] = false;
            if (bdd_low(node) != 0 && bdd_low(node) != 1)
                 nextLow = bdd_var(bdd_low(node));
            else
                 nextLow = bdd_varnum() + 1;
            for (int i = bdd_var(node)+1; i < nextLow; i++) {
                randNum = (float)random()/(float)RAND_MAX;
                if (randNum > 0.5)
                    result[i] = true;
                else
                    result[i] = false;
            }
            node = bdd_low(node);
        }
    }
    return result;
}

void buddyFactory::computeProbabilities() {
    int parent, low, high, levelLow, levelHigh;
    mpq_class flowlow, flowhigh, temphalf;
    std::map<BDD, bool> mark;
    std::list<BDD> stack;
    independent = 0;
    marginals.resize(bdd_varnum());
    std::vector<bool> varAppears(bdd_varnum());
    mark[0] = true;
    mark[1] = true;
    BDD root = theBDD.id();
    Pr[root] = 1.0;
    std::vector<std::pair<int, int> > theHeap;
    theHeap.push_back(std::pair<int, int>(-1, root));
    push_heap(theHeap.begin(), theHeap.end());
    while (! theHeap.empty()) {
        parent = theHeap.front().second;
        varAppears[bdd_var(parent)] = true;
        stack.push_front(parent);
        pop_heap(theHeap.begin(), theHeap.end());
        theHeap.pop_back();
        low       = bdd_low(parent);
        high      = bdd_high(parent);
        mpq_div_2exp(temphalf.get_mpq_t(), Pr[parent].get_mpq_t(), 1);
        Pr[low]       += temphalf;
        Pr[high]      += temphalf;
        
        if (!mark[low]) {
            theHeap.push_back(std::pair<int, int>(-bdd_var2level(bdd_var(low)),low));
            mark[low] = true;
            push_heap(theHeap.begin(), theHeap.end());
        }
        
        if (!mark[high]) {
            theHeap.push_back(std::pair<int, int>(-bdd_var2level(bdd_var(high)), high));
            mark[high] = true;
            push_heap(theHeap.begin(), theHeap.end());
        }
    }
    
    Pr1[0] = 0;
    Pr1[1] = 1;
    mpq_class temp, templow, temphigh;
    for(std::list<BDD>::iterator itl = stack.begin(); itl != stack.end(); itl++) {
        low  = bdd_low(*itl);
        high = bdd_high(*itl);
        
        mpq_div_2exp(temp.get_mpq_t(), Pr[*itl].get_mpq_t(), 1);
        flowlow  = Pr1[low]/Pr[low]*temp;
        flowhigh = Pr1[high]/Pr[high]*temp;
        Problow[*itl]  = flowlow;
        Probhigh[*itl] = flowhigh;
        //flowlow  = (Pr1[low]*Pr[*itl]) / (2*Pr[low]);
        //flowhigh = (Pr1[high]*Pr[*itl]) / (2*Pr[high]);
        
        Pr1[*itl] = flowlow + flowhigh;
        marginals[bdd_var2level(bdd_var(*itl))] += flowhigh;
        
        if (low != 0 && low != 1)
            levelLow = bdd_var2level(bdd_var(low));
        else
            levelLow = bdd_varnum();
        
        if (high != 0 && high != 1)
            levelHigh = bdd_var2level(bdd_var(high));
        else
            levelHigh = bdd_varnum();
        
        int templevel = bdd_var2level(bdd_var(*itl));
        if (flowlow > 0 && flowhigh > 0) {
            avLength += (levelLow -  templevel)*mpq_get_d(flowlow.get_mpq_t());
            avLength += (levelHigh - templevel)*mpq_get_d(flowhigh.get_mpq_t());
        }
        
        
        mpq_div_2exp(templow.get_mpq_t(), flowlow.get_mpq_t(), 1);
        
        for(int i = templevel+1; i < levelLow; i++)
            marginals[i] += templow;
        
        mpq_div_2exp(temphigh.get_mpq_t(), flowhigh.get_mpq_t(), 1);
        for(int i = templevel+1; i < levelHigh; i++)
            marginals[i] += temphigh;
    }
    
    for(int j = 0; j < marginals.size(); j++)
        if (marginals[j] == 1) alwaysTrue++;
        else if (marginals[j] == 0) alwaysFalse++;
    
    for(int j = 0; j < bdd_varnum(); j++)
        if (varAppears[j] == false) independent++;
}

const int buddyFactory::nodecount(std::string s) {
    return bdd_nodecount(storage[s]);
}

const int buddyFactory::nodecount() {
    return bdd_nodecount(theBDD);
}

void    buddyFactory::saveBDD(const std::string id, const std::string& base, const std::string &suffix)          {
    std::string  varfilename;
    if (suffix == "")
        varfilename = base + ".buddy";
    else
        varfilename = base + "-" + suffix + ".buddy";
    bdd_save(fopen(varfilename.c_str(), "w"), storage[id]);
}

void    buddyFactory::saveBDD(const std::string& base, const std::string &suffix) {
    std::string  varfilename;
    if (suffix == "")
        varfilename = base + ".buddy";
    else
        varfilename = base + "-" + suffix + ".buddy";
	bdd_save(fopen(varfilename.c_str(), "w"), theBDD);
}

void    buddyFactory::readBDD(const std::string id, const std::string& base, const std::string &suffix) {
    std::cerr << "Unimplemented" << std::endl;
}

void    buddyFactory::readBDD(const std::string& base, const std::string &suffix) {
    std::cerr << "Unimplemented" << std::endl;
}

bool    buddyFactory::sameBDD(std::string s1, std::string s2) {
    return storage[s1] == storage[s2];
}



void buddyFactory::done() {
	bdd_done();
}

void buddyFactory::setCacheRatio(double x) {
	bdd_setcacheratio(x);
}

void buddyFactory::setMaxNodeNum(int size) {
	bdd_setmaxnodenum(size);
}

std::vector<int> buddyFactory::traverse() {
    std::vector<int> output(3 * bdd_getnodenum());
    
    FILE* file;
    file = fopen("traverse_fprintall.txt", "w");
    bdd_fprintall(file);
    fclose(file);

    std::ifstream infile("traverse_fprintall.txt");
    std::string currentNode;
    int i = 0;

    while (std::getline(infile, currentNode)) {
        while (currentNode[0] != ']') {
			currentNode = currentNode.substr(1);
		}

        if (currentNode[0] == ']') {
            currentNode = currentNode.substr(1);
        }
        
        while (currentNode[0] == ' ') {
			currentNode = currentNode.substr(1);
		}

        std::string indexString = "";
        while (currentNode[0] >= '0' && currentNode[0] <= '9') {
			indexString += currentNode[0];
			currentNode = currentNode.substr(1);
		}

		output[i] = std::stoi(indexString);
        i++;

        currentNode = currentNode.substr(1);
        while (currentNode[0] == ' ') {
			currentNode = currentNode.substr(1);
		}

        std::string lowString = "";
        while (currentNode[0] >= '0' && currentNode[0] <= '9') {
			lowString += currentNode[0];
			currentNode = currentNode.substr(1);
		}
		output[i] = std::stoi(lowString);
        i++;

        while (currentNode[0] == ' ') {
			currentNode = currentNode.substr(1);
		}

        std::string highString = "";
        while (currentNode[0] >= '0' && currentNode[0] <= '9') {
			highString += currentNode[0];
			currentNode = currentNode.substr(1);
		}
		output[i] = std::stoi(highString);
        i++;
    }
	
	infile.close();

    remove("traverse_fprintall.txt");

    return output;
}
