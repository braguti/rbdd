
#include "rbddFunctions.hpp"

std::vector<std::string> reorderMethods;
std::vector<RbddElem> rbddElems;
bddParserBuddy parserBuddy;
bddParserCudd parserCudd;
std::vector<TraverseElem> traverseElems;

// [[Rcpp::export]]
void init_bdd(std::string library = "buddy", int node_num = 0, int cache_size = 0, std::string bdd_name = "default") {

	while (bdd_name[0] == ' ') {
		bdd_name = bdd_name.substr(1);
	}

	while (bdd_name[bdd_name.size() - 1] == ' ') {
		bdd_name = bdd_name.substr(0, bdd_name.size() - 1);
	}
	
	if (bdd_name == "") {
		std::cerr << "Invalid bdd_name." << std::endl;
	} else {

		bool valid = true;
		int i = 0;
	
		while (valid && i < bdd_name.size()) {
			char current = bdd_name[i];
			valid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_';
			i++;
		}

		bool found = false;
		i = 0;

		while (i < rbddElems.size() && !found) {
			RbddElem temp = rbddElems[i];
			found = bdd_name == temp.getName();
			if (found) {
				rbddElems.erase(rbddElems.begin() + i);
			}
			i++;
		}
		
		if (library == "buddy" || library == "cudd") {
			RbddElem *elem = new RbddElem;
			elem->initFactory(bdd_name, library, node_num, cache_size);
			rbddElems.push_back(*elem);
		} else {
			std::cerr << "Unknown BDD library: " << library << ". Expected values are \"buddy\" and \"cudd\"" << std::endl;
		}
	}
}

// [[Rcpp::export]]
int new_variable(std::string variable_name, std::string var_type = "boolean", std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	}

	if (var_type != "boolean" && var_type != "tristate") {
		std::cerr << "Parameter var_type has a wrong value. Initialize it with value \"boolean\" or \"tristate\"." << std::endl;
		return -1;
	}

	while (variable_name[0] == ' ') {
		variable_name = variable_name.substr(1);
	}

	while (variable_name[variable_name.size() - 1] == ' ') {
		variable_name = variable_name.substr(0, variable_name.size() - 1);
	}
	
	if (variable_name.size() == 0) {
		std::cerr << "A variable_name must be provided." << std::endl;
		return -1;
	}

	int nameCount = 0;
	bool isValid = true;
	char current;
	while (isValid && nameCount < variable_name.size()) {
		current = variable_name[nameCount];
		isValid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9');
		nameCount++;
	}

	if (!isValid) {
		std::cerr << "The name of the variable can not contain the \"" << current << "\" character." << std::endl;
		return -1;
	}

	std::vector<synExp*> varMapTemp = rbddElem.getVarMap();
	bool found = false;
	int i = 0;

	while (i < varMapTemp.size() && !found) {
		synExp* synExpTemp = varMapTemp[i];
		found = variable_name == synExpTemp->getSymbol();
		i++;
	}
	
	if (found) {
		std::cerr << "Variable has not been created because it is duplicated." << std::endl;
		return -1;
	}

	configInfo* ci = new configInfo();
	ci->setName(variable_name);
	ci->setVartype(var_type);
	
	symbolTable::addSymbol(variable_name, ci);

	rbddElem.getFactory()->newVar(variable_name, var_type);

	rbddElem.addSynExpMap(variable_name);
	rbddElem.addVarMap(variable_name);
	rbddElem.addNameList(variable_name);

	rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

	return rbddElem.getSynExpMap().size();
}

