//
//  configInfo.cpp
//  myKconf
//
//  Created by david on 28/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "configInfo.hpp"
#include <iomanip>

std::map<std::string, std::map<std::string, std::vector<selectHelper>>> configInfo::mapSelect;
std::map<std::string, std::list<configInfo*> >   configInfo::mapAlternative;
int                                              configInfo::artificialCount = 0;
std::map<std::string, std::set<std::string>>     configInfo::stringAlternatives;
std::map<std::string, std::set<std::string>>     configInfo::stringValues;
std::map<std::string, int>                       configInfo::timesProcessed;
std::map<std::string, synExp*>                   configInfo::dependencies4Name;
std::map<std::string, std::list<synExp*>>        configInfo::redefinitionsIfPart;
std::map<std::string, std::list<synExp*>>        configInfo::redefinitionsThenPart;

synExp*     selectHelper::getExpression() { return expression->copy_exp(); }
synExp*     selectHelper::getDep()        { return c->getDependsVar()->copy_exp();        }

std::string selectHelper::getVar()        { return variable;               }
configInfo* selectHelper::getConfig()     { return c;                      }

void configInfo::addAlternative(std::string name, configInfo* val) {
    mapAlternative[name].push_front(val);


}

void configInfo::addMapSelect(const std::string& x, synExp* e, synExp* dep) {
        configsISelect.insert(x);
        selectHelper h(name, e, dep, this);
        mapSelect[x][name].push_back(h);
}

std::list<synExp*> configInfo::giveConstraint() {
    timesProcessed[name]++;
    if (vartype == "boolean" || vartype == "tristate")
        return giveLogicalConstraint();
    else {
        return giveStringConstraint();
    
    }
}
std::list<synExp*> configInfo::giveStringConstraint() {
    std::list<synExp*> result;

    if (timesProcessed[name] != symbolTable::timesDeclared(name))
        return result;
    Rcpp::Rcerr << "------------------------------------------------" << std::endl;
    Rcpp::Rcerr << "Name " << name << " file definition " << getFile() << std::endl;
    
    synExp *userInput         = processPrompts();
    synExp *defaultsCondition = processAllStringDefaults();
    checkInternal();
    artificial = internal;
    Rcpp::Rcerr << "userInput       "   << userInput         << std::endl;
    Rcpp::Rcerr << "defaultsCondition " << defaultsCondition << std::endl;
    Rcpp::Rcerr << "depStack ";
    for(synExp *s : getDepStack()) {
        Rcpp::Rcerr << s << " ";
    }
    Rcpp::Rcerr << std::endl;
    synExp* innerIf  = makeIfThenElse(userInput,
                                      new synSymbol(name),
                                      defaultsCondition);
    Rcpp::Rcerr << "innerIf " << innerIf << std::endl;
    
    if (innerIf != synTrue)
        result.push_back(innerIf);
    
    valuesImplyVariable(name, value, result);
    noTwoDifferentValues(name, result);
    
    return result;
}

synExp *configInfo::allStringValuesFalse() {
    synExp* res = synTrue;
    for(std::string val : stringAlternatives[name])
        res = makeAnd(res, makeNot(new synSymbol(val)));
    return res;
}
void configInfo::valuesImplyVariable(std::string s, synExp* val, std::list<synExp*>& res) {
    for(std::string alt : stringAlternatives[s]) {
        synExp *ss = makeImplies(new synSymbol(alt), val->copy_exp());
        Rcpp::Rcerr << "3 " << ss << std::endl;
        res.push_back(ss);
    }
}
    

void configInfo::noTwoDifferentValues(std::string s, std::list<synExp*>& res) {
    for(std::string alt1 : stringAlternatives[s])
        for(std::string alt2 : stringAlternatives[s]) {
            if (alt1 > alt2) {
                synExp *notTwo = makeNot(makeAnd(new synSymbol(alt1),
                                                 new synSymbol(alt2)));
                res.push_back(notTwo);
            }
        }
}
bool inExpSet(synExp* exp, std::set<synExp*> ss) {
    for(synExp* o : ss)
        if (equal(o, exp))
            return true;
    return false;
}

