//
//  MultiComponents.cpp
//  myKconf
//
//  Created by David on 01/12/15.
//  Copyright ?? 2015 david. All rights reserved.
//

#include "MultiComponents.hpp"

MultiComponents::MultiComponents(std::vector<int>& var2pos, std::vector<int>& pos2var) :
var2pos(var2pos), pos2var(pos2var){ init(var2pos.size());  numMultiComponents = var2pos.size(); }

void   MultiComponents::init(int num) {
    rank.clear();
    rank.resize(num);
    parent.clear();
    parent.resize(num);
    start.clear();
    start.resize(num);
    length.clear();
    length.resize(num);
    changed.clear();
    changed.resize(num);
    numMultiComponents = num;
    maxLength     = 1;
    outofsync     = false;
    for(int var = 0; var < num; var++)
        makeSet(var);
}

void MultiComponents::makeSet(int var) {
    parent[var]     = var;
    rank[var]       = 0;
    start[var]      = var2pos[var];
    length[var]     = 1;
    changed[var]    = 1;
}


void MultiComponents::changeOrder(std::vector<int> x) {
    pos2var = x;
    int c = 0;
    for(int v : pos2var)
        var2pos[v] = c++;
}

std::vector<int> MultiComponents::getOrder() {
    return pos2var;
}
void MultiComponents::newVariable() {
    int var = var2pos.size();
    numMultiComponents++;
    var2pos.push_back(var);
    pos2var.push_back(var);
    parent.push_back(var);
    rank.push_back(0);
    start.push_back(var);
    length.push_back(1);
    changed.push_back(1);
}

bool MultiComponents::alreadyJoined(int x, int y) {
    return (find(x) == find(y));
}
// Joins two components just as makeUnion, but also returns 
// the differential in entropy
float MultiComponents::joinEntropy(int x, int y) {
    float res;
    int  xRoot   = find(x);
    int  yRoot   = find(y);
    
    if (xRoot == yRoot)
        return 0;
    

    float pr1 = length.at(xRoot)/(float)var2pos.size();
    float pr2 = length.at(yRoot)/(float)var2pos.size();
    res =  pr1*log2(pr1) + pr2*log2(pr2);
    numMultiComponents--;
    int newRep, other;
    if (rank.at(xRoot) < rank.at(yRoot)) {
        newRep = xRoot;
        other  = yRoot;
    }
    else {
        newRep = yRoot;
        other  = xRoot;
        if (rank.at(xRoot) == rank.at(yRoot))
            rank.at(newRep)++;
    }
    parent.at(other)  = newRep;
    length.at(newRep) += length.at(other);
    if (length.at(newRep) > maxLength)
        maxLength = length.at(newRep);
    

    if (start.at(other) < start.at(newRep))
        start.at(newRep) = start.at(other);
    
    float pr = length.at(newRep)/(float)var2pos.size();
    res -= pr*log2(pr);
    return res;
}
bool MultiComponents::makeUnion(int x, int y, bool changeOrder) {
    bool changes = false;
    int  xRoot   = find(x);
    int  yRoot   = find(y);
    
    if (xRoot == yRoot)
        return changes;
    
    numMultiComponents--;
    int newRep, other;
    if (rank.at(xRoot) < rank.at(yRoot)) {
        newRep = xRoot;
        other  = yRoot;
    }
    else {
        newRep = yRoot;
        other  = xRoot;
        if (rank.at(xRoot) == rank.at(yRoot))
            rank.at(newRep)++;
    }
    parent.at(other)  = newRep;
    length.at(newRep) += length.at(other);
    if (length.at(newRep) > maxLength)
        maxLength = length.at(newRep);
    

    if (start.at(other) < start.at(newRep))
        start.at(newRep) = start.at(other);
    return true;
}

int MultiComponents::getLength(int x) { return length[x]; }

void MultiComponents::flip(int x) {
    std::list<int> temp;
    int pos = start[x];
    for(int c = 0; c != length[x]; c++) {
        temp.push_front(pos2var[pos++]);
    }
    pos = start[x];
    for(int var : temp) {
        pos2var[pos] = var;
        var2pos[var] = pos++;
    }
}

int MultiComponents::find(int x) {
    if (parent[x] == x)
        return x;
    
    return find(parent[x]);
}

bool MultiComponents::join(synExp* s, bool changeOrder) {
    played.insert(s);
    return internalJoin(s, changeOrder);
}
bool MultiComponents::internalJoin(synExp* s, bool changeOrder) {
    bool changes = false;
    //printComponents();
    std::set<int>::iterator its1 = s->giveSymbolIndices().begin();
    for(std::set<int>::iterator its2 = its1; its2 != s->giveSymbolIndices().end(); its2++)
            if (its1 != its2) {
                changes |= makeUnion(*its1, *its2, changeOrder);
                changed[find(*its1)] = 1;
            }
    return changes;
}
// Watch out. The first bit in the pair is a position, not a variable
// The set is of the for <position, length>
std::set<std::pair<int, int> > MultiComponents::listChangedComponents() {
    check();
    int l = 0;
    std::set<std::pair<int, int> > theSet;
    while (static_cast<unsigned int>(l) < pos2var.size()) {
        int ol = pos2var[l];
        int fol = find(ol);
        int ll = length[fol];
        if (changed[fol] == 1) {
            theSet.insert(std::pair<int, int>(l, ll));
        }
        l += ll;
    }
    return theSet;
}

