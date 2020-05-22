//
//  choiceInfo.cpp
//  myKconf
//
//  Created by david on 25/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "choiceInfo.hpp"
#include "MenuBlock.hpp"

std::map<std::string, std::string>            choiceInfo::member2name;
std::map<std::string, int>                    choiceInfo::name2numerofmembers;
std::map<std::string, choiceInfo*>            choiceInfo::name2choice;
bool                                          choiceInfo::useXOR = false;


std::map<configInfo*, int> otherMap;

// Checks to see if a member depends on another choice member
bool        choiceInfo::checkDeps(std::set<std::string> s, std::string member) {
    if (symbolList.empty())
        return true;
    
    std::string last = symbolList.back()->getName();
    for(const std::string & ss : s)
        for(configInfo* ci : symbolList)
            if (ci->getName() == ss) {
                if (ss == last)
                    return false;
                else {
                    std::ostringstream ost;
                    ost << std::endl << "Choice member "  << member  << " in choice "
                        << "\"" << promptext << "\"" << " depends on member " << ss
                        << " in the same choice, which is not even the previous choice entry." << std::endl;
                    throw std::logic_error(ost.str());
                }
            }
    return true;
}

std::vector<MenuBlock*> choiceInfo::createBlocks() {
    std::set<std::string> origSymbols, orderSymbols;
    std::map<std::pair<std::set<std::string>, std::set<std::string>>, int> blockMap;
    std::vector<MenuBlock*> blocks;
    int blockNumber = 0;
    std::string first = symbolList.front()->getName();
    for(configInfo* ci : symbolList) {
        origSymbols.insert(ci->getName());
        if (blockMap.find(std::pair<std::set<std::string>, std::set<std::string>>(
                          ci->getDependsOn(), ci->getIsDepended())) ==
            blockMap.end())
            blockMap[std::pair<std::set<std::string>, std::set<std::string>>(
                       ci->getDependsOn(), ci->getIsDepended())] = ++blockNumber;
        else
            if (ci->getSelects())
                blockNumber++;
        otherMap[ci] = blockNumber;
        
    }
    std::vector<std::vector<configInfo*>> theBlocks(blockNumber);
    for(configInfo* ci : symbolList) {
        int b = blockMap[std::pair<std::set<std::string>, std::set<std::string>>(ci->getDependsOn(), ci->getIsDepended())];
        theBlocks[b-1].push_back(ci);
    }

    std::vector<configInfo*> flattened;
    int c = 0;
    for(auto vec : theBlocks) {
        c++;
        std::vector<std::string> contents;
        for(auto ci : vec) {
            flattened.push_back(ci);
            contents.push_back(ci->getName());
        }
        std::ostringstream ost;
        ost << promptext;
        blocks.push_back(new MenuBlock(contents.begin(), contents.end(), ost.str(), "choiceBlock"));
    }
    
    std::vector<configInfo*>::iterator it = symbolTable::getIterator(first);
    for(configInfo* ci : flattened) {
        orderSymbols.insert((*it)->getName());
        if (origSymbols.find((*it)->getName()) == origSymbols.end()) {
            Rcpp::stop("Symbol %s being replaced by %s", (*it)->getName(), ci->getName());
        }
        *it++ = ci;
    }
    if (origSymbols.size() != orderSymbols.size()) {
        Rcpp::Rcerr << "orig size " << origSymbols.size() << " order size " << orderSymbols.size() << std::endl;
    }
    return blocks;
}

// According to Berger et al. a choice is mandatory and bool by default
choiceInfo::choiceInfo()  {
    hasConfig       = true;
    optional        = false;
    visibilityVar   = synTrue;
    prompt          = synFalse;
    promptGuard     = synTrue;
}

void        choiceInfo::push_symbol(configInfo* ci)            {
    member2name[ci->getName()] = name;
    name2choice[name] = this;
    symbolList.push_back(ci);
    name2numerofmembers[name]++;
}
// Return an expression meaning exactly one of the choices is chosen as true.
synExp* choiceInfo::giveExactlyOne() {
    synExp* exactlyOne = synFalse;
//    for(configInfo* vf : symbolList)
//        exactlyOne = makeOr(vf->getValue(), exactlyOne);
   
    for(std::list<configInfo*>::iterator    first  = symbolList.begin();
        first  != symbolList.end();
        first++)
        for(std::list<configInfo*>::iterator last = first++;
            last != symbolList.end();
            last++)
            exactlyOne = makeAnd(exactlyOne,
                                 makeNot((makeAnd((*first)->getValue()->copy_exp(),
                                                   (*last)->getValue()->copy_exp()))));
    return exactlyOne;
}

// Return an expression meaning every one of the choices is module.

synExp* choiceInfo::allModules() {
    synExp* allmod = synTrue;
    for(configInfo* vf : symbolList)
        allmod = makeAnd(allmod, vf->getValue()->copy_exp());
    return allmod;
}

// At most one of the symbols is synTrue, the others are false
synExp* choiceInfo::noMoreThanOneTrue() {
    synExp* noMore = NULL;
    for(std::list<configInfo*>::iterator    first  = symbolList.begin();
        first  != symbolList.end();
        first++)
        for(std::list<configInfo*>::iterator last = symbolList.begin(); last != first;
            last++) {
            if (noMore != NULL)
                noMore = new synCompound(synAnd, noMore,
                                     makeImplies(new synCompound(synAnd,
                                                        (*first)->getValue()->copy_exp(),
                                                        (*last)->getValue()->copy_exp()),
                                                 synFalse));
            else
                noMore = makeImplies(new synCompound(synAnd,
                                            (*first)->getValue()->copy_exp(),
                                            (*last)->getValue()->copy_exp()),
                            synFalse);
                
        }
    if (noMore == NULL)
        noMore = synTrue;
    
    return noMore;
}



