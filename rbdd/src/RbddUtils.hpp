
#ifndef __RbddUtils__
#define __RbddUtils__

#include "RbddElem.hpp"

class RbddUtils {

    public:
        static RbddElem getRbddElem(std::string bddName, std::vector<RbddElem> rbddElems);
        static std::vector<RbddElem> replaceRbddElem(RbddElem rbddElem, std::vector<RbddElem> rbddElems);
        static std::vector<RbddElem> deleteRbddElem(RbddElem rbddElem, std::vector<RbddElem> rbddElems);
};

#endif
