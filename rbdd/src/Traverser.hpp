//
//  Traverser.hpp
//  myKconf
//
//  Created by david on 17/05/2018.
//  Copyright Â© 2018 david. All rights reserved.
//

#ifndef __Traverser__
#define __Traverser__

#include <stdio.h>
#include <sstream>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "cudd.h"
#include "cuddInt.h"
#include <cuddObj.hh>
#include "TravInfo.hpp"
#include "cuddAdapter.hpp"

class Comparator {
public:
    Comparator(cuddAdapter* a) : adapter(a) {};
    bool operator() (DdNode* a, DdNode* b) {
        return adapter->getLevel(a) < adapter->getLevel(b);
    }
    cuddAdapter *adapter;
};
    
template <typename T>
class Traverser {
    public :
    
    void    setFast() { fast = true; }
    void    setTrace() { trace = true; }
    Traverser(cuddAdapter* adapter,
              T resZero,
              T resOne,
              T (*combine)(int pindex, int tindex, int eindex,
                           const T& tr, const T& er, Rcpp::Function function_to_apply),
              bool fakeZeroLevel) :      comp(adapter),  adapter(adapter),
    resZero(resZero),
    resOne(resOne),
    combine(combine),
    fakeZeroLevel(fakeZeroLevel) {
        extendedCombiner = false;
        extcombine = NULL;
        one     = adapter->getOne();
        zero    = adapter->getZero();
        marked  = 0;
        deleted = 0;
        
    };
    // The extended combiner passes a pointer to the node in question to the combine function
    Traverser(cuddAdapter* adapter,
              T resZero,
              T resOne,
              T (*extcombine)(int pindex, int tindex, int eindex,
                              const T& tr, const T& er, int n, Rcpp::Function function_to_apply),
              bool fakeZeroLevel) :  comp(adapter), adapter(adapter),
    resZero(resZero),
    resOne(resOne),
    extcombine(extcombine),
    fakeZeroLevel(fakeZeroLevel) {
        extendedCombiner = true;
        combine = NULL;
        one     = adapter->getOne();
        zero    = adapter->getZero();
        marked  = 0;
        deleted = 0;
        
    };
    
    T traverse() {
        // First go, traverse BDD to count references
        trav_recur1(adapter->getBDD(adapter->getTraverseBDD()));
        // Second go, use combine
        DdNode*  n = adapter->getBDD(adapter->getTraverseBDD());
        trav_recur2(n);
        T* t  = storage[n].getRes();
        T res;
        int rindex = adapter->getLevel(n);
        // It makes no sense to call a extendedCombiner if the node
        // does not exist
        if (fakeZeroLevel && rindex != 0 && !extendedCombiner)
            res = combine(0, rindex, rindex, *t, *t, adapter->getFunction());
        else
            res = T(*t);
        incVisit(n);
        std::ostringstream ost;
        ost << marked << " nodes marked and " << deleted << " nodes deleted " << std::endl;
        std::string ms = ost.str();
        if (marked != deleted)
            throw std::logic_error(ms);
        return res;
    };
    
    T traverseMP(int numThreads) {
        // First go, traverse BDD to count references
        std::make_heap(nodeBuffer.begin(),nodeBuffer.end());
        fillBuffer(adapter->getBDD(adapter->getTraverseBDD()), numThreads);
        //
        std::thread* th  = new std::thread[numThreads];
        // Second, release the threads
        for(int i = 0; i < numThreads; i++) {
            th[i] = std::thread(&Traverser::computeNode, this, i);
        }
        for(int i = 0; i < numThreads; i++) {
            th[i].join();
        }
        DdNode*  n = adapter->getBDD(adapter->getTraverseBDD());
        T* t  = storage[n].getRes();
        T res;
        int rindex = adapter->getLevel(n);
        // It makes no sense to call a extendedCombiner if the node
        // does not exist
        if (fakeZeroLevel && rindex != 0 && !extendedCombiner)
            res = combine(0, rindex, rindex, *t, *t, adapter->getFunction());
        else
            res = T(*t);
        incVisit(n);
        /* std::ostringstream ost;
        ost << marked << " nodes marked and " << deleted << " nodes deleted " << std::endl;
        std::string ms = ost.str();
        if (marked != deleted)
            throw std::logic_error(ms);
         */
        return res;
    };
    

    
private:
    
    Comparator                                comp;
    int                                       marked, deleted;
    bool                                      extendedCombiner, fast;
    bool                                      trace = false;
    DdNode                                   *one,*zero;
    std::unordered_map<DdNode*, TravInfo<T>>  storage;
    cuddAdapter                              *adapter;
    T                                         resZero, resOne;
    std::mutex 		                          bufferMutex;
    
