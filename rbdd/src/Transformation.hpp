
#ifndef __Transformation__
#define __Transformation__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "RbddElem.hpp"
#include "SiftScorer.hpp"
#include "synExpDriver.hpp"
#include "OtherScorer.hpp"
#include "bddBuilder.hpp"
#include "PermScorer.hpp"
#include "forceScorer.hpp"
#include "forceBlockScorer.hpp"
#include "RandomScorer.hpp"
#include "minmaxSorter.hpp"
#include "nullSorter.hpp"

using namespace std;

class Transformation {

    private:
        Components*                     pcomp;
        vector<synExp*>            expList;
        map<string, int>      varMap;
        vector<string>        variables;
        vector<int>                pos2var, var2pos;

    public:
        Transformation(RbddElem elem);
        void readVars(string varFile);
        void readExp(string expFile);
        void treatVarsAndConstraints();
        BDDAdapter* buildBDD(std::string bdd_name);

        string                     base;
        vector<MenuBlock*>         menuBlocks;
        double                          scoreThreshold    = 1.0;
        bool                            static_components = true;
        bool                            dynamic_components= false;
        bool                            outputComponents  = false;
        bool                            doCNF             = false;
        string                     reorderMethod, suffix;
        BDDAdapter                     *adapter;
        string                     constReorder      = "none";
        int                             reorderInterval   = 0;
        int                             minNodes          = 0;
        int                             lineLength        = 95;
        int                             maxNodes          = std::numeric_limits<int>::max();
        int                             minConstraint     = 0;
        int                             reorderEvery      = 1;
        bool                            blocks            = true;
        double                          inflation         = 10;
        bool                            scoreReorder      = false;
        string                     scoreMethod       = "none";
        int                             window            = 0;
        string                     varOrderFile;

        constraintSorter*               newCnstOrder(const string & s);

        bool reorder = true;
};

#endif /* defined(__Transformation__) */
