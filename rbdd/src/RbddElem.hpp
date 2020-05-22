
#ifndef __RbddElem__
#define __RbddElem__

#include <string>
#include <map>
#include <vector>
#include "buddyAdapter.hpp"
#include "cuddAdapter.hpp"

class RbddElem {

    private:
        std::string name;
        int nodenum;
        int cachesize;
        BDDAdapter* adapter = NULL;
        std::vector<synExp*> synExpMap;
        std::vector<synExp*> varMap;
        std::vector<std::string> nameList;
    
    public:
        RbddElem();
        std::string getName();
        void setName(std::string name);
        int getNodenum();
        void setNodenum(int nodenum);
        int getCachesize();
        void setCachesize(int cachesize);
        BDDAdapter* getAdapter();
        void setAdapter(BDDAdapter* adapter);
        std::vector<synExp*> getSynExpMap();
        void addSynExpMap(std::string variableName);
        void addSynExpMap(synExp* variable);
        void resetSynExpMap();
        std::vector<synExp*> getVarMap();
        void addVarMap(std::string variableName);
        void resetVarMap();
        std::vector<std::string> getNameList();
        void addNameList(std::string variableName);
        void resetNameList();
        void initAdapter(std::string bddName, std::string library, int nodenum, int cachesize);
};

#endif
