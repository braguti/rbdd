//
//  RandomScorer.cpp
//  myKconf
//
//  Created by david on 03/11/15.
//  Copyright Â© 2015 david. All rights reserved.
//

#include "RandomScorer.hpp"

void RandomScorer::randomHeuristic(double threshold) {
    double sc, other, improvement;
    sc = computeOrderScore1();
    Rcpp::Rcerr << "Random Initial score : " << showHuman(sc) << std::endl;
    // Step one: We build a tree of menublocks from the menublock structure
    
    
    buildTreeOut();
    tree->updateOrder(var2pos);
    randomHelper(tree, threshold);
    other       = computeOrderScore1();
    improvement = 100.0*(sc - other)/sc;
    Rcpp::Rcerr << "Final score : " << showHuman(other) << " improvement " << improvement <<  std::endl;
    deleteTree(tree);
}
void RandomScorer::randomHelper(MenuBlock *ptree, double threshold) {
    Rcpp::Rcerr << "Helping with " << ptree->getDescription() << std::endl;
    double sc, other, improvement;
    other = computeOrderScore1();

    do {
        sc          = other;
        ptree        = permutate(ptree);
        other       = computeOrderScore1();
        improvement = 100.0*(sc - other)/sc;
        Rcpp::Rcerr.precision(2);
        Rcpp::Rcerr << "Intermediate score: " << showHuman(other)
                  << " improvement "        << improvement << "%" << std::endl;
    } while (improvement > threshold);
    for(MenuBlock *Treeterator = ptree; Treeterator != NULL; Treeterator = Treeterator->getNext()) {
        if (Treeterator->getSubTree() != NULL)
            randomHelper(Treeterator->getSubTree(), threshold);
    }
    
}
MenuBlock* RandomScorer::permutate(MenuBlock* top) {
    double sc, other;
    int len, cmax = 0;
    int first;
    int second;
    int counter = 0;
    int ntimes = 0;
    MenuBlock *oneBlock, *otherBlock;
    other = computeOrderScore1();
    len    = top->getLength();
    
    if (len > 2) {
        do {
            sc     = other;
            second = 1 + 5 % (len - 1);
            first  = 5 % second;
            
            oneBlock = otherBlock = top;
            for(int x = 0; x < first; x++)  oneBlock   = oneBlock->getNext();
            for(int x = 0; x < second; x++) otherBlock = otherBlock->getNext();
            
            top = swapBlocks(oneBlock, otherBlock);
            other = computeOrderScore1();
            if (other >= sc) {
                top = swapBlocks(otherBlock, oneBlock);
                other = sc;
            }
            else {
                counter = 0;
            }
            if (counter > cmax) cmax = counter;
            if (ntimes % 10000 == 0)
                Rcpp::Rcerr << "xIntermediate score: " << showHuman(other)
                            << " cmax:" << std::setw(6) << cmax << " len " << std::setw(6) << len << std::endl;
            
            counter++;
            ntimes++;
            
        } while (counter < len);
    }
    MenuBlock* it = top;
    while (it->getPrev() != NULL)
        it = it->getPrev();
    
    while (it != NULL) {
        if (it->getSubTree() != NULL) {
            it->setSubTree(permutate(it->getSubTree()));
        }
        it = it->getNext();
    }
    it = top;
    while (it->getPrev() != NULL)
        it = it->getPrev();
    return it;
}

MenuBlock *RandomScorer::swapBlocks(MenuBlock* left, MenuBlock *right) {
    int leftsize = 0;
    int len = left->getLength();
    int cont = 0;
    for(MenuBlock *it = left->getNext(); it != right; it = it->getNext()) {
        Rcpp::Rcerr << "4 addIncrement menuBlock " << it->getDescription() << std::endl;
        if (it == NULL) {
            Rcpp::stop("it is NULL");
        }
        leftsize += it->getSize();
        Rcpp::Rcerr << "1 Incrementing " << it->getDescription() << " by " << right->getSize() - left->getSize() << " right->getSize() " << right->getSize()  << " left->getSize() "
            << left->getSize() << std::endl;
        it->addIncrement(var2pos, pos2var, right->getSize() - left->getSize());
        cont++;
        if (len < cont) {
            Rcpp::stop("Too many blocks, len %d cont %d", len, cont);
        }
    }
    
    Rcpp::Rcerr << "2 Incrementing " << left->getDescription() << " by " << leftsize+right->getSize() << std::endl;
    
    left->addIncrement(var2pos, pos2var, leftsize+right->getSize());
    
    Rcpp::Rcerr << "3 Incrementing " << right->getDescription() << " by " << -leftsize - left->getSize() << std::endl;
    
    right->addIncrement(var2pos, pos2var, -leftsize - left->getSize());
    
    /* Swap left and right data in the order */
    if (left->getNext() != right) {
        MenuBlock *x, *y;
        x = left->getNext();
        y = right->getPrev();
        left->setNext(right->getNext());
        right->setPrev(left->getPrev());
        left->setPrev(y);
        right->setNext(x);
    }
    else {
        left->setNext(right->getNext());
        right->setPrev(left->getPrev());
        left->setPrev(right);
        right->setNext(left);
        
    }
    
    if (right->getPrev() != NULL)
        right->getPrev()->setNext(right);
    
    if (right->getNext() != NULL)
        right->getNext()->setPrev(right);
    
    
    if (left->getNext() != NULL)
        left->getNext()->setPrev(left);
    
    if (left->getPrev() != NULL)
        left->getPrev()->setNext(left);
    
    
    int n = left->getPos();
    left->setPos(right->getPos());
    right->setPos(n);
    
    n = left->getLength();
    left->setLength(right->getLength());
    right->setLength(n);
    
    MenuBlock *p = left;
    while (p->getPrev() != NULL)
        p = p->getPrev();
    
    return p;
}
