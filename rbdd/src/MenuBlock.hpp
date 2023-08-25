//
//  MenuBlock.h
//  myKconf
//
//  Created by David on 22/06/15.
//  Copyright (c) 2015 david. All rights reserved.
//

#ifndef __myKconf__MenuBlock__
#define __myKconf__MenuBlock__

#include <stdio.h>
#include <string>
#include <set>
#include <sstream>
#include <vector>
#include <deque>
#include <exception>

#include "symbolTable.hpp"
#include "varInfo.hpp"


class MenuBlock {
    
private:
    std::set<int>               indices;
    std::string                 first, last, description, type;
    int max, min;
    std::deque<std::string>     contents;
    MenuBlock                  *next, *prev, *subtree;
    int                         seqlength, pos;
    std::vector<synExp*>        myExpAll, myExpOut;
    
public:
    std::deque<std::string>& getContents() { return contents; }
    const std::vector<synExp*>& getSynExpAll() { return myExpAll;     }
    const std::vector<synExp*>& getSynExpOut() { return myExpOut;     }
    std::string getType() { return type; }
    void  setLength(int x)                     { seqlength = x;    }
    void  setPos(int x)                        { pos = x;          }
    int   getLength()        const             { return seqlength; }
    int   getPos()                             { return pos;       }
    std::string print(int x) const ;
    
    template <class Q> void computeIndices(const Q& q) {
        indices.clear();
        for(std::string& s : contents) {
            if (q.find(s) != q.end())
                indices.insert(q.at(s));
        }
    }
    
    template <class Q>    MenuBlock(Q beg, Q end, const std::string& d, std::string type)
    : description(d), type(type) {
       std::insert_iterator<std::deque<std::string> > it(contents, contents.begin());
       std::copy (beg, end, it);
       next      = prev = subtree = NULL;
       seqlength = contents.size();
       pos       = 0;
    
    }
    
    template <class T> void buildExpAll(const T& exp) {
        //std::cerr << "Starting block " << description << ": contents " << std::endl;
        //for(int i : indices)
        //    std::cerr << i << " ";
        //std::cerr << std::endl;
        myExpAll.clear();
        // Here we build a vector of the synExp involving the vars in this block
        for(synExp *s : exp) {
           bool found    = false;
            for(int index : s->giveSymbolIndices()) {
          //      std::cerr << "Looking for index " << index << " in this block" << std::endl;
                if (indices.find(index) != indices.end()) {
                   found = true;
                    break;
                }
            }
            //std::cerr << "Found " << found << " allfound " << allfound << std::endl;
//           for(std::string ss : s->giveSymbols())
//               if (std::find(contents.begin(), contents.end(), ss) != contents.end()) {
//                   found = true;
//                   //std::cerr << "exp " << s << std::endl;
//               }
               //else allfound = false;
           if (found) myExpAll.push_back(s);
       }
                                                
    }
    
    template <class T> void buildExpOut(const T& exp) {
        //std::cerr << "Starting block " << description << ": contents " << std::endl;
        //for(int i : indices)
        //    std::cerr << i << " ";
        //std::cerr << std::endl;
        myExpOut.clear();
        // Here we build a vector of the synExp involving the vars in this block
        for(synExp *s : exp) {
            bool found    = false;
            bool allfound = true;
            for(int index : s->giveSymbolIndices()) {
                //      std::cerr << "Looking for index " << index << " in this block" << std::endl;
                if (indices.find(index) != indices.end())
                    found = true;
                else
                    allfound = false;
            }
            //if (found)
            //    std::cerr << s << " Found " << found << " allfound " << allfound << std::endl;
            //           for(std::string ss : s->giveSymbols())
            //               if (std::find(contents.begin(), contents.end(), ss) != contents.end()) {
            //                   found = true;
            //                   //std::cerr << "exp " << s << std::endl;
            //               }
            //else allfound = false;
            if (found && !allfound) myExpOut.push_back(s);
        }
        
    }
    const std::set<int>& getIndices()      const            { return indices;           }
    const std::string& getFirst()          const            { return first;             }
    const std::string& getLast()           const            { return last;              }
    const std::string& getDescription()    const            { return description;       }
    const int          getMin()            const            { return min;               }
    const int          getMax()            const            { return max;               }
          MenuBlock*   getNext()           const            { return next;              }
          MenuBlock*   getPrev()           const            { return prev;              }
          MenuBlock*   getSubTree()        const            { return subtree;           }
    void               setNext(MenuBlock *s)                { next = s;                 }
    void               setPrev(MenuBlock *p)                { prev = p;                 }
    void               setSubTree(MenuBlock *e)             { subtree = e;              }
    
