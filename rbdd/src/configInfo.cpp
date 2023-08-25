//
//  configInfo.cpp
//  myKconf
//
//  Created by david on 28/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "configInfo.hpp"
#include <iomanip>

void configInfo::addSymbolsToPrimalGraph(const std::set<std::string>& theSet, primalGraph& pg) {
    for(std::set<std::string>::iterator its1 = theSet.begin(); its1 != theSet.end(); its1++) {
        varInfo *var1 = symbolTable::getSymbol(*its1);
        for(std::set<std::string>::iterator its2 = its1; its2 != theSet.end(); its2++) {
             varInfo *var2 = symbolTable::getSymbol(*its2);
            // If both vars have config, do not have already a value determined
            // then we increment the co-occurrence graph
            //std::cerr << " var1 " << var1->getName()
            //<< " var2 " << var2->getName() << std::endl;

            // NOTE: It is important that a single variable gets written to the clusters
            // that's why we don't check if the variables are different
             if (var1->getHasConfig() && var2->getHasConfig()
                 && (var1->getValue()->get_type() == synExp_Symbol)
                 && (var2->getValue()->get_type() == synExp_Symbol))
                pg.incGraph(*its1, *its2);
        }
        //std::cerr << "?? var name " << name << " var1 " << var1->getName() << std::endl;
        if (var1->getHasConfig() && (var1->getValue()->get_type() == synExp_Symbol)
            && getValue()->get_type() == synExp_Symbol)
                pg.incGraph(name, *its1);
    }
}


void configInfo::addSelect(const std::string& v, synExp *e) {
    selects = true;
    selectList.push_back(std::pair<std::string, synExp *>(v, e));
    if (symbolTable::getSymbol(v) == NULL) {
        configInfo *p = new configInfo();
        p->setName(v);
        
        // Warning. Here again we can be inserting variables which are not defined in this architechture
        symbolTable::addSymbol(v, p);
    }
     selectVar.push_back(makeIfThenElse(e, makeImplies(value, symbolTable::getSymbol(v)->getValue()), synTrue));
}



std::list<synExp*> configInfo::giveConstraint() {
    std::list<synExp*> aux;
    synExp* elsePart;
    
    // Hand edited linux/lib/Kconfig to remove "default n" in config GENERIC_IO. config HAS_IOMEM has select GENERIC_IO, which means HAS_IOMEM was assigned false with catastrophics effects (bdd = false).
    
    // Same problem with HAVE_AOUT and X86_32. We have to ignore default values, at least, in the
    // negative case.
    
    // Added "selectable" set to driver, so if a variable is not selectable, then we set it to false if it
    // has no prompt and defaults to false.
    
    //std::cerr << "give constraint " << getName() << std::endl;
    if (resolve && xdefault == synTrue)
        elsePart = makeEqual(value, xdefault);
    else
        elsePart = synTrue;
    
    //std::cerr << "prompt " << prompt << " visibilityVar " << visibilityVar << " promptGuard " << promptGuard << std::endl;
    synExp* condition = makeAnd(prompt, makeAnd(visibilityVar, promptGuard));
    synExp* things = makeIfThenElse(condition,
                                    makeImplies(value, getDependsVar()),
                                    elsePart);
    
    //std::cerr << "condition is " << condition << " xdefault is " << xdefault << std::endl;
    if ((eval(condition) == synFalse && resolve)
         && (xdefault == synTrue || (xdefault == synFalse && !selectable))) {
        std::cerr << "Variable " << std::setw(30) << name << " is assigned " << xdefault << std::endl;
        value = xdefault;
        if (xdefault == synTrue) {
            for(unsigned itx = 0; itx < selectList.size(); itx++) {
                // If the guard is true
                if (eval(selectList[itx].second) == synTrue) {
                    //std::cerr << "Variable " << std::setw(30) << selectList[itx].first << " is assigned true" << std::endl;
                    if (symbolTable::getSymbol(selectList[itx].first) != NULL)
                        symbolTable::getSymbol(selectList[itx].first)->setValue(synTrue);
                    else {
                        configInfo *p =  new configInfo();
                        p->setName(selectList[itx].first);
                        p->setValue(synTrue);
                        symbolTable::addSymbol(selectList[itx].first, p);
                    }
                        
                }
                else {
                    //std::cerr << "Guard is not true, is " << eval(selectList[itx].second) << std::endl;
                    //std::cerr << "itx " << itx << std::endl;
                    //std::cerr << "selectVar " << selectVar[itx] << std::endl;
                    //std::cerr << "eval(selectVar )" << eval(selectVar[itx]) << std::endl;

                    aux.push_back(eval(selectVar[itx]));
                }
                
        // if xdefault == synFalse, then all the implications are true  and there is nothing to do
            }
        }
    }
    else {
        //std::cerr << "eval(" << things << "is " << eval(things) << std::endl;
          aux.push_back(eval(things));
        for(int itx = 0; itx < selectVar.size(); itx++) {
            //std::cerr << "Also adding " << eval(selectVar[itx]) << " to constraint" << std::endl;
            aux.push_back(eval(selectVar[itx]));
        }
    }
   
    
    return aux;
}

void configInfo::processDefaults() {
    //std::cerr << "processDefaults for " << getName() << std::endl;
    // In principle there is not a default value...
    resolve = false;
    if (vartype == "boolean" || vartype == "tristate") {
        synExp *temp = synFalse;
        for(std::list<std::pair<synExp*, synExp*> >::iterator itl = defaultList.begin();
            itl != defaultList.end(); itl++) {
            synExp *guard = itl->first;
            synExp *value = itl->second;
            //std::cerr << "temp = makeIfthenelse " << guard << ", " << value << ", " << temp << std::endl;
            temp = makeIfThenElse(guard, value, temp);
            //std::cerr << "Finally it is " << temp << std::endl;
        }
        if (!defaultList.empty()) {
            // There is actually a default value
            resolve = true;
            //xdefault = makeEqual(symbolTable::getSymbol(name), makeIfThenElse(getDependsVar(), temp, synFalse));
            //std::cerr << "makeIfThenElse" << getDependsVar() << ", " << temp << ", false" << std::endl;
            xdefault = makeIfThenElse(getDependsVar(), temp, synFalse);
        }
        else xdefault = synTrue;
    }
    // For now, we don't apply defaults to string variables
    else
        xdefault = synTrue;
}

void configInfo::print(std::ostream& os) {
    os << "-----------------------------------------------------" << std::endl;
    os << "Config" << std::endl;
    os << "Name:         "     << name             << std::endl;
    os << "Prompt text:  "     << promptext        << std::endl;
    os << "Prompt Guard: "     << promptGuard      << std::endl;
    os << "Type:         "     << vartype          << std::endl;
 //   os << static_cast<varInfo*>(this);
    os << "Prompt:       "     <<  prompt           << std::endl;
    os << "Default:      "     << xdefault         << std::endl;
    os << "Select:       "     << std::endl;
    for(std::vector<std::pair<std::string, synExp *> >::iterator itl = selectList.begin();
        itl != selectList.end(); itl++)
        os << itl->first << " if " << itl->second  << std::endl;
    
    os << "Value:        "      << value                 << std::endl;
    os << "Visibility:   "      << visibilityVar         << std::endl;
    std::list<synExp*> theList = giveConstraint();
    os << "Constraint:   " << std::endl;
    for(std::list<synExp*>::iterator itl = theList.begin(); itl != theList.end(); itl++)
        os << *itl << std::endl;
}

