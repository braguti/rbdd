//
//  primalGraph.h
//  myKconf
//
//  Created by david on 16/12/13.
//  Copyright (c) 2013 david. All rights reserved.
//

#ifndef __primalGraph__
#define __primalGraph__

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <set>

class primalGraph {
    
    private:
    
        std::map<std::pair<std::string, std::string>, int>  pgraph;
        std::map<std::string, int>                          weight;

    public:
    
        int  getArc(std::string a, std::string b) {
            if (a < b) {
                if (pgraph.find(std::pair<std::string, std::string>(a,b)) != pgraph.end())
                    return pgraph[std::pair<std::string, std::string>(a,b)];
                else return 0;
                }
            else {
                if (pgraph.find(std::pair<std::string, std::string>(b,a)) != pgraph.end())
                    return pgraph[std::pair<std::string, std::string>(b,a)];
                else return 0;
            }
        }
    
        int  getWeight(std::string s) { return weight[s]; }
        void incGraph(std::string a, std::string b) {
            weight[a]++;
            weight[b]++;
            if (a < b)
                pgraph[std::pair<std::string, std::string>(a,b)]++;
            else
                pgraph[std::pair<std::string, std::string>(b,a)]++;

        }
        std::set<std::string> getNeighbours(std::string s);
        friend std::ostream& operator<<(std::ostream& os, const primalGraph& t);


};
#endif /* defined(__primalGraph__) */
