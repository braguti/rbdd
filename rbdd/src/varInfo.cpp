//
//  varInfo.cpp
//  myKconf
//
//  Created by david on 12/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "varInfo.hpp"

void            varInfo::simplifyDependency() {
    int counter = 0;
    synExp* depBit = synTrue;
    for(synExp* s : dependStack) {
        if (s->isSymbol() && dependStack.size() > 1) {
            std::string sym = *(s->giveSymbols().begin());
            if (varInfo::depCount[sym] == 1 && static_cast<unsigned int>(counter+1) != dependStack.size()) {
                synExp *newDep = synTrue;
                std::list<synExp*>::iterator itl = dependStack.begin();
                for(int x = 0; x <= counter; x++) {
                    depBit = *itl++;
                    newDep = makeAnd(newDep, depBit->copy_exp());
                }
                newDep = makeAnd(newDep, innerDep);
                Rcpp::Rcerr << "In " << std::setw(40)       << getName()
                << " with depStack size "       << dependStack.size()
                << " replacing dependency "     << std::setw(20) << getDependsVar()
                << " with " << std::setw(20)    << newDep << std::endl;
                dependsVar->destroy();
                dependsVar = newDep;
                return;
            }
        }
        counter++;
    }
    
}
std::map<std::string, int> varInfo::depCount;

std::ostream& operator<<(std::ostream& os, varInfo* ps) {
    if (ps == NULL) {
        os << "NULL";
        return os;
    }
    ps->print(os);
    os << "Menu:         "     << ps->menuName         << std::endl;
    os << "File:         "     << ps->file             << std::endl;
    os << "DependsVar:   "     << ps->dependsVar       << std::endl;
    os << "-----------------------------------------------------" << std::endl;
    return os;
}

void varInfo::print(std::ostream& os) {
    os << "Menu:         "     << menuName         << std::endl;
    os << "File:         "     << file             << std::endl;
    os << "DependsVar:   "     << dependsVar       << std::endl;
}