void flattenAnd(synExp* exp, std::set<synExp*>& ss) {
    if (exp->isAnd()) {
        flattenAnd(exp->first(), ss);
        flattenAnd(exp->second(), ss);
    }
    else
        ss.insert(exp);
}
void intersect(std::set<synExp*>& inter, std::set<synExp*> other) {
    std::set<synExp*>::iterator its = inter.begin();
    while (its != inter.end()) {
        if (!inExpSet(*its, other))
            its = inter.erase(its);
        else
            its++;
    }
}

std::set<synExp*> configInfo::intersectionSelectorSelectorDeps(std::string selector) {
    // Let's make the intersection of the selector's selectors
    std::set<synExp*> intersection;
    bool firstTime = true;
    for(std::pair<std::string, std::vector<selectHelper>> p : mapSelect[selector])
        for(selectHelper h : p.second) {
            std::set<synExp*> theOther;
            Rcpp::Rcerr << " selector " << selector << " selector " << p.first
            <<  " deps " << h.getDep() << std::endl;
            if (firstTime) {
                flattenAnd(h.getDep(), intersection);
                firstTime = false;
            }
            else {
                flattenAnd(h.getDep(), theOther);
                intersect(intersection, theOther);
            }
        }
    return intersection;
}


synExp*            configInfo::fixDeps(std::set<synExp*> inter, synExp* deps) {
    std::set<synExp*> setDeps;
    flattenAnd(deps, setDeps);
    std::set<synExp*>::iterator its = setDeps.begin();
    while (its != setDeps.end()) {
        if (inExpSet(*its, inter))
            its = setDeps.erase(its);
        else
            its++;
    }
    synExp *res = synTrue;
    for(synExp *bitDep : setDeps)
        if (!inExpSet(bitDep, inter))
            res = makeAnd(res, bitDep->copy_exp());
    return res;
}
synExp*            configInfo::processSelects() {
    // select inside a choice is ignored, as per original conf program
    if (isChoiceMember()) return synFalse;
    synExp*     orFactors = synFalse;
    for(std::pair<std::string, std::vector<selectHelper>> p : mapSelect[name]) {
        std::string selector = p.first;
        std::set<synExp*> intersectionSelectorsSelectorDeps = intersectionSelectorSelectorDeps(selector);
        
        Rcpp::Rcerr << "Intersection is ";
        bool first = true;
        for(synExp* xs : intersectionSelectorsSelectorDeps)
            if (first) {
                Rcpp::Rcerr << xs;
                first = false;
            }
            else{
                Rcpp::Rcerr << " and " << xs;
            }
        Rcpp::Rcerr << std::endl;
        
        synExp *    oneVarAlternatives = synFalse;
        bool        dropSelectorDeps       = false;
        bool        selectorSelectable = false;
        
        selectorSelectable = mapSelect[selector].size() > 0;
        bool allSelect = static_cast<unsigned int>(symbolTable::timesDeclared(selector)) == mapSelect[name][selector].size();
        
        if (!selectorSelectable && allSelect)
            dropSelectorDeps = true;
        
        Rcpp::Rcerr   << "3.5 selector "          << selector
        << " declared "             << symbolTable::timesDeclared(selector) << " times "
        << " selectorSelectable "   << selectorSelectable
        << " dropSelectorDeps "            << dropSelectorDeps
        << " select size "          << mapSelect[name][selector].size() << std::endl;
        
        synExp*     selectorExp      = synTrue;
        synExp*     ifexpr        = synTrue;
        synExp*     expression    = synTrue;
        bool        dummySelector = false;
        configInfo *c = symbolTable::getSymbol(p.first);
        if (c != NULL) {
            c->checkInternal();
            c->checkSimple();
        }
        dummySelector = c->isDummy();
        
        Rcpp::Rcerr << " dummy "                << dummySelector << std::endl;
        
        // If the selector is dummy we don't use the name, only the dependencies and the guard
        // if dropSelectorDeps, we don't use the dependencies of the selector, only the name and the guard
        // Otherwise, we use the name, the dependencies and the select guard.
        if (dummySelector) {
            for(selectHelper h : p.second) {
                ifexpr            = h.getExpression();
                Rcpp::Rcerr << "selector dummy "
                            << " ifexpr " << ifexpr
                            << " deps " << h.getDep() << std::endl;
                orFactors = makeOr(orFactors, makeAnd(h.getDep(), ifexpr));
                Rcpp::Rcerr << "1 orFactors = " << orFactors << std::endl;
            }
        }
        else
        // We use the selector and the condition, but omit the dependency.
        if (dropSelectorDeps) {
            for(selectHelper h : p.second) {
                selectorExp  = new synSymbol(h.getConfig()->getName());
                ifexpr            = h.getExpression();
                Rcpp::Rcerr << "selectorExp " << selectorExp << " ifexpr " << ifexpr << std::endl;
                //orFactors = makeOr(orFactors, makeAnd(h.getDep(), ifexpr));
                orFactors = makeOr(orFactors, makeAnd(new synSymbol(selector), ifexpr));
                Rcpp::Rcerr << "1 orFactors = " << orFactors << std::endl;
            }
        }
        else {
            // Now we use the intersection of the selectors selector to try to trim the
            // dependencies
            
            selectorExp  = new synSymbol(selector);
            Rcpp::Rcerr << "selectorExp " << selectorExp << std::endl;
            for(selectHelper h : p.second) {
                synExp* dependencies = h.getDep();
                if (allSelect) 
                    dependencies = fixDeps(intersectionSelectorsSelectorDeps,
                                           h.getDep());
                Rcpp::Rcerr << "Dependencies before " << h.getDep() << ", after " << dependencies << std::endl;
                expression = h.getExpression();
                Rcpp::Rcerr << "Expression " << expression << std::endl;
                ifexpr            = makeAnd(expression, dependencies);
                Rcpp::Rcerr << "3.7 ifexpr " << ifexpr << std::endl;
                oneVarAlternatives = makeOr(oneVarAlternatives, ifexpr);
                Rcpp::Rcerr << "3.8 oneVarAlternatives " << oneVarAlternatives << std::endl;
            }
            
            oneVarAlternatives = makeAnd(selectorExp->copy_exp(), oneVarAlternatives);
            Rcpp::Rcerr << "3.9 oneVarAlternatives " << oneVarAlternatives << std::endl;
            
            orFactors                     = makeOr(orFactors, oneVarAlternatives);
            Rcpp::Rcerr << "2 orFactors = " << orFactors << std::endl;
        }
    }
    return orFactors;
}

