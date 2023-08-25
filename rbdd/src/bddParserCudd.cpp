
#include "bddParserCudd.hpp"

std::pair<SemanticsCudd, int> bddParserCudd::parseBdd(std::string expression, std::vector<RbddElem> rbddElems) {
	SemanticsCudd semCudd;
	
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
		
		if (rbddElem.getFactory() == NULL) {
			std::cerr << "There is not defined a BDD called " << currentBDD << std::endl;
			std::pair<SemanticsCudd, int> result(semCudd, 0);
			return result;
		}
		
		cuddFactory* cFactory = dynamic_cast<cuddFactory*> (rbddElem.getFactory());
		bool cudd = cFactory != nullptr;

		if (!cudd && isFirst) {
			std::pair<SemanticsCudd, int> result(semCudd, -1);
			return result;
		} else if (!cudd && !isFirst) {
			std::cerr << "There are BDD defined with different BDD libraries." << std::endl;
			std::pair<SemanticsCudd, int> result(semCudd, 0);
			return result;
		}
		
		SemanticsCudd currentSemCudd(!cFactory->getVarBDD(currentBDD).main(), !cFactory->getVarBDD(currentBDD).module());
		if (isNeg) {
			SemanticsCudd currentSemCudd(!currentSemCudd.main(), !currentSemCudd.module());
		}
		if (currentOperation == "&&") {
			SemanticsCudd semCudd((semCudd && currentSemCudd).main(), (semCudd && currentSemCudd).module());
		} else if (currentOperation == "||") {
			SemanticsCudd semCudd((semCudd || currentSemCudd).main(), (semCudd || currentSemCudd).module());
		} else if (currentOperation == "==") {
			SemanticsCudd semCudd((semCudd == currentSemCudd).main(), (semCudd == currentSemCudd).module());
		} else if (currentOperation == "!=") {
			SemanticsCudd semCudd((semCudd != currentSemCudd).main(), (semCudd != currentSemCudd).module());
		} else if (currentOperation == "<") {
			SemanticsCudd semCudd((semCudd < currentSemCudd).main(), (semCudd < currentSemCudd).module());
		} else if (currentOperation == ">") {
			SemanticsCudd semCudd((semCudd > currentSemCudd).main(), (semCudd > currentSemCudd).module());
		} else if (currentOperation == "") {
			SemanticsCudd semCudd(currentSemCudd.main(), currentSemCudd.module());
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
	
	std::pair<SemanticsCudd, int> result(semCudd, 1);
	return result;
}
