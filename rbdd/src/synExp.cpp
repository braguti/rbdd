//
//  synExp.cpp
//  myKconf
//
//  Created by david on 04/12/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#include "synExp.hpp"

std::vector<int> synExp::ocSoFar;
std::vector<int> synExp::weights;
//std::map<std::string&, std::set<synExp*> > synExp::inverse;

std::ostream& operator<<(std::ostream& os, synExp* ps) {
    if (ps == NULL) {
        os << "NULL";
        return os;
    }
    ps->print(os);
    return os;
}


void synExp::print(std::ostream& os) const {
    return;
}

void synConst::print(std::ostream& os) const {
    switch (get_const()) {
            case synConst_true   : os << "synTrue";   break;
            case synConst_false  : os << "synFalse";  break;
            case synConst_module : os << "synModule"; break;
        default: os << "Error, const is " << get_const() << std::endl;
    }
}

void synSymbol::print(std::ostream& os) const {
    os << getSymbol();
    //if (symbolTable::getSymbol(getSymbol()) != NULL)
    //    os << "[" << symbolTable::getSymbol(getSymbol())->getValue() << "]";
        
}

void synCompound::print(std::ostream& os) const {
    std::ostringstream ost1, ost2, ost3;
    std::string stFirst, stSecond, stThird;
    if (Ce1 == NULL || Ce2 == NULL || Ce3 == NULL) {
        std::cerr << "Null pointer in SynCompound::print" << std::endl;
        exit(-1);
    }
    if (Ce1 &&  (Ce1->opPriority() < opPriority() ||
        (Ce1->get_type() == synExp_Compound && Ce1->get_op() == Cop)))  {
        ost1 << Ce1;
        stFirst = ost1.str();
    }
    else {
        ost1 << "(" << Ce1 << ")";
        stFirst = ost1.str();
    }
    if (Ce2 && (Ce2->opPriority() < opPriority() ||
        (Ce2->get_type() == synExp_Compound && Ce2->get_op() == Cop)))  {
        ost2 << Ce2;
        stSecond = ost2.str();
    }
    else {
        ost2 << "(" << Ce2 << ")";
        stSecond = ost2.str();
    }
    if (Ce3 && (Ce3->opPriority() < opPriority() ||
        (Ce3->get_type() == synExp_Compound && Ce3->get_op() == Cop)))  {
        ost3 << Ce3;
        stThird = ost3.str();
    }
    else {
        ost3 << "(" << Ce3 << ")";
        stThird = ost3.str();
    }
    switch (Cop) {
        case synAnd :
            os << stFirst << " and " << stSecond; break;
        case synOr  :
            os << stFirst << " or "  << stSecond; break;
        case synXor  :
            os << stFirst << " xor "  << stSecond; break;
        case synNand  :
            os << stFirst << " nand "  << stSecond; break;
        case synNor  :
            os << stFirst << " nor "  << stSecond; break;
        case synXnor  :
            os << stFirst << " xnor "  << stSecond; break;
            
        case synNot :
            os << "not " << stFirst; break;
        case synImplies: {
            int len = os.width();
            //os << "len =" << len << " ";
            //os << stFirst  << " -> " <<  stSecond; break;
            if (stFirst.length() <= len/2 - 2) {
                os << std::right << std::setw(len/2 -2) << stFirst  << " -> "
                    << std::left  << std::setw(len/2 -2) << stSecond; break;
            }
            else {
                os  << std::setw(0) << stFirst << " -> " << std::setw(len - stFirst.length() - 4 - 1)
                    << stSecond; break;
            }
        }
        case synIfThenElse : {
            int len  = os.width();
            int len1 = stFirst.length();
            int len2 = stSecond.length();
            int len3 = stThird.length();
            //os << "len =" << len << " ";
            if (Ce3 == synTrue || Ce3 == NULL)
                os << std::setw(3) << "if " << stFirst << " then "
                   <<  std::setw(len - len1 - 10) << stSecond;
            else
                os << std::setw(3) << "if " << stFirst << " then " << stSecond
                   << " else " << std::setw(len - len1 - len2 - 9) << stThird;
            break;
        }
        case synEqual : os <<  stFirst << " = " << stSecond; break;
        default : os << "Error, operator is " << get_op() << std::endl;
    }
    
}