synExp*            configInfo::processPrompts() {
    synExp* condition = synFalse;
    for(configInfo* c : mapAlternative[name]) {
            Rcpp::Rcerr << " vis " << c->visibilityVar
                        << " prompt " << c->prompt
                        << " promptguard " << c->promptGuard << std::endl;
            synExp* auxBit =  makeAnd(c->visibilityVar->copy_exp(),
                                               makeAnd(c->prompt->copy_exp(),
                                                       c->promptGuard->copy_exp()));
            synExp* otherBit;
                otherBit = makeAnd(c->getDependsVar()->copy_exp(), auxBit);
        
            Rcpp::Rcerr << "otherBit " << otherBit << std::endl;
            condition = makeOr(condition, otherBit);
            Rcpp::Rcerr << "condition for " << name << ": " << condition << std::endl;
        }
    return condition;
}

bool            configInfo::checkInternal() {
    internal = true;
    for(configInfo* c : mapAlternative[name])
        if (c->prompt != synFalse)
            internal = false;
    return internal;
}
synExp*            configInfo::processAllStringDefaults() {
    // We need a list with the following things:
    // Dependencies, list of pairs <guard, value>
    std::vector<synExp*>              dep;
    std::vector<std::vector<synExp*>> guard;
    std::vector<std::vector<synExp*>> value;
    synExp* lastDep = synFalse;
    bool lastIgnored = false;
    for(configInfo* c : mapAlternative[name]) {
        std::vector<synExp*> oneGuard, oneValue;
        synExp *ui = makeAnd(c->visibilityVar->copy_exp(),
                             makeAnd(c->prompt->copy_exp(),
                                     c->promptGuard->copy_exp()));
        if (c->prompt->copy_exp() == synTrue) addValue("OTHERVAL");
        // If this config has a viable prompt, the defaults will never be used
        // so we just pass on them
        if (ui != synTrue) {
            // If there is a streak of the same dependencies, we join them
            if (equal(c->getDependsVar(), lastDep) && !lastIgnored) {
                Rcpp::Rcerr << "Joining two dependencies " << lastDep << std::endl;
                for(std::list<std::pair<synExp*, synExp*> >::iterator itl = c->defaultList.begin();
                    itl != c->defaultList.end(); itl++) {
                    guard.back().push_back(itl->first);
                    value.back().push_back(itl->second);
                }
                Rcpp::Rcerr << "Now, dep " << dep.back() << " ";
                for(int zz = 0; static_cast<unsigned int>(zz) < guard.back().size(); zz++) {
                    Rcpp::Rcerr << " guard " << guard.back()[zz]
                    << " value " << value.back()[zz] << std::endl;
                }
            }
            else   {
                Rcpp::Rcerr << "defaultList size is " << c->defaultList.size() << std::endl;
                for(std::list<std::pair<synExp*, synExp*> >::iterator itl = c->defaultList.begin();
                    itl != c->defaultList.end(); itl++) {
                    oneGuard.push_back(itl->first);
                    oneValue.push_back(itl->second);
                }
                lastDep = c->getDependsVar();
                // If a config has no defaults, we ignore it
                if (c->defaultList.size() > 0) {
                    guard.push_back(oneGuard);
                    value.push_back(oneValue);
                    dep.push_back(c->getDependsVar()->copy_exp());
                    lastIgnored = false;
                }
                else
                    lastIgnored = true;
        
            }
        }
        ui->destroy();
    }
    
    // Now for a bit of improvement.
    // If the defaults are the same we also join the dependencies
    for(int x = 1; static_cast<unsigned int>(x) < dep.size(); x++) {
        bool sameDef = true;
        if (guard[x].size() == guard[x-1].size()) {
            for(int z = 0; sameDef && static_cast<unsigned int>(z) < guard[x].size(); z++)
                sameDef &= (equal(guard[x][z], guard[x-1][z]) &&
                            equal(value[x][z], value[x-1][z]));
            if (sameDef) {
                // Entries x and x-1 have the same defaults
                // We join them
                Rcpp::Rcerr << "We join two entries with the same defaults" << std::endl;
                dep[x-1] = makeOr(dep[x-1], dep[x]);
                dep.erase(dep.begin()+x);
                guard.erase(guard.begin()+x);
                value.erase(value.begin()+x);
                x = x-1;
            }
        }
    }

    
    synExp* condition = makeNot(new synSymbol(name));
    bool lastConfig = true;
    std::set<std::string> members, choiceNames;
    if (dep.size() == 1) {
        bool continueTrying = true;
        for(int w = 0; static_cast<unsigned int>(w) < guard[0].size() && continueTrying; w++) {
            if (!guard[0][w]->isSymbol())
                continueTrying = false;
            else {
                std::string member = guard[0][w]->getSymbol();
                if (choiceInfo::isConfigChoiceMember(member)) {
                    members.insert(member);
                    choiceNames.insert(choiceInfo::getChoiceName(member));
                }
                else continueTrying = false;
            }
        }
        if (continueTrying && choiceNames.size() == 1) {

            std::string cn = *(choiceNames.begin());
            Rcpp::Rcerr << "ZZZ We could simplify this, cn " << cn <<  " members size "
                    << members.size() << std::endl;
        }
    }
    for(int q = 0; static_cast<unsigned int>(q) < dep.size(); q++) {
        synExp* orGuards = synFalse;
        synExp* def;
        if (lastConfig) {
            lastConfig = false;
            def = makeNot(new synSymbol(name));
        }
        else def = synTrue;
        for(int w = 0; static_cast<unsigned int>(w) < guard[q].size(); w++) {
            orGuards = makeOr(orGuards, guard[q][w]->copy_exp());
            synExp *defvalue;
            std::string sval = value[q][w]->get_string();
            // If the defining value is a string literal...
            if (value[q][w]->get_type() == synExp_String)
                defvalue = defaultWithString(sval);
            else
                // We have a symbol as defining value
                if (value[q][w]->get_type() == synExp_Symbol)
                    defvalue = defaultWithSymbol(value[q][w]->getSymbol());
                else
                    throw std::logic_error("Don't know what to do with default "+sval+"\n");
            def = makeIfThenElse(guard[q][w],
                                 defvalue->copy_exp(),
                                 def->copy_exp());
        }
        // If there is only one entry there's no need to repeat the dependencies
        
        if (dep.size() == 1)
            condition = def->copy_exp();
        else
            condition = makeIfThenElse(makeAnd(dep[q]->copy_exp(), orGuards),
                                       def,
                                       condition);
    }
    return condition;
}

