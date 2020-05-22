
#include "RbddFunctions.hpp"

vector<string> reorderMethods;
vector<RbddElem> rbddElems;
bddParserBuddy parserBuddy;
bddParserCudd parserCudd;

/**
 * Initialization and finalization functions
 */

// [[Rcpp::export]]
void bdd_manager_init(std::string library = "cudd", int node_num = 0, int cache_size = 0, std::string bdd_name = "default") {

	while (bdd_name[0] == ' ') {
		bdd_name = bdd_name.substr(1);
	}

	while (bdd_name[bdd_name.size() - 1] == ' ') {
		bdd_name = bdd_name.substr(0, bdd_name.size() - 1);
	}
	
	if (bdd_name == "") {
		Rcerr << "Invalid bdd_name." << endl;
	} else {

		bool valid = true;
		int i = 0;
	
		while (valid && i < (int) bdd_name.size()) {
			char current = bdd_name[i];
			valid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9') || current == '_';
			i++;
		}

		bool found = false;
		i = 0;

		while (i < (int) rbddElems.size() && !found) {
			RbddElem temp = rbddElems[i];
			found = bdd_name == temp.getName();
			if (found) {
				temp.getAdapter()->done();
				rbddElems.erase(rbddElems.begin() + i);
			}
			i++;
		}
		
		if (library == "buddy" || library == "cudd") {
			RbddElem *elem = new RbddElem;
			elem->initAdapter(bdd_name, library, node_num, cache_size);
			rbddElems.push_back(*elem);
		} else {
			Rcerr << "Unknown BDD library: " << library << ". Expected values are \"buddy\" and \"cudd\"" << endl;
		}
	}
}

