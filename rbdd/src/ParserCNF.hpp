
#ifndef __ParserCNF__
#define __ParserCNF__

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <regex>

static std::vector<std::string> parseCNF(std::string fileName) {
	std::vector<std::string> variables;
	std::vector<std::string> clauses;

	if (fileName.substr(fileName.size() - 4) == ".cnf") {
		std::ifstream inputFile;
		std::string line;

		inputFile.open(fileName);
		if (!inputFile.is_open()) {
			Rcpp::Rcout << "File could not be opened. Check if the name is correct." << std::endl;
			return variables;
		}

		bool hasProblem = false;
		int numberOfVars = 0;
		int numberOfLines = 0;

		std::regex commentLine("c(\\s+.*)?");
		std::regex problemLine("p\\s+cnf\\s+\\d+\\s+\\d+");
		std::regex clauseLine("(-?\\d+\\s+)+0");

		while (!inputFile.eof()) {
			std::getline(inputFile, line);

			while (line[0] == ' ') {
				line = line.substr(1);
			}

			while (line[line.size() - 1] == ' ') {
				line = line.substr(0, line.size() - 1);
			}

			if (hasProblem && std::regex_match(line, commentLine)) {
				Rcpp::Rcout << "There is a comment line (c) in a wrong place." << std::endl;
				return variables;
			}

			if (hasProblem && std::regex_match(line, problemLine)) {
				Rcpp::Rcout << "There are multiple problem lines (p)." << std::endl;
				return variables;
			}

			if (!hasProblem && std::regex_match(line, problemLine)) {
				hasProblem = true;
				line = line.substr(line.find("cnf") + 4);
				while (line[0] == ' ') {
					line = line.substr(1);
				}

				numberOfVars = std::stoi(line.substr(0, line.find(" ")));
				line = line.substr(line.find(" "));

				while (line[0] == ' ') {
					line = line.substr(1);
				}

				numberOfLines = std::stoi(line);
			}

			if (!hasProblem && std::regex_match(line, clauseLine)) {
				Rcpp::Rcout << "There are clause lines before a problem line (p)." << std::endl;
				return variables;
			}

			if (hasProblem && std::regex_match(line, clauseLine)) {
				std::string clause = "";

				while (line.size() > 0) {
					if (line[0] == '-') {
						clause = clause + "not ";
						line = line.substr(1);
					}

					std::string currentVar = line.substr(0, line.find(" "));
					if (currentVar != "0" && std::find(variables.begin(), variables.end(), currentVar) == variables.end()) {
						variables.push_back(currentVar);
					}

					line = line.substr(line.find(" "));
					while (line[0] == ' ') {
						line = line.substr(1);
					}

					clause = clause + currentVar + " or ";

					if (line[0] == '0') {
						clause = clause.substr(0, clause.size() - 4);
						clauses.push_back(clause);
						line = line.substr(1);
					}
				}
			}

		}
	
		inputFile.close();

		//if (static_cast<int>(variables.size()) != numberOfVars) {
		//	Rcpp::Rcout << "The number of variables is not the same than the number defined in the problem line (p)." << std::endl;
		//	return variables;
		//}

		if (static_cast<int>(clauses.size()) != numberOfLines) {
			Rcpp::Rcout << "The number of clauses is not the same than the number defined in the problem line (p)." << std::endl;
			return variables;
		}

		std::ofstream varFile(fileName.substr(0, fileName.size() - 4) + ".var");
		if (varFile.is_open()) {
			int current = 0;
			for (std::vector<std::string>::iterator it = variables.begin(); it != variables.end(); ++it) {
				varFile << *it;
				if (current < static_cast<int>(variables.size()) - 1) {
					varFile << " ";
				}
				current++;
			}
			varFile.close();
		}

		std::ofstream expFile(fileName.substr(0, fileName.size() - 4) + ".exp");
		if (expFile.is_open()) {
			for (std::vector<std::string>::iterator it = clauses.begin(); it != clauses.end(); ++it) {
				expFile << *it;
					expFile << std::endl;
			}
			expFile.close();
		}

		return variables;
	} else {
		Rcpp::Rcout << "File has not .cnf extension." << std::endl;
		return variables;
	}
}

#endif /* ParserCNF */