bool          configInfo::checkSimple() {
    simple = true;
    for(configInfo* c : mapAlternative[name]) {
        if (c->defaultList.size() == 0)
            simple = false;
        for(std::list<std::pair<synExp*, synExp*> >::iterator itl = c->defaultList.begin();
            itl != c->defaultList.end(); itl++) {
            if (itl->second != synTrue)
                simple = false;
        }
    }
    return simple;
}


synExp*            configInfo::processAllLogicalDefaults() {
    // We need a list with the following things:
    // Dependencies, list of pairs <guard, value>
    std::vector<synExp*>              dep;
    std::vector<std::vector<synExp*>> guard;
    std::vector<std::vector<synExp*>> value;
    synExp* lastDep = synFalse;
    bool lastIgnored = false;

    for(configInfo* c : mapAlternative[name]) {
        std::vector<synExp*> oneGuard, oneValue;
        // If there is a streak of the same dependencies, we join them
        if (equal(c->getDependsVar(), lastDep) && !lastIgnored) {
            Rcpp::Rcerr << "Joining two dependencies 1 (" << c->getDependsVar() << ") and 2 (" << lastDep << ")" << std::endl;
            for(std::list<std::pair<synExp*, synExp*> >::iterator itl = c->defaultList.begin();
                itl != c->defaultList.end(); itl++) {
                guard.back().push_back(itl->first);
                value.back().push_back(itl->second);
            }
        }
        else {
        for(std::list<std::pair<synExp*, synExp*> >::iterator itl = c->defaultList.begin();
            itl != c->defaultList.end(); itl++) {
                oneGuard.push_back(itl->first);
                oneValue.push_back(itl->second);
            }
            lastDep = c->getDependsVar();
            // If a config has no defaults, we ignore it
            if (c->defaultList.size() > 0) {
                guard.push_back(oneGuard);
                value.push_back(oneValue);
                dep.push_back(c->getDependsVar()->copy_exp());
                lastIgnored = false;
            }
            else lastIgnored = true;

        }
    }
    
    // Now for a bit of improvement.
    // If the defaults are the same we also join the dependencies
    for(int x = 1; static_cast<unsigned int>(x) < dep.size(); x++) {
        bool sameDef = true;
        if (guard[x].size() == guard[x-1].size()) {
            for(int z = 0; sameDef && static_cast<unsigned int>(z) < guard[x].size(); z++)
                sameDef &= (equal(guard[x][z], guard[x-1][z]) &&
                            equal(value[x][z], value[x-1][z]));
            if (sameDef) {
            // Entries x and x-1 have the same defaults
            // We join them
            Rcpp::Rcerr << "We join two entries with the same defaults" << std::endl;
            dep[x-1] = makeOr(dep[x-1], dep[x]);
            dep.erase(dep.begin()+x);
            guard.erase(guard.begin()+x);
            value.erase(value.begin()+x);
            x = x-1;
            }
        }
    }
    if (dep.size() == 1 && guard[0].size() == 1 && guard[0][0] == synTrue && value[0][0] == synTrue)
        simple = true;
    else
        simple = false;
  
    synExp* condition = makeNot(new synSymbol(name));
    Rcpp::Rcerr << "*******" << std::endl;
    synExp* def = makeNot(new synSymbol(name));;
    synExp* orGuards = synFalse;
    for(int q = 0; static_cast<unsigned int>(q) < dep.size(); q++) {
        for(int w = 0; static_cast<unsigned int>(w) < guard[q].size(); w++) {
            orGuards = makeOr(orGuards, guard[q][w]->copy_exp());
            def = makeIfThenElse(guard[q][w], // last time we use it
                                 makeEqual(new synSymbol(name), value[q][w]),
                                 def);
        }
        condition = makeIfThenElse(makeAnd(dep[q]->copy_exp(), orGuards),
                                   def->copy_exp(),
                                   condition);
    }
    return condition;
}

