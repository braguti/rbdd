//
//  synExp.h
//  myKconf
//
//  Created by david on 04/12/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __myKconf__synExp__
#define __myKconf__synExp__

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <limits>
#include "symbolTable.hpp"

enum synExpT    { synExp_Const,   synExp_Symbol,   synExp_Compound, synExp_String                        };
enum synConstsT { synConst_false, synConst_module, synConst_true                                         };
enum synOp      { synNot,         synAnd,          synOr          , synXor, synNand, synNor, synXnor, synImplies, synIff, synIfThenElse, synEqual  };

class synExp {
    
    public:
    
        synExp() {}
        virtual ~synExp() { std::cerr << "In destructor " << this << std::endl;};
        virtual synExpT                         get_type()          const                   = 0;
        virtual std::string                     get_string()        const { return "";           }
        virtual std::string                     getSymbol()         const {
            std::cerr << "Error accessing getSymbol in wrong object: " << this << std::endl;
            int *p = NULL;
            std::cerr << *p << std::endl;
            exit(-1);
            return "";
        }
        virtual synOp                           get_op()            const {
            std::cerr << "Error accessing get_op in wrong object: " << this << std::endl;
            int *p = NULL;
            std::cerr << *p << std::endl;
            exit(-1);
        }
        virtual synExp*                         first()             const {
            //std::cerr << "Error accessing first in wrong object: " << this << std::endl;
            //int *p = NULL;
            //std::cerr << *p << std::endl;
            //exit(-1);
            return NULL;
        }
        virtual synExp*                         second()            const {
            //std::cerr << "Error accessing second in wrong object: " << this << std::endl;
            //int *p = NULL;
            //std::cerr << *p << std::endl;
            //exit(-1);
            return NULL;
        }
        virtual synExp*                         third()             const {
            //std::cerr << "Error accessing third in wrong object: " << this << std::endl;
            //int *p = NULL;
            //std::cerr << *p << std::endl;
            //exit(-1);
            return NULL;
        }
        virtual const std::set<synOp>&          giveOps()           const   {
            std::cerr << "Error accesing giveOps in wrong object: " << this << std::endl;
            int *p = NULL;
            std::cerr << *p << std::endl;
            exit(-1);
        }
        void addModules() { mySymbols.insert("MODULES"); }
        const std::set<std::string>&            giveSymbols()       const { return mySymbols;    }
        const std::set<int>&                    giveSymbolIndices() const { return myIndices;    }
        virtual bool                            isGround()          const                   = 0;
        virtual int                             opPriority()        const  { return 0;  }
        void                                    computeIndices(std::map<std::string, int>& m) {
                                                    myIndices.clear();
                                                    for(const std::string & s : mySymbols) {
                                                        myIndices.insert(m[s]);
                                                    }
                                                }

        friend  std::ostream&          operator<<(std::ostream& os, synExp* ps);
        friend  bool                   equal(synExp *e1, synExp *e2);
    
    template <class Q>
        int giveMax(const Q& q) const {
        int pos;
        int max = -1;
        for(int x : giveSymbolIndices()) {
            //std::cerr << "index " << x << " pos " << q[x] << std::endl;
            if ((pos = q.at(x)) > max) {
                    max = pos;
                }
                //else {
                //    std::cerr << "var id " << x << " not found in order" << std::endl;
                //    exit(-1);
                //}
        }
        return max;
    }
    template <class Q>
    int giveMaxInd(const Q& q) const {
        int pos, ind;
        int max = -1;
        for(int x : giveSymbolIndices()) {
            //std::cerr << "index " << x << " pos " << q[x] << std::endl;
            if ((pos = q.at(x)) > max) {
                max = pos;
                ind = x;
            }
            //else {
            //    std::cerr << "var id " << x << " not found in order" << std::endl;
            //    exit(-1);
            //}
        }
        return ind;
    }

    template <class Q>
    int giveMinInd(const Q& q) const {
        int pos, ind;
        int min = std::numeric_limits<int>::max();
        for(int x : giveSymbolIndices()) {
            if ((pos = q.at(x)) < min) {
                min = pos;
                ind = x;
            }
            //else {
            //    std::cerr << "var id " << x << " not found in order" << std::endl;
            //    exit(-1);
            //}
        }
        return ind;
    }