    std::vector<DdNode*>	                  nodeBuffer;
    
    T (*combine)(int pindex, int tindex, int eindex, const T& tr, const T& er, Rcpp::Function function_to_apply);
    T (*extcombine)(int pindex, int tindex, int eindex, const T& tr, const T& er, int node, Rcpp::Function function_to_apply);
	
    bool                                      fakeZeroLevel;
	
    bool levelCompare(DdNode* a, DdNode* b) {
        return adapter->getLevel(a) < adapter->getLevel(b);
    }
    
    void incVisit(DdNode* n, TravInfo<T>& ti) {
        ti.incVisit();
        if (ti.getVisit() == ti.getRef()) {
            deleteInfo(n);
        }
    }
    
    void incVisitMP(DdNode* n, TravInfo<T>& ti) {
        ti.incVisit();
        if (ti.getVisit() == ti.getRef()) {
            delete ti.getRes();
        }
    }
    
    void incVisit(DdNode* n) {
        storage[n].incVisit();
        if (storage[n].getVisit() == storage[n].getRef()) {
            deleteInfo(n);
        }
    }
    void setRes(DdNode* n, T t) {
        storage[n].setRes(t);
        marked++;
    }
 
    void deleteInfo(DdNode* n) {
        delete storage[n].getRes();
        storage.erase(n);
        deleted++;
    }
    void markNode(DdNode* n) {
        storage[n].setMark();
    }
    bool getMark(DdNode* n) {
        return storage[n].getMark();
    }
    void fillBuffer(DdNode *node, int numThreads) {
        DdNode *g1,*g0;
        TravInfo<T>& infonode = storage[node];
        infonode.incRef();
        if (infonode.getRef() > numThreads || node == zero || node == one)
            infonode.setUsedOften();
        // Already visited
        if (infonode.getRef() != 1)
            return;
        
        nodeBuffer.push_back(node);
        // Result is still not ready
        infonode.lockThenSem();
        infonode.lockElseSem();

        std::push_heap(nodeBuffer.begin(), nodeBuffer.end(), comp);
        if (node != zero && node != one) {
            cuddGetBranches(node, &g1, &g0);
            storage[g1].addParentThenSem(infonode);
            storage[g0].addParentElseSem(infonode);
            fillBuffer(g1, numThreads);
            fillBuffer(g0, numThreads);
        }
    }
    void computeNode(int id) {
        DdNode *g1,*g0;
        for(;;) {
            bufferMutex.lock();
            if (nodeBuffer.empty()) {
                bufferMutex.unlock();
                return;
            }
            DdNode *node = nodeBuffer.front();
            //std::cerr << "In thread id " << id << " buffer size " << nodeBuffer.size()
            //            << " node is " << node << " level " << adapter->getLevel(node)
            //            << " is zero " << (node == zero) << " is one " << (node == one )<<  std::endl;

            std::pop_heap(nodeBuffer.begin(), nodeBuffer.end(), comp);
            nodeBuffer.pop_back();
            bufferMutex.unlock();
            TravInfo<T>& infonode = storage[node];
            if (node == zero)  {
                infonode.setResMP(resZero);
                continue;
            }
            if (node == one) {
                infonode.setResMP(resOne);
                continue;
            }
            infonode.lockThenSem();
            infonode.lockElseSem();
            cuddGetBranches(node, &g1, &g0);
            TravInfo<T>& infog0 = storage[g0];
            TravInfo<T>& infog1 = storage[g1];
            T* ptr = infog1.getRes();
            T* per = infog0.getRes();
            if (extendedCombiner) {
                int pos = adapter->getNodePos(node);
                if (pos == -1) {
                    pos = adapter->addNode(node);
                }
                const T& temp = extcombine(adapter->getLevel(node),
                                     adapter->getLevel(g1),
                                     adapter->getLevel(g0),
                                     *ptr,
                                     *per, pos,
                                     adapter->getFunction());
                infonode.setResMP(temp);
            }
            else
            {
                const T& temp = combine(adapter->getLevel(node),
                                  adapter->getLevel(g1),
                                  adapter->getLevel(g0),
                                  *ptr,
                                  *per,
                                  adapter->getFunction());
                infonode.setResMP(temp);
            }
            if (infog0.getRef() == 1)
                incVisitMP(g0, infog0);
            else
                if (!fast) {
                    infog0.lockVisit();
                    incVisitMP(g0, infog0);
                    infog0.unlockVisit();
                }

            if (infog1.getRef() == 1)
                incVisitMP(g1, infog1);
            else
                if (!fast) {
                    infog1.lockVisit();
                    incVisitMP(g1, infog1);
                    infog1.unlockVisit();
                }
        }
    }
    void trav_recur1(DdNode* node) {
        DdNode *g1,*g0;
        TravInfo<T>& infonode = storage[node];
        infonode.incRef();
        if (infonode.getRef() != 1)
            return;
        
        if (node != zero && node != one) {
            cuddGetBranches(node, &g1, &g0);
            trav_recur1(g1);
            trav_recur1(g0);
        }
    }
    void trav_recur2(DdNode* node) {
        DdNode *g1,*g0;
        TravInfo<T>& infonode = storage[node];
        infonode.setMark();
        if (node == zero)  {
            infonode.setRes(resZero);
            marked++;
            return;
        }
        if (node == one) {
            infonode.setRes(resOne);
            marked++;
            return;
        }
        cuddGetBranches(node, &g1, &g0);
        TravInfo<T>& infog1 = storage[g1];
        if (!infog1.getMark())
            trav_recur2(g1);
        
        TravInfo<T>& infog0 = storage[g0];
        if (!infog0.getMark())
            trav_recur2(g0);
        
        if (extendedCombiner) {
            int pos = adapter->getNodePos(node);
            if (pos == -1) {
                pos = adapter->addNode(node);
            }
            infonode.setRes(extcombine(adapter->getLevel(node),
                                    adapter->getLevel(g1),
                                    adapter->getLevel(g0),
                                    *infog1.getRes(), *infog0.getRes(), pos, adapter->getFunction()));
        } else {
            infonode.setRes(combine(adapter->getLevel(node),
                             adapter->getLevel(g1),
                             adapter->getLevel(g0),
                              *infog1.getRes(), *infog0.getRes(), adapter->getFunction()));
        }
        if (trace) {
            Rcpp::Rcout << "Call to traverse. Level: " << adapter->getLevel(node) << /*", Step result: " << *infonode.getRes() <<*/ std::endl;
        }

        marked++;

        incVisit(g1, infog1);
        incVisit(g0, infog0);
    }
    