synExp*            configInfo::processDependencies() {
    synExp* condition = synFalse;
    for(configInfo* c : mapAlternative[name])
        condition = makeOr(condition, c->getDependsVar()->copy_exp());
    
    return condition;
}

bool configInfo::checkRecursiveSelects() {
    std::vector<std::string> traversal;
    traversal.push_back(name);
    return checkRecursiveSelectsRecur(traversal);
}
bool configInfo::checkRecursiveSelectsRecur(std::vector<std::string> vec) {
    // We take the last element in the backtracking
    std::string sel = vec.back();
    // Recover the selects
    if (configsISelect.empty())
        return false;
    configInfo *c      = symbolTable::getSymbol(sel);
    if (c == NULL)
        throw std::logic_error("Error looking for symbol "+sel);
    
    std::set<std::string> varsels;
    for(std::string varsel : varsels) {
        if (std::find(vec.begin(), vec.end(), varsel) != vec.end()) {
            Rcpp::Rcerr << "Recursive select detected for config " << name << ":" << std::endl;
            bool firstTime = true;
            for(std::string s : vec)
                if (firstTime) {
                    firstTime = false;
                    Rcpp::Rcerr << "config " << s << " selects ";
                }
                else {
                    Rcpp::Rcerr << s << " which in turn selects on ";
                }
            Rcpp::Rcerr << varsel << std::endl;
            return true;
        }
        std::vector<std::string> localCopy = vec;
        localCopy.push_back(varsel);
        if (checkRecursiveSelectsRecur(localCopy))
            return true;
    }
    return false;
}

   
bool configInfo::checkRecursiveDeps() {
    std::vector<std::string> traversal;
    traversal.push_back(name);
    return checkRecursiveDepsRecur(traversal);
}