// [[Rcpp::export]]
void bdd_manager_quit(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else {
		rbddElem.getAdapter()->done();
		rbddElems = RbddUtils::deleteRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void bdd_manager_reset(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else {
		buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
		bool buddy = bAdapter != nullptr;
		string library = "cudd";
		if (buddy) {
			library = "buddy";
		}
		int nodenum = rbddElem.getNodenum();
		int cachesize = rbddElem.getCachesize();
		bdd_manager_quit(bdd_name);
		bdd_manager_init(library, nodenum, cachesize, bdd_name);
	}
}





/**
 * Setting logical formulas functions
 */

// [[Rcpp::export]]
int bdd_new_variable(std::string variable_name, std::string var_type = "boolean", std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	if (var_type != "boolean" && var_type != "tristate") {
		Rcerr << "Parameter var_type has a wrong value. Initialize it with value \"boolean\" or \"tristate\"." << endl;
		return -1;
	}

	while (variable_name[0] == ' ') {
		variable_name = variable_name.substr(1);
	}

	while (variable_name[variable_name.size() - 1] == ' ') {
		variable_name = variable_name.substr(0, variable_name.size() - 1);
	}
	
	if (variable_name.size() == 0) {
		Rcerr << "A variable_name must be provided." << endl;
		return -1;
	}

	int nameCount = 0;
	bool isValid = true;
	char current;
	while (isValid && nameCount < (int) variable_name.size()) {
		current = variable_name[nameCount];
		isValid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9');
		nameCount++;
	}

	if (!isValid) {
		Rcerr << "The name of the variable can not contain the \"" << current << "\" character." << endl;
		return -1;
	}

	vector<synExp*> varMapTemp = rbddElem.getVarMap();
	bool found = false;
	int i = 0;

	while (i < (int) varMapTemp.size() && !found) {
		synExp* synExpTemp = varMapTemp[i];
		found = variable_name == synExpTemp->getSymbol();
		i++;
	}
	
	if (found) {
		Rcerr << "Variable has not been created because it is duplicated." << endl;
		return -1;
	}

	configInfo* ci = new configInfo();
	ci->setName(variable_name);
	ci->setVartype(var_type);
	
	symbolTable::addSymbol(variable_name, ci);

	rbddElem.getAdapter()->newVar(variable_name, var_type);

	rbddElem.addSynExpMap(variable_name);
	rbddElem.addVarMap(variable_name);
	rbddElem.addNameList(variable_name);

	rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

	return rbddElem.getSynExpMap().size();
}

// [[Rcpp::export]]
int bdd_parse_cnf(std::string file, std::string bdd_name = "default", std::string score = "none", int window = 1, std::string reorder = "minspan", int min_nodes = 100000, int line_length = 27, bool dyn_comp = true) {
	if (score != "none" && score != "perm" && score != "sifting" && score != "force" && score != "forceblocks" && score != "random" && score != "other" && score != "file") {
		Rcerr << "Unknow score method \"" << score << "\". Allowed values are \"none\", \"perm\", \"sifting\", \"force\", \"forceblocks\", \"random\", \"other\" and \"file\"." << endl;
		return -1;
	}

	if (reorder != "minspan" && reorder != "minimax" && reorder != "remember" && reorder != "none") {
		Rcerr << "Unknow constraint reorder \"" << reorder << "\". Allowed values are \"minspan\", \"minimax\", \"remember\" and \"none\"." << endl;
		return -1;
	}
	
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	std::vector<std::string> result = parseCNF(file);

	Transformation* trans = new Transformation(rbddElem);

	string rootName = file.substr(0, file.find_last_of("."));
	string varFile = rootName + ".var";
	string expFile = rootName + ".exp";

	if (result.size() > 0) {
		for (int i = 0; static_cast<unsigned int>(i) < result.size(); i++) {
			rbddElem.addNameList(result[i]);
		}

		trans->readVars(varFile);
		trans->readExp(expFile);
		if (score != "none") {
			trans->scoreReorder = true;
			trans->scoreMethod = score;
		}
		trans->window = window;
		trans->constReorder = reorder;
		trans->base = bdd_name;
		trans->minNodes = min_nodes;
		trans->lineLength = line_length;
		trans->dynamic_components = dyn_comp;
		trans->treatVarsAndConstraints();
		BDDAdapter* adapter = trans->buildBDD(bdd_name);
		adapter->saveBDDToStorage(bdd_name);
		rbddElem.setAdapter(adapter);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

		synConst::delConst();
	}

	remove(varFile.c_str());
	remove(expFile.c_str());

	return result.size() > 0 ? 1 : 0;
}

// [[Rcpp::export]]
int bdd_parse_splot(std::string file, std::string bdd_name = "default", std::string score = "none", int window = 1, std::string reorder = "minspan", int min_nodes = 100000, int line_length = 27, bool dyn_comp = true) {
	if (score != "none" && score != "perm" && score != "sifting" && score != "force" && score != "forceblocks" && score != "random" && score != "other" && score != "file") {
		Rcerr << "Unknow score method \"" << score << "\". Allowed values are \"none\", \"perm\", \"sifting\", \"force\", \"forceblocks\", \"random\", \"other\" and \"file\"." << endl;
		return -1;
	}

	if (reorder != "minspan" && reorder != "minimax" && reorder != "remember" && reorder != "none") {
		Rcerr << "Unknow constraint reorder \"" << reorder << "\". Allowed values are \"minspan\", \"minimax\", \"remember\" and \"none\"." << endl;
		return -1;
	}
	
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	string rootName = file.substr(0, file.find_last_of("."));
	string varFile = rootName + ".var";
	string expFile = rootName + ".exp";
	remove(varFile.c_str());
	remove(expFile.c_str());

	splotScanner* scanner = new splotScanner;
	int result = scanner->parseSplot(file);
	Transformation* trans = new Transformation(rbddElem);

	if (result == 0) {
		std::ifstream dump(varFile);

		std::string word;
        while (dump >> word) {
            rbddElem.addNameList(word);
        }
        dump.close();

		trans->readVars(varFile);
		trans->readExp(expFile);
		if (score != "none") {
			trans->scoreReorder = true;
			trans->scoreMethod = score;
		}
		trans->window = window;
		trans->constReorder = reorder;
		trans->base = bdd_name;
		trans->minNodes = min_nodes;
		trans->lineLength = line_length;
		trans->dynamic_components = dyn_comp;
		trans->treatVarsAndConstraints();
		BDDAdapter* adapter = trans->buildBDD(bdd_name);
		adapter->saveBDDToStorage(bdd_name);
		rbddElem.setAdapter(adapter);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

		synConst::delConst();
	}

	return result;
}

// [[Rcpp::export]]
int bdd_parse_boolstr(std::string expression, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	string myExpression = expression;
	
	string copyExp = expression;
	int i = 0;
	vector<char> specialChars;
	specialChars.push_back(' ');
	specialChars.push_back('-');
	specialChars.push_back('>');
	specialChars.push_back('=');
	specialChars.push_back('(');
	specialChars.push_back(')');

	vector<string> reservedWords;
	reservedWords.push_back("and");
	reservedWords.push_back("or");
	reservedWords.push_back("not");
	reservedWords.push_back("xor");
	reservedWords.push_back("nand");
	reservedWords.push_back("nor");
	reservedWords.push_back("xnor");
	reservedWords.push_back("if");
	reservedWords.push_back("then");
	reservedWords.push_back("else");
	reservedWords.push_back("iff");
	reservedWords.push_back("true");
	reservedWords.push_back("false");

	string currentVar = "";
	vector<string> nameList = rbddElem.getNameList();

	while (i < (int) copyExp.size()) {
		char currentChar = copyExp[i];

		if (std::find(specialChars.begin(), specialChars.end(), currentChar) != specialChars.end()) {
			if (currentVar != "" && std::find(nameList.begin(), nameList.end(), currentVar) == nameList.end()) {
				bdd_new_variable(currentVar, "boolean", bdd_name);
				nameList.push_back(currentVar);
			}
			currentVar = "";
		} else {
			currentVar += currentChar;
			string lower = currentVar;
			transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c){return tolower(c);});
			if (std::find(reservedWords.begin(), reservedWords.end(), lower) != reservedWords.end() 
					&& i < (int) copyExp.size() - 1 
					&& std::find(specialChars.begin(), specialChars.end(), copyExp[i + 1]) != specialChars.end()) {
				currentVar = "";
			}
		}
		i++;
	}

	if (currentVar != "" 
			&& std::find(nameList.begin(), nameList.end(), currentVar) == nameList.end()
			&& std::find(reservedWords.begin(), reservedWords.end(), currentVar) == reservedWords.end()) {
		bdd_new_variable(currentVar, "boolean", bdd_name);
	}

	map<int, synExp*> varMapTemp;
	RbddElem rbddElem2(RbddUtils::getRbddElem(bdd_name, rbddElems));
	vector<synExp*> rbddVarMap = rbddElem2.getVarMap();
	for (int i = 0; i < (int) rbddVarMap.size(); i++) {
		varMapTemp[i] = rbddVarMap[i];
	}

	SiftMinScore ss(expression + "\n", varMapTemp);

	if (ss.getParserResult()) {
		vector<string> nameListTemp = rbddElem2.getNameList();
		bool found = false;
		int i = 0;

		stringstream buffer;
		buffer << ss.getParsingResult();
		string str = buffer.str();

		while (i < (int) nameListTemp.size() && !found) {
			string nameTemp = nameListTemp[i];
			found = str == nameTemp;
			i++;
		}

		if (!found) {
			configInfo* ci = new configInfo();
			ci->setName(expression);
			ci->setVartype("boolean");

			symbolTable::addSymbol(expression, ci);

			rbddElem2.getAdapter()->newVarExpression(expression, ss.getParsingResult());

			//rbddElem2.addNameList(expression);
			rbddElem2.addSynExpMap(expression);

			int index = rbddElem2.getSynExpMap().size();

			// apply
			synExp* se = rbddElem2.getSynExpMap()[index - 1];

			rbddElem2.getAdapter()->apply(bdd_name, se);
			rbddElems = RbddUtils::replaceRbddElem(rbddElem2, rbddElems);

			return index;
		} else {
			Rcerr << "Expression has not been created because it is duplicated." << endl;
			return -1;
		}
	} else {
		Rcerr << "Undefined variable. Check the expression you have chosen or create it with the bdd_new_variable command." << endl;
		return -1;
	}
}





