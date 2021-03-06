
#include "bddParserBuddy.hpp"

std::pair<SemanticsBuddy, int> bddParserBuddy::parseBdd(std::string expression, std::vector<RbddElem> rbddElems) {
	SemanticsBuddy semBuddy;
	
	std::string currentOperation;
	bool isFirst = true;
			
	while (expression.size() > 0) {
		bool isNeg = false;
		if (expression[0] == '!') {
			isNeg = true;
			expression = expression.substr(1);
		}
		
		std::string currentBDD = "";
		while ((expression[0] >= 'a' && expression[0] <= 'z') || (expression[0] >= 'A' && expression[0] <= 'Z') || (expression[0] >= '0' && expression[0] <= '9') || expression[0] == '_') {
			currentBDD += expression[0];
			expression = expression.substr(1);
		}
		
		RbddElem rbddElem = RbddUtils::getRbddElem(currentBDD, rbddElems);
		if (rbddElem.getAdapter() == NULL) {
			Rcpp::Rcerr << "There is not defined a BDD called " << currentBDD << std::endl;
			std::pair<SemanticsBuddy, int> result(semBuddy, 0);
			return result;
		}
		
		buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
		bool buddy = bAdapter != nullptr;
		
		if (!buddy && isFirst) {
			std::pair<SemanticsBuddy, int> result(semBuddy, -1);
			return result;
		} else if (!buddy && !isFirst) {
			Rcpp::Rcerr << "There are BDD defined with different BDD libraries." << std::endl;
			std::pair<SemanticsBuddy, int> result(semBuddy, 0);
			return result;
		}
		
		SemanticsBuddy currentSemBuddy(!bAdapter->getVar(currentBDD).main(), !bAdapter->getVar(currentBDD).module());
		if (isNeg) {
			SemanticsBuddy currentSemBuddy(!currentSemBuddy.main(), !currentSemBuddy.module());
		}
		if (currentOperation == "&&") {
			SemanticsBuddy semBuddy((semBuddy && currentSemBuddy).main(), (semBuddy && currentSemBuddy).module());
		} else if (currentOperation == "||") {
			SemanticsBuddy semBuddy((semBuddy || currentSemBuddy).main(), (semBuddy || currentSemBuddy).module());
		} else if (currentOperation == "==") {
			SemanticsBuddy semBuddy((semBuddy == currentSemBuddy).main(), (semBuddy == currentSemBuddy).module());
		} else if (currentOperation == "!=") {
			SemanticsBuddy semBuddy((semBuddy != currentSemBuddy).main(), (semBuddy != currentSemBuddy).module());
		} else if (currentOperation == "<") {
			SemanticsBuddy semBuddy((semBuddy < currentSemBuddy).main(), (semBuddy < currentSemBuddy).module());
		} else if (currentOperation == ">") {
			SemanticsBuddy semBuddy((semBuddy > currentSemBuddy).main(), (semBuddy > currentSemBuddy).module());
		} else if (currentOperation == "") {
			SemanticsBuddy semBuddy(currentSemBuddy.main(), currentSemBuddy.module());
		}
			
		currentOperation = "";
			
		while (expression.size() > 0 && expression[0] == ' ') {
			expression = expression.substr(1);
		}
			
		while (expression.size() > 0 && (expression[0] == '&' || expression[0] == '|' || expression[0] == '=' || expression[0] == '!' || expression[0] == '<' || expression[0] == '>')) {
			currentOperation += expression[0];
			expression = expression.substr(1);
		}
			
		while (expression.size() > 0 && expression[0] == ' ') {
			expression = expression.substr(1);
		}
	}
	
	std::pair<SemanticsBuddy, int> result(semBuddy, 1);
	return result;
}