bool configInfo::checkRecursiveDepsRecur(std::vector<std::string> vec) {
    // We take the last element in the backtracking
    std::string dep = vec.back();
    // Recover the dependencies
    std::set<std::string> vardeps = symbolTable::getSymbol(dep)->getDependsVar()->giveSymbols();
    if (vardeps.empty())
        return false;
    
    for(std::string vardep : vardeps) {
        if (std::find(vec.begin(), vec.end(), vardep) != vec.end()) {
            Rcpp::Rcerr << "Recursive dependency detected for config " << name << ":" << std::endl;
            bool firstTime = true;
            for(std::string s : vec)
                if (firstTime) {
                    firstTime = false;
                    Rcpp::Rcerr << "config " << s << " depends on ";
                }
                else {
                    Rcpp::Rcerr << s << " which in turn depends on ";
                }
            Rcpp::Rcerr << vardep << std::endl;
            return true;
        }
        std::vector<std::string> localCopy = vec;
        localCopy.push_back(vardep);
        if (checkRecursiveDepsRecur(localCopy))
            return true;
    }
    return false;
}

std::list<synExp*> configInfo::giveLogicalConstraint() {
    std::list<synExp*> result;
    //mapAlternative[name].push_front(this);
    //processLogicalDefaults();

    if (timesProcessed[name] != symbolTable::timesDeclared(name))
        return result;
    

    Rcpp::Rcerr << "------------------------------------------------"  << std::endl;
    Rcpp::Rcerr << "Name " << name << " file definition " << getFile() << std::endl;
    Rcpp::Rcerr << "mapAlternative size " << mapAlternative[name].size() << std::endl;
    
    checkRecursiveSelects();
    
    synExp* innerIf = synFalse;
   
        checkInternal();
        checkSimple();
        //synExp *dependencies      = processDependencies();
        synExp *selectCondition   = processSelects();
        Rcpp::Rcerr << " env " << env << std::endl;
        synExp *userInput         = makeOr(env, processPrompts());
        synExp *defaultsCondition = processAllLogicalDefaults();
        artificial = internal;
        Rcpp::Rcerr << "Internal? " << internal << " simple? " << simple
        << " dummy? " << isDummy()  << " artificial? " << artificial << std::endl;
        
        Rcpp::Rcerr << "selectCondition "   << selectCondition   << std::endl;
        Rcpp::Rcerr << "userInput       "   << userInput         << std::endl;
        Rcpp::Rcerr << "defaultsCondition " << defaultsCondition << std::endl;
        Rcpp::Rcerr << "depStack ";
        
        for(synExp *s : getDepStack()) {
            Rcpp::Rcerr << s << " ";
        }
        Rcpp::Rcerr << std::endl;
    
        if (checkRecursiveDeps())
            innerIf = makeNot(new synSymbol(name));
        else
            innerIf  = makeIfThenElse(userInput, synTrue, defaultsCondition);
    
    Rcpp::Rcerr << "innerIf  " << innerIf  << std::endl;
    synExp* outerIf  = makeIfThenElse(selectCondition,
                                      new synSymbol(name),
                                      innerIf);
    Rcpp::Rcerr << "outerIf  " << outerIf  << std::endl;
    
    if (outerIf != synTrue)
        result.push_back(outerIf);
    return result;
}