    template <class Q>
    int giveMin(const Q& q) const {
        int pos;
        int min = std::numeric_limits<int>::max();
        for(int x : giveSymbolIndices()) {
            if ((pos = q.at(x)) < min) {
                    min = pos;
            }
            //else {
            //    std::cerr << "var id " << x << " not found in order" << std::endl;
            //    exit(-1);
            //}
        }
        return min;
    }
    
    int   getMax()   const { return max;   }
    int   getMin()   const { return min;   }
    int   getLCA()   const { return lca;   }
    float getScore() const { return score; }
    
    template <class Q>   void computeMaxMin(const Q& q) {
        max = giveMax(q);
        min = giveMin(q);
        //std::cerr << "synExp max " << max << " min " << min << std::endl;
        if (min > max) {
            std::cerr << "Min is bigger than max in " << this << std::endl;
            exit(-1);
        }
    }
    
    template <class O>   void computeScore(const O& o) {
        score = 0;
        for(int x = min; x <= max; x++) 
            score += weights[x];
            //int l = ocSoFar[o[x]];
            //if (l != 0)
            //    score++;
        
        return;
        //if (max != min)
        //    score = score / (max - min + 1);
        
        //score = log(score)/2.0 + (max - min)/2.0;
        //score += max - min;
    }

    void setScore(double x) { score = x;}
    
    template <class O>   double computeSumSpan(const O& o) const {
        double tot = 0;
        for(std::set<int>::iterator its =  myIndices.begin(); its !=  myIndices.end(); its++)
            for(std::set<int>::iterator its2 = its; its2 != myIndices.end(); its2++)
                if (its2 != its) {
                    //std::cerr << "index " << *its << " pos " << o[*its] << " index " << *its2 << " pos " << o[*its2] << std::endl;
                    tot += log(abs(o[*its] - o[*its2]));
                }
        return tot;

    }
    

    void setLCA(int x) { lca = x; }
    
    void thisOneIsProcessed() {
        for(int index : myIndices)
            weights[index]++;
            //for(synExp *s : inverse[ss])
            //    s->computeScore();
        return;
    }
//    static void computeInverse(const std::vector<synExp*>& pending) {
//        for(synExp* s : pending)
//            for(const std::string& ss : s->giveSymbols())
//                inverse[ss].insert(s);
//    }
    
    //    static void firstScores(const std::vector<synExp*>& pending, const std::vector<std::string>& varList) {
    //        for(synExp *s : pending) {
    //
    //        }
    //    }
    static void numVars(int s) {
        ocSoFar.resize(s);
        weights.resize(s);
    }
protected:
    
    static std::vector<int> ocSoFar;
    static std::vector<int> weights;
    //static std::map<std::string&, std::set<synExp*> > inverse;
    std::set<std::string> mySymbols;
    std::set<int>         myIndices;
    int max, min, lca;
    float score;
    virtual void print(std::ostream& os) const;    
};


extern synExp *synTrue;
extern synExp *synModule;
extern synExp *synFalse;
synExp *makeAnd(synExp* e1, synExp* e2);
synExp *makeOr(synExp* e1, synExp* e2);
synExp *makeNot(synExp* e1);
synExp *makeImplies(synExp* e1, synExp* e2);
synExp *makeIfThenElse(synExp* e1, synExp* e2, synExp* e3);
synExp *makeEqual(synExp* e1, synExp* e2);
std::list<synExp *> expandImplication(synExp* e);

class synConst : public synExp {
public:
    
    static synConst*       getSynFalse()  {
        if (!syncFalse)  syncFalse  = new synConst(synConst_false);  return syncFalse;
        }
    static synConst*       getSynModule() {
            if (!syncModule) syncModule = new synConst(synConst_module); return syncModule;
        }
    static synConst*       getSynTrue()   {
        if (!syncTrue)   syncTrue   = new synConst(synConst_true);   return syncTrue;
        }

