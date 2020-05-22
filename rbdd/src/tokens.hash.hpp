//
//  tokens.hash.hpp
//  OtherKconf
//
//  Created by david on 17/09/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __tokens_hash__
#define __tokens_hash__

#include "kconf_id.hpp"

class Perfect_Hash {
    public :
    static const struct kconf_id * kconf_id_lookup (const char *str, unsigned int len);
};


#endif
