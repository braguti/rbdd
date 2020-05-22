//
//  Scorer.cpp
//  myKconf
//
//  Created by david on 03/11/15.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "Scorer.hpp"

void Scorer::check() {
    std::set<int> ctl;
    for(int v : pos2var) {
        if (ctl.find(v) != ctl.end()) {
            Rcpp::Rcerr << "var " << v << " is repeated in pos2var" << std::endl;
            for(std::pair<std::string, int> p : varMap)
                if (p.second == v) {
                    Rcpp::Rcerr << " var is " << p.first << std::endl;
                }
            throw std::logic_error(" repetition in pos2var");
        }
        ctl.insert(v);
    }
    if (ctl.size() != pos2var.size()) {
        Rcpp::Rcerr << "1 Siftscorer check size is " << ctl.size()
        << " pos2var size is " << pos2var.size()
        << std::endl;
        for(int x = 0; static_cast<unsigned int>(x) < pos2var.size(); x++)
            if (ctl.find(x) == ctl.end()) {
                Rcpp::Rcerr << std::setw(5) << x << " is not in check" << std::endl;
                throw std::logic_error(" is not in check");
                
            }
    }
}
double Scorer::computeOrderScore1() {
    double tot = 0;
    double x = 0;
    for(synExp* s :  pendingConstraints)
        if (s != synTrue) {
            x = computeExp(s);
            tot += x;
        }
    return tot;
}

double Scorer::computeOrderScore2(const std::vector<synExp*>& v, const std::vector<synExp*>& w) {
    double tot = 0;
    for(synExp* s :  v) {
        tot += computeExp(s);
    }
    
    for(synExp* s :  w) {
        if (std::find(v.begin(), v.end(), s) == v.end()) {
            tot += computeExp(s);
        }
    }
    return tot;
}

int Scorer::computeSpan() {
    int res = 0;
    for(synExp *s : pendingConstraints) {
        int max = -1;
        int min = std::numeric_limits<int>::max();
        //for(const std::string & ss : s->giveSymbols()) {
        for(int x : s->giveSymbolIndices()) {
            int pos = pos2var[x];
            if (pos < min)
                min = pos;
            if (pos > max)
                max = pos;
        }
        if (max < min) {
            Rcpp::stop("Negative span in expression");
        }
        res += max - min;
    }
    return res;
}

double Scorer::computeExp(synExp *s) const {
    return scoreTable[s->computeSpan(var2pos)+1];
    //return log(s->computeSpan(var2pos)+1);
    
}