/**
 * Ordering functions
 */

// [[Rcpp::export]]
void bdd_order(std::string reorder_method = "sift", std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else if (rbddElem.getAdapter()->getNumVars() == 0) {
		Rcerr << "There are not variables created in the BDD " << bdd_name << ". Add them with the bdd_new_variable and bdd_parse commands." << endl;
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
		while (!found && i < (int) reorderMethods.size()) {
			found = reorder_method == reorderMethods[i];
			i++;
		}
		
		if (found) {
			rbddElem.getAdapter()->reorder(reorder_method);
		} else {
			Rcerr << reorder_method << " is not a valid reorder method. For more information check help(reorderBDD)." << endl;
		}
	}
}





/**
 * Storing and reading BDDs
 */

// [[Rcpp::export]]
void bdd_write(std::string file_name, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else if (file_name == "") {
		Rcerr << "file_name must not be an empty string" << endl;
	} else {
		rbddElem.getAdapter()->saveBDD(bdd_name, file_name, "");
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void bdd_read(std::string file_name, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() != NULL) {
		Rcerr << "There is a BDD created with name " << bdd_name << "." << endl;
	} else if (file_name == "") {
		Rcerr << "file_name must not be an empty string" << endl;
	} else if (file_name.size() < 7) {
		Rcerr << "file_name must have a length greater than 6." << endl;
	} else {
		if (file_name.substr(file_name.size() - 6) == ".buddy") {
			RbddElem *elem = new RbddElem;
			elem->initAdapter(bdd_name, "buddy", 0, 0);
			elem->getAdapter()->readBDD(bdd_name, file_name, "");
			rbddElems.push_back(*elem);
		} else if ((file_name.substr(file_name.size() - 5) == ".blif") || (file_name.substr(file_name.size() - 6) == ".dddmp")) {
			if (file_name.substr(file_name.size() - 5) == ".blif") {
				file_name = file_name.substr(0, file_name.size() - 5);
			} else {
				file_name = file_name.substr(0, file_name.size() - 6);
			}
			RbddElem *elem = new RbddElem;
			elem->initAdapter(bdd_name, "cudd", 0, 0);
			elem->getAdapter()->readBDD(bdd_name, file_name, "");
			rbddElems.push_back(*elem);
			delete elem;

		} else {
			Rcerr << "file_name extension must be \".buddy\" for BuDDy, or \".blif\" or \".dddmp\" for CUDD." << endl;
		}
	}
}





/**
 * Operation functions
 */

// [[Rcpp::export]]
bool bdd_identical(std::string name_bdd_1, std::string name_bdd_2) {
	RbddElem rbddElem1(RbddUtils::getRbddElem(name_bdd_1, rbddElems));
	RbddElem rbddElem2(RbddUtils::getRbddElem(name_bdd_2, rbddElems));
	
	SemanticsBuddy semBuddy1;
	SemanticsBuddy semBuddy2;
	SemanticsCudd semCudd1;
	SemanticsCudd semCudd2;
	
	bool isBuddy = true;
	
	string expression = name_bdd_1;
	while (expression.size() > 0 && expression[0] == ' ') {
		expression = expression.substr(1);
	}

	while (expression.size() > 0 && expression[expression.size() - 1] == ' ') {
		expression = expression.substr(0, expression.size() - 1);
	}
	
	if (expression == "false" || expression == "true") {
		Rcerr << "First parameter must be a BDD expression." << endl;
		return false;
	}

	//!?[a-zA-Z0-9_]+[[:space:]]*([&&|\\|\\||==|!=|<|>]?[[:space:]]*!?[a-zA-Z0-9_]+[[:space:]]*)*
	bool isName = false;
	bool isOperator = true;
	bool isValidName = false;
	int expressionCount = 0;
	bool isValid = true;

	while (expressionCount < (int) expression.size() && isValid) {
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
				if (((int) expression.size() > (expressionCount + 1)) && current == '!' && expression[expressionCount + 1] != '=') {
					isValid = false;
				} else if (current == '<' || current == '>') {
					isName = false;
					isOperator = true;
				} else if ((int) expression.size() > (expressionCount + 1) && ((current == '&' && expression[expressionCount + 1] == '&') || (current == '|' && expression[expressionCount + 1] == '|') || (current == '=' && expression[expressionCount + 1] == '=') || (current == '!' && expression[expressionCount + 1] == '='))) {
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
			if (((int) expression.size() > (expressionCount + 1)) && isName && ((expression[expressionCount + 1] >= 'a' && expression[expressionCount + 1] <= 'z') || (expression[expressionCount + 1] >= 'A' && expression[expressionCount + 1] <= 'Z') || (expression[expressionCount + 1] >= '0' && expression[expressionCount + 1] <= '9') || expression[expressionCount + 1] == '_')) {
				isValid = false;
			}
		}
		expressionCount++;
	}
		if (!isValid || isOperator || !isName || (isName && !isValidName)) {
		Rcerr << "Syntax error in the expression." << endl;
		return false;
	}
	
	pair<SemanticsBuddy, int> resultParsing = parserBuddy.parseBdd(expression, rbddElems);
	if (resultParsing.second == 0) {
		return false;
	} else if (resultParsing.second == 1) {
		SemanticsBuddy semBuddy1(resultParsing.first.main(), resultParsing.first.module());
	} else if (resultParsing.second == -1) {
		pair<SemanticsCudd, int> resultParsing = parserCudd.parseBdd(expression, rbddElems);
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
			buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (elem.getAdapter());
			SemanticsBuddy semBuddy2(bAdapter->getFalseBDD().main(), bAdapter->getFalseBDD().module());
		} else {
			cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (elem.getAdapter());
			SemanticsCudd semCudd2(cAdapter->getFalseBDD().main(), cAdapter->getFalseBDD().module());
		}
	} else if (expression == "true") {
		if (isBuddy) {
			buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (elem.getAdapter());
			SemanticsBuddy semBuddy2(bAdapter->getTrueBDD().main(), bAdapter->getTrueBDD().module());
		} else {
			cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (elem.getAdapter());
			SemanticsCudd semCudd2(cAdapter->getTrueBDD().main(), cAdapter->getTrueBDD().module());
		}
	} else {
		//!?[a-zA-Z0-9_]+[[:space:]]*([&&|\\|\\||==|!=|<|>]?[[:space:]]*!?[a-zA-Z0-9_]+[[:space:]]*)*
		bool isName = false;
		bool isOperator = true;
		bool isValidName = false;
		int expressionCount = 0;
		bool isValid = true;

		while (expressionCount < (int) expression.size() && isValid) {
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
					if (((int) expression.size() > (expressionCount + 1)) && current == '!' && expression[expressionCount + 1] != '=') {
						isValid = false;
					} else if (current == '<' || current == '>') {
						isName = false;
						isOperator = true;
					} else if ((int) expression.size() > (expressionCount + 1) && ((current == '&' && expression[expressionCount + 1] == '&') || (current == '|' && expression[expressionCount + 1] == '|') || (current == '=' && expression[expressionCount + 1] == '=') || (current == '!' && expression[expressionCount + 1] == '='))) {
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
				if (((int) expression.size() > (expressionCount + 1)) && isName && ((expression[expressionCount + 1] >= 'a' && expression[expressionCount + 1] <= 'z') || (expression[expressionCount + 1] >= 'A' && expression[expressionCount + 1] <= 'Z') || (expression[expressionCount + 1] >= '0' && expression[expressionCount + 1] <= '9') || expression[expressionCount + 1] == '_')) {
					isValid = false;
				}
			}
			expressionCount++;
		}

		if (!isValid || isOperator || !isName || (isName && !isValidName)) {
			Rcerr << "Syntax error in the expression." << endl;
			return false;
		}

		if (isBuddy) {
			pair<SemanticsBuddy, int> resultParsing = parserBuddy.parseBdd(expression, rbddElems);
			if (resultParsing.second == 0) {
				return false;
			} else if (resultParsing.second == -1) {
				Rcerr << "There are BDDs build with different libraries." << endl;
				return false;
			} else {
				SemanticsBuddy semBuddy2(resultParsing.first.main(), resultParsing.first.module());
				return semBuddy1.main() == semBuddy2.main();
			}
		} else {
			pair<SemanticsCudd, int> resultParsing = parserCudd.parseBdd(expression, rbddElems);
			if (resultParsing.second == 0) {
				return false;
			} else if (resultParsing.second == -1) {
				Rcerr << "There are BDDs build with different libraries." << endl;
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
bool bdd_restrict(std::string restriction, std::string restriction_name, int expression = 0, bool positive_form = true, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return false;
	}

	std::string name;


	while (restriction[0] == ' ') {
		restriction = restriction.substr(1);
	}

	while (restriction[restriction.size() - 1] == ' ') {
		restriction = restriction.substr(0, restriction.size() - 1);
	}
	
	if (restriction.size() == 0) {
		Rcerr << "A restriction must be provided." << endl;
		return false;
	}

	if (!rbddElem.getAdapter()->existVar(restriction)) {
		Rcerr << "The restriction variable provided is not included in the BDD." << endl;
		return false;
	}

	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		if (expression <= 0 || expression > (int) rbddElem.getSynExpMap().size()) {
			Rcerr << "Undefined expression. Check the expression you have chosen or create it with the bdd_new_variable or bdd_parse commands." << endl;
			return false;
		}

		vector<synExp*> varMapTemp = rbddElem.getVarMap();
		bool found = false;
		int i = 0;

		while (i < (int) varMapTemp.size() && !found) {
			string symbolTemp = varMapTemp[i]->getSymbol();
			found = restriction == symbolTemp;
			i++;
		}
		
		if (!found) {
			Rcerr << "Variable " << restriction << " does not exist." << endl;
			return false;
		}

		vector<synExp*> varMapTemp2 = rbddElem.getVarMap();
		found = false;
		i = 0;

		while (i < (int) varMapTemp2.size() && !found) {
			string symbolTemp = varMapTemp2[i]->getSymbol();
			found = restriction_name == symbolTemp;
			i++;
		}
		
		if (found) {
			Rcerr << "Variable has not been created because it is duplicated." << endl;
			return false;
		}

		configInfo* ci = new configInfo();
		ci->setName(restriction_name);
		ci->setVartype("boolean");
		
		symbolTable::addSymbol(restriction_name, ci);

		vector<synExp*> synExpMapTemp = rbddElem.getSynExpMap();
		i = 1;
		int j = 0;
		string name = "";

		while (i <= expression) {
			if (i == expression) {
				name = synExpMapTemp[j]->getSymbol();
			}
			j++;
			i++;
		}
	}

	while (restriction_name[0] == ' ') {
		restriction_name = restriction_name.substr(1);
	}

	while (restriction_name[restriction_name.size() - 1] == ' ') {
		restriction_name = restriction_name.substr(0, restriction_name.size() - 1);
	}
	
	if (restriction_name.size() == 0) {
		Rcerr << "A restriction_name must be provided." << endl;
		return false;
	}

	int nameCount = 0;
	bool isValid = true;
	char current;
	while (isValid && nameCount < (int) restriction_name.size()) {
		current = restriction_name[nameCount];
		isValid = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9');
		nameCount++;
	}

	if (!isValid) {
		Rcerr << "The name of the variable can not contain the \"" << current << "\" character." << endl;
		return false;
	}

	if (buddy) {
		rbddElem.getAdapter()->restrictBDD(name, restriction, restriction_name, positive_form);
	} else {
		rbddElem.getAdapter()->restrictBDD(bdd_name, restriction, restriction_name, false);
	}

	rbddElem.addSynExpMap(restriction_name);
	rbddElem.addVarMap(restriction_name);
	rbddElem.addNameList(restriction_name);

	rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);

	return true;
}

// [[Rcpp::export]]
Rcpp::List bdd_traverse(Rcpp::List value_zero, Rcpp::List value_one, std::string function_to_apply, bool return_node = false, int num_threads = 1, std::string bdd_name = "default", bool trace = false) {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	Environment env = Environment::global_env();
	
	CharacterVector names = env.ls(false);
	bool found = false;
	int current = 0;

	while (!found && current < names.size()) {
		string nameFunction = as<string>(names[current]);
		found = nameFunction == function_to_apply;
		current++;
	}

	if (!found) {
		Rcerr << "Function provided does not exist in the environment." << endl;
		return 0;
	}

	Function myFunct = env[function_to_apply];
	SEXP sexp_funct = myFunct.get__();
	SEXP funct_formals = FORMALS(sexp_funct);

	if (Rf_length(funct_formals) < 5 || Rf_length(funct_formals) > 6) {
		Rcerr << "Function provided (" << function_to_apply << ") must include 5 or 6 parameters." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	cAdapter->clearNodeList();
	cAdapter->setFunction(myFunct);
	cAdapter->setTraverseBDD(bdd_name);
	
	if (num_threads <= 1) {
		if (return_node) {
			return traverse(trace, cAdapter, value_zero, value_one, RbddFunction::traverseBddNode);
		} else {
			return traverse(trace, cAdapter, value_zero, value_one, RbddFunction::traverseBdd);
		}
	} else {
		if (return_node) {
			return traverseMP(trace, num_threads, cAdapter, value_zero, value_one, RbddFunction::traverseBddNode, false);
		} else {
			return traverseMP(trace, num_threads, cAdapter, value_zero, value_one, RbddFunction::traverseBdd, false);
		}
	}
}

// [[Rcpp::export]]
int bdd_traverse_root_node(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	return cAdapter->getNodePos(cAdapter->getBDD(cAdapter->getTraverseBDD()));
}

// [[Rcpp::export]]
bool bdd_traverse_is_node_zero(int node, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	return cAdapter->isNodeZero(node);
}

// [[Rcpp::export]]
bool bdd_traverse_is_node_one(int node, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	return cAdapter->isNodeOne(node);
}

// [[Rcpp::export]]
int bdd_traverse_get_level(int node, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	return cAdapter->getLevel(cAdapter->getNode(node));
}

// [[Rcpp::export]]
int bdd_traverse_get_var_at_pos(int pos, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	return cAdapter->varAtPos(pos);
}

// [[Rcpp::export]]
Rcpp::NumericVector bdd_traverse_get_children(int node, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));

	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return 0;
	}
	
	buddyAdapter* bAdapter = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter());
	bool buddy = bAdapter != nullptr;
	if (buddy) {
		Rcerr << "Instruction not suppported by the BuDDy library." << endl;
		return 0;
	}

	cuddAdapter* cAdapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	
	std::vector<int> children = cAdapter->getChildrenNodes(node);
	Rcpp::NumericVector result = {(double) children[0], (double) children[1]};

	return result;
}





