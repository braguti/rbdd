//
//  cuddFactory.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __myKconf__cuddFactory__
#define __myKconf__cuddFactory__

#include "vBDDFactory.hpp"
#include "SemanticsCudd.hpp"
#include "varInfo.hpp"
#include "humanNums.hpp"
#include "mytime.hpp"


#include "gmpxx.h"

#include <mtr.h>
#include <limits>
#include <cuddObj.hh>
#include <cudd.h>
#include <dddmp.h>
#include <iomanip>
#include <list>
#include <map>
#include <set>

using namespace cudd;

class distInfo {
public:
    int ref, tempref;
    bool mark;
    std::vector<long> *dist;
};

class cuddFactory : public vBDDFactory {
    public :
                                cuddFactory(double cacheMultiplier);
								
			void                setValue(std::string, synExp*);
            std::vector<long>   computeDistribution();
            void                reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod);
            void                reduce(const std::string& slow, const std::string& shigh);
            int                 getNodes() { return 0; }
            void                computeLevelNodes();
            void                setUpVariable(const std::string& var);
            void                printBDD();
            void                init(bool verbose, int nodenum, int cachesize);
            void                reorder(std::string reorderMethod);
            int                 addblock(std::string x);
            int                 addMenublock(MenuBlock *m);
            void                initblocks();
			std::vector<std::string>    giveOrder();
            void                namevar(std::string name, int index);
            void                newVar(std::string var, std::string type);
            void                newVarExpression(std::string var, std::string type, synExp* mySynExp);
            void                moduleImplies(std::string var);
            void                apply(synExp * s);
            void                apply(std::string s, synExp * exp);
            std::map<std::string, Cudd_ReorderingType> reorderMap;
			const   int         nodecount();
			const   int         nodecount(std::string s);
	
			void    			saveBDD(const std::string& base, const std::string &suffix);
			void    			saveBDD(const std::string id, const std::string& b, const std::string &s);
			void    			readBDD(const std::string& b, const std::string &s);
			void    			readBDD(const std::string id, const std::string& b, const std::string &s);
			bool    			sameBDD(std::string s1, std::string s2);
			static  int MAXVAR;
			
			Cudd    mgr;
			const   cudd::BDD   process(synExp* exp);
			int                 getLevelNodes(int level)                                            const;
			int                 getNumComponents()                                                  const;
			void                shuffle(const std::vector<std::string>& order);
			void                computeProbabilities();
			std::vector<bool>   generateRandom();

			void				done();
			void				setCacheRatio(double x);
			void				setMaxNodeNum(int size);
			SemanticsCudd       getVarBDD(std::string var);
            SemanticsCudd       getTrueBDD();
            SemanticsCudd       getFalseBDD();

void printBDD(std::string bddName);
void restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive);
std::vector<int> traverse();

private:
		mpz_class tCoef, eCoef, temp, oneTemp;
		
		mpf_class atCoef, aeCoef, ptemp;
		int nodeCounter, oneCounter;
		double getProb(DdNode *node);
		mpz_class computeeCoef(DdNode *node);
		
		void computeProbRecur(DdNode* node);
		void distrecur(DdNode* n);
		void traverseRecur(DdNode *n);
		void changeUp(int pos, int len, int ind, int lenhigh, int* perm);
		void changeDown(int pos, int len, int ind, int lenlow, int* perm);
		std::vector<int>        int2extLevel, levelnodes;
		std::set<std::string>   setImplies;
        int                 countVar = 0;
        cudd::BDD           theBDD;
        void                printVars();
		cudd::BDD          getVar(std::string var);
		std::map<std::string, std::pair<cudd::BDD, cudd::BDD> >     vars;
		std::map<std::string, std::pair<int, int> >                 positions;
		std::vector<std::string> inVars;
		std::vector<bool>       startComponent;
		std::map<DdNode*, distInfo> mapInfo;
		int     reorderMin, reorderMax, numComponents;
		std::string minVar, maxVar;
        bool emptyTree;
        std::pair<int, int> findSmallestBlock(int pos, int pos2);
        int numVars;
        std::set<std::pair<int, int> > currentBlocks, presentBlocks;
		std::map<DdNode*, double>  probabilities;
		std::map<DdNode*, mpz_class>  coefficients;
		std::map<DdNode*, bool>  mark;
    
		std::map<std::string, cudd::BDD> storage;
		
};

#endif /* defined(__myKconf__cuddFactory__) */
