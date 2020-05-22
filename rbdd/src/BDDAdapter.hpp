//
//  BDDAdapter.hpp
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __BDDAdapter__
#define __BDDAdapter__

#include <fstream>
#include <vector>
#include <gmp.h>
#include <gmpxx.h>

#include "Components.hpp"
#include "synExp.hpp"
#include "MenuBlock.hpp"
#include <cstdlib>

class BDDAdapter {
public:

    virtual         int                 getNumVars()                                                = 0;
    virtual         int                 getNumComponents()                                          = 0;
    virtual         int                 getMaxComponent()                                           = 0;
    virtual         int                 getComponent(int x)                                         = 0;
    virtual         int                 getCompSize(int x)                                          = 0;

    virtual         std::vector<int>    pos2var()                                                   = 0;
    virtual         std::vector<int>    var2pos()                                                   = 0;
    virtual         void                changeOrder(std::vector<int> pos2var)                       = 0;
    virtual         void                useComponents()                                             = 0;
    virtual         void                useComponents(std::vector<int> var2pos, std::vector<int> pos2var)                                             = 0;
    virtual         void                setValue(std::string, synExp*)                              = 0;
    virtual         void                setName(const std::string& name) {};
    virtual         void                reduce(const std::string& slow, const std::string& shigh)   = 0;
    virtual         int                 getNodes()                                                  = 0;
    virtual         void                setUpVariable(const std::string& var)                       = 0;
    virtual         void                printBDD()                                                  = 0;
    virtual         void                init(int nodenum, int cachesize, std::string bddName)                            = 0;
    virtual         void                reorder(std::string reorderMethod)                          = 0;
    virtual         void                reorder()                                                   = 0;
    virtual         void                namevar(std::string name, int index)                        = 0;
    virtual         std::vector<std::string>    giveOrder()                                         = 0;
    virtual         void                newVar(std::string var, std::string type)   				= 0;
    virtual         void                newVar(std::string var, std::string type, int pos)          = 0;
    virtual         void                moduleImplies(std::string var)                              = 0;
    virtual         bool                apply(synExp * s)                                           = 0;
    virtual         bool                apply(std::string s, synExp * exp)                          = 0;
    virtual const   int                 nodecount()                                                 = 0;
    virtual const   int                 nodecount(std::string s)                                    = 0;
    virtual         void                saveBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                saveBDD(const std::string id, const std::string& b, const std::string &s)  = 0;
    virtual         void                readBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                readBDD(const std::string id, const std::string& b, const std::string &s)         = 0;
    virtual         bool                sameBDD(std::string s1, std::string s2)                     = 0;
    virtual         void                existentialQ(const std::set<std::string>& v)                = 0;
    virtual         void                destroyInternal(const std::set<std::string>& v)             = 0;
    virtual         std::set<std::string>   checkOneSolution()                                      = 0;
    virtual         void                syncOrder(std::string bdd_name)                                                 = 0;
    virtual         void                checkOrder()                                                = 0;
    virtual         void                setXORBlocks(synExp* s)                                     = 0;

    virtual         void                newVarExpression(std::string var, synExp* mySynExp)         = 0;
	virtual			void				done()														= 0;
	virtual			void				setCacheRatio(double x)										= 0;
	virtual			void				setMaxNodeNum(int size)										= 0;
	virtual			std::string 		printBDD(std::string bddName)                               = 0;
    virtual         void 				saveBDDToStorage(std::string s)                             = 0;
    virtual         void				restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive) = 0;
	virtual         std::string			getTraverseBDD()                                            = 0;
	virtual         void				setTraverseBDD(std::string bddName)                         = 0;
	virtual         std::vector<mpf_class> computeCommonality(int num, bool verbose, bool fast) = 0;
	virtual         std::vector<mpz_class> computeDistribution(int num, bool verbose, bool fast) = 0;
    virtual         std::string getVarName(int x) = 0;
    virtual         std::vector<std::string> getVars(bool xverbose) = 0;
    virtual         bool existVar(std::string name) = 0;
	virtual         std::pair<std::vector<std::string>, std::vector<std::string>> getCoreDead(int num, bool verbose, bool fast) = 0;
	virtual         std::vector<bool>	uniformRandomSamplingBool(int num, bool verbose, bool fast) = 0;
	virtual         std::string			uniformRandomSamplingName(int num, bool verbose, bool fast) = 0;

protected:
	std::string bddTrav;// = "";
};

#endif /* defined(__BDDAdapter__) */
