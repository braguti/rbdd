//
//  symbolTable.hpp
//  myKconf
//
//  Created by david on 02/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#ifndef __myKconf__symbolTable__
#define __myKconf__symbolTable__

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>

class varInfo;

//class configInfo;

class symbolTable {
    
  private:
    
    static std::map<std::string, varInfo*> table;
    
  public:
    
    typedef std::map<std::string, varInfo*>::iterator iterator;
    static int      size()  { return table.size();  }
    static iterator begin() { return table.begin(); }
    static iterator end()   { return table.end();   }
    
    static  void     addSymbol(std::string s, varInfo* p);
    //static  configInfo *addString(std::string name, std::string value);
    static  varInfo *getSymbol(std::string s);
    
};

#endif /* defined(__myKconf__symbolTable__) */