void configInfo::processLogicalDefaults() {
    // In principle there is not a default value...
    Rcpp::Rcerr << "processDefaults " << name << " defaultList size " << defaultList.size() << std::endl;
        synExp *temp = synFalse;
        for(std::list<std::pair<synExp*, synExp*> >::iterator itl = defaultList.begin();
            itl != defaultList.end(); itl++) {
            synExp *guard = itl->first;
            synExp *value = itl->second;
            Rcpp::Rcerr << "guard " << guard << " value " << value << std::endl;
            temp = makeIfThenElse(guard->copy_exp(),
                                  value->copy_exp(),
                                  temp->copy_exp());
            Rcpp::Rcerr << "temp is " << temp << std::endl;
        }
        Rcpp::Rcerr << " 1 logicalDefault = " << temp << std::endl;
        logicalDefault = temp;
    
}

synExp* configInfo::defaultWithString(std::string sval) {

    synExp *defvalue = synTrue;
    // Sometimes default values include the value
    // of another string config in the form $(NAME)
    
    // We visit the alternatives for $NAME so we can build
    // the alternatives for this config
    
    // We will save a pair of the implier and the value
    // As in XX_EQ_A -> C_EQ_BEFORE_A_AFTER

    std::vector<std::pair<synExp*,std::string>> stack;
    if (sval == "") {
        //return new synSymbol(name);
        sval = "EMPTY";
        addValue(sval);
        return new synSymbol(buildNameValue(sval));
        //return makeNot(regularValue->copy_exp());
    }
    
    stack.push_back(std::pair<synExp*, std::string>(synTrue,sval));
    while (!stack.empty()) {
        sval = stack.back().second;
       
        stack.pop_back();
        
        std::size_t dollar;
        std::size_t rightParen = 0;
        // If the value includes a reference to another symbol's value
        // we investigate the default values to expand the possibilities
        dollar = sval.find('$');
        if (dollar != std::string::npos) {
            if (sval[dollar+1] == '(')
                rightParen = sval.find(')');
            else
                if (sval[dollar+1] == '{')
                    rightParen = sval.find('}');
            
        }
        if (dollar != std::string::npos && rightParen != std::string::npos) {
            std::string otherVar = sval.substr(dollar+2, rightParen - dollar -2);
            std::string before   = sval.substr(0, dollar);
            std::string after    = sval.substr(rightParen+1, sval.length() - rightParen);
            configInfo* other    = symbolTable::getSymbol(otherVar);
            synExp*     emptyval;
            if (other != NULL)
                    emptyval = makeImplies(makeNot(other->getValue()->copy_exp()),
                                  new synSymbol(buildNameValue(before+after)));
             else
                 emptyval = new synSymbol(buildNameValue(before+after));
            
            addValue(before+after);
            defvalue = makeAnd(defvalue, emptyval);
            // If the other symbol's info has not been
            // filled in yet...
            if (other != NULL && configInfo::stringValues[otherVar].empty())
                other->processAllStringDefaults();
            
            for(const std::string& osa : configInfo::stringValues[otherVar]) {
                std::string newValue;
                if (osa == "EMPTY")
                    newValue = before+after;
                else
                    newValue = before+osa+after;
                
                addValue(newValue);
                synExp *implier = new synSymbol(other->buildNameValue(osa));
                synExp *implied = new synSymbol(buildNameValue(newValue));

                defvalue = makeAnd(defvalue, makeImplies(implier, implied));
                Rcpp::Rcerr << "Pushing value " << newValue
                << " because of " << osa << std::endl;
                Rcpp::Rcerr << "so defvalue is  " << defvalue << std::endl;

            }
        }
        // If not, any possible reference to another string config
        // has been processed
        else {
            defvalue = makeAnd(defvalue, new synSymbol(buildNameValue(sval)));
            addValue(sval);
        }
    }
    return defvalue;
}

