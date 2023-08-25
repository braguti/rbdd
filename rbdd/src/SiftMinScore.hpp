//
//  SiftMinScore.hpp
//
//
//  Created by david on 21/06/16.
//  Copyright © 2016 david. All rights reserved.
//

#ifndef SiftMinScore_hpp
#define SiftMinScore_hpp

#include <stdio.h>
#include <set>
#include <fstream>
#include <vector>
#include <map>

#include "humanNums.hpp"
#include "synExpDriver.hpp"

struct siftRes {
    siftRes() {
    score = std::numeric_limits<int>::max();
    };
    siftRes(int p, double s, std::vector<int> v2p, std::vector<int> p2v) : score(s), pos(p), v2p(v2p), p2v(p2v) {};
    double score;
    int    pos;
    std::vector<int> v2p, p2v;
};

class SiftMinScore {
    
    public :
    
    SiftMinScore(const std::string& varFile, const std::string& expFile);
	SiftMinScore(const std::string expression, std::map<int, synExp*> synExpMap);
    
    void go();

	void setParsingResult(synExp* expression);
	synExp* getParsingResult();
    
	void setParserResult(int result);
	int getParserResult();
    
    protected :
    
    bool    sift(int var);
    void    computeScore();
    int     computeSpan(int exp);
    double  computeScore(int exp);
    double  computeOrderScore(const std::vector<int>& v2p, int exp);
    double  computeOrderScore(const std::vector<int>& v2p);
    int     computeOrderSpan(const std::vector<int>& v2p, int exp);
    bool    tooBigSpan(const std::vector<int>& o, int pos1, int pos2);
    
    void    siftSet();
    siftRes siftUp  (std::vector<int> v2p, std::vector<int> p2v, int var);
    siftRes siftDown(std::vector<int> v2p, std::vector<int> p2v, int var);

    
    double  computeScore2(const std::vector<int>& v2p, int pos, int otherpos);
    
    
    void exchange(std::vector<int>& v2p, std::vector<int>& p2v, int pos1, int pos2);

    std::map<std::string, int>                      varMap;
    std::vector<std::vector<int> >                  expressions;
    std::vector< std::vector<int> >                 expressionsVar;
    std::vector<std::string>                        variables;
    std::vector<int>                                pos2var, var2pos;
    int                                             max;
    double                                          score;
    std::set<int>                                   maxSet;
    int                                             maxRollBacks = 10;
    
    private:

	synExp* parsingResult;
	int parserResult;
    
};
#endif /* SiftMinScore_hpp */
