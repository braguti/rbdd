//
//  symbolTable.cpp
//  myKconf
//
//  Created by david on 02/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "symbolTable.hpp"

std::map<std::string, varInfo*> symbolTable::table;

void symbolTable::addSymbol(std::string s,varInfo* ci) {
        table[s] = ci;
}

varInfo *symbolTable::getSymbol(std::string s) {
    if (table.find(s) != table.end())
        return table[s];
    //std::cerr << "symbolTable, looking for " << s << " which is not here." << std::endl;
    return NULL;
}


//configInfo *symbolTable::addString(std::string name)
//    synString *sS = new synString(value);
//    table[name] = sS;
//    return sS;
//}