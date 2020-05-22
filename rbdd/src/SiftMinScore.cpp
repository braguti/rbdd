//
//  SiftMinScore.cpp
//  myKconf
//
//  Created by david on 21/06/16.
//  Copyright Â© 2016 david. All rights reserved.
//

#include "SiftMinScore.hpp"

class auxOrder {
public:
    auxOrder(int exp, int span) : exp(exp), span(span) {};
    int exp, span;
    bool operator()(auxOrder a, auxOrder b) {
        if (a.span != b.span)
            return a.span > b.span;
        return a.exp > b.exp;
    }
};


SiftMinScore::SiftMinScore(const std::string& varFile, const std::string& expFile) {
    readInfo(varFile, expFile);
    max = std::numeric_limits<int>::max();
    mbExpPosSet.resize(expressions.size());
    expMin.resize(expressions.size());
    expMax.resize(expressions.size());
    expScore.resize(expressions.size());
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

void SiftMinScore::readInfo(const std::string& varFile, const std::string& expFile) {
    // We read the variables in order
    std::ifstream vf(varFile.c_str());
    std::ifstream ef(expFile.c_str());
    std::string temp;
    while (vf >> temp)
        variables.push_back(temp);
    
    // We parse the expression file
    kconf::synExpDriver driver;
    driver.parse_file(expFile);
    expList  = driver.getConstraints();
    // We initialize the order and the inverse mapping
    for(int x = 0; static_cast<unsigned int>(x) < variables.size(); x++) {
        var2pos.push_back(x);
        pos2var.push_back(x);
        varMap[variables.at(x)] = x;
    }
    // Now we go with the expressions
    for(synExp *s : expList) {
        s->computeIndices(varMap);
        std::set<int> vtemp;
        for(int i : s->giveSymbolIndices())
            vtemp.insert(i);
        expressions.push_back(vtemp);
    }
   fillExpressionsVar();
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

void SiftMinScore::goGroup() {
    computeScore();
    Rcpp::Rcerr.precision(10);
    bool change = false;
    Rcpp::Rcerr << "Score is " << std::setw(10) << std::fixed << score << std::endl;
    std::vector<auxOrder> orderVector;
    for(int exp = 0; static_cast<unsigned int>(exp) < expressions.size(); exp++)
        orderVector.push_back(auxOrder(exp, computeSpan(exp)));
    auxOrder o(0,0);
    std::sort(orderVector.begin(), orderVector.end(), o);
    std::set<int> sExp;
    for(;;) {
        computeScore();
        lastScore = score;
        for(auxOrder aux : orderVector) {
            change |= siftGroup(aux.exp,sExp);
        }
        float reduction = 100.0*(lastScore - score)/lastScore;
        Rcpp::Rcerr << "Score is " << std::setw(10) << std::fixed << score
                    << " reduction " << std::setw(4) << std::fixed << reduction << "%"
                    << " less than 10?" << (reduction < 10.0) << std::endl;
        
        if (!change || (reduction < 10.0)) break;
    }
    wrapUp();
}

void SiftMinScore::go() {
    computeScore();
    Rcpp::Rcerr << "Score is " << std::setw(10) << std::fixed << score << " vars " << variables.size() << std::endl;
    MultiComponents comp;
    //for(auto p : var2pos)
	for (int i = 0; static_cast<unsigned int>(i) < var2pos.size(); i++)
        comp.newVariable();
    for(synExp* s : expList)
        comp.join(s, false);
    comp.reorder();
    computeScore();
    utScore startScore = score;
    utScore lastScore;
    Rcpp::Rcerr.precision(10);
    bool change;
    Rcpp::Rcerr << "Score is (after MultiComponents) " << std::setw(10) << std::fixed << score << " vars " << variables.size() << std::endl;
    computeExpInfo(var2pos);
    for(;;) {
        change    = false;
        lastScore = score;
        computeScore();
        if (lastScore != score) {
            Rcpp::stop("Last score %f score %f", lastScore, score);
        }
        for(int i = 0; static_cast<unsigned int>(i) < variables.size(); i++) {
            change |= sift(i);
        }
        double reduction, lastreduction;
        if (lastScore != 0 && startScore != 0) {
            reduction     = 100.0*(startScore - score)/startScore;
            lastreduction = 100.0*(lastScore - score)/lastScore;
        }
        else {
            reduction     = 0;
            lastreduction = 0;
        }
        Rcpp::Rcerr << "Score is " << std::setw(10) << std::fixed << score
                    << " reduction " << std::setw(4) << std::fixed << reduction << "%"
                    << " last reduction " << std::setw(4) << std::fixed << lastreduction << "%"<< std::endl;
        wrapUp();

        if (!change) break;
    }
   }
    
void  SiftMinScore::wrapUp() {
    MultiComponents comp;
    //for(auto p : var2pos)
	for (int i = 0; static_cast<unsigned int>(i) < var2pos.size(); i++)
        comp.newVariable();
    for(synExp* s : expList)
        comp.join(s, false);
    comp.reorder();
    computeScore();
    // Check that pos2var and var2pos are inverse of each other
    if (var2pos.size() != pos2var.size()) {
        Rcpp::stop("Error type 1");
    }
    for(int x = 0; static_cast<unsigned int>(x) < pos2var.size(); x++)
        if (var2pos.at(pos2var.at(x)) != x || pos2var.at(var2pos.at(x)) != x) {
            Rcpp::stop("Error, var2pos and pos2var not inverses of each other");
        }
    
    std::set<int> check1, check2;
    for(int i : var2pos) check1.insert(i);
    for(int j : pos2var) check2.insert(j);
    if (check1.size() != check2.size()) {
        Rcpp::Rcerr << "Error type 2" << std::endl;
    }
    if (check1.size() != var2pos.size()) {
        Rcpp::Rcerr << "Error type 3" << std::endl;
    }
    int c = 0;
    for(int i : check1)
        if (i != c++) {
            Rcpp::stop("Error type 4 in %d %d", i, c-1);
        }
    c = 0;
    for(int i : check2)
        if (i != c++) {
            Rcpp::stop("Error type 5 in %d %d", i, c-1);
        }
    
    for(int x : pos2var) {
        Rcpp::Rcout << variables.at(x) << " ";
    }
    Rcpp::Rcout << std::endl;
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

bool SiftMinScore::stopCondition(const std::vector<int>& v2p, int var1, int var2) {
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
    getBounds(var);
    Rcpp::Rcerr << "Sift " << std::setw(6) << var << std::setw(50) << variables.at(var) << " score " << score << std::endl;
    resUp   = siftUp  (score, var2pos, pos2var, expMin, expMax, expScore, mbExpPosSet, var);
    resDown = siftDown(score, var2pos, pos2var, expMin, expMax, expScore, mbExpPosSet, var);
    
    int pos = var2pos[var];
    if ((resUp.score < resDown.score) && resUp.score < score ) {
        while (pos != resUp.pos) {
            exchangeUpScore(var2pos, pos2var, expMin, expMax, expScore, mbExpPosSet, pos, score);
            pos--;
        }
        if (score != resUp.score) {
            Rcpp::stop("Down score %f resUp.score %f", score, resDown.score);
        }
        change  = true;
    }
    else if (resDown.score < score) {

        while (pos != resDown.pos) {
            exchangeDownScore(var2pos, pos2var, expMin, expMax, expScore, mbExpPosSet, pos, score);
            pos++;
        }
        if (score != resDown.score) {
            Rcpp::stop("score %f resDown.score %f", score, resDown.score);
        }
        change  = true;
    }
    return change;
   }

bool SiftMinScore::siftGroup(int exp, std::set<int>& sExp) {
    
    siftRes resUp, resDown;
    sExp.insert(exp);
    bool change = false;
    std::set<int> varGroup;
    
    for(int i : expressions.at(exp))
        varGroup.insert(i);
    
    utScore myScore  = computeSetScore(var2pos, sExp);
    Rcpp::Rcerr << " Before Score is " << std::setw(15) << myScore;
    
    resUp           = siftUpGroup  (var2pos, pos2var, varGroup, sExp);
    resDown         = siftDownGroup(var2pos, pos2var, varGroup, sExp);
    
    if ((resUp.score < resDown.score) && resUp.score < myScore ) {
        myScore     = resUp.score;
        //var2pos     = resUp.v2p;
        //pos2var     = resUp.p2v;
        change      = true;
    }
    else if (resDown.score < myScore) {
        myScore     = resDown.score;
        //var2pos     = resDown.v2p;
        //pos2var     = resDown.p2v;
        change      = true;
    }
    
    Rcpp::Rcerr << " Exp " << std::setw(5) << sExp.size() << " Score is " << std::setw(15) << myScore;
    Rcpp::Rcerr << std::endl;
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

siftRes SiftMinScore::siftUp(utScore startScore, std::vector<int> v2p, std::vector<int> p2v,
                              std::vector<int> eMin,
                              std::vector<int> eMax,
                              std::vector<utScore> eScore,
                              std::vector<std::vector<bool> > eps,
                              int var) {

    int pos                     =   v2p[var];
    utScore localScore           =   startScore;
    siftRes res(localScore, pos);
    while (pos > lowerBound) {
        exchangeUpScore(v2p, p2v, eMin, eMax, eScore, eps, pos, localScore);
        pos--;
        if (localScore < res.score) {
            res = siftRes(localScore, pos);
        }
    }
    return res;
}

siftRes SiftMinScore::siftDown(utScore startScore, std::vector<int> v2p, std::vector<int> p2v,
                               std::vector<int> eMin,
                               std::vector<int> eMax,
                               std::vector<utScore> eScore,
                               std::vector<std::vector<bool> > eps,
                               int var) {
    int pos                      =   v2p[var];
    utScore localScore           =   startScore;

    siftRes res(localScore, pos);

    while (pos < upperBound - 1) {
        exchangeDownScore(v2p, p2v, eMin, eMax, eScore, eps, pos, localScore);
        pos++;
        if (localScore < res.score) {
            res = siftRes(localScore, pos);
        }
    }
    return res;
}

void SiftMinScore::exchangeUpScore(std::vector<int>& v2p,
                                   std::vector<int>& p2v,
                                   std::vector<int>& eMin,
                                   std::vector<int>& eMax,
                                   std::vector<utScore>& eScore,
                                   std::vector<std::vector<bool> >& eps,
                                   int pos, utScore& localScore) {
    localScore          -= computeScore2(v2p, p2v.at(pos),  p2v.at(pos-1));
    exchange(v2p, p2v, pos, pos-1);
    localScore          += computeScore2(v2p, p2v.at(pos), p2v.at(pos-1));
}

void SiftMinScore::exchangeDownScore(std::vector<int>& v2p,
                                     std::vector<int>& p2v,
                                     std::vector<int>& eMin,
                                     std::vector<int>& eMax,
                                     std::vector<utScore>& eScore,
                                     std::vector<std::vector<bool> >& eps,
                                     int pos, utScore& localScore) {
    localScore          -= computeScore2(v2p, p2v.at(pos), p2v.at(pos+1));
    exchange(v2p, p2v, pos, pos+1);
    localScore          += computeScore2(v2p, p2v.at(pos), p2v.at(pos+1));

}

siftRes SiftMinScore::siftUpGroup(std::vector<int> v2p, std::vector<int> p2v, std::set<int> varPack, std::set<int> s)  {
    std::set<int>   varPos;
    siftRes         res;
    utScore          beforeScore, afterScore, firstScore;
    for(int x : varPack)
        varPos.insert(var2pos.at(x));
    
    int posmaxblock         =   *std::max_element(varPos.begin(), varPos.end());
    firstScore              =   computeSetScore(v2p, s);
    for(;;) {
        int posminblock         =   posmaxblock;
        while (posminblock > 0 && varPack.find(p2v.at(posminblock-1)) != varPack.end())
            posminblock--;
        
        beforeScore = computeSetScore(v2p, s);

        // Now move block to the left
        if (posminblock > 0) {
            for(int x = posminblock; x <= posmaxblock; x++)
                exchange(v2p, p2v, x-1, x);
            
            posmaxblock--;
            posminblock--;
        }
        else
            break;
        
        afterScore = computeSetScore(v2p, s);
        if (afterScore > factor*firstScore) break;

//        if (score > localScore) {
//            // If the span is now higher, we roll back last block movement
//            for(int x = posmaxblock; x >= posminblock; x--)
//                exchange(v2p, p2v, x, x+1);
//            
//            computeOrderScore(v2p);
//            posmaxblock++;
//            posminblock++;
//            if (rollcounter++ < maxRollBacks)
//                posminblock--;
//            else
//                break;
//        }
//        else
            if (afterScore < beforeScore) {
            //res.v2p     = v2p;
            //res.p2v     = p2v;
            res.score   = afterScore;
        }
         }
    return res;
}

siftRes SiftMinScore::siftDownGroup(std::vector<int> v2p, std::vector<int> p2v, std::set<int> varPack, std::set<int> s) {
    std::set<int>   varPos;
    utScore          beforeScore, afterScore, firstScore;
    siftRes         res;
    for(int x : varPack)
        varPos.insert(var2pos.at(x));
    
    int posminblock         =   *std::min_element(varPos.begin(), varPos.end());
    firstScore              = computeSetScore(v2p, s);
    for(;;) {
        int posmaxblock         =   posminblock;
        while (static_cast<unsigned int>(posmaxblock+1) < p2v.size() && varPack.find(p2v.at(posmaxblock+1)) != varPack.end())
            posmaxblock++;
        
        beforeScore  = computeSetScore(v2p, s);
        // Now move block to the right
        if (static_cast<unsigned int>(posmaxblock) <  p2v.size() - 1) {
            for(int x = posmaxblock; x >= posminblock; x--)
                exchange(v2p, p2v, x, x+1);
            
            posmaxblock++;
            posminblock++;
        }
        else
            break;
    
        afterScore = computeSetScore(v2p, s);
        if (afterScore > factor*firstScore) break;
            if  (afterScore < beforeScore) {
            res.score   = afterScore;
        }
    }
    return res;
}


void SiftMinScore::computeScore() {
    utScore tot = 0;
    for(int exp = 0; static_cast<unsigned int>(exp) < expressions.size(); exp++)
        tot += computeScore(exp);
    
    score = tot;
}

utScore SiftMinScore::computeOrderScore(const std::vector<int>& v2p) {
    utScore tot = 0;
    for(int exp = 0; static_cast<unsigned int>(exp) < expressions.size(); exp++) {
        tot += computeOrderScore(v2p, exp, true);
    }
    
    return tot;
}

utScore SiftMinScore::computeSetScore(const std::vector<int>& v2p, std::set<int> s) {
    utScore tot = 0;
    for(int exp : s) {
        tot += computeOrderScore(v2p, exp, false);
    }
    return tot;
}

utScore SiftMinScore::computeScore(int exp) {
    int factor =  1;
    int smax   = -1;
    int smin   = std::numeric_limits<int>::max();
    for(auto v : expressions[exp]) {
        factor += weight(v);
        if (var2pos.at(v) < smin) smin = var2pos.at(v);
        if (var2pos.at(v) > smax) smax = var2pos.at(v);
    }

return giveScore(smax, smin);
}

utScore SiftMinScore::computeOrderScore(const std::vector<int>& v2p, int exp, bool verbose) {
    int factor =  1;
    int smax   = -1;
    int smin   = std::numeric_limits<int>::max();
    for(auto v : expressions.at(exp)) {
        factor += weight(v);
        if (v2p.at(v) < smin) smin = v2p.at(v);
        if (v2p.at(v) > smax) smax = v2p.at(v);
    }
    return giveScore(smax, smin);
}

int SiftMinScore::weight(int v) {
    return expressionsVar.at(v).size();
}

utScore SiftMinScore::computeScore2(const std::vector<int>& v2p, int var1, int var2) {
    utScore tot  = 0;
    for(auto exp : expressionsVar.at(var1)) {
        tot += computeOrderScore(v2p, exp, false);
    }
    
    for(auto exp : expressionsVar.at(var2) )
        if (std::find(expressions.at(exp).begin(), expressions.at(exp).end(), var1) == expressions.at(exp).end()) {
            tot += computeOrderScore(v2p, exp, false);
        }
    
    return tot;
}

utScore SiftMinScore::giveScore(int a, int b) {
    if (a == b) return 0;
    return (a-b)*log(a-b);
    
    if (a > b)
        return (a-b)*log(a-b);
    else
        return (b-a)*log(b-a);
}

void SiftMinScore::fillExpressionsVar() {
// For each var we make a list of the related expressions
expressionsVar.resize(variables.size());
for(int exp = 0; static_cast<unsigned int>(exp) < expressions.size(); exp++)
    for(auto var : expressions.at(exp))
        expressionsVar.at(var).insert(exp);
    
}

void    SiftMinScore::getBounds(int var) {
    lowerBound = 0;
    upperBound = variables.size();
    int min = std::numeric_limits<int>::max();
    int max = -1;
    for(auto exp : expressionsVar[var])
        for(int v : expressions[exp]) {
            if (var2pos[v] > max)
                max = var2pos[v];
            if (var2pos[v] < min)
                min = var2pos[v];
        }
    lowerBound = min;
    upperBound = max;
}