    void printStorage() {
        for(auto p : storage) {
            if (p.second.getRef() != p.second.getVisit())
                Rcpp::Rcerr << p.first << " ref " <<  p.second.getRef() << " vis "
                << p.second.getVisit() << std::endl;
        }
    }
};

template <typename T> inline
T traverse(bool trace, cuddAdapter* adapter,
              T resZero,
              T resOne,
              T (*combine)(int pindex, int tindex, int eindex,
                           const T& tr, const T& er, Rcpp::Function function_to_apply),
              bool fakeZeroLevel = true) {
                  Traverser<T> tr(adapter, resZero, resOne, combine, fakeZeroLevel);
                  if (trace) {
                        tr.setTrace();
                    }
                  return tr.traverse();
              };

template <typename T> inline
T traverseMP(bool trace, int threads, cuddAdapter* adapter,
             T resZero,
             T resOne,
             T (*combine)(int pindex, int tindex, int eindex,
                          const T& tr, const T& er, Rcpp::Function function_to_apply),
             bool fast,
             bool fakeZeroLevel = true) {
    Traverser<T> tr(adapter, resZero, resOne, combine, fakeZeroLevel);
    if (trace) {
        tr.setTrace();
    }
    if (fast)
        tr.setFast();
    
    return tr.traverseMP(threads);
};


template <typename T> inline
T traverse(bool trace, cuddAdapter* adapter,
           T resZero,
           T resOne,
           T (*extcombine)(int pindex, int tindex, int eindex,
                        const T& tr, const T& er, int node, Rcpp::Function function_to_apply),
            bool fakeZeroLevel = true) {
               Traverser<T> tr(adapter, resZero, resOne, extcombine, fakeZeroLevel);
               if (trace) {
                   tr.setTrace();
               }
               return tr.traverse();
           };

template <typename T> inline
T traverseMP(bool trace, int threads, cuddAdapter* adapter,
           T resZero,
           T resOne,
           T (*extcombine)(int pindex, int tindex, int eindex,
                           const T& tr, const T& er, int node, Rcpp::Function function_to_apply),
             bool fast,
             bool fakeZeroLevel = true) {
               Traverser<T> tr(adapter, resZero, resOne, extcombine, fakeZeroLevel);
               if (trace) {
                   tr.setTrace();
               }
            if (fast)
                tr.setFast();
               return tr.traverseMP(threads);
           };



#endif /* Traverser_hpp */