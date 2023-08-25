//
//  configInfo.h
//  myKconf
//
//  Created by david on 28/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __myKconf__configInfo__
#define __myKconf__configInfo__

#include <iostream>
#include <map>
#include <vector>

#include "varInfo.hpp"
#include "synExp.hpp"
#include "symbolTable.hpp"
#include "eval.hpp"

class configInfo : public varInfo {
    
    public :
    
    configInfo() {
        selectable    =  false;
        firstVar      =  secondVar = pos = 0;
        vartype       =  name      =  promptext = "";
        value         =  NULL;
        visibilityVar =  synTrue;
        promptGuard   =  synFalse;
        xdefault      =  prompt    = synFalse;
        resolve       =  false;
        hasConfig     =  false;
        selects       =  false;
    }
    
    ~configInfo()       {
        firstVar      = secondVar = 0;
        vartype       = name      =  promptext = "";
        value         = NULL;
        visibilityVar = synTrue;
        promptGuard   = synFalse;
        xdefault      = prompt = synFalse;
        }
    
    void clear()        {
        firstVar      = secondVar = 0;
        vartype       = name      =  promptext = "";
        value         = NULL;
        visibilityVar = synTrue;
        selectVar.clear();
        promptGuard   = synFalse;
        xdefault = prompt = synFalse;
        selectList.clear();
        }
    void        setPos(int x)       { pos = x;                      }
    int         getPos()            { return pos;                   }
    void        activatePrompt()    { prompt = synTrue;             }
    bool        isPrompt()          { return (prompt == synTrue);   }

    void        addSelect(const std::string& v, synExp *e2);
    void        addVisibility(synExp* e)           { visibilityVar = makeAnd(visibilityVar, e); }
    void        push_default(synExp* a, synExp* b) { defaultList.push_front(std::pair<synExp*, synExp*>(a, b)); }

    void            setName(const std::string& s)  { name         = s; value = new synSymbol(s); }
    void            setPrompText(const std::string& s)    { promptext    = s; }
    void            setPromptGuard(synExp *e)      { promptGuard  = e; }

    void            setVartype(const std::string s)      { vartype      = s; }
    void            setValue(synExp* e)            { value        = e; }
    void            setFirstVar(int b)             { firstVar     = b; }
    void            setSecondVar(int b)            { secondVar    = b; }
    void            setHasConfig()                 { hasConfig    = true; }
    
    synExp*         getVisibility()                { return visibilityVar; }
    std::string     getPrompText()                 { return promptext;     }
    synExp*         getPromptGuard()               { return promptGuard;   }
    std::string     getVartype()                   { return vartype;       }
    synExp*         getValue()                     { return value;         }
    std::string     getName()                      { return name;          }
    int             getFirstVar()                  { return firstVar;      }
    int             getLastVar()                   { return secondVar;     }
    bool            getHasConfig()                 { return hasConfig;     }
    
    
    std::list<synExp*>  giveConstraint();
    void processDefaults();
    int  selectSize() { return selectList.size(); }
    friend std::ostream& operator<<(std::ostream& os, configInfo& ci);
    void print(std::ostream& os);
 //   void            addToPrimalGraph(primalGraph& pg);
    void addSymbolsToPrimalGraph(const std::set<std::string>& theSet, primalGraph& pg);
    
    void addToMenu(const std::string& s)        { contents.push_back(s);    }
    void setSelectable()                        { selectable = true;        }
    void setSelects()                           { selects    = true;        }
    bool getSelects()                           { return selects;           }
    
//    std::vector<const std::string>::iterator    getContentBegin()  { return contents.begin();  }
//    std::vector<const std::string>::iterator    getContentEnd()    { return contents.end();    }
    int                                         getContentSize()   { return contents.size();   }

    
    private:
    
    int                                            pos;
    bool                                           selectable, selects;
    bool                                           hasConfig;
    int                                            firstVar, secondVar;
    std::string                                    vartype;
    std::string                                    name;
    std::string                                    promptext;
    synExp                                        *promptGuard, *visibilityVar;
    // A logic formula to express permitted values by default clauses
    std::list<std::pair<synExp*, synExp*> >        defaultList;
    synExp*                                        xdefault;
    // A logic formula to express permitted values input by the user
    synExp*                                        prompt;
    std::vector<synExp*>                           selectVar;
    std::vector<std::pair<std::string, synExp*> >  selectList;
    // The logic variable associated with this variable
    synExp*        value;
    // Is there a default value that can be assigned to the config or not
    bool              resolve;
    
    // Used for menus. The first and the last config/menuconfig of the menu.
    // Contents holds the all the configs between first and last, inclusive.
    // We use this to set up block sifting
    std::vector<std::string> contents;
    
};
#endif /* defined(__myKconf__configInfo__) */