void synString::print(std::ostream& os) const {
    os << '"' << st << '"';
}

bool equal(synExp *e1, synExp *e2) {
    if ((e1 == NULL && e2 == NULL) || (e1 == synFalse && e2 == NULL) || (e1 == NULL && e2 == synFalse))
        return true;
    if (e1->get_type() != e2->get_type())
        return false;
    switch (e1->get_type()) {
        case synExp_Const : { return e1 == e2; }
        case synExp_Symbol :
        { return e1->getSymbol() == e2->getSymbol();}
        case synExp_Compound : {
            if (e1->get_op() != e2->get_op()) return false;
            if (e1->get_op() == synOr || e1->get_op() == synAnd)
                return (equal(e1->first(), e2->first()) && equal(e1->second(), e2->second()))
                    || (equal(e1->first(), e2->second()) && equal(e1->second(), e2->first()));
            
            return    (equal(e1->first(), e2->first())
                       && equal(e1->second(), e2->second())
                       && equal(e1->third(), e2->third()));
        }
        case synExp_String : { return e1->get_string() == e2->get_string(); }
    }
}


synConst *synConst::syncTrue, *synConst::syncFalse, *synConst::syncModule;

synExp *synTrue   = synConst::getSynTrue();
synExp *synModule = synConst::getSynModule();
synExp *synFalse  = synConst::getSynFalse();


synExp *makeAnd(synExp* e1, synExp* e2) {
    if (e1 == synTrue)                        return e2;
    if (e2 == synTrue)                        return e1;
    if (e1 == synFalse) {
        //destroy(e2);
        return synFalse;
    }
    if (e2 == synFalse) {
        //destroy(e1);
        return synFalse;
    }
    //if ((e1 == synFalse) || (e2 == synFalse)) return synFalse;
    if (equal(e1, e2))  {
        //destroy(e2);
        return e1;
    }
    if (equal(e1, makeNot(e2))) {
        //destroy(e1);
        //destroy(e2);
        return synFalse;
    }
    
    if (e1->giveOps().size() == 1 &&
        e1->giveOps().find(synAnd) != e1->giveOps().end() &&
        e2->giveOps().size() == 1 &&
        e2->giveOps().find(synAnd) != e2->giveOps().end()) { 
        //std::cout <<  "e1 "  << std::setw(50) << e1
        //          <<  " e2 " << std::setw(50) << e2 << std::endl;
        synExp* res = synTrue;
        std::set<std::string> soFar;
        for(std::set<std::string>::iterator it  = e1->giveSymbols().begin();
                                            it != e1->giveSymbols().end();it++)
            if (soFar.find(*it) == soFar.end()) {
                //std::cout << "1 *it " << *it << std::endl;
                res = makeAnd(res, new synSymbol(*it));
                soFar.insert(*it);
            }
        for(std::set<std::string>::iterator it  = e2->giveSymbols().begin();
            it != e2->giveSymbols().end();it++)
            if (soFar.find(*it) == soFar.end()) {
                //std::cout << "2 *it " << *it << std::endl;
                res = makeAnd(res, new synSymbol(*it));
                soFar.insert(*it);
            }
        //std::cout << " res " << std::setw(30) << res << std::endl;
        //destroy(e1);
        //destroy(e2);
        return res;

    }
    return new synCompound(synAnd, e1, e2);
}

