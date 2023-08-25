//
//  MenuBlock.cpp
//  myKconf
//
//  Created by David on 22/06/15.
//  Copyright (c) 2015 david. All rights reserved.
//

#include "MenuBlock.hpp"

std::ostream&          operator<<(std::ostream& os, const MenuBlock* m) {
    if (m == NULL) {
        os << "NULL" << std::endl;
    }
    else
        os << m->print(0);
    return os;
}

std::string MenuBlock::print(int x) const {
    
    std::ostringstream res;
    for(int l = 0; l < x; l++) res << " ";
    
    res << description << " (" << min << "," << max << ") size " << getSize() << " length " << getLength() << std::flush;
    if (getPrev() != NULL) res << "*" << getPrev()->getDescription();
    res << std::endl;
    //os << "Contents: ";
    //for(const std::string& s : m->contents)
    //    os << s << " ";
    //os << std::endl;
    
    if (subtree != NULL)
        res << subtree->print(x+10);
    if (next != NULL)
        res << next->print(x);
    
    return res.str();
}


