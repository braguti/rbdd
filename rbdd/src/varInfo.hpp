//
//  varInfo.h
//  myKconf
//
//  Created by david on 12/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __myKconf__varInfo__
#define __myKconf__varInfo__

#include <string>
#include <iostream>
#include <list>

#include "synExp.hpp"
#include "primalGraph.hpp"

class Driver;

class varInfo {
    
    public:
    
    varInfo()       { dependsVar = synTrue; }
    ~varInfo()      {  }
    void clear()    {  }
    
    virtual void setPos(int x) {};
    virtual int getPos() { return 0;}
    
    virtual void setSelectable() {};
    virtual void print(std::ostream& os);
    virtual bool isChoice() { return false; }
    void    addDepends(synExp* dep)  { dependsVar = makeAnd(dependsVar, dep); };
    virtual void    addVisibility(synExp *e) {};
    virtual int     selectSize() { return 0;}
    friend  std::ostream& operator<<(std::ostream& os, varInfo* vi);
    
    virtual void            processDefaults() {};
    virtual bool            isPrompt() = 0;
    virtual std::list<synExp*>         giveConstraint()    = 0;
    virtual void            activatePrompt() { std::cerr << "In varInfo. Error" << std::endl;}
            void            setMenuName(std::string s) { menuName = s;}
    virtual void            setName(std::string s){}
    virtual void            setPrompText(std::string s){}
    virtual void            setPromptGuard(synExp* g) {};
    virtual void            setVartype(std::string s){}
            void            setFile(std::string s){ file = s; }
    virtual void            setValue(synExp* s) {}
    virtual void            setFirstVar(int b) {}
    virtual void            setSecondVar(int b) {}
    virtual void            setHasConfig() {};
    virtual void            push_default(synExp *a, synExp *b) {}
    virtual void            addSelect(std::string s, synExp *e) {}
    virtual synExp*         getVisibility()                { return synTrue; }

            synExp*         getDependsVar()      { return dependsVar; };
    virtual std::string     getVartype()   = 0;
    virtual synExp*         getValue()     = 0;
    virtual std::string     getName()      = 0;
    virtual std::string     getPrompText() = 0;
    virtual synExp*         getPromptGuard()     { return NULL; }
    virtual bool            getHasConfig()       { return false; }
    virtual void            addToPrimalGraph(primalGraph& pg) {};
    virtual void            addSymbolsToPrimalGraph(const std::set<std::string>& theSet, primalGraph& pg) {};
    
    
    private :
    
    std::string             file;
    std::string             menuName;
    synExp*                 dependsVar;
    
};
#endif /* defined(__myKconf__varInfo__) */