// [[Rcpp::export]]
int new_variable_from_expression(std::string expression, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	}

	std::string myExpression = expression;
	
	if (expression.substr(expression.size() - 4) == ".cnf") {
		std::ifstream inputFile;
		std::string line;

		inputFile.open(expression);
		if (!inputFile.is_open()) {
			std::cerr << "File could not be opened. Check if the name is correct." << std::endl;
			return -1;
		}

		int numberOfVars = 0;
		int numberOfLines = 0;
		synExp* result;
		bool first = true;
		int currentLines = 0;
		std::string nameExpr = "";
		std::vector<synExp*> tempMap;

		while (!inputFile.eof()) {
			getline(inputFile, line);

			while (line[0] == ' ') {
				line = line.substr(1);
			}

			while (line[line.size() - 1] == ' ') {
				line = line.substr(0, line.size() - 1);
			}

			if (line[0] == 'c' || line == "") {
				// Comment line or end line, nothing to do
			} else if (line[0] == 'p' && (numberOfVars != 0 || numberOfLines != 0)) {
				std::cerr << "Sysntax error: there are more than one problem line (p)" << std::endl;
				tempMap.clear();
				return -1;
			} else if (line[0] == 'p') {
				line = line.substr(1);
				while (line[0] == ' ') {
					line = line.substr(1);
				}

				//cnf[[:space:]]+[1-9][0-9]*[[:space:]]+[1-9][0-9]*[[:space:]]*
				int lineCount = 4;
				bool isValid = true;
				bool firstNumber = false;
				bool startSecondNumber = false;
				bool secondNumber = false;
				bool endNumbers = false;
				if (line.size() < 5 || line.substr(0, 4) != "cnf " || line.substr(4).find_first_not_of("0123456789 ") != std::string::npos) {
					isValid = false;
				}
				while (lineCount < line.size() && isValid) {
					char current = line[lineCount];
					if (current != ' ' && !firstNumber) {
						firstNumber = true;
					} else if (current == ' ' && firstNumber && !startSecondNumber) {
						startSecondNumber = true;
					} else if (current != ' ' && firstNumber && startSecondNumber && !secondNumber) {
						secondNumber = true;
					} else if (current == ' ' && secondNumber) {
						endNumbers = true;
					} else if (current != ' ' && endNumbers) {
						isValid = false;
					}
					lineCount++;
				}

				if (!isValid) {
					std::cerr << "Syntax error in the problem line (p)" << std::endl;
					tempMap.clear();
					return -1;
				}

				while (line[0] < '0' || line[0] > '9') {
					line = line.substr(1);
				}

				std::string numberOfVarsString = "";				
				while (line[0] >= '0' && line[0] <= '9') {
					numberOfVarsString += line[0];
					line = line.substr(1);
				}
				numberOfVars = std::stoi(numberOfVarsString);
				if (numberOfVars <= 0) {
					std::cerr << "The number of variables must be higher than 0" << std::endl;
					tempMap.clear();
					return -1;
				}

				while (line[0] < '0' || line[0] > '9') {
					line = line.substr(1);
				}

				std::string numberOfLinesString = "";				
				while (line[0] >= '0' && line[0] <= '9') {
					numberOfLinesString += line[0];
					line = line.substr(1);
				}
				numberOfLines = std::stoi(numberOfLinesString);
				if (numberOfLines <= 0) {
					std::cerr << "The number of lines must be higher than 0" << std::endl;
					tempMap.clear();
					return -1;
				}

				for (int i = 0; i < numberOfVars; i++) {
					std::string name = std::to_string(rbddElem.getSynExpMap().size() + 1) + "_" + std::to_string(i + 1);
					tempMap.push_back(new synSymbol(name));
				}
			} else {
				if (numberOfVars <= 0 || numberOfLines <= 0) {
					std::cerr << "The problem line (p) must be defined before the clauses" << std::endl;
					tempMap.clear();
					return -1;
				}

				while (line[0] == ' ') {
					line = line.substr(1);
				}

				while (line[line.size() - 1] == ' ') {
					line = line.substr(0, line.size() - 1);
				}

				//(-?[1-9][0-9]*[[:space:]]+)+0[[:space:]]*
				bool isValid = true;
				bool isNumber = false;
				bool negation = false;
				bool isZero = false;
				bool oneNumber = false;
				int lineCount = 0;
				if (line.find_first_not_of("-0123456789 ") != std::string::npos) {
					isValid = false;
				}
				
				while (isValid && lineCount < line.size()) {
					char current = line[lineCount];
					if (isZero && current != ' ' && current != '0') {
						isValid = false;
					} else if (isZero && (current == ' ' || current == '0')) {
						// Nothing to do
					} else if (current == '-' && (negation || isNumber)) {
						isValid = false;
					} else if (current == '-' && !negation && !isNumber) {
						negation = true;
					} else if ((!isNumber && current >= '1' && current <= '9') || (isNumber && current >= '0' && current <= '9')) {
						isNumber = true;
						oneNumber = true;
					} else if (current == '0' && !isNumber) {
						isZero = true;
					} else if (current == ' ' && negation && !isNumber) {
						isValid = false;
					} else if (current == ' ' && isNumber) {
						isNumber = false;
						negation = false;
					}
					lineCount++;
				}

				if (!isValid || (isValid && !isZero) || !oneNumber) {
					std::cerr << "Syntax error in a clause line" << std::endl;
					tempMap.clear();
					return -1;
				}

				nameExpr += line;

				ParserCNF parser(tempMap);

				if (first) {
					result = parser.parse(line);
					first = false;
				} else {
					result = makeAnd(result, parser.parse(line));
				}

				if (result == NULL) {
					tempMap.clear();
					return -1;
				}

				currentLines++;
			}
		}
	
		inputFile.close();

		if (currentLines != numberOfLines) {
			std::cerr << "The number of clauses is not the same than the number defined in the problem line (p)" << std::endl;
			tempMap.clear();
			return -1;
		}

		std::vector<std::string> nameListTemp = rbddElem.getNameList();
		bool found = false;
		int i = 0;

		std::stringstream buffer;
		buffer << result;
		std::string str = buffer.str();

		while (i < nameListTemp.size() && !found) {
			std::string nameTemp = nameListTemp[i];
			found = str == nameTemp;
			i++;
		}

		if (!found) {
			std::map<std::string, synExp*> myMap;

			for (i = 1; i <= tempMap.size(); i++) {
				std::string name = std::to_string(rbddElem.getSynExpMap().size() + 1) + "_" + std::to_string(i);				
				configInfo* ci = new configInfo();
				ci->setName(name);
				ci->setVartype("boolean");
			
				symbolTable::addSymbol(name, ci);

				rbddElem.getFactory()->newVar(name, "boolean");
			}
			tempMap.clear();

			configInfo* ci = new configInfo();
			ci->setName(nameExpr);
			ci->setVartype("boolean");
			
			symbolTable::addSymbol(nameExpr, ci);

			rbddElem.getFactory()->newVarExpression(nameExpr, "boolean", result);

			rbddElem.addNameList(str);
			rbddElem.addSynExpMap(result);

			rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

			return rbddElem.getSynExpMap().size();
		} else {
			std::cerr << "Expression has not been created because it is duplicated." << std::endl;
			tempMap.clear();
			return -1;
		}
	} else if (myExpression.find_first_not_of("-0123456789 ") == std::string::npos) {
		while (myExpression[0] == ' ') {
			myExpression = myExpression.substr(1);
		}

		while (myExpression[myExpression.size() - 1] == ' ') {
			myExpression = myExpression.substr(0, myExpression.size() - 1);
		}

		myExpression += " ";

		//((-?[1-9][0-9]*[[:space:]]+)+0[[:space:]]+)+
		bool isValid = true;
		bool isNumber = false;
		bool negation = false;
		bool isZero = false;
		bool oneNumber = false;
		int lineCount = 0;
		if (myExpression.find_first_not_of("-0123456789 ") != std::string::npos) {
			isValid = false;
		}
		while (isValid && lineCount < myExpression.size()) {
			char current = myExpression[lineCount];
			if (current == '0' && !oneNumber) {
				isValid = false;
			} else if (current == '0' && isZero) {
				isValid = false;
			} else if (current == '0' && !isNumber) {
				isZero = true;
			} else if (current == '-' && (negation || isNumber)) {
				isValid = false;
			} else if (current == '-' && !negation && !isNumber) {
				if (isZero) {
					if (myExpression[lineCount - 1] != ' ') {
						isValid = false;
					} else {
						isZero = false;
						oneNumber = false;
					}
				}
				negation = true;
			} else if ((!isNumber && current >= '1' && current <= '9') || (isNumber && current >= '0' && current <= '9')) {
				if (isZero) {
					if (myExpression[lineCount - 1] != ' ') {
						isValid = false;
					}
				}
				isNumber = true;
				oneNumber = true;
				isZero = false;
			} else if (current == ' ' && negation && !isNumber) {
				isValid = false;
			} else if (current == ' ' && isNumber) {
				isNumber = false;
				negation = false;
				if (isZero) {
					isZero = false;
					oneNumber = false;
				}
			}
			lineCount++;
		}

		if (!isValid || (isValid && !isZero) || !oneNumber) {
			std::cerr << "Syntax error in the CNF expression" << std::endl;
			return -1;
		}

		myExpression = myExpression.substr(0, myExpression.size() - 1);

		ParserCNF parser(rbddElem.getVarMap());

		synExp* result = parser.parse(myExpression);

		if (result == NULL) {
			return -1;
		} else {
			std::vector<std::string> nameListTemp = rbddElem.getNameList();
			bool found = false;
			int i = 0;

			std::stringstream buffer;
			buffer << result;
			std::string str = buffer.str();

			while (i < nameListTemp.size() && !found) {
				std::string nameTemp = nameListTemp[i];
				found = str == nameTemp;
				i++;
			}

			if (!found) {

				configInfo* ci = new configInfo();
				ci->setName(expression);
				ci->setVartype("boolean");
			
				symbolTable::addSymbol(expression, ci);

				rbddElem.getFactory()->newVarExpression(expression, "boolean", result);

				rbddElem.addNameList(str);
				rbddElem.addSynExpMap(result);

				rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

				return rbddElem.getSynExpMap().size();
			} else {
				std::cerr << "Expression has not been created because it is duplicated." << std::endl;
				return -1;
			}
		}
	} else {
		std::map<int, synExp*> varMapTemp;
		std::vector<synExp*> rbddVarMap = rbddElem.getVarMap();
		for (int i = 0; i < rbddVarMap.size(); i++) {
			varMapTemp[i] = rbddVarMap[i];
		}

		SiftMinScore ss(expression + "\n", varMapTemp);

		if (ss.getParserResult()) {
			std::vector<std::string> nameListTemp = rbddElem.getNameList();
			bool found = false;
			int i = 0;

			std::stringstream buffer;
			buffer << ss.getParsingResult();
			std::string str = buffer.str();

			while (i < nameListTemp.size() && !found) {
				std::string nameTemp = nameListTemp[i];
				found = str == nameTemp;
				i++;
			}

			if (!found) {
				configInfo* ci = new configInfo();
				ci->setName(expression);
				ci->setVartype("boolean");
			
				symbolTable::addSymbol(expression, ci);

				rbddElem.getFactory()->newVarExpression(expression, "boolean", ss.getParsingResult());

				rbddElem.addNameList(expression);
				rbddElem.addSynExpMap(expression);

				rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

				return rbddElem.getSynExpMap().size();
			} else {
				std::cerr << "Expression has not been created because it is duplicated." << std::endl;
				return -1;
			}
		} else {
			std::cerr << "Undefined variable. Check the expression you have chosen or create it with the new_variable command." << std::endl;
			return -1;
		}
	}
}

