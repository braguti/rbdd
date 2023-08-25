
#include <vector>
#include "synExp.hpp"

class ParserCNF {
	
	public:
		ParserCNF(std::vector<synExp*> synExpMap);
		synExp* parse(std::string expression);

	private:
		std::vector<synExp*> synExpMap;

		std::string removeSpaces(std::string expression);
		synExp* makeOrOperation(std::string expression);
};

