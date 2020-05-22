//
//  PermScorer.cpp
//  myKconf
//
//  Created by david on 10/03/19.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "PermScorer.hpp"

void PermScorer::generateSequence(int n) {
    std::vector<int> res;
    if (n < 2) {
        Rcpp::stop("The argument to generateSequence must be at least 2. It is %d", n);
    }
    if (n == 2) {
        res.push_back(0);
        permSeq.push_back(res);
        return;
    }
    generateSequence(n-1);
    std::vector<int> recursive = permSeq.back();
    int polarity = -1;
    for(auto element : recursive) {
        for(int j = 0; j < n-1; j++)
            if (polarity == -1)
                res.push_back(n-j-2);
            else
                res.push_back(j);
        
        if (polarity == -1)
            res.push_back(element+1);
        else
            res.push_back(element);
        
        polarity *= -1;
    }
    for(int j = 0; j < n-1; j++)
        if (polarity == -1)
            res.push_back(n-j-2);
        else
            res.push_back(j);
    
    permSeq.push_back(res);
}


void PermScorer::permGo() {
    double sc, improvement;
    score = computeOrderScore1();
    double origScore = score;
    Rcpp::Rcerr.precision(2);
    Rcpp::Rcerr << "Order Score:           " << std::fixed << std::showbase << std::right
    << score << std::endl;
    // Step one: We build a tree of menublocks from the menublock structure
    buildTreeOut();
    score = computeOrderScore1();
    do {
        sc = score;
        tree = processTree(tree);
        improvement = 100.0*(sc - score)/sc;
        if (improvement < 0.0000000001)
            improvement = 0;
        Rcpp::Rcerr << "Intermediate score: "
        << std::setw(10) << std::right << std::showbase << std::fixed << score
        << " improvement "        << std::right << std::setw(10)
        << std::showbase << std::fixed
        << improvement << "%" << std::endl;
        
    } while (improvement > threshold);
    
    Rcpp::Rcerr << "Final score:        " << std::setw(10) << std::showbase << score
    << " reduction   "        << std::right << std::setw(10)
    << 100.0*(origScore - score)/origScore << "%" << std::endl;
    deleteTree(tree);
}


MenuBlock* PermScorer::processTree(MenuBlock *tree) {
    tree = slidingWindow(tree);
    
    for(MenuBlock *trav = tree; trav != NULL; trav = trav->getNext()) {
        if (trav->getSubTree() != NULL)
            trav->setSubTree(slidingWindow(trav->getSubTree()));
    }
    return tree;
}
    
MenuBlock* PermScorer::slidingWindow(MenuBlock* sequence) {
    MenuBlock* res = sequence;
    std::vector<MenuBlock*> vec;
    MenuBlock *before, *after;
    before = NULL;
    int members;
    bool firstTime = true;
    for(;;) {
        members = 0;
        while (members < num && sequence != NULL) {
            vec.push_back(sequence);
            sequence = sequence->getNext();
            members++;
        }
        // We always do it the first time regardless
        // unless there is only one of course
        if (members == 1 || (!firstTime && (members != num)))
            break;
        
        after    = sequence;
        sequence = applyBestPerm(before, after, vec);
        if (firstTime)
            res = sequence;
        vec.clear();
        before   = sequence;
        sequence = sequence->getNext();
        firstTime = false;
    }
    return res;
}
// The result is a pointer to the beginning of the window
MenuBlock* PermScorer::applyBestPerm(MenuBlock *before, MenuBlock *after,
                                     std::vector<MenuBlock*>& vec) {
    int start = vec.front()->getMin();
    // First we determine the best permutation
    std::vector<int> bestPerm = applyPerms(vec);
    // Now we fix the block links structure according to bestPerm
    // and the ordering

    MenuBlock* blk = vec[bestPerm[0]-1];
    MenuBlock* res = vec[bestPerm[0]-1];
    if (before != NULL)
        before->setNext(res);
    
    for(int i = 0; static_cast<unsigned int>(i) <  vec.size(); i++) {
        blk = vec[bestPerm[i]-1];
        int nowPos = blk->getMin();
        blk->addIncrement(var2pos, pos2var, start - nowPos);
        blk->setPrev(before);
        start += blk->getSize();
        before = blk;
        if (static_cast<unsigned int>(i) != vec.size() - 1) {
            blk->setNext(vec[bestPerm[i+1]-1]);
        }
        
    }
    blk->setNext(after);
    if (after != NULL) {
        after->setPrev(blk);
    }
    return res;
}
std::vector<int> PermScorer::applyPerms(std::vector<MenuBlock*> vec) {
    int members = vec.size();
    double rn       = score;
    // Permutations start being interesting at size 2
    std::vector<int> currentPerm = firstPerm[members-2];
    std::vector<int> bestPerm    = currentPerm;

    for(int c : permSeq[members-2]) {
        std::swap(currentPerm[c], currentPerm[c+1]);
        rn -= computeOrderScore2(vec[c]->getSynExpOut(), vec[c+1]->getSynExpOut());
        swapBlocks(vec[c], vec[c+1]);
        std::swap (vec[c], vec[c+1]);
        rn += computeOrderScore2(vec[c]->getSynExpOut(), vec[c+1]->getSynExpOut());
        if (rn < score) {
            score       = rn;
            bestPerm    = currentPerm;
        }
    }
    return bestPerm;
}

void PermScorer::swapBlocks(MenuBlock* left, MenuBlock* right) {
    left ->addIncrement(var2pos, pos2var, right->getSize());
    right->addIncrement(var2pos, pos2var, -left->getSize());
}
