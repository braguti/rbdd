//
//  eval.cpp
//  myKconf
//
//  Created by david on 15/10/14.
//  Copyright (c) 2014 david. All rights reserved.
//

#include "eval.hpp"

synExp *eval(synExp* exp) {
    if (exp == NULL) return NULL;
    switch (exp->get_type()) {
        case synExp_Const :  { return exp; }
        case synExp_Symbol : {
            std::string theSymbol = exp->getSymbol();
            varInfo* var = symbolTable::getSymbol(theSymbol);
            if (var->getValue() == NULL) {
                // The name of the symbol
                return exp;
            }
            else {
                // The value of the symbol
                synExp *res = var->getValue();
//                if (res == synFalse && var->getHasConfig()) {
//                    std::cerr << "Symbol " << std::setw(40) << exp << " value is "
//                              << synFalse << std::endl;
//                }
                return res;
            }
        }
        case synExp_Compound :
        {
            switch (exp->get_op()) {
                case synNot        : { return makeNot(eval(exp->first()));                               }
                case synAnd        : {
                    synExp *one   = eval(exp->first());
                    synExp *two   = eval(exp->second());
                    synExp *three = makeAnd(one, two);
                    return three;
                }
                case synOr         : {
                    synExp *one   = eval(exp->first());
                    synExp *two   = eval(exp->second());
                    synExp *three = makeOr(one, two);               
                    return three; }
                case synImplies    : {
                                        return makeImplies(eval(exp->first()), eval(exp->second())); }
                case synIfThenElse : { return makeIfThenElse(eval(exp->first()), eval(exp->second()), eval(exp->third())); }
                case synEqual      : { return makeEqual(eval(exp->first()), eval(exp->second())); }
            }
        }
        case synExp_String : { return exp; }
            
    }
    return NULL;
}