//
//  buddyFactory.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __myKconf__buddyFactory__
#define __myKconf__buddyFactory__

#include "SemanticsBuddy.hpp"
#include "vBDDFactory.hpp"
#include "varInfo.hpp"

#include <set>
#include <list>
#include <iomanip>
#include <map>
#include <bdd.h>

#include "mytime.hpp"
#include "gmpxx.h"

using namespace buddy;

class buddyFactory : public vBDDFactory {
    
    public:
                                buddyFactory();
			void                setValue(std::string, synExp*);
            std::vector<long>   computeDistribution(){ return std::vector<long>(); }
			void                reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod);
            void                reduce(const std::string& slow, const std::string& shigh);
            int                 getNodes();
            void                computeLevelNodes();
            void                setUpVariable(const std::string& var);
            void                printBDD();
            void                init(bool verbose, int nodenum, int cachesize);
            void                reorder(std::string reorderMethod);
             int                addblock(std::string x);
            int                 addMenublock(MenuBlock *m);
            void                initblocks();
			std::vector<std::string>    giveOrder();
            void                namevar(std::string name, int index);
            void                newVar(std::string var, std::string type);
            void                newVarExpression(std::string var, std::string type, synExp* mySynExp);
            void                moduleImplies(std::string  var);
            void                apply(synExp * s);
            void                apply(std::string s, synExp * exp);
            void                saveBDD(const std::string& base, const std::string &suffix) ;
			void                saveBDD(const std::string id, const std::string& base, const std::string &suffix)  ;
			
            int                 getLevelNodes(int level)                                        const;
            int                 getNumComponents()                                              const;
            void                shuffle(const std::vector<std::string>& order);
            void                readBDD(const std::string& b, const std::string &s);
            void                readBDD(const std::string id, const std::string& b, const std::string &s);

            void                computeProbabilities();
            std::vector<bool>   generateRandom();
            bool    			sameBDD(std::string s1, std::string s2);

			const   int         nodecount();
			const   int         nodecount(std::string s);
		
			void				done();
			void				setCacheRatio(double x);
			void				setMaxNodeNum(int size);
            SemanticsBuddy      getVarBDD(std::string var);
            SemanticsBuddy      getTrueBDD();
            SemanticsBuddy      getFalseBDD();
void printBDD(std::string bddName);
void restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive);
std::vector<int> traverse();

   private:
            bdd                                                  		theBDD;
            std::map<std::string, bdd>                                  storage;
            std::map<BDD, mpq_class>									Pr, Pr1, Problow, Probhigh;
            std::vector<mpq_class>										marginals;
            double 														avLength;
            int 														alwaysTrue, alwaysFalse, independent;
            SemanticsBuddy                                              getVar(std::string var);
			
            int                                                         totalNodes, numComponents;
            std::vector<bool>                                           startComponent;
            std::vector<int>                                            levelnodes, int2extLevel;
            SemanticsBuddy                                              process(synExp* exp);
            std::map<std::string, std::pair<bdd, bdd> >                 vars;
            std::vector<std::string>                                    inVars;
            std::map<std::string, int>                                  firstMenuBlocks, lastMenuBlocks;
            int                                                         index = 0;
            std::set<std::pair<int, int> >                              currentBlocks;
			
            std::vector<SemanticsBuddy*>                                temporaries;
};

#endif /* defined(__myKconf__buddyFactory__) */
