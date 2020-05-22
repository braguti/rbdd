//
//  primalGraph.cpp
//  myKconf
//
//  Created by david on 16/12/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "primalGraph.hpp"
#include <iomanip>

std::set<std::string> primalGraph::getNeighbours(std::string s) {
    std::set<std::string> res;
    for(std::map<std::pair<std::string, std::string>, int>::iterator itm = pgraph.begin(); itm != pgraph.end(); itm++)
        if (itm->second > 0) {
            if (itm->first.first  == s) res.insert(itm->first.second);
            if (itm->first.second == s) res.insert(itm->first.first);
        }
    return res;
    
}

std::ostream& operator<<(std::ostream& os, const primalGraph& pg) {
    for(std::map<std::pair<std::string, std::string>, int>::const_iterator itm = pg.pgraph.begin(); itm != pg.pgraph.end();
            itm++)
        os << std::setw(50) << itm->first.first << " " << std::setw(50) << itm->first.second << " " << std::setw(10) << itm->second << std::endl;
    return os;
}