/**
 * Debugging functions
 */

// [[Rcpp::export]]
std::vector<std::string> bdd_info_variables(bool xverbose = true, std::string bdd_name = "default") {
	std::vector<std::string> result;

	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return result;
	}

	if (xverbose) {
		Rcout << "++++++++++++++++++++++++++++" << endl;
		Rcout << "Index variable -> Expression" << endl;
		Rcout << "++++++++++++++++++++++++++++" << endl;
	}

	return rbddElem.getAdapter()->getVars(xverbose);
}

// [[Rcpp::export]]
int bdd_info_variable_number(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	return rbddElem.getAdapter()->getNumVars();
}

// [[Rcpp::export]]
void bdd_info_boolstr(int expression, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else if (expression <= 0 || expression > (int) rbddElem.getSynExpMap().size()) {
		Rcerr << "Undefined expression. Check the expression you have chosen or create it with the bdd_parse_boolstr command." << endl;
	} else {
		synExp* se = rbddElem.getSynExpMap()[expression - 1];
		Rcout << "The expression is " << se << endl;
	}
}

// [[Rcpp::export]]
int bdd_info_node_number(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return -1;
	}

	return rbddElem.getAdapter()->nodecount(bdd_name);
}

// [[Rcpp::export]]
bool bdd_manager_is_initialized(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return false;
	}
	
	return rbddElem.getAdapter() != NULL;
}

