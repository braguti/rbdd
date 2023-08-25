//
//  SiftMinScore.cpp
//  myKconf
//
//  Created by david on 21/06/16.
//  Copyright © 2016 david. All rights reserved.
//

#include "SiftMinScore.hpp"


SiftMinScore::SiftMinScore(const std::string& varFile, const std::string& expFile) {
    // We read the variables in order
    std::ifstream vf(varFile.c_str());
    std::ifstream ef(expFile.c_str());
    std::string temp;
    while (vf >> temp)
        variables.push_back(temp);
    
    // We parse the expression file
    kconf::synExpDriver driver;
    driver.parse_file(expFile);
    std::vector<synExp*> expList  = driver.getConstraints();
    // We initialize the order and the inverse mapping
    for(int x = 0; x < variables.size(); x++) {
        var2pos.push_back(x);
        pos2var.push_back(x);
        varMap[variables.at(x)] = x;
    }
    //std::cerr << "Read " << variables.size() << " variables " << std::endl;
    //exit(-1);
    // Now we go with the expressions
    for(synExp *s : expList) {
        s->computeIndices(varMap);
        std::vector<int> vtemp;
        for(int i : s->giveSymbolIndices())
            vtemp.push_back(i);
        expressions.push_back(vtemp);
    }
    // For each var we make a list of the related expressions
    expressionsVar.resize(variables.size());
    for(int exp = 0; exp < expressions.size(); exp++)
        for(auto var : expressions.at(exp)) {
            if (std::find(expressionsVar.at(var).begin(), expressionsVar.at(var).end(), exp) == expressionsVar.at(var).end())
                expressionsVar.at(var).push_back(exp);
        }
}

SiftMinScore::SiftMinScore(const std::string expression, std::map<int, synExp*> synExpMap) {
	// We parse the expression file
    kconf::synExpDriver driver;
    bool success = driver.parse_string(expression, synExpMap);
	setParserResult(driver.getCorrectParsing() && success);
	if (getParserResult()) {
		setParsingResult(driver.getConstraints()[driver.getConstraints().size() - 1]);
	}
}

void SiftMinScore::setParsingResult(synExp* expression) {
	this->parsingResult = expression;
}

synExp* SiftMinScore::getParsingResult() {
	return this->parsingResult;
}

void SiftMinScore::setParserResult(int result) {
	this->parserResult = result;
}

int SiftMinScore::getParserResult() {
	return this->parserResult;
}

void SiftMinScore::go() {
    computeScore();
    double startScore = score;
    double lastScore;
    std::cerr.precision(10);
    bool change = false;
    std::cerr << "Score is " << std::setw(10) << std::fixed << score << std::endl;

    for(;;) {
        computeScore();
        lastScore = score;
        

        for(int i = 0; i < variables.size(); i++)
            change |= sift(i);
        std::cerr << "Score is " << std::setw(10) << std::fixed << score
                  << " reduction " << std::setw(4) << std::fixed << 100 - 100.0*score/startScore << "%"
                    << std::endl;

        if (!change|| (lastScore - score)/lastScore < 1E-10) break;
    }
    std::cerr << "Waro " << (lastScore -score)/lastScore << std::endl;
    computeScore();
    std::cerr << "Going to write result, score is " << std::setw(10) << std::fixed << score << std::endl;
    // Check that pos2var and var2pos are inverse of each other
    if (var2pos.size() != pos2var.size()) {
        std::cerr << "Error type 1" << std::endl;
        exit(-1);
    }
    for(int x = 0; x < pos2var.size(); x++)
        if (var2pos.at(pos2var.at(x)) != x || pos2var.at(var2pos.at(x)) != x) {
            std::cerr << "Error, var2pos and pos2var not inverses of each other" << std::endl;
            exit(-1);
        }
    std::set<int> check1, check2;
    for(int i : var2pos) check1.insert(i);
    for(int j : pos2var) check2.insert(j);
    if (check1.size() != check2.size()) {
        std::cerr << "Error type 2" << std::endl;
    }
    if (check1.size() != var2pos.size()) {
        std::cerr << "Error type 3" << std::endl;
    }
    int c = 0;
    for(int i : check1)
        if (i != c++) {
            std::cerr << "Error type 4 in " << i << " " << c-1 << std::endl;
            exit(-1);
        }
    c = 0;
    for(int i : check2)
        if (i != c++) {
            std::cerr << "Error type 5 in " << i << " " << c-1 << std::endl;
            exit(-1);
        }
        
    for(int x = 0; x < variables.size(); x++) {
        if (varMap.at(variables.at(x)) != x) {
            std::cerr << "Error type 6 in " << x << std::endl;
            exit(-1);
            }
    }
    for(int x : pos2var)
        std::cout << variables.at(x) << " ";
    std::cout << std::endl;
    std::cerr << "Done!" << std::endl;
}

int SiftMinScore::computeSpan(int exp) {
    int smax = -1;
    int smin = std::numeric_limits<int>::max();
    for(auto v : expressions[exp]) {
        if (var2pos[v] < smin) smin = var2pos[v];
        if (var2pos[v] > smax) smax = var2pos[v];
    }
    return smax - smin;
}

int SiftMinScore::computeOrderSpan(const std::vector<int>& v2p, int exp) {
    int smax = -1;
    int smin = std::numeric_limits<int>::max();
    for(auto v : expressions[exp]) {
        if (v2p[v] < smin) smin = v2p[v];
        if (v2p[v] > smax) smax = v2p[v];
    }
    return smax - smin;
}

