//
//  symbolTable.cpp
//  myKconf
//
//  Created by david on 02/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "symbolTable.hpp"
#include "configInfo.hpp"

std::map<std::string, configInfo*> symbolTable::table;
std::map<std::string, int>         symbolTable::mapTimesDeclared;
std::vector<configInfo*>           symbolTable::ordered;
std::set<std::string>              symbolTable::undefined;

std::vector<configInfo*>::iterator symbolTable::getIterator(std::string s) {
    configInfo *ci = table[s];
    if (std::find(ordered.begin(), ordered.end(), ci) != ordered.end())
        return std::find(ordered.begin(), ordered.end(), ci);
    
    throw std::logic_error("1 Error looking for symbol "+s);
}

int     symbolTable::timesDeclared(std::string s) {
    return mapTimesDeclared[s];
}
void symbolTable::declareSymbol(std::string s,configInfo* ci) {
    mapTimesDeclared[s]++;
    if (undefined.find(ci->getName()) != undefined.end())
        undefined.erase(ci->getName());
    
    std::vector<configInfo*>::iterator it = ordered.begin();
    while (it != ordered.end())
        if ((*it)->getName() == s) {
            it = ordered.erase(it);
        }
        else
            it++;
    ordered.push_back(ci);
    
    // Always do the assignment because the might have been added but not defined
    // and it is important that it is defined
    
    table[s] = ci;
}

void symbolTable::addSymbol(std::string s,configInfo* ci) {
    if (table.find(s) == table.end()) {
        undefined.insert(s);
    }
    table[s] = ci;

}
configInfo *symbolTable::getSymbol(std::string s) {
    if (table.find(s) != table.end())
        return table[s];
    return NULL;
}

void    symbolTable::deleteSymbol(std::string s) {
    ordered.erase(std::remove(ordered.begin(), ordered.end(), table[s]), ordered.end());
    table.erase(s);
}