// At least one of the symbols is true
synExp* choiceInfo::atLeastOneTrue() {
    synExp* atLeast = NULL;
    for(configInfo* vf : symbolList) {
        if (atLeast != NULL)
            atLeast = new synCompound(synOr, atLeast, vf->getValue()->copy_exp());
        else
            atLeast = vf->getValue()->copy_exp();
    }
    if (atLeast == NULL)
        atLeast = synTrue;
    return atLeast;
}

// All the symbols are less than or equal to that
synExp* choiceInfo::noBiggerThan(synExp* that) {
    synExp* noBigger = synTrue;
    for(configInfo* vf : symbolList)
        noBigger = makeAnd(noBigger, makeImplies(vf->getValue()->copy_exp(), that));
    return noBigger;
}

synExp* choiceInfo::atLeastOneMoreThanFalse() {
    synExp* noFalse = synFalse;
    for(configInfo* vf : symbolList)
        noFalse = makeOr(noFalse, vf->getValue()->copy_exp());
   return noFalse;
}

synExp* choiceInfo::allFalse() {
    synExp* aux = NULL;
    for(configInfo* vf : symbolList) {
        if (aux != NULL)
            aux = new synCompound(synAnd,
                              aux,
                              makeOr(makeNot(vf->getValue()->copy_exp()),
                                    vf->processSelects())
                       );
        else
            aux = makeOr(makeNot(vf->getValue()->copy_exp()),
                         vf->processSelects());
    }
    return aux;
}
synExp* choiceInfo::orMemberDeps() {
    synExp *res = synFalse;
    for(configInfo* vf : symbolList) {
        res = makeOr(res, vf->getDependsVar()->copy_exp());
        
    }
    return res;
}
std::list<synExp*>  choiceInfo::giveConstraint() {
    //processDefaults();
    synExp* aux = synTrue;
    std::list<synExp*> res;
    // If the choice  is mandatory and bool, it represents an XOR   group
    // If the choice is optional and bool,  it represents a  MUTEX group
    //if (vartype == "boolean") {
    synExp* XOR = NULL;
    if (!useXOR) {
        synExp* nmtot = noMoreThanOneTrue();
        Rcpp::Rcerr << "choice nmtot " << nmtot << std::endl;
    //res.push_back(nmtot);
        if (!optional)
            aux = makeAnd(aux, atLeastOneTrue());
        XOR = makeAnd(nmtot, aux);
    }
    else {
        std::vector<synExp*> v;
        for(configInfo *ci : symbolList)
            v.push_back(ci->getValue()->copy_exp());
        if (optional)
            throw std::logic_error("Optional boolean choice. Implement MUTEX");
        XOR = makeXOR(v);
    }
    Rcpp::Rcerr << "choice visibility " << visibilityVar
                << " prompt " << prompt << " guard "
                << promptGuard << std::endl;
    
    synExp* innerIf = makeIfThenElse(makeAnd(orMemberDeps(),
                                     makeAnd(getDependsVar()->copy_exp(),
                                             makeAnd(visibilityVar->copy_exp(),
                                             makeAnd(prompt->copy_exp(),
                                                     promptGuard->copy_exp())))),
                                 XOR,
                                 allFalse());
    Rcpp::Rcerr << "choice inner if : " << innerIf << std::endl;
    res.push_back(innerIf);
    return res;
        
}

void choiceInfo::processDefaults() {
    // In principle there is not a default value...
    if (vartype == "boolean" || vartype == "tristate") {
        synExp *temp = synTrue;
        for(std::list<std::pair<synExp*, synExp*> >::iterator itl = defaultList.begin();
            itl != defaultList.end(); itl++) {
            synExp *guard = itl->first;
            synExp *value = itl->second;
            temp = makeIfThenElse(guard->copy_exp(), value->copy_exp(), temp);
        }
        xdefault = temp;
    }
    // For now, we don't apply defaults to string variables
    else
        xdefault = synTrue;
    // We keep the first symbol
    if (xdefault == synTrue) {
        xdefault = symbolList.front()->getValue()->copy_exp();
        Rcpp::Rcerr << "First symbol is " << symbolList.front()->getName() << " ";
        Rcpp::Rcerr << "RV " << symbolList.front()->getValue() << std::endl;
        if (xdefault == NULL) {
            throw std::logic_error("xdefault is NULL");
        }
}
}

std::ostream& operator<<(std::ostream& os, choiceInfo& ci) {
    os << "-----------------------------------------------------" << std::endl;
    os << "Choice" << std::endl;
    os << "Name:       "     << ci.name             << std::endl;
    os << "Prompt text:"     << ci.promptext        << std::endl;
    os << "Type:       "     << ci.vartype          << std::endl;
    os << static_cast<varInfo*>(&ci);
    os << "SymbolList "      << ci.symbolList.size() << " elements." << std::endl;
    std::list<synExp*> theList = ci.giveConstraint();
    os << "Constraint:   " << std::endl;
    for(std::list<synExp*>::iterator itl = theList.begin(); itl != theList.end(); itl++)
        os << *itl << std::endl;
    os << "-----------------------------------------------------" << std::endl;
    return os;
}
