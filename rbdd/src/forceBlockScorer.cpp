//
//  forceBlockScorer.cpp
//  myKconf
//
//  Created by david on 03/11/15.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "forceBlockScorer.hpp"

bool orderBlocks (const std::pair<double, MenuBlock*>& a,
                  const std::pair<double, MenuBlock*>& b) {
    return (a.first < b.first); }

void forceBlockScorer::forceBlocks() {
    buildTreeAll();
    int sc, other;
    double improvement;
    int origScore = computeSpan();
    Rcpp::Rcout << "Span Score: " << showHuman(origScore) << std::endl;
    do {
        sc = computeSpan();
        forceTree(tree);
        other = computeSpan();
        improvement = 100.0*(sc - other)/sc;
        Rcpp::Rcerr.precision(2);
        Rcpp::Rcerr << "Intermediate score: " << showHuman(other)
                  << " improvement "        << improvement << "%" << std::endl;
        tree->updateOrder(var2pos);
    } while (sc > other);
    Rcpp::Rcout.precision(2);
    Rcpp::Rcout << "Final score: " << showHuman(other)
                << " reduction "   << 100.0*(origScore - other)/origScore
                << "%"             << std::endl;
    deleteTree(tree);
}

void forceBlockScorer::forceTree(MenuBlock *top) {
    int seqMin = std::numeric_limits<int>::max();
    
    MenuBlock *t = top;
    std::list<MenuBlock*> seq;
    for(; t != NULL; t = t->getNext()) {
        seq.push_back(t);
        if (t->getMin() < seqMin)
            seqMin = t->getMin();
    }
    
    if (seqMin < 0) {
        Rcpp::stop("Error, block %s min %d", top->getDescription(), top->getMin());
    }
    forceSequence(seq, seqMin);
    top->updateOrder(var2pos);
    for(MenuBlock* p : seq) {
        if (p->getSubTree() != NULL) {
            forceTree(p->getSubTree());
        }
    }
}

void forceBlockScorer::forceSequence(const std::list<MenuBlock*>& seq, int min) {
    int                           span, newSpan;
    std::map<synExp*, double>     cog;
    std::map<MenuBlock*, double>  pos;
    do {
        span = computeSpan();
        
        for(synExp *s : pendingConstraints) {
            cog[s] = 0.0;
            for(int index : s->giveSymbolIndices())
                cog[s] += var2pos[index];
            if (s->giveSymbolIndices().size() != 0)
                cog[s] = cog[s] / s->giveSymbolIndices().size();
        }
        
        for(MenuBlock *m : seq) {
            pos[m] = 0.0;
            for(synExp *s : m->getSynExpAll())
                pos[m] += cog.at(s);
            if (m->getSynExpAll().size() != 0)
                pos[m] = pos[m] / m->getSynExpAll().size();
            
        }
        
        std::vector<std::pair<double, MenuBlock *> > temp;
        for(std::pair<MenuBlock*, double> p : pos)
            temp.push_back(std::pair<double, MenuBlock*>(p.second, p.first));
        
        std::sort(temp.begin(), temp.end(), orderBlocks);
        int counter = min;
        for(std::pair<double, MenuBlock*> p : temp) {
            p.second->updateOrder(var2pos);
            int increment = counter - p.second->getMin();
            p.second->addIncrement(var2pos, pos2var, increment);
            p.second->updateOrder(var2pos);
            counter += p.second->getSize();
        }
        newSpan = computeSpan();
    } while (newSpan < span);
    
}