// Watch out. The first bit in the pair is a position, not a variable
// The set is of the for <position, length>
std::set<std::pair<int, int> > MultiComponents::listComponents() {
    //check(order);
    int l = 0;
    std::set<std::pair<int, int> > theSet;
    while (static_cast<unsigned int>(l) < pos2var.size()) {
        int ol = pos2var[l];
        int fol = find(ol);
        int ll = length[fol];
        theSet.insert(std::pair<int, int>(l, ll));
        l += ll;
    }
    return theSet;
}

void MultiComponents::printComponents() {
    //check(order);
    int l = 0;
    std::set<std::pair<int, int> > theSet;
    while (static_cast<unsigned int>(l) < pos2var.size()) {
        Rcpp::Rcerr << "< ";
        int ol  = pos2var[l];
        int fol = find(ol);
        int ll  = length[fol];
        for(int a = 0; a < ll; a++) {
            Rcpp::Rcerr << pos2var[l++] << " ";
        }
        
        Rcpp::Rcerr << ">";
    }
}
void MultiComponents::setUnchanged(int x) {
     changed[find(x)] = 0;
}

void MultiComponents::showInfo(int x = 0) {
    int max;
    if (static_cast<unsigned int>(x+10) < parent.size()) max = x+30;
    else max = parent.size();
    for(int a = x; a < max; a++) {
        Rcpp::Rcerr << " pos " << std::setw(5) << a << " var " << std::setw(4) << pos2var[a]
                    << " component " << std::setw(4) << find(pos2var[a]) << " start "
                    << std::setw(4) << start[find(pos2var[a])]
                    << " length " << std::setw(4) << length[find(pos2var[a])] << std::endl;
    }
}

void MultiComponents::reorder() {
    int c = 0;
    std::map<int, float> avg;
    std::map<int, int>   num;
    for(int i : pos2var) {
        avg[find(i)] += var2pos.at(i);
        num[find(i)]++;
    }

    for(std::pair<int, int> p : num)
        avg[p.first] /= (float) p.second;
    

    std::vector<orderHelper> h;
    for(int i : pos2var) {
        h.push_back(orderHelper(i, var2pos.at(i), start[find(i)], avg.at(find(i))));
    }
    orderHelper o(0, 0, 0, 0);
    std::sort(h.begin(), h.end(), o);
    pos2var.clear();
    for(orderHelper& x : h) {
        pos2var.push_back(x.id);
        var2pos[x.id] = c++;
    }
    
    int pos = 0;
    while (static_cast<unsigned int>(pos) < pos2var.size()) {
        start.at(find(pos2var[pos])) = pos;
        pos += length.at(find(pos2var[pos]));
    }

}
void MultiComponents::check() {
    std::set<int> s;
    for(int q : pos2var) {
        s.insert(q);
    }
    if (s.size() != parent.size()) {
        showInfo(0);
        Rcpp::stop("Error. parent size %d only %d different values.", parent.size(), s.size());
    }
    int lastcomponent = -1, remaining = 0;
    for(int a = 0; static_cast<unsigned int>(a) < parent.size(); a++) {
        if (find(pos2var[a]) != lastcomponent) {
            lastcomponent = find(pos2var[a]);
            if (remaining != 0) {
                int x = 0;
                if (a > 5) x = a - 5;
                showInfo(x);
                Rcpp::stop("Error processing pos %d remaining %d", a, remaining);
            }
            if (a != start[find(pos2var[a])]) {
                int x = 0;
                if (a > 5) x = a - 5;
                showInfo(x);
                Rcpp::stop("Error processing pos %d start is %d", a, start[find(pos2var[a])]);
            }
            remaining = length[find(pos2var[a])] - 1;
        }
        else remaining--;
    }
}

void MultiComponents::replayConstraints() {
    Rcpp::Rcerr << "Replaying " << played.size() << " expressions ----------------------------------" << std::endl;
    for(auto exp : played) {
        internalJoin(exp, false);
    }
}
void MultiComponents::checkLengths() {
    int sumLengths = 0;
    std::set<std::pair<int, int> > theSet = listComponents();
    for(auto pair : theSet) {
        sumLengths += pair.second;
    }
    if (static_cast<unsigned int>(sumLengths) != var2pos.size()) {
        std::ostringstream ost;
        ost << "In checkLengths, sumLengths is "  << sumLengths << " and var2pos size is " << var2pos.size() << std::endl;
        throw std::logic_error(ost.str());
    }
}
float MultiComponents::computeEntropy() {
    reorder();
    check();
    int sumLengths = 0;
    std::set<std::pair<int, int> > theSet = listComponents();
    float res = 0;
    int  size = var2pos.size();
    for(auto pair : theSet) {
        float ss = pair.second;
        res += -ss*log2(ss/size);
        sumLengths += ss;
    }
    if (static_cast<unsigned int>(sumLengths) != var2pos.size()) {
        std::ostringstream ost;
        ost << "In computeEntropy, sumLengths is "  << sumLengths << " and var2pos size is " << var2pos.size() << std::endl;
        throw std::logic_error(ost.str());
    }
    return res /size;
}