    synExpT                         get_type()              const { return synExp_Const; }
    synConstsT                      get_const()             const { return theConst;     }
  
    const std::set<synOp>&          giveOps()               const { return myOps;        }
    bool                            isGround()              const { return true;         }
    int                             opPriority()            const { return 1;            }



private:
    

    std::set<synOp>       myOps;
    static synConst *syncTrue, *syncFalse, *syncModule;
    synConst(synConstsT oneConst) : theConst(oneConst) {};
    synConstsT theConst;
    void print(std::ostream& os) const;

};



class synSymbol : public synExp {
public:
    
                                    synSymbol(std::string s)    { name = s; mySymbols.insert(s);        }
    synExpT                         get_type()          const   { return synExp_Symbol;                 }
    std::string                     getSymbol()         const   { return name;                          }
    const std::set<synOp>&          giveOps()           const   { return myOps;                         }
    bool                            isGround()          const   { return false;                         }
    int                             opPriority()        const   { return 1;                             }

    
private:
    
    std::string           name;
    std::set<synOp>       myOps;

    
    void print(std::ostream& os) const;

};


class synCompound : public synExp {
public:
    
    synCompound(synOp op, synExp *e1, synExp *e2 = synFalse, synExp *e3 = synFalse) : Cop(op), Ce1(e1), Ce2(e2), Ce3(e3) {
        //mySymbols = e1->giveSymbols();
        mySymbols.insert(e1->giveSymbols().begin(), e1->giveSymbols().end());
        if (op != synNot) {// It is NULL for not
            std::set<std::string> theSet = e2->giveSymbols();
            mySymbols.insert(theSet.begin(), theSet.end());
        }
        if (op != synIfThenElse) {// It is NULL always except for IfThenElse
            std::set<std::string> theSet = e3->giveSymbols();
            mySymbols.insert(theSet.begin(), theSet.end());
        }
        myOps.insert(op);
        if (e1->get_type() == synExp_Compound) {
            myOps.insert(e1->giveOps().begin(), e1->giveOps().end());
        }
        if (e2->get_type() == synExp_Compound) {
            myOps.insert(e2->giveOps().begin(), e2->giveOps().end());
        }
    }
    int     opPriority() const  {
        switch(Cop) {
            case synEqual      : return  2;
            case synNot        : return  3;
            case synAnd        : return  4;
            case synOr         : return  5;
            case synXor        : return  6;
            case synNand       : return  7;
            case synXnor       : return  8;
            case synNor        : return  9;
            case synImplies    : return  10;
            case synIff        : return  11;
            case synIfThenElse : return  12;
            }
        return 0;
    }

    synExpT                         get_type()          const   { return synExp_Compound;   }
    synOp                           get_op()            const   { return Cop;               }
    synExp                          *first()            const   { return Ce1;               }
    synExp                          *second()           const   { return Ce2;               }
    synExp                          *third()            const   { return Ce3;               }
    const std::set<synOp>&          giveOps()           const   { return myOps;             }

    
    bool    isGround() const { return mySymbols.empty(); };

    friend std::ostream& operator<<(std::ostream& os, synCompound* ps);
    
private:
    
    std::set<synOp>       myOps;

    synOp Cop;
    synExp *Ce1, *Ce2, *Ce3;
    void print(std::ostream& os) const;
    friend  bool equal(synExp *e1, synExp *e2);

};

class synString : public synExp {
public:
    
                                    synString(std::string s) : st(s)     {};
    synExpT                         get_type()          const { return synExp_String;   }
    std::string                     get_string()        const { return st;              }
    synExp*                         eval()                    { return this;            }
    bool                            isGround()          const { return false;           }
    int                             opPriority()        const { return 1;               }
    const std::set<synOp>&          giveOps()           const { return myOps;           }


    friend std::ostream& operator<<(std::ostream& os, synString* ps);

private:
    std::set<synOp>       myOps;
    std::string st;
    void print(std::ostream& os) const;

};

void destroy(synExp* s);




#endif /* defined(__myKconf__synExp__) */