// [[Rcpp::export]]
void bdd_info_manager_library(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "The BDD does not exist" << endl;
	} else {
		bool buddy = dynamic_cast<buddyAdapter*> (rbddElem.getAdapter()) != nullptr;
		bool cudd = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter()) != nullptr;

		if (buddy) {
			Rcout << "BuDDy" << endl;
		} else if (cudd) {
			Rcout << "CUDD" << endl;
		}
	}
}

// [[Rcpp::export]]
void bdd_print(std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else {
		Rcpp::Rcout << rbddElem.getAdapter()->printBDD(bdd_name) << std::endl;
	}
}





/**
 * Customizing functions
 */

// [[Rcpp::export]]
void bdd_set_cache_ratio(double cache_ratio, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else {
		rbddElem.getAdapter()->setCacheRatio(cache_ratio);
		
		rbddElem.setCachesize((int) cache_ratio);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}

// [[Rcpp::export]]
void bdd_set_max_node_num(int size, std::string bdd_name = "default") {
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
	} else {
		rbddElem.getAdapter()->setMaxNodeNum(size);
		
		rbddElem.setNodenum(size);
		rbddElems = RbddUtils::replaceRbddElem(rbddElem, rbddElems);
	}
}





/**
 * Algorithms implementation
 */

// [[Rcpp::export]]
StringVector bdd_get_var_probabilities(int num_threads = 1, bool trace = false, bool fast = false, std::string bdd_name = "default", std::string restriction = "") {
	StringVector result;
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return result;
	}

	cuddAdapter* adapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	bool cudd = adapter != nullptr;

	if (!cudd) {
		Rcerr << "Function not available for BuDDy. Initialize the BDD using CUDD in order to use the function." << endl;
		return result;
	}

	if (restriction != "") {
		adapter->setTraverseBDD(restriction);
	}

	std::vector<mpf_class> resultInt = adapter->computeCommonality(num_threads, trace, fast);
	adapter->setTraverseBDD(bdd_name);

	std::vector<std::string> vars = adapter->getVars(true);

	for (int i = 0; static_cast<unsigned int>(i) < resultInt.size(); i++) {
		result.push_back(vars[i]);

		mp_exp_t e;
        std::string elem = resultInt[i].get_str(e, 10, 512);
        int exp = (int) e;
        exp = exp*-1;
        std::string pre = "0.";
        for (int j = 0; j < exp; j++) {
            pre = pre + "0";
        }
        elem = pre + elem;
		result.push_back(elem);
	}

	return result;
}