synExp* configInfo::defaultWithSymbol(std::string sval) {
    synExp *defvalue = synTrue;
        configInfo* confsym = symbolTable::getSymbol(sval);
        if (confsym->getVartype() != "string")
            throw std::logic_error("Using symbol "+sval+" of type "+
                                   confsym->getVartype()+
                                   " in the definition of symbol "
                                   +name +" which is of type string\n");
        
        // We access all the different values for the symbol
        synExp *translation = synTrue;
        for(std::string theVal : stringValues[sval]) {
            synExp *implier = new synSymbol(confsym->buildNameValue(theVal));
            synExp *implied = new synSymbol(buildNameValue(theVal));
            translation = makeAnd(translation,
                                  makeImplies(implier, implied));
            Rcpp::Rcerr << "Translation " << translation << std::endl;
            addValue(theVal);
        }
        // And a last bit. If the symbol is negated, we add the value EMPTY and imply it
        synExp *emptybit = makeEqual(new synSymbol(confsym->name),
                                     value->copy_exp());
        // We override the value
        defvalue = makeAnd(translation, emptybit);
        return defvalue;
}
void configInfo::processStringDefaults() {
    // In principle there is not a default value...
    synExp* temp = makeNot(value->copy_exp());
    for(std::pair<synExp*, synExp*> pair : defaultList) {
            synExp *guard = pair.first;
            synExp *defvalue = synTrue;
            std::string sval = pair.second->get_string();
            // If the defining value is a string literal...
            if (pair.second->get_type() == synExp_String)
                    defvalue = defaultWithString(sval);
            else
                // We have a symbol as defining value
                if (pair.second->get_type() == synExp_Symbol)
                    defvalue = defaultWithSymbol(pair.second->getSymbol());
                else
                    throw std::logic_error("Don't know what to do with default "+sval+"\n");
            temp = makeIfThenElse(guard->copy_exp(),
                                  defvalue->copy_exp(),
                                  temp->copy_exp());
        }
        stringDefault = temp;

}
std::string configInfo::buildNameValue(std::string s) {
    std::string newName = name + "_EQ_" + s;
    std::replace(newName.begin(), newName.end(), ' ', '_');
    std::replace(newName.begin(), newName.end(), '(', '_');
    std::replace(newName.begin(), newName.end(), ')', '_');
    std::replace(newName.begin(), newName.end(), '{', '_');
    std::replace(newName.begin(), newName.end(), '}', '_');
    return newName;
}



configInfo* configInfo::addValue(std::string s) {
    std::string newName;
    stringValues[name].insert(s);
    newName = buildNameValue(s);
    stringAlternatives[name].insert(newName);
    return  defineName(newName);
}

configInfo* configInfo::defineName(std::string newName) {
    if (symbolTable::getSymbol(newName) == NULL) {
        synSymbol* variant  = new synSymbol(newName);
        configInfo *c = new configInfo();
        c->setName(newName);
        c->setVartype("boolean");
        c->setValue(variant);
        c->setIsStringValue();
        c->artificial = true;
        symbolTable::declareSymbol(newName, c);
    }
    return symbolTable::getSymbol(newName);
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
    os << "Default:      "     << logicalDefault         << std::endl;
    os << "Select:       "     << std::endl;
    
    os << "Value:        "      << value                 << std::endl;
    os << "Visibility:   "      << visibilityVar         << std::endl;
    std::list<synExp*> theList = giveConstraint();
    os << "Constraint:   " << std::endl;
    for(std::list<synExp*>::iterator itl = theList.begin(); itl != theList.end(); itl++)
        os << *itl << std::endl;
}

