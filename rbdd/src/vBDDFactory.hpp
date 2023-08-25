//
//  vBDDFactory.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __myKconf__vBDDFactory__
#define __myKconf__vBDDFactory__

#include "synExp.hpp"
#include <fstream>
#include <vector>
#include "MenuBlock.hpp"

class vBDDFactory {
public:
    virtual         void                setValue(std::string, synExp*)                              = 0;
    virtual         std::vector<long>   computeDistribution()                                       = 0;
    virtual         void                reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod)               = 0;
    virtual         int                 getNumComponents()                              const       = 0;
    virtual         void                shuffle(const std::vector<std::string>& order)              = 0;
    virtual         void                reduce(const std::string& slow, const std::string& shigh)   = 0;
    virtual         int                 getNodes()                                                  = 0;
    virtual         void                computeLevelNodes()                                         = 0;
    virtual         int                 getLevelNodes(int level)     const                          = 0;
    virtual         void                setUpVariable(const std::string& var)                       = 0;
    virtual         void                printBDD()                                                  = 0;
	virtual         void                init(bool verbose, int nodenum, int cachesize)              = 0;
    virtual         void                reorder(std::string reorderMethod)                          = 0;
    virtual         int                 addblock(std::string var)                                   = 0;
    virtual         int                 addMenublock(MenuBlock*)                                    = 0;
    virtual         void                initblocks()                                                = 0;
    virtual         void                namevar(std::string name, int index)                        = 0;
    virtual         std::vector<std::string>    giveOrder()                                         = 0;
    virtual         void                newVar(std::string var, std::string type)   				= 0;
    virtual         void                newVarExpression(std::string var, std::string type, synExp* mySynExp)   				= 0;
    virtual         void                moduleImplies(std::string var)                              = 0;
    virtual         void                apply(synExp * s)                                           = 0;
    virtual         void                apply(std::string s, synExp * exp)                          = 0;
    virtual const   int                 nodecount()                                                 = 0;
    virtual const   int                 nodecount(std::string s)                                    = 0;
	
    virtual         void                saveBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                saveBDD(const std::string id, const std::string& b, const std::string &s)  = 0;

    virtual         void                readBDD(const std::string& b, const std::string &s)         = 0;
    virtual         void                readBDD(const std::string id, const std::string& b, const std::string &s)         = 0;
    virtual         void                computeProbabilities()                                      = 0;
    virtual         std::vector<bool>   generateRandom()                                            = 0;
    virtual         bool                sameBDD(std::string s1, std::string s2)                     = 0;
	
	virtual			void				done()														= 0;
	virtual			void				setCacheRatio(double x)										= 0;
	virtual			void				setMaxNodeNum(int size)										= 0;

	virtual			void 				printBDD(std::string bddName) = 0;
	virtual			void 				restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive) = 0;
    virtual         std::vector<int>      traverse()                                                  = 0;
};

#endif /* defined(__myKconf__vBDDFactory__) */
