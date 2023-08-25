
#include "parserCNF.hpp"

ParserCNF::ParserCNF(std::vector<synExp*> synExpMap) {
	this->synExpMap = synExpMap;
}

synExp* ParserCNF::parse(std::string expression) {
	bool first = true;
	bool remain = true;
	synExp* result;

	do {
		std::string orExpr = removeSpaces(expression.substr(0, expression.find(" 0")));
		synExp* orResult = makeOrOperation(orExpr);

		if (orResult == NULL) {
			return NULL;
		}
		
		if (!first) {
			result = makeAnd(result, orResult);
		} else {
			result = orResult;
			first = false;
		}

		if ((expression.find(" 0") != (expression.size() - 2)) && (expression.size() > (expression.find(" 0") + 4))) {
			expression = expression.substr(expression.find(" 0") + 2);
			remain = true;
		} else {
			remain = false;
		}
	} while (remain);

	return result;
}

std::string ParserCNF::removeSpaces(std::string expression) {
	while (expression[0] == ' ') {
		expression = expression.substr(1);
	}

	while (expression[expression.size() - 1] == ' ') {
		expression = expression.substr(0, expression.size() - 1);
	}

	return expression;
}

synExp* ParserCNF::makeOrOperation(std::string expression) {
	std::string variable = "";
	bool first = true;
	bool madeOperation = true;
	bool isNeg = false;
	synExp* result;
	
	for (int i = 0; i < expression.size(); i++) {
		if (expression[i] != ' ') {
			if (expression[i] == '-') {
				isNeg = true;
			} else {
				variable += expression[i];
				madeOperation = false;
			}
		}

		if (((expression[i] == ' ') || (i == expression.size() - 1)) && !madeOperation) {
			int variableInt = std::stoi(variable) - 1;
			
			if (variableInt >= synExpMap.size()) {
				std::cerr << "Variable " << variable << " does not exist. Check the variables defined with the rbdd_printVariables() command" << std::endl;
				return NULL;
			}

			if (first) {
				result = this->synExpMap[variableInt];
				if (isNeg) {
					result = makeNot(result);
				}

				first = false;
			} else {
				if (!isNeg) {
					result = makeOr(result, this->synExpMap[variableInt]);
				} else {
					result = makeOr(result, makeNot(this->synExpMap[variableInt]));
				}
			}

			madeOperation = true;
			isNeg = false;
			variable = "";
		}
	}

	return result;
}