    friend  std::ostream&          operator<<(std::ostream& os, const MenuBlock* m);

    template <class Q> bool fix(const Q& q) {
        //std::cerr << "Fixing " << description << std::endl;
        //std::cerr << "Contents: " << std::endl;
        //for(std::string & s : contents) std::cerr << s << " ";
        //std::cerr << std::endl;
        varInfo *v;
        if (!contents.empty()) {
                        // Manage front
            v = symbolTable::getSymbol(contents.front());
            while (!contents.empty() && (v->getValue()->get_type() == synExp_Const
                                         || (v->getVartype() != "boolean" && v->getVartype() != "tristate")
                                         || q.find(contents.front()) == q.end())
                   ) {
                contents.pop_front();
                if (!contents.empty())
                    v = symbolTable::getSymbol(contents.front());
            }
        }
        if (!contents.empty()) {            // Manage back
            v = symbolTable::getSymbol(contents.back());
            while (!contents.empty() && (v->getValue()->get_type() == synExp_Const
                                         || (v->getVartype() != "boolean" && v->getVartype() != "tristate")
                                         || q.find(contents.back()) == q.end())
                   )
            {
                contents.pop_back();
                if (!contents.empty())
                    v = symbolTable::getSymbol(contents.back());
            }
        }
        //std::cerr << "Final size " << contents.size() << std::endl;
        return contents.size() > 1;
        
    }

    template <class Q> void update(const Q& q) {
        max = -1;
        min = std::numeric_limits<int>::max();
        
        for(const std::string& s : contents)  {
                if (q.find(s) != q.end() && q.at(s) > max) {
                    last = s;
                    max  = q.at(s);
                }
                if (q.find(s) != q.end() && q.at(s) < min) {
                    first = s;
                    min   = q.at(s);
                }
        }
        if (next != NULL) next->update(q);
        if (subtree != NULL) subtree->update(q);
    }
    template <class Q> void updateOrder(const Q& var2pos) {
        max = -1;
        min = std::numeric_limits<int>::max();
        
        for(int index : indices)  {
            //std::cerr << "index : " << index << " position: " << var2pos[index] << std::endl;
            if (var2pos.at(index) > max) {
                max  = var2pos.at(index);
            }
            if (var2pos.at(index) < min) {
                min   = var2pos.at(index);
            }
        }
        if (next != NULL)    next->updateOrder(var2pos);
        if (subtree != NULL) subtree->updateOrder(var2pos);
    }

    int  getSize() const {
        return max - min + 1;
    }
    
    template <class Q> void addIncrement(Q& var2pos, Q& pos2var, int delta) {
        for(int var : indices) {
            if (var2pos[var] + delta < 0) {
                std::cerr << "Error addIncrement index " << var << "from " << var2pos[var] << " to " << var2pos[var] + delta << std::endl;
                exit(-1);
            }
            int pos = var2pos[var];
            var2pos[var] += delta;
            pos2var[pos+delta] = var;
        }
        //int last = 0;
        //for(std::string s : contents)
         //   if (q.find(s) != q.end()) {
                //std::cerr << "Incrementing " << std::setw(60) << s << " from " << q[s] << " to "
                //          << q[s] + delta    << std::endl;
                //if (q[s] != last+1) std::cerr << "Non-Consecutive" << std::endl;
         //       last = q[s];
          //      q[s] += delta;
         //        if (q[s] < 0) {
          //          std::cerr << "Error setting "  << s           << " to position " << q[s]
          //                    << " in block "      << description << std::endl;
          //          exit(-1);
          //      }
          //  }
    }
    
};
#endif /* defined(__myKconf__MenuBlock__) */
