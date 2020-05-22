//
//  symbolTable.hpp
//  myKconf
//
//  Created by david on 02/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __symbolTable__
#define __symbolTable__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

class configInfo;

class symbolTable {
    
  private:
    
    static std::map<std::string, configInfo*> table;
    static std::map<std::string, int>         mapTimesDeclared;
    static std::vector<configInfo*>           ordered;
    static std::set<std::string>              undefined;

  public:
    
    static std::vector<configInfo*>::iterator getIterator(std::string s);
    
    typedef std::vector<configInfo*>::iterator iterator;
    static int      size()  { return ordered.size();  }
    static iterator begin() { return ordered.begin(); }
    static iterator end()   { return ordered.end();   }
    static std::set<std::string> getUndefined() { return undefined;}
    
    static  int     timesDeclared(std::string s);
    static  void    declareSymbol(std::string s, configInfo* p);
    static  void    addSymbol(std::string s, configInfo* p);
    static  void    deleteSymbol(std::string s);
    //static  configInfo *addString(std::string name, std::string value);
    static  configInfo *getSymbol(std::string s);
    
};

#endif /* defined(__symbolTable__) */
