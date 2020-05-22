//
//  buddyAdapter.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __buddyAdapter__
#define __buddyAdapter__

#include "SemanticsBuddy.hpp"
#include "BDDAdapter.hpp"
#include "varInfo.hpp"

#include <set>
#include <list>
#include <iomanip>
#include <map>
#include <bdd.h>

//#include "gmpxx.h"
#include <Rcpp.h>

using namespace buddy;

class buddyAdapter : public BDDAdapter {
    
    public:
                                buddyAdapter();
            int                 getNumVars();
            int                 getNumComponents();
            int                 getMaxComponent();
            int                 getComponent(int x);
            int                 getCompSize(int x);
            std::vector<int>    pos2var();
            std::vector<int>    var2pos();
            void                changeOrder(std::vector<int> pos2var);

            bdd                 getBDD(std::string bddName);
            bdd                 getZero();
            bdd                 getOne();
            void                useComponents();
            void                useComponents(std::vector<int> var2pos, std::vector<int> pos2var) {};
			void                setValue(std::string, synExp*);
			void                reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod);
            void                reduce(const std::string& slow, const std::string& shigh);
            int                 getNodes();
            void                setUpVariable(const std::string& var);
            void                printBDD();
            void                init(int nodenum, int cachesize, std::string bddName);
            void                reorder(std::string reorderMethod);
            int                 addblock(std::string x);
            int                 addMenublock(MenuBlock *m);
            void                initblocks();
			std::vector<std::string>    giveOrder();
            void                namevar(std::string name, int index);
            void                newVar(std::string var, std::string type);
            void                newVar(std::string var, std::string type, int pos){};
            void                moduleImplies(std::string  var);
            bool                apply(synExp * s);
            bool                apply(std::string s, synExp * exp);
            void                saveBDD(const std::string& base, const std::string &suffix);
			void                saveBDD(const std::string id, const std::string& base, const std::string &suffix);
            void                shuffle(const std::vector<std::string>& order);
            void                readBDD(const std::string& b, const std::string &s);
            void                readBDD(const std::string id, const std::string& b, const std::string &s);
            bool    			sameBDD(std::string s1, std::string s2);
            void                existentialQ(const std::set<std::string>& v) {};
            void                destroyInternal(const std::set<std::string>& v) {};
            std::set<std::string>   checkOneSolution() {
                std::set<std::string> res;
                return res;
            };
            void                syncOrder(std::string bdd_name) {};
            void                checkOrder() {};
            void                reorder()   {};
            void                setXORBlocks(synExp* s) {};

            SemanticsBuddy      getVar(std::string var);
            void                newVarExpression(std::string var, synExp* mySynExp);
		
			void				done();
			void				setCacheRatio(double x);
			void				setMaxNodeNum(int size);
            SemanticsBuddy      getTrueBDD();
            SemanticsBuddy      getFalseBDD();
            std::string         printBDD(std::string bddName);
            void                restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive);
			std::string			getTraverseBDD();
			void				setTraverseBDD(std::string bddName);
			void 				saveBDDToStorage(std::string s);
			std::vector<mpf_class> computeCommonality(int num, bool verbose, bool fast);
			std::vector<mpz_class> computeDistribution(int num, bool verbose, bool fast);
            std::string getVarName(int x);
            std::vector<std::string> getVars(bool xverbose);
            bool existVar(std::string name);
			std::pair<std::vector<std::string>, std::vector<std::string>> getCoreDead(int num, bool verbose, bool fast);
			std::vector<bool>	uniformRandomSamplingBool(int num, bool verbose, bool fast);
			std::string			uniformRandomSamplingName(int num, bool verbose, bool fast);

   private:
            bdd                                                  		theBDD;
            std::map<std::string, bdd>                                  storage;
            std::map<BDD, mpq_class>									Pr, Pr1, Problow, Probhigh;
            std::vector<mpq_class>										marginals;
            double 														avLength;
            int 														alwaysTrue, alwaysFalse, independent;

            int                                                         totalNodes, numComponents;
            std::vector<bool>                                           startComponent;
            std::vector<int>                                            levelnodes, int2extLevel;
            SemanticsBuddy                                              process(synExp* exp);
            std::map<std::string, std::pair<bdd, bdd> >                 vars;
            std::vector<std::string>                                    inVars;
            std::map<std::string, int>                                  firstMenuBlocks, lastMenuBlocks;
            int                                                         index;// = 0;
            std::set<std::pair<int, int> >                              currentBlocks;
            const int                                                   nodecount();
            const int                                                   nodecount(std::string s);

            std::vector<SemanticsBuddy*>                                temporaries;
};

#endif /* defined(__buddyAdapter__) */