bool SiftMinScore::tooBigSpan(const std::vector<int>& v2p, int var1, int var2) {
    for(auto exp : expressionsVar[var1])
        if (computeOrderSpan(v2p, exp) > max)
            return true;
    
    for(auto exp : expressions[var2] )
        if (std::find(expressions[exp].begin(), expressions[exp].end(), var1) == expressions[exp].end())
            if (computeOrderSpan(v2p, exp) > max)
                return true;
    return false;
}

bool SiftMinScore::sift(int var) {
    siftRes resUp, resDown;
    bool change = false;
    resUp   = siftUp  (var2pos, pos2var, var);
    resDown = siftDown(var2pos, pos2var, var);
    int bestPos;
    if ((resUp.score < resDown.score) && resUp.score < score ) {
        bestPos = resUp.pos;
        score   = resUp.score;
        var2pos = resUp.v2p;
        pos2var = resUp.p2v;
        change  = true;
    }
    else if (resDown.score < score) {
        bestPos = resDown.pos;
        score   = resDown.score;
        var2pos = resDown.v2p;
        pos2var = resDown.p2v;
        change  = true;
    }
//    double otherScore = score;
//    computeScore();
//    if (score - otherScore > 1E-15) {
//        std::cerr << "Score and otherScore are different: " << score - otherScore << std::endl;
//        exit(-1);
//    }
    return change;
   }

void SiftMinScore::exchange(std::vector<int>& v2p, std::vector<int>& p2v, int pos1, int pos2) {
    int var1 = p2v.at(pos1);
    int var2 = p2v.at(pos2);
    
    v2p.at(var1)            = pos2;
    v2p.at(var2)            = pos1;
    p2v.at(pos1)            = var2;
    p2v.at(pos2)            = var1;
}

siftRes SiftMinScore::siftUp(std::vector<int> v2p, std::vector<int> p2v, int var) {
    int pos                     =   v2p[var];
    double localScore           =   score;
    int bestPos                 =   pos;
    double bestScore            =   localScore;
    std::vector<int> bestv2p    =   v2p;
    std::vector<int> bestp2v    =   p2v;
    int temp;
    while (pos > 0) {
        int var2             = p2v.at(pos-1);
        localScore          -= computeScore2(v2p, var, var2);
        exchange(v2p, p2v, pos, pos-1);
        localScore          += computeScore2(v2p, var, var2);
//        if (tooBigSpan(v2p, pos, pos-1)) {
//            std::cerr << "span is too big" << std::endl;
//            break;
//        }
        
        if (localScore < bestScore) {
            //std::cerr << "Best score siftUp " << std::fixed << localScore << std::endl;
            bestPos     = pos;
            bestScore   = localScore;
            bestv2p     = v2p;
            bestp2v     = p2v;
        }
        pos--;
    }
    siftRes res(bestPos, bestScore, bestv2p, bestp2v);
    return res;
}

siftRes SiftMinScore::siftDown(std::vector<int> v2p, std::vector<int> p2v, int var) {
    int pos                     = v2p[var];
    double localScore           = score;
    int bestPos                 = pos;
    double bestScore            = localScore;
    std::vector<int> bestv2p    = v2p;
    std::vector<int> bestp2v    = p2v;
    int temp;

    while (pos < v2p.size() - 1) {
        int var2 = p2v[pos+1];
        localScore          -= computeScore2(v2p, var, var2);
        exchange(v2p, p2v, pos, pos+1);
            localScore          += computeScore2(v2p, var, var2);
//        if (tooBigSpan(p2v, pos, pos+1) > max) {
//            std::cerr << "span is too big" << std::endl;
//            break;
//        }
        
        if (localScore < bestScore) {
            //std::cerr << "Best score siftDown " << std::fixed << localScore << std::endl;

            bestPos     = pos;
            bestScore   = localScore;
            bestv2p     = v2p;
            bestp2v     = p2v;
        }
        pos++;
    }
    siftRes res(bestPos, bestScore, bestv2p, bestp2v);
    return res;
}

void SiftMinScore::computeScore() {
    double tot = 0;
    for(int exp = 0; exp < expressions.size(); exp++) {
        tot += computeScore(exp);
    }
    
    score = tot;

}

double SiftMinScore::computeOrderScore(const std::vector<int>& v2p) {
    double tot = 0;
    for(int exp = 0; exp < expressions.size(); exp++) {
        tot += computeOrderScore(v2p, exp);
    }
    
    return tot;
}

double SiftMinScore::computeScore(int exp) {
    int smax = -1;
    int smin = std::numeric_limits<int>::max();
    for(auto v : expressions[exp]) {
        if (var2pos.at(v) < smin) smin = var2pos.at(v);
        if (var2pos.at(v) > smax) smax = var2pos.at(v);
    }
    return log(smax - smin +1);
}

double SiftMinScore::computeOrderScore(const std::vector<int>& v2p, int exp) {
    int smax = -1;
    int smin = std::numeric_limits<int>::max();
    for(auto v : expressions.at(exp)) {
        if (v2p.at(v) < smin) smin = v2p.at(v);
        if (v2p.at(v) > smax) smax = v2p.at(v);
    }
    return log(smax - smin +1);
}

double SiftMinScore::computeScore2(const std::vector<int>& v2p, int var1, int var2) {
    double tot  = 0;
    for(auto exp : expressionsVar.at(var1)) {
        tot += computeOrderScore(v2p, exp);
    }
    
    for(auto exp : expressionsVar.at(var2) )
        if (std::find(expressions.at(exp).begin(), expressions.at(exp).end(), var1) == expressions.at(exp).end()) {
            tot += computeOrderScore(v2p, exp);
        }
    
    return tot;
}


