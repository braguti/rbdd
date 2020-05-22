//
//  OtherScorer.cpp
//  myKconf
//
//  Created by david on 04/11/15.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "OtherScorer.hpp"

void OtherScorer::otherHeuristic(double threshold) {
    int total = 0, pivot = 0, l, r, ran, dir = 0, var = 0, swap, swaporder, theSpan = 0, newSpan, span, temp, temp2, last = 999999999;
    double sc, other, improvement = 1000;
    // First we want to find a constraint to improve
    
    do {
        sc = computeOrderScore1();
        for(int i = 0; static_cast<unsigned int>(i) < pendingConstraints.size() ;i++) {
            total = 0;
            probTable.clear();
            for(synExp* s : pendingConstraints) {
                s->computeMaxMin(var2pos);
                span = s->getMax() - s->getMin();
                total += span+1;
                probTable.push_back(total);
            }
            //ran = random() % total;
            ran = 5 % total;
            l   = 0;
            r   = probTable.size();
            bool keep = true;
            while (r-l > 1 && keep) {
                pivot = (r+l) / 2;
                if (probTable[pivot] > ran)
                    r = pivot;
                else
                    if (probTable[pivot] < ran)
                        l = pivot;
                if (probTable[pivot] == ran)
                    keep = false;
            }
            if (keep) {
                if (probTable[r] <= ran) pivot = r;
                else
                    if (probTable[l] <= ran) pivot = l;
            }
            synExp *myconst = pendingConstraints[pivot];
            theSpan =  myconst->getMax() - myconst->getMin();

            if (theSpan > 2) {
                int heads = 5 % 2;
                if (heads == 0) {
                    dir = -1;
                    var   = myconst->giveMaxInd(var2pos);
                }
                else {
                    dir = +1;
                    var   = myconst->giveMinInd(var2pos);
                }
                
                swaporder = var2pos[var] + dir*(5 % theSpan);
                swap      = pos2var[swaporder];
                
                temp = var2pos[var];
                var2pos[swap] = var2pos[var];
                var2pos[var]  = swaporder;
                
                pos2var[swaporder]  = var;
                pos2var[temp]       = swap;
                
                myconst->computeMaxMin(var2pos);
                newSpan =  myconst->getMax() - myconst->getMin();
                
                if (newSpan > theSpan) {
                    Rcpp::stop("span is growing");
                }
                other  = computeOrderScore1();
                
                if (other > last) {
                    temp  = var2pos[var];
                    
                    temp2 = var2pos[swap];
                    var2pos[swap] = var2pos[var];
                    var2pos[var]  = temp2;
                    
                    pos2var[temp2]  = var;
                    pos2var[temp]       = swap;
                }
                else {
                    last =  computeOrderScore1();
               }
            }
        }
        other       = computeOrderScore1();
        improvement = 100.0*(sc - other) / sc ;
        Rcpp::Rcerr << "score " << (int) other << " improvement " << improvement << "%" <<  std::endl;
    } while (improvement > threshold);
}
