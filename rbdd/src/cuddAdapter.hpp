//
//  cuddAdapter.h
//  myKconf
//
//  Created by david on 15/12/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __cuddAdapter__
#define __cuddAdapter__

#include "BDDAdapter.hpp"
#include "OneComponent.hpp"
#include "MultiComponents.hpp"
#include "SemanticsCudd.hpp"
#include "varInfo.hpp"
#include "humanNums.hpp"

//#include "gmpxx.h"

#include <mtr.h>
#include <limits>
#include <cuddObj.hh>
#include <cudd.h>
#include <dddmp.h>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <fstream>
#include <cstdio>
#include <regex>

using namespace cudd;


class cuddAdapter : public BDDAdapter {
    public :
        
    	cuddAdapter();
		cuddAdapter(double cacheMultiplier);
    	void useComponents();
    	void useComponents(std::vector<int> var2pos, std::vector<int> pos2var);
    	void init(int nodenum, int cachesize, std::string bddName);
		int getNumComponents();
    	int getMaxComponent();
    	int getComponent(int x);
    	int getCompSize(int x);
    	int getNumVars();
    	std::vector<int> pos2var();
    	std::vector<int> var2pos();
    	void changeOrder(std::vector<int> pos2var);

		DdNode* getBDD(std::string bddName);
		DdNode* getZero();
		DdNode* getOne();
    	Cudd getCudd() { return mgr; };
    	int getLevel(DdNode const * node);
    	int getLevel(DdNode* node);
    	int varAtPos(int pos);
    	void destroyQuantified(int n);
		void setValue(std::string, synExp*);
        void reorderComponent(std::vector<std::string>& ss, std::vector<MenuBlock*>& menus, std::string rmethod);
        void reduce(const std::string& slow, const std::string& shigh);
        int getNodes() { return 0; }
        void setUpVariable(const std::string& var);
        void printBDD();
        void reorder(std::string reorderMethod);
        int addblock(std::string x);
        int addMenublock(MenuBlock *m);
        void initblocks();
		std::vector<std::string> giveOrder();
        void namevar(std::string name, int index);
        void newVar(std::string var, std::string type);
    	void newVar(std::string var, std::string type, int pos);
        void moduleImplies(std::string var);
        bool apply(synExp * s);
        bool apply(std::string s, synExp * exp);
        std::map<std::string, Cudd_ReorderingType> reorderMap;
		const int nodecount();
		const int nodecount(std::string s);

		void saveBDD(const std::string& base, const std::string &suffix);
		void saveBDD(const std::string id, const std::string& b, const std::string &s);
		void    readBDD(const std::string& b, const std::string &s);
		void    readBDD(const std::string id, const std::string& b, const std::string &s);
		bool sameBDD(std::string s1, std::string s2);
		static int MAXVAR;

		Cudd mgr;
		const cudd::BDD process(synExp* exp);
    	int getNumComponents() const;
		void shuffle(const std::vector<std::string>& order);
		void computeProbabilities();
		std::vector<bool> generateRandom();
    	mpz_class getTProducts(DdNode* node, bool parity);
    	mpz_class getEProducts(DdNode* node, bool parity);
    	mpz_class getProducts(DdNode* node, bool parity);
    	void existentialQ(const std::set<std::string>& v);
    	void destroyInternal(const std::set<std::string>& v);
    	std::set<std::string> checkOneSolution();
    	std::set<std::string> checkOneSolutionRec(DdNode* node, int level);
    	std::string getVarName(int x);
    	void putInternalAtTheBottom(std::set<std::string> ss);
    	void reorderComponents(int nreorder);
    	void checkComponents();
    	void checkOrder();
    	void syncOrder(std::string bdd_name);
    	void incInternalUsed(synExp *s);
    	void reorder(); 
		void                        setXORBlocks(synExp* s);

            void                newVarExpression(std::string var, synExp* mySynExp);

			void				done();
			void				setCacheRatio(double x);
			void				setMaxNodeNum(int size);
			SemanticsCudd       getVar(std::string var);
            SemanticsCudd       getTrueBDD();
            SemanticsCudd       getFalseBDD();
			std::string			printBDD(std::string bddName);
			void				restrictBDD(std::string expression, std::string varToRestrict, std::string variableName, bool isPositive);
			Rcpp::Function		getFunction();
			void				setFunction(Rcpp::Function funcTrav);
			std::string			getTraverseBDD();
			void				setTraverseBDD(std::string bddName);
			void 				saveBDDToStorage(std::string s);
			std::vector<mpf_class> computeCommonality(int num, bool verbose, bool fast);
			std::vector<mpz_class> computeDistribution(int num, bool verbose, bool fast);
			std::vector<std::string> getVars(bool xverbose);
			bool existVar(std::string name);
			std::pair<std::vector<std::string>, std::vector<std::string>> getCoreDead(int num, bool verbose, bool fast);
			std::vector<bool>	uniformRandomSamplingBool(int num, bool verbose, bool fast);
			std::string			uniformRandomSamplingName(int num, bool verbose, bool fast);

			int					addNode(DdNode* node);
			DdNode*				getNode(int node);
			int					getNodePos(DdNode* node);
			void				clearNodeList();
			bool				isNodeZero(int node);
			bool				isNodeOne(int node);
			std::vector<int>	getChildrenNodes(int node);

private:

    	mpz_class solutions_rec(DdNode* node);
		mpf_class getProb(DdNode *node);

		void auxXOR(synExp* exp);
    	void createXORBlock(std::vector<std::string>& vec);

		void changeUp(int pos, int len, int ind, int lenhigh, int* perm);
		void changeDown(int pos, int len, int ind, int lenlow, int* perm);
    	void internalRefs();
		cudd::BDD   auxReadBDD(const std::string& base, const std::string &suffix);

    	std::map<std::string, int> internalReferenced, internalUsed;

    	Components* pcomp;
    	std::set<std::string> quantify;

		std::vector<int> int2extLevel, levelnodes;
		std::set<std::string> setImplies;
        int countVar;// = 0;
        cudd::BDD theBDD;
        void printVars();
		cudd::BDD getVarBDD(std::string var);
		std::map<std::string, std::pair<cudd::BDD, cudd::BDD> > vars;
		std::map<std::string, std::pair<int, int> > indices;
		std::vector<std::string> inVars;
		std::vector<bool> startComponent;
		int reorderMin, reorderMax, numComponents;
		std::string minVar, maxVar;
        bool emptyTree;
    	bool withComponents;
        std::pair<int, int> findSmallestBlock(int pos, int pos2);
        unsigned int numVars;
        std::set<std::pair<int, int> > currentBlocks, presentBlocks;

    	std::vector<mpf_class> commonalities;
    	std::vector<mpz_class> productsPerVar;
    	std::map<DdNode*, mpz_class> tProducts, eProducts;
		std::map<DdNode*, bool> mark;
    	std::set<std::string> artVars;
		std::map<std::string, cudd::BDD> storage;
		std::vector<MenuBlock*>             theMenuBlocks;

		Rcpp::Function funcTrav = Rcpp::Function("ls");
		bool hasNewVar;

		std::vector<DdNode*> nodeList;
		
};

#endif /* defined(__cuddAdapter__) */