synExp *makeOr(synExp* e1, synExp* e2) {
    if (e1 == synFalse) return e2;
    if (e2 == synFalse) return e1;
    if ((e1 == synTrue) || (e2 == synTrue)) return synTrue;
    if (equal(e1, e2)) return e1;
    if (equal(e1, makeNot(e2))) return synTrue;
    
    if (e1->giveOps().size() == 1 &&
        e1->giveOps().find(synOr) != e1->giveOps().end() &&
        e2->giveOps().size() == 1 &&
        e2->giveOps().find(synOr) != e2->giveOps().end()) {
        //std::cout <<  "e1 "  << std::setw(50) << e1
        //          <<  " e2 " << std::setw(50) << e2 << std::endl;
        synExp* res = synFalse;
        std::set<std::string> soFar;
        for(std::set<std::string>::iterator it  = e1->giveSymbols().begin();
            it != e1->giveSymbols().end();it++)
            if (soFar.find(*it) == soFar.end()) {
                //std::cout << "1 *it " << *it << std::endl;
                res = makeOr(res, new synSymbol(*it));
                soFar.insert(*it);
            }
        for(std::set<std::string>::iterator it  = e2->giveSymbols().begin();
            it != e2->giveSymbols().end();it++)
            if (soFar.find(*it) == soFar.end()) {
                //std::cout << "2 *it " << *it << std::endl;
                res = makeOr(res, new synSymbol(*it));
                soFar.insert(*it);
            }
        //std::cout << " res " << std::setw(30) << res << std::endl;
        return res;
        
    }
    return new synCompound(synOr, e1, e2);
}

synExp *makeImplies(synExp* e1, synExp* e2) {
    if (e1 == synFalse)  return synTrue;
    if (e1 == synTrue)   return e2;
    if (e1 == synModule) return e2;
    if (e2 == synFalse)  return makeNot(e1);
    if (e2 == synTrue)   return synTrue;
    return new synCompound(synImplies, e1, e2);
}


synExp *makeNot(synExp* e1) {
    if (e1 == synFalse)  return  synTrue;
    if (e1 == synTrue)   return  synFalse;
    if (e1 == synModule) return  synModule;
    
    if (e1->get_type() == synExp_Compound && e1->get_op() == synNot) return e1->first();
    return new synCompound(synNot, e1);
}

synExp *makeIfThenElse(synExp* e1, synExp* e2, synExp* e3 = synTrue) {
    if (e1 == synTrue)                    return e2;
    if (e1 == synModule)                  return e2;
    if (e1 == synFalse)                   return e3;
    if (e2 == synTrue  && e3 == synFalse) return e1;
    if (e2 == synFalse && e3 == synTrue)  return makeNot(e1);
    if (e2 == synFalse && e3 == synFalse) return synFalse;
    if (e2 == synTrue  && e3 == synTrue)  return synTrue;
    if (equal(e1, makeNot(e2)) && e3 == synTrue) return e2;
    if (e2 == synTrue) return makeIfThenElse(makeNot(e1),e3);
    
    return new synCompound(synIfThenElse, e1, e2, e3);
}

synExp *makeEqual(synExp *e1, synExp *e2) {
    if (e2 == synTrue)  return e1;
    if (e1 == synTrue)  return e2;
    if (e2 == synFalse) return makeNot(e1);
    if (e1 == synFalse) return makeNot(e2);
    return new synCompound(synEqual, e1, e2);
}

std::list<synExp *> expandImplication(synExp* e) {
    std::list<synExp*> res;
    std::vector<synExp *> stack;
    synExp* left;
    synExp* right;
    stack.push_back(e);
    while (! stack.empty()) {
        synExp *s = stack.back();
        stack.pop_back();
        left  = s->first();
        right = s->second();
        if (s->get_type()     == synExp_Compound  &&
            s->get_op()       == synImplies       &&
            right->get_type() == synExp_Compound  &&
            right->get_op()   == synAnd) {
            
            stack.push_back(makeImplies(s->first(), right->first()));
            stack.push_back(makeImplies(s->first(), right->second()));
        }
        else
            if (s->get_type()     == synExp_Compound  &&
                s->get_op()       == synAnd) {
                stack.push_back(s->first());
                stack.push_back(s->second());
            }
        
        else
            res.push_back(s);
    }
    return res;
}

// We can't recover the memory because it is shared
// For instance, inherited dependencies are shared

//void destroy(synExp* s) {
    //std::cerr << "Enter destroy " << s << std::endl;
//    if (s == NULL || s == synTrue || s == synModule || s == synFalse) return;
//    std::cerr << "Destruyendo " << s << std::endl << std::flush;
//    destroy(s->first());
//    destroy(s->second());
//    destroy(s->third());
//    delete s;
//}

//synCompound::~synCompound() {
//    destroy(Ce1);
//    destroy(Ce2);
//    destroy(Ce3);
//}

