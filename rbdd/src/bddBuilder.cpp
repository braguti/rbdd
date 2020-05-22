//
//  bddBuilder.cpp
//  myKconf
//
//  Created by david on 11/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "bddBuilder.hpp"

void bddBuilder::build(double inflation) {
    bool lastReorder      = true;
    int  nreorder         = 0;
    int  constLastReorder = 0;
    bool componentChanges = false;
    
    varFile.open(varFileName.c_str());
    statsFile.open(statsFileName.c_str());
    
    int  counter   = 1;
    //long beginning = get_cpu_time();
    int  lastNodes = 100;
    // Main loop. A new constraint is added to the BDD each time
    for(std::vector<synExp*>::iterator itl = consList.begin(); itl != consList.end(); itl++) {
        
        //long start = get_cpu_time();
        myOrder->reorderConstraints(itl, consList.end(), lastReorder || componentChanges);
        componentChanges = false;
        //myOrder->reorderConstraints(itl, consList.end(), lastReorder);
        synExp *pconst = *itl;
        printPartOne(pconst, counter);
        adapter->apply(pconst);
        //adapter->syncOrder();
        int nn = adapter->nodecount(adapter->getTraverseBDD());
        printPartTwo(nn);
        statsFile       << 100.0*counter/consList.size() << " " << nn << std::endl;
        lastReorder = false;
        if (counter > minConstraint ) {
            if (isReorderTime(counter, lastNodes, nn, constLastReorder, inflation)) {
                Rcpp::Rcout << " -> " << std::flush;
                adapter->syncOrder(adapter->getTraverseBDD());
                componentChanges = true;
                nn = adapter->nodecount();
                Rcpp::Rcout << std::setw(8) << showHuman(nn);
                lastReorder      = true;
                if (isReorderTime(counter, lastNodes, nn, constLastReorder, inflation)) {
                    Rcpp::Rcout << std::endl;
                    constLastReorder = counter;
                    nreorder++;
                    adapter->reorder();
                    Rcpp::Rcout << " => " << std::setw(8) << showHuman(adapter->nodecount());
                    writeNameOrder(adapter->giveOrder());
                }
            }
            
        }
        
        //if (!lastReorder) {
            Rcpp::Rcout << std::endl;
        //}
        
        lastNodes = adapter->nodecount();
        counter++;
        myOrder->thisOneIsProcessed(pconst);
        pconst->thisOneIsProcessed();
        pconst->destroy();
    }
    
    if (reorder && !lastReorder) {
        Rcpp::Rcout << /**get_timestamp()      <<*/ " Last reordering. Nodes "
                    << showHuman(lastNodes) << std::endl;
        Rcpp::Rcout << std::endl << "Syncing components..." << std::flush;
        // If nreorder is 0 we don't need to sync anything and we save the time of building the
        // interaction matrix, which could take a while
        adapter->syncOrder(adapter->getTraverseBDD());
        Rcpp::Rcout << "done" << std::endl << std::flush;
        adapter->reorder();
        Rcpp::Rcout << /**get_timestamp()          <<*/ " Reordering done. "
        << showHuman(adapter->nodecount())     << " nodes. " << std::endl << std::flush;
    }
    // Write the last order
    writeNameOrder(adapter->giveOrder());
    
    //adapter->destroyInternal(internal);
    statsFile.close();
}

void bddBuilder::printOrder(std::vector<int> ord) {
    Rcpp::Rcout << std::endl << std::setw(5) << ord.size() << "              ";
    for(std::vector<int>::iterator itv = ord.begin(); itv != ord.end(); itv++){
        Rcpp::Rcout << *itv << " ";
    }
    Rcpp::Rcout << std::endl;
}

void bddBuilder::writeNameOrder(const std::vector<std::string>& ord) {
    for(const std::string &s : ord)
        varFile << s << " ";
    
    varFile << std::endl;
}

void bddBuilder::printPartOne(synExp* pconst, int counter) {
    std::stringstream ost;
    //ost  << std::setw(95) << std::left << pconst;
    ost  << pconst;
    std::string stConst = ost.str();
    Rcpp::Rcout << " Cnstr #" << std::setw(5)     << counter      << " "
                << std::setw(3) << 100*counter/consList.size() << "% ";
    std::string s;
    s =  myOrder->giveInfo(pconst);
    Rcpp::Rcout << s;
    
    Rcpp::Rcout << ": ";
    if (stConst.length() <= static_cast<unsigned int>(lineLength)) {
        Rcpp::Rcout <<  std::setw(lineLength) << std::left << stConst;
    } else {
        Rcpp::Rcout <<   std::left <<  stConst  << std::endl
        << std::setw(38+lineLength+s.length());
    }
}

void bddBuilder::printPartTwo(int nn) {
    Rcpp::Rcout << std::right << " Nodes "        << std::setw(8) << std::right
                    << showHuman(nn)    << std::flush;
}


bool bddBuilder::isReorderTime(int counter, int lastNodes, int nn, int constLastReorder, double inflation) {
    return reorder && (counter > reorderFromConst) &&
    ((reorderEvery == 1 && reorder && nn > (1.0+inflation/100.0)*lastNodes && nn > minNodes && nn < maxNodes
      && counter - constLastReorder > reorderInterval)
     || (reorderEvery > 1 && counter % reorderEvery == 0));
    
}

