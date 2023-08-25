
#include "RbddElem.hpp"

RbddElem::RbddElem() {
}

std::string RbddElem::getName() {
	return this->name;
}

void RbddElem::setName(std::string name) {
	this->name = name;
}

int RbddElem::getNodenum() {
	return this->nodenum;
}

void RbddElem::setNodenum(int nodenum) {
	this->nodenum = nodenum;
}

int RbddElem::getCachesize() {
	return this->cachesize;
}

void RbddElem::setCachesize(int cachesize) {
	this->cachesize = cachesize;
}

vBDDFactory* RbddElem::getFactory() {
	return this->factory;
}

void RbddElem::resetFactory() {
	this->factory = NULL;
}

std::vector<synExp*> RbddElem::getSynExpMap() {
	return this->synExpMap;
}

void RbddElem::addSynExpMap(std::string variableName) {
	this->synExpMap.push_back(new synSymbol(variableName));
}

void RbddElem::addSynExpMap(synExp* variable) {
	this->synExpMap.push_back(variable);
}

void RbddElem::resetSynExpMap() {
	this->synExpMap.clear();
}

std::vector<synExp*> RbddElem::getVarMap() {
	return this->varMap;
}

void RbddElem::addVarMap(std::string variableName) {
	this->varMap.push_back(new synSymbol(variableName));
}

void RbddElem::resetVarMap() {
	this->varMap.clear();
}

std::vector<std::string> RbddElem::getNameList() {
	return nameList;
}

void RbddElem::addNameList(std::string variableName) {
	this->nameList.push_back(variableName);
}

void RbddElem::resetNameList() {
	this->nameList.clear();
}

void RbddElem::initFactory(std::string bddName, std::string library, int nodenum, int cachesize) {
	if (library == "buddy") {
		this->factory = new buddyFactory();
		this->factory->init(true, nodenum, cachesize);
	} else if (library == "cudd") {
		this->factory = new cuddFactory(1);
		this->factory->init(true, nodenum, cachesize);
	}

	this->synExpMap.clear();
	this->varMap.clear();
	this->nameList.clear();
	
	setName(bddName);
	setNodenum(nodenum);
	setCachesize(cachesize);
}