// [[Rcpp::export]]
void print_variables(std::string bdd_name = "default") {

	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else {
		std::cout << "++++++++++++++++++++++++++++" << std::endl;
		std::cout << "Index variable -> Expression" << std::endl;
		std::cout << "++++++++++++++++++++++++++++" << std::endl;

		std::vector<synExp*> synExpMapTemp(rbddElem.getSynExpMap());

		for (int i = 0; i < synExpMapTemp.size(); i++) {
			std::cout << "Variable " << i + 1 << " -> " << synExpMapTemp[i] << std::endl;
		}
	}
}

// [[Rcpp::export]]
void expression_to_string(int expression, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else if (expression <= 0 || expression > rbddElem.getSynExpMap().size()) {
		std::cerr << "Undefined expression. Check the expression you have chosen or create it with the new_variable() command." << std::endl;
	} else {
		synExp* se = rbddElem.getSynExpMap()[expression - 1];
		std::cout << "The expression is " << se << std::endl;
	}
}

// [[Rcpp::export]]
void apply_bdd(int expression, std::string bdd_name = "default") {

	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory()== NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else if (expression <= 0 || expression > rbddElem.getSynExpMap().size()) {
		std::cerr << "Undefined expression. Check the expression you have chosen or create it with the new_variable() or new_variable_from_expression() commands." << std::endl;
	} else {
		synExp* se = rbddElem.getSynExpMap()[expression - 1];

		rbddElem.getFactory()->apply(bdd_name, se);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
int get_node_num(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	}

	return rbddElem.getFactory()->nodecount(bdd_name);
}

// [[Rcpp::export]]
void save_bdd(std::string file_name, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else if (file_name == "") {
		std::cerr << "file_name must not be an empty string" << std::endl;
	} else {
		rbddElem.getFactory()->saveBDD(bdd_name, file_name, "");
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void read_bdd(std::string file_name, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is a BDD created with name " << bdd_name << "." << std::endl;
	} else if (file_name == "") {
		std::cerr << "file_name must not be an empty string" << std::endl;
	} else if (file_name.size() < 6) {
		std::cerr << "file_name must have a length greater than 5." << std::endl;
	} else {
		if (file_name.substr(file_name.size() - 6) == ".buddy") {
			RbddElem *elem = new RbddElem;
			elem->initFactory(bdd_name, "buddy", 0, 0);
			elem->getFactory()->readBDD(bdd_name, file_name, "");
			rbddElems.push_back(*elem);
		} else if (file_name.substr(file_name.size() - 5) == ".blif") {
			RbddElem *elem = new RbddElem;
			elem->initFactory(bdd_name, "cudd", 0, 0);
			elem->getFactory()->readBDD(bdd_name, file_name, "");
			rbddElems.push_back(*elem);
		} else {
			std::cerr << "file_name extension must be \".buddy\" or \".blif\"." << std::endl;
		}
	}
}

// [[Rcpp::export]]
void done_bdd(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else {
		rbddElem.getFactory()->done();
		rbddElems = RbddUtils::deleteRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
bool is_initialized(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return false;
	}
	
	return rbddElem.getFactory() != NULL;
}

// [[Rcpp::export]]
void reset_bdd(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else {
		rbddElem.getFactory()->done();
		rbddElem.resetFactory();
		rbddElem.getFactory()->init(true, rbddElem.getNodenum(), rbddElem.getCachesize());
		
		rbddElem.resetSynExpMap();
		rbddElem.resetVarMap();
		rbddElem.resetNameList();

		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void set_cache_ratio(double cache_ratio, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else {
		rbddElem.getFactory()->setCacheRatio(cache_ratio);
		
		rbddElem.setCachesize((int) cache_ratio);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void set_max_node_num(int size, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else {
		rbddElem.getFactory()->setMaxNodeNum(size);
		
		rbddElem.setNodenum(size);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void reorder_bdd(std::string reorder_method = "sift", std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	} else if (rbddElem.getSynExpMap().size() == 0) {
		std::cerr << "There are not variables created in the BDD " << bdd_name << ". Add them with the new_variable and new_variable_from_expression commands." << std::endl;
	} else {
		reorderMethods.push_back("none");
		reorderMethods.push_back("window2");
		reorderMethods.push_back("window3");
		reorderMethods.push_back("sift");
		reorderMethods.push_back("random");
	
		if (reorder_method == "") {
			reorder_method = "none";
		}
		bool found = false;
		int i = 0;
		while (!found && i < reorderMethods.size()) {
			found = reorder_method == reorderMethods[i];
			i++;
		}
		
		if (found) {
			rbddElem.getFactory()->reorder(reorder_method);
		} else {
			std::cerr << reorder_method << " is not a valid reorder method. For more information check help(reorderBDD)." << std::endl;
		}
	}
}

// [[Rcpp::export]]
int add_cnf_var(std::string name, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	}

	if (symbolTable::getSymbol(name) == NULL) {
		std::cerr << "Variable " << name << " is not a variable defined in a CNF expression" << std::endl;
		return -1;
	}

	std::vector<synExp*> varMapTemp = rbddElem.getVarMap();
	bool found = false;
	int i = 0;

	while (i < varMapTemp.size() && !found) {
		std::string symbolTemp = varMapTemp[i]->getSymbol();
		found = name == symbolTemp;
		i++;
	}
	
	if (found) {
		std::cerr << "Variable has not been created because it is duplicated." << std::endl;
		return -1;
	}

	rbddElem.addNameList(name);
	rbddElem.addSynExpMap(name);

	rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

	return rbddElem.getSynExpMap().size();
}

// [[Rcpp::export]]
void get_bdd_library(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "The BDD does not exist" << std::endl;
	} else {
		bool buddy = dynamic_cast<buddyFactory*> (rbddElem.getFactory()) != nullptr;
		bool cudd = dynamic_cast<cuddFactory*> (rbddElem.getFactory()) != nullptr;

		if (buddy) {
			std::cout << "BuDDy" << std::endl;
		} else if (cudd) {
			std::cout << "CUDD" << std::endl;
		}
	}
}

// [[Rcpp::export]]
bool same_bdd(std::string name_bdd_1, std::string name_bdd_2) {
	RbddElem rbddElem1(RbddUtils::getRbddElem(name_bdd_1, rbddElems));
	RbddElem rbddElem2(RbddUtils::getRbddElem(name_bdd_2, rbddElems));
	
	SemanticsBuddy semBuddy1;
	SemanticsBuddy semBuddy2;
	SemanticsCudd semCudd1;
	SemanticsCudd semCudd2;
	
	bool isBuddy = true;
	
	std::string expression = name_bdd_1;
	while (expression.size() > 0 && expression[0] == ' ') {
		expression = expression.substr(1);
	}

	while (expression.size() > 0 && expression[expression.size() - 1] == ' ') {
		expression = expression.substr(0, expression.size() - 1);
	}
	
	if (expression == "false" || expression == "true") {
		std::cerr << "First parameter must be a BDD expression" << std::endl;
		return false;
	}

	//!?[a-zA-Z0-9_]+[[:space:]]*([&&|\\|\\||==|!=|<|>]?[[:space:]]*!?[a-zA-Z0-9_]+[[:space:]]*)*
	bool isName = false;
	bool isOperator = true;
	bool isValidName = false;
	int expressionCount = 0;
	bool isValid = true;

	while (expressionCount < expression.size() && isValid) {
		char current = expression[expressionCount];
		if (current != ' ') {
			if (!isName) {
				if (!isValidName && (current == '!' || (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_')) {
					isName = true;
					isOperator = false;
					if (current != '!') {
						isValidName = true;
					}
				} else {
					isValid = false;
				}
			} else if (isName && ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_')) {
				isValidName = true;
			} else if (isName && isValidName) {
				if ((expression.size() > (expressionCount + 1)) && current == '!' && expression[expressionCount + 1] != '=') {
					isValid = false;
				} else if (current == '<' || current == '>') {
					isName = false;
					isOperator = true;
				} else if (expression.size() > (expressionCount + 1) && ((current == '&' && expression[expressionCount + 1] == '&') || (current == '|' && expression[expressionCount + 1] == '|') || (current == '=' && expression[expressionCount + 1] == '=') || (current == '!' && expression[expressionCount + 1] == '='))) {
					isName = false;
					isOperator = true;
					expressionCount++;
				} else {
					isValid = false;
				}
				isValidName = false;
			} else if (isName && !isValidName) {
				isValid = false;
			}
		} else {
			if ((expression.size() > (expressionCount + 1)) && isName && ((expression[expressionCount + 1] >= 'a' && expression[expressionCount + 1] <= 'z') || (expression[expressionCount + 1] >= 'A' && expression[expressionCount + 1] <= 'Z') || (expression[expressionCount + 1] >= '0' && expression[expressionCount + 1] <= '9') || expression[expressionCount + 1] == '_')) {
				isValid = false;
			}
		}
		expressionCount++;
	}
		if (!isValid || isOperator || !isName || (isName && !isValidName)) {
		std::cerr << "Syntax error in the expression" << std::endl;
		return false;
	}
	
	std::pair<SemanticsBuddy, int> resultParsing = parserBuddy.parseBdd(expression, rbddElems);
	if (resultParsing.second == 0) {
		return false;
	} else if (resultParsing.second == 1) {
		SemanticsBuddy semBuddy1(resultParsing.first.main(), resultParsing.first.module());
	} else if (resultParsing.second == -1) {
		std::pair<SemanticsCudd, int> resultParsing = parserCudd.parseBdd(expression, rbddElems);
		if (resultParsing.second == 0) {
			return false;
		} else if (resultParsing.second == 1) {
			isBuddy = false;
			SemanticsCudd semCudd1(resultParsing.first.main(), resultParsing.first.module());
		} else if (resultParsing.second == -1) {
			return false;
		}
	}
		
	expression = name_bdd_2;
	while (expression.size() > 0 && expression[0] == ' ') {
		expression = expression.substr(1);
	}

	while (expression.size() > 0 && expression[expression.size() - 1] == ' ') {
		expression = expression.substr(0, expression.size() - 1);
	}
		
	RbddElem elem = rbddElems[0];
	if (expression == "false") {
		if (isBuddy) {
			buddyFactory* bFactory = dynamic_cast<buddyFactory*> (elem.getFactory());
			SemanticsBuddy semBuddy2(bFactory->getFalseBDD().main(), bFactory->getFalseBDD().module());
		} else {
			cuddFactory* cFactory = dynamic_cast<cuddFactory*> (elem.getFactory());
			SemanticsCudd semCudd2(cFactory->getFalseBDD().main(), cFactory->getFalseBDD().module());
		}
	} else if (expression == "true") {
		if (isBuddy) {
			buddyFactory* bFactory = dynamic_cast<buddyFactory*> (elem.getFactory());
			SemanticsBuddy semBuddy2(bFactory->getTrueBDD().main(), bFactory->getTrueBDD().module());
		} else {
			cuddFactory* cFactory = dynamic_cast<cuddFactory*> (elem.getFactory());
			SemanticsCudd semCudd2(cFactory->getTrueBDD().main(), cFactory->getTrueBDD().module());
		}
	} else {
		//!?[a-zA-Z0-9_]+[[:space:]]*([&&|\\|\\||==|!=|<|>]?[[:space:]]*!?[a-zA-Z0-9_]+[[:space:]]*)*
		bool isName = false;
		bool isOperator = true;
		bool isValidName = false;
		int expressionCount = 0;
		bool isValid = true;

		while (expressionCount < expression.size() && isValid) {
			char current = expression[expressionCount];
			if (current != ' ') {
				if (!isName) {
					if (!isValidName && (current == '!' || (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_')) {
						isName = true;
						isOperator = false;
						if (current != '!') {
							isValidName = true;
						}
					} else {
						isValid = false;
					}
				} else if (isName && ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_')) {
					isValidName = true;
				} else if (isName && isValidName) {
					if ((expression.size() > (expressionCount + 1)) && current == '!' && expression[expressionCount + 1] != '=') {
						isValid = false;
					} else if (current == '<' || current == '>') {
						isName = false;
						isOperator = true;
					} else if (expression.size() > (expressionCount + 1) && ((current == '&' && expression[expressionCount + 1] == '&') || (current == '|' && expression[expressionCount + 1] == '|') || (current == '=' && expression[expressionCount + 1] == '=') || (current == '!' && expression[expressionCount + 1] == '='))) {
						isName = false;
						isOperator = true;
						expressionCount++;
					} else {
						isValid = false;
					}
					isValidName = false;
				} else if (isName && !isValidName) {
					isValid = false;
				}
			} else {
				if ((expression.size() > (expressionCount + 1)) && isName && ((expression[expressionCount + 1] >= 'a' && expression[expressionCount + 1] <= 'z') || (expression[expressionCount + 1] >= 'A' && expression[expressionCount + 1] <= 'Z') || (expression[expressionCount + 1] >= '0' && expression[expressionCount + 1] <= '9') || expression[expressionCount + 1] == '_')) {
					isValid = false;
				}
			}
			expressionCount++;
		}

		if (!isValid || isOperator || !isName || (isName && !isValidName)) {
			std::cerr << "Syntax error in the expression" << std::endl;
			return false;
		}

		if (isBuddy) {
			std::pair<SemanticsBuddy, int> resultParsing = parserBuddy.parseBdd(expression, rbddElems);
			if (resultParsing.second == 0) {
				return false;
			} else if (resultParsing.second == -1) {
				std::cerr << "There are BDDs build with different libraries." << std::endl;
				return false;
			} else {
				SemanticsBuddy semBuddy2(resultParsing.first.main(), resultParsing.first.module());
				return semBuddy1.main() == semBuddy2.main();
			}
		} else {
			std::pair<SemanticsCudd, int> resultParsing = parserCudd.parseBdd(expression, rbddElems);
			if (resultParsing.second == 0) {
				return false;
			} else if (resultParsing.second == -1) {
				std::cerr << "There are BDDs build with different libraries." << std::endl;
				return false;
			} else {
				SemanticsCudd semCudd2(resultParsing.first.main(), resultParsing.first.module());
				return semCudd1.main() == semCudd2.main();
			}
		}
	}
	
	return false;
}

// [[Rcpp::export]]
void print_bdd(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
	}

	rbddElem.getFactory()->printBDD(bdd_name);
}

// [[Rcpp::export]]
int restrict_bdd(int expression, std::string var_to_restrict, std::string variable_name, bool positive_form = true, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	}

	if (expression <= 0 || expression > rbddElem.getSynExpMap().size()) {
		std::cerr << "Undefined expression. Check the expression you have chosen or create it with the new_variable() or new_variable_from_expression() commands." << std::endl;
		return -1;
	}

	while (var_to_restrict[0] == ' ') {
		var_to_restrict = var_to_restrict.substr(1);
	}

	while (var_to_restrict[var_to_restrict.size() - 1] == ' ') {
		var_to_restrict = var_to_restrict.substr(0, var_to_restrict.size() - 1);
	}
	
	if (var_to_restrict.size() == 0) {
		std::cerr << "A variable_to_restrict must be provided." << std::endl;
		return -1;
	}

	std::vector<synExp*> varMapTemp = rbddElem.getVarMap();
	bool found = false;
	int i = 0;

	while (i < varMapTemp.size() && !found) {
		std::string symbolTemp = varMapTemp[i]->getSymbol();
		found = var_to_restrict == symbolTemp;
		i++;
	}
	
	if (!found) {
		std::cerr << "var_to_restrict does not exist." << std::endl;
		return -1;
	}

	while (variable_name[0] == ' ') {
		variable_name = variable_name.substr(1);
	}

	while (variable_name[variable_name.size() - 1] == ' ') {
		variable_name = variable_name.substr(0, variable_name.size() - 1);
	}
	
	if (variable_name.size() == 0) {
		std::cerr << "A variable_name must be provided." << std::endl;
		return -1;
	}

	int nameCount = 0;
	bool isValid = true;
	char current;
	while (isValid && nameCount < variable_name.size()) {
		current = variable_name[nameCount];
		isValid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9');
		nameCount++;
	}

	if (!isValid) {
		std::cerr << "The name of the variable can not contain the \"" << current << "\" character." << std::endl;
		return -1;
	}

	std::vector<synExp*> varMapTemp2 = rbddElem.getVarMap();
	bool found2 = false;
	i = 0;

	while (i < varMapTemp2.size() && !found) {
		std::string symbolTemp = varMapTemp2[i]->getSymbol();
		found2 = variable_name == symbolTemp;
		i++;
	}
	
	if (found2) {
		std::cerr << "Variable has not been created because it is duplicated." << std::endl;
		return -1;
	}

	configInfo* ci = new configInfo();
	ci->setName(variable_name);
	ci->setVartype("boolean");
	
	symbolTable::addSymbol(variable_name, ci);

	buddyFactory* bFactory = dynamic_cast<buddyFactory*> (rbddElem.getFactory());
	bool buddy = bFactory != nullptr;
	if (buddy) {
		std::vector<synExp*> synExpMapTemp = rbddElem.getSynExpMap();
		int i = 1;
		int j = 0;
		std::string name = "";
	
		while (i <= expression) {
			if (i == expression) {
				name = synExpMapTemp[j]->getSymbol();
			}
			j++;
			i++;
		}

		rbddElem.getFactory()->restrictBDD(name, var_to_restrict, variable_name, positive_form);
	} else {
		std::cerr << "Instruction not suppported by the CUDD library." << std::endl;
		return -1;
	}

	rbddElem.addSynExpMap(variable_name);
	rbddElem.addVarMap(variable_name);
	rbddElem.addNameList(variable_name);

	rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

	return rbddElem.getSynExpMap().size();
}

// [[Rcpp::export]]
Rcpp::NumericVector traverse_scalar(Rcpp::Function function_to_apply, int initial_scalar = 0, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	} else {
		buddyFactory* bFactory = dynamic_cast<buddyFactory*> (rbddElem.getFactory());
		bool buddy = bFactory != nullptr;
		if (buddy) {
			std::vector<int> printall = rbddElem.getFactory()->traverse();
			std::vector<TraverseElem> traverseElems2(printall.size() / 3);
			traverseElems = traverseElems2;
			int currentNodeIndex = 0;

			for (int i = 0; i < printall.size(); i++) {
				TraverseElem *elem = new TraverseElem;
				elem->setPosition(currentNodeIndex);
				elem->setIndex(printall[i]);
				i++;
				elem->setLow(printall[i]);
				i++;
				elem->setHigh(printall[i]);
				elem->setMark(false);
				elem->setResult(Rcpp::NumericVector::create(initial_scalar));
				traverseElems[currentNodeIndex] = *elem;
				currentNodeIndex++;
			}

			RbddFunctions rbddFunctions;
			rbddFunctions.traverseRec(traverseElems.size() - 1, function_to_apply);

			return traverseElems[traverseElems.size() - 1].getResult();
		} else {
			std::cerr << "Instruction not suppported by the CUDD library." << std::endl;
			return -1;
		}
	}
}

// [[Rcpp::export]]
Rcpp::NumericVector traverse_vector(Rcpp::Function function_to_apply, Rcpp::NumericVector initial_vector = Rcpp::NumericVector::create(1, 1), std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getFactory() == NULL) {
		std::cerr << "There is not a BDD created with name " << bdd_name << ". Create it with the init_bdd command." << std::endl;
		return -1;
	} else {
		buddyFactory* bFactory = dynamic_cast<buddyFactory*> (rbddElem.getFactory());
		bool buddy = bFactory != nullptr;
		if (buddy) {
			std::vector<int> printall = rbddElem.getFactory()->traverse();
			std::vector<TraverseElem> traverseElems2(printall.size() / 3);
			traverseElems = traverseElems2;
			int currentNodeIndex = 0;

			for (int i = 0; i < printall.size(); i++) {
				TraverseElem *elem = new TraverseElem;
				elem->setPosition(currentNodeIndex);
				elem->setIndex(printall[i]);
				i++;
				elem->setLow(printall[i]);
				i++;
				elem->setHigh(printall[i]);
				elem->setMark(false);
				elem->setResult(initial_vector);
				traverseElems[currentNodeIndex] = *elem;
				currentNodeIndex++;
			}

			RbddFunctions rbddFunctions;
			rbddFunctions.traverseRec(traverseElems.size() - 1, function_to_apply);

			return traverseElems[traverseElems.size() - 1].getResult();
		} else {
			std::cerr << "Instruction not suppported by the CUDD library." << std::endl;
			return -1;
		}
	}
}

void RbddFunctions::traverseRec(int node, Rcpp::Function functionToApply) {
	TraverseElem trav = traverseElems[node];

	trav.setMark(!trav.getMark());
	traverseElems[node] = trav;

	if (node != 0 & node != 1) {
		int low = trav.getLow();
		int high = trav.getHigh();

		TraverseElem traverseElemLow = traverseElems[low];
		TraverseElem traverseElemHigh = traverseElems[high];

		if (trav.getMark() != traverseElemLow.getMark()) { 
			RbddFunctions::traverseRec(low,  functionToApply);
		}
		if (trav.getMark() != traverseElemHigh.getMark()) {
			RbddFunctions::traverseRec(high, functionToApply);
		}
		
		trav.setResult(functionToApply(traverseElemLow.getResult(), traverseElemHigh.getResult()));
	} else {
		trav.setResult(functionToApply(0, 0));
	}

	traverseElems[node] = trav;
}
