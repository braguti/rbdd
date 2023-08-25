//
//  varInfo.cpp
//  myKconf
//
//  Created by david on 12/11/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "varInfo.hpp"

std::ostream& operator<<(std::ostream& os, varInfo* ps) {
    if (ps == NULL) {
        os << "NULL";
        return os;
    }
    ps->print(os);
    os << "Menu:         "     << ps->menuName         << std::endl;
    os << "File:         "     << ps->file             << std::endl;
    os << "DependsVar:   "     << ps->dependsVar       << std::endl;
    os << "-----------------------------------------------------" << std::endl;
    return os;
}

void varInfo::print(std::ostream& os) {
    os << "Menu:         "     << menuName         << std::endl;
    os << "File:         "     << file             << std::endl;
    os << "DependsVar:   "     << dependsVar       << std::endl;
}