// [[Rcpp::export]]
StringVector bdd_get_sat_distribution(int num_threads = 1, bool trace = false, bool fast = false, std::string bdd_name = "default", std::string restriction = "") {
	StringVector result;
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return result;
	}

	cuddAdapter* adapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	bool cudd = adapter != nullptr;

	if (!cudd) {
		Rcerr << "Function not available for BuDDy. Initialize the BDD using CUDD in order to use the function." << endl;
		return result;
	}

	if (restriction != "") {
		adapter->setTraverseBDD(restriction);
	}
	
	std::vector<mpz_class> resultInt = adapter->computeDistribution(num_threads, trace, fast);
	adapter->setTraverseBDD(bdd_name);

	for (int i = 0; static_cast<unsigned int>(i) < resultInt.size(); i++) {
		result.push_back(resultInt[i].get_str());
	}

	return result;
}

// [[Rcpp::export]]
List bdd_get_core_dead(int num_threads = 1, bool trace = false, bool fast = false, std::string bdd_name = "default") {
	List result;
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return result;
	}

	cuddAdapter* adapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	bool cudd = adapter != nullptr;

	if (!cudd) {
		Rcerr << "Function not available for BuDDy. Initialize the BDD using CUDD in order to use the function." << endl;
		return result;
	}

	std::pair<std::vector<std::string>, std::vector<std::string>> coreDead = adapter->getCoreDead(num_threads, trace, fast);

	result = List::create(Named("core") = coreDead.first, Named("dead") = coreDead.second);

	return result;
}

