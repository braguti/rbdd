
#include "RbddUtils.hpp"

RbddElem RbddUtils::getRbddElem(std::string bddName, std::vector<RbddElem> rbddElems) {
	
	RbddElem result;
	
	bool found = false;
	int i = 0;

	while (i < (int) rbddElems.size() && !found) {
		RbddElem temp = rbddElems[i];
		found = bddName == temp.getName();
		if (found) {
			result = temp;
		}
		i++;
	}

	return result;
}

std::vector<RbddElem> RbddUtils::replaceRbddElem(RbddElem rbddElem, std::vector<RbddElem> rbddElems) {

	bool found = false;
	int i = 0;

	while (i < (int) rbddElems.size() && !found) {
		RbddElem temp = rbddElems[i];
		found = rbddElem.getName() == temp.getName();
		if (found) {
			rbddElems[i] = rbddElem;
		}
		i++;
	}

	return rbddElems;
}

std::vector<RbddElem> RbddUtils::deleteRbddElem(RbddElem rbddElem, std::vector<RbddElem> rbddElems) {

	bool found = false;
	int i = 0;

	while (i < (int) rbddElems.size() && !found) {
		RbddElem temp = rbddElems[i];
		found = rbddElem.getName() == temp.getName();
		if (found) {
			rbddElems.erase(rbddElems.begin() + i);
		}
		i++;
	}

	return rbddElems;
}
