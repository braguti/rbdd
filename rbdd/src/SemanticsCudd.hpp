//
//  SemanticsCudd.hpp
//  myKconf
//
//  Created by david on 14/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __SemanticsCudd__
#define __SemanticsCudd__

#include <iostream>
#include <string>

#include <cudd.h>
#include <mtr.h>
#include <cuddObj.hh>
#include <Rcpp.h>

using namespace cudd;

class SemanticsCudd {
    
    private:
    static  Cudd                                    *pmgr;
    
            std::string                             strValue;
    std::pair<const cudd::BDD, const cudd::BDD>   logicValue;
        bool isStr;
    

    public:
    
    void static printMgr() { Rcpp::Rcout << "Manager: " << pmgr << std::endl << std::flush;  }
    SemanticsCudd() : logicValue(std::pair<const cudd::BDD&, const cudd::BDD&>(pmgr->bddZero(), pmgr->bddZero()))
    { isStr = false;
    }
    SemanticsCudd(std::string s);
    SemanticsCudd(const cudd::BDD& a, const cudd::BDD& b);
    
    static  void  setManager(Cudd* pmanager)  { pmgr = pmanager; }
    SemanticsCudd& operator=(const SemanticsCudd& t) {
        Rcpp::Rcout << "SemanticsCudd const copy constructor" << std::endl;
        exit(-1); }
    ~SemanticsCudd() {}
    
    bool   getIsStr()     { return isStr; }
    void   setStr(bool b) { isStr = b;    }
    
   
    
    
    
    const   cudd::BDD&      main()   const { return logicValue.first; }
    const   cudd::BDD&      module() const { return logicValue.second;}
            std::string     getStr() const { return strValue;         }
    
    friend          std::ostream&   operator<<(std::ostream& os, const SemanticsCudd& t);
    friend const    SemanticsCudd  operator&&(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend const    SemanticsCudd  operator||(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend const    SemanticsCudd  operator==(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend const    SemanticsCudd  operator!=(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend const    SemanticsCudd  operator!(const SemanticsCudd& e);
    friend const    SemanticsCudd  operator<(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend const    SemanticsCudd  operator>(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend          bool            equal(const SemanticsCudd& e1, const SemanticsCudd& e2);
    friend          bool            different(const SemanticsCudd& e1, const SemanticsCudd& e2);
    
    //SemanticsCudd operator&&=(SemanticsCudd& e1, SemanticsCudd& e2);

    
};
#endif /* defined(__SemanticsCudd__) */