// [[Rcpp::export]]
List bdd_get_uniform_random_sampling(bool name = false, int num_threads = 1, bool trace = false, bool fast = false, std::string bdd_name = "default") {
	List result;
	RbddElem rbddElem(RbddUtils::getRbddElem(bdd_name, rbddElems));
	if (rbddElem.getAdapter() == NULL) {
		Rcerr << "There is not a BDD created with name " << bdd_name << ". Create it with the bdd_manager_init command." << endl;
		return result;
	}

	cuddAdapter* adapter = dynamic_cast<cuddAdapter*> (rbddElem.getAdapter());
	bool cudd = adapter != nullptr;

	if (!cudd) {
		Rcerr << "Function not available for BuDDy. Initialize the BDD using CUDD in order to use the function." << endl;
		return result;
	}

	if (name) {
		std::vector<std::string> res;
		res.push_back(adapter->uniformRandomSamplingName(num_threads, trace, fast));
		result = List::create(res);
	} else {
		result = List::create(adapter->uniformRandomSamplingBool(num_threads, trace, fast));
	}

	return result;
}

Rcpp::List RbddFunction::traverseBdd(int plevel, int tlevel, int elevel, const Rcpp::List& tr, const Rcpp::List& er, Rcpp::Function functToApply) {
	Rcpp::List result = functToApply(plevel, tlevel, elevel, tr, er);
	return result;
}

Rcpp::List RbddFunction::traverseBddNode(int plevel, int tlevel, int elevel, const Rcpp::List& tr, const Rcpp::List& er, int node, Rcpp::Function functToApply) {
	Rcpp::List result = functToApply(plevel, tlevel, elevel, tr, er, node);
	return result;
}
