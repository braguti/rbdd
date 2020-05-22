//
//  SiftScorer.cpp
//  myKconf
//
//  Created by david on 03/11/15.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "SiftScorer.hpp"



void SiftScorer::siftGo(double threshold) {
    double sc, other, improvement;
    Rcpp::Rcerr.precision(2);
    double origScore = computeOrderScore1();
    Rcpp::Rcerr << "Order Score:          " << std::fixed << std::showbase << std::right
    << origScore << std::endl;
    // Step one: We build a tree of menublocks from the menublock structure
    buildTreeOut();
    other = computeOrderScore1();
    do {
        sc = other;
        tree = sift_sequence(tree, other);
        improvement = 100.0*(sc - other)/sc;
        if (improvement < 0.0000000001) improvement = 0;
        Rcpp::Rcerr << "Intermediate score: "
        << std::setw(10) << std::right << std::showbase << std::fixed << other
        << " improvement "        << std::right << std::setw(10)
        << std::showbase << std::fixed
        << improvement << "%" << std::endl;

    } while (improvement > threshold);
    if (abs(other - computeOrderScore1()) > 0.1) {
        std::ostringstream ost;
        ost << "Error. relative score " << other << " absolute score " << computeOrderScore1() << " dif "
        << abs(computeOrderScore1() - other) << std::endl;
    }
    Rcpp::Rcerr << "Final score:        " << std::setw(10) << std::showbase << other
    << " reduction   "        << std::right << std::setw(10)
    << 100.0*(origScore - other)/origScore << "%" << std::endl;
    deleteTree(tree);
}

MenuBlock* SiftScorer::sift_sequence(MenuBlock* top, double& sc) {
    int pos = 0;
    MenuBlock *t = top;
    std::list<MenuBlock*> seq;
    for(; t != NULL; t = t->getNext()) {
        t->setPos(pos++);
        seq.push_back(t);
    }
    for(MenuBlock* p : seq) {
        double oldsc = sc;
        sc = update_best_pos(p, seq.size()/2, sc);
        if (oldsc < sc) {
            Rcpp::stop("old score was better than this score");
        }
        
    }
    /* Find first block */
    for (;top->getPrev() != NULL ; top=top->getPrev());
    
    
    for(MenuBlock *trav = top; trav != NULL; trav = trav->getNext())
        if (trav->getSubTree() != NULL)
            trav->setSubTree(sift_sequence(trav->getSubTree(), sc));
        
    return top;
}



double SiftScorer::update_best_pos(MenuBlock* blk, int middlePos, double score) {
    double best    = score;
    int bestpos = 0;
    int dirIsUp = 1;
    int n;
    double rn;
    rn = score;
    /* Determine initial direction */
    if (blk->getPos() > middlePos)
        dirIsUp = 0;
    int min, max;
    min = -1;
    max = std::numeric_limits<int>::max();
    /* Move block back and forth */
    for (n=0 ; n<2 ; n++) {
        if (dirIsUp) {
            while ((blk->getPrev() != NULL) && blk->getPos() > min) {
                MenuBlock *pre = blk->getPrev();
                rn -= computeOrderScore2(blk->getSynExpOut(), pre->getSynExpOut());
                blockdown(blk->getPrev());
                rn += computeOrderScore2(blk->getSynExpOut(), pre->getSynExpOut());
                bestpos--;
                if (rn < best) {
                    best        = rn;
                    bestpos = 0;

                }
            }
        }
        else {
            while ((blk->getNext() != NULL) && blk->getPos() < max) {//  && (rn <= maxAllowed || first)) {
                MenuBlock *nex = blk->getNext();
                rn -= computeOrderScore2(blk->getSynExpOut(), nex->getSynExpOut());
                blockdown(blk);
                rn += computeOrderScore2(blk->getSynExpOut(), nex->getSynExpOut());
                bestpos++;
                if (rn < best) {
                    bestpos = 0;
                    best        = rn;
                }
            }
        }
        dirIsUp = !dirIsUp;
    }
    /* Move to best pos */
    while (bestpos < 0) {
        blockdown(blk);
        bestpos++;
    }
    while (bestpos > 0) {
        blockdown(blk->getPrev());
        bestpos--;
    }
    double temp = computeOrderScore1();

    if (abs(temp-best) > 0.1) {
        Rcpp::Rcerr << " best " << best << " score " << temp << " dif " << abs(temp-best) <<  std::endl;
        while (blk->getPrev() != NULL) {
            blockdown(blk->getPrev());
            bestpos--;
            Rcpp::Rcerr << "3 dif " << abs(computeOrderScore1()-best)
            << " bestpos " << bestpos << std::endl;
        }
        Rcpp::stop("");
    }
    return best;
}
