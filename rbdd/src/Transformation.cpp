
#include "Transformation.hpp"

Transformation::Transformation(RbddElem elem) {
  adapter = elem.getAdapter();
}

void Transformation::readVars(string varFile) {
  // We read the variables in order
  ifstream vf(varFile.c_str());
  string temp;
  while (vf >> temp)
    variables.push_back(temp);
}

void Transformation::readExp(string expFile) {
  // We parse the expression file
  kconf::synExpDriver driver;
  driver.parse_file(expFile);
  expList = driver.getConstraints();
  // We initialize the order and the inverse mapping
  for(int x = 0; static_cast<unsigned int>(x) < variables.size(); x++) {
    var2pos.push_back(x);
    pos2var.push_back(x);
    varMap[variables.at(x)] = x;
  }  
}

void Transformation::treatVarsAndConstraints() {
  int as = variables.size();
  int counter = 0;
  for(const std::string& s : variables)
      varMap[s] = counter++;
  
  // We create numeric indices for the constraints
  for(synExp* s: expList)
      if (s != synTrue)
          s->computeIndices(varMap);
  
  // We start with the identity order
  pos2var.resize(as);
  var2pos.resize(as);
  for(int x = 0; x < as; x++) {
      pos2var[x] = x;
      var2pos[x] = x;
  }
  
  
  if (scoreReorder) {
      if (scoreMethod == "perm") {
          PermScorer scorer(varMap, expList, menuBlocks, pos2var, var2pos, window, scoreThreshold);
          scorer.permGo();
      } else
      if (scoreMethod == "sifting") {
          SiftScorer scorer(varMap, expList, menuBlocks, pos2var, var2pos);
          scorer.siftGo(scoreThreshold);
      }
      else
          if (scoreMethod == "force") {
              blocks = false;
              forceScorer scorer(varMap, expList, pos2var, var2pos);
              scorer.force();
          }
          else if (scoreMethod == "forceblocks") {
              forceBlockScorer scorer(varMap, expList, menuBlocks, pos2var, var2pos);
              scorer.forceBlocks();
          }
          else
              if (scoreMethod == "random") {
                  RandomScorer scorer(varMap, expList, menuBlocks, pos2var, var2pos);
                  scorer.randomHeuristic(scoreThreshold);
              }
              else
                  if (scoreMethod == "other") {
                      SiftScorer scorer(varMap, expList, menuBlocks, pos2var, var2pos);
                      scorer.siftGo(scoreThreshold);
                      
                      OtherScorer scorer2(varMap, expList, menuBlocks, pos2var, var2pos);
                      scorer2.otherHeuristic(scoreThreshold);
                  }
                  else
                      if (scoreMethod == "file") {
                          std::ifstream vf(varOrderFile);
                          int c = 0;
                          pos2var.clear();
                          var2pos.clear();
                          var2pos.resize(as);
                          std::string var;
                          try {
                              while (vf >> var) {
                                  pos2var.push_back(varMap.at(var));
                                  c++;
                              }
                          }
                          catch(std::exception e) {
                              Rcpp::stop("Error looking up variable %s from file %s in variable list.", var, varOrderFile);
                          }
                          vf.close();
                          if (static_cast<unsigned int>(c) != variables.size()) {
                              Rcpp::stop("Number of variables read does not match");
                          }
                          Rcpp::Rcout << "Successfully read " << c << " variables from file " << varOrderFile << ". varMap size is " << varMap.size() << std::endl;
                      }
                      else {
                          Rcpp::stop("Scoring method %s unknown.", scoreMethod);
                      }
      
  }
  
  for(int x = 0; static_cast<unsigned int>(x) < var2pos.size(); x++) {
      if (var2pos[pos2var[x]] != x) {
          Rcpp::stop("Type 1 error x %d pos2var[%d] %d var2pos[%d] %d", x, x, pos2var[x], pos2var[x], var2pos[pos2var[x]]);
      }
      if (pos2var[var2pos[x]] != x) {
          Rcpp::stop("Type 2 error x %d", x);
      }
  }
  int min = std::numeric_limits<int>::max();
  int max = -1;
  for(int p : var2pos) {
      if (p < min) min = p;
      if (p > max) max = p;
  }
  if (min != 0 || max != as-1) {
      Rcpp::stop("1 Error in treatVarSAndConstraints. min is %d and max is %d", min, max);
  }
  if (static_components) {
      pcomp = new MultiComponents(var2pos, pos2var);
      for(synExp* s : expList) {
          pcomp->join(s, false);
      }
      
      pcomp->reorder();
      pcomp->checkLengths();
      int biggest = -1;
      for(std::pair<int, int> p : pcomp->listComponents())
          if (p.second > biggest)
              biggest = p.second;
      
      Rcpp::Rcout << pcomp->getNumComponents() << " components. Biggest "
              << biggest << " number of variables " << pos2var.size() << std::endl;

      pos2var = pcomp->getOrder();
      
  }
  else
      pcomp = new OneComponent(var2pos, pos2var);
  

  
  min = std::numeric_limits<int>::max();
  max = -1;
  for(int p : var2pos) {
      if (p < min) min = p;
      if (p > max) max = p;
  }
  if (min != 0 || max != as-1) {
      Rcpp::stop("2 Error in treatVarSAndConstraints. min is %d and max is %d", min, max);
  }
  
  int ncomp = 0;
  if (outputComponents) {
      Rcpp::Rcout << "Number of components " << pcomp->getNumComponents() << std::endl;
      for(std::pair<int, int> p : pcomp->listComponents()) {
          Rcpp::Rcout << "Component " << ncomp << " starts in " << std::setw(6) << p.first
          << " length " << std::setw(6) << p.second << " :" << std::endl;
          std::ostringstream ostvar, ostexp;
          ostvar << base+"-component-" << ncomp << ".var";
          std::ofstream ofvar(ostvar.str().c_str());
          std::set<int> varSet;
          for(int x = p.first; x < p.first+p.second; x++) {
              ofvar << variables[pos2var[x]] << std::endl;
              varSet.insert(pos2var[x]);
          }
          ofvar.close();
          ostexp << base+"-component-" << ncomp << ".exp";
          std::ofstream ofexp(ostexp.str().c_str());
          for(synExp* s: expList) {
              bool found = false;
              for(int index : s->giveSymbolIndices())
                  if (varSet.find(index) != varSet.end()) {
                      found = true;
                      break;
                  }
              if (found)
                  ofexp << s << std::endl;
          }
          ofexp.close();
          ncomp++;
      }
  }
  
  // variables is the inverse of varMap, it does no longer indicate ordering.
  
  // This vector DOES indicate variable ordering. We write it to a file.
  std::vector<std::string> var4File;
  var4File.resize(varMap.size());
  for(int var : pos2var)
      var4File.push_back(variables[var]);
  
  for(synExp* s: expList) {
      if (s != synTrue) {
          s->computeIndices(varMap);
      }
  }

  if (doCNF) {
      std::ofstream dimacs(base+".dimacs");
      dimacs << "p cnf " <<  varMap.size() << " " << expList.size() << std::endl;
      for(synExp* s : expList) {
          for(int i : s->toDimacs(varMap))
              // First variable has to be 1, not 0. And so on...
              dimacs << i << " ";
          dimacs << "0" << std::endl;
      }
  }
  delete pcomp;
}

BDDAdapter* Transformation::buildBDD(std::string bdd_name) {
  string varfilename;
  string statsfile;
  if (suffix == "") {
    varfilename = base + ".reorder";
    statsfile   = base + ".data";
  } else {
    varfilename = base + "-" + suffix + ".reorder";
    statsfile   = base + "_" + suffix + ".data";
  }

  if (dynamic_components)
    adapter->useComponents(var2pos, pos2var);
  else
    adapter->changeOrder(pos2var);
    
  for(std::string var : variables)
    adapter->newVar(var, "boolean");
        
  // We need to add the blocks for XOR expressions
  for(auto exp : expList) {
    adapter->setXORBlocks(exp);
  }

  // We set the order in var2pos
  adapter->syncOrder(bdd_name);
  synExp::numVars(variables.size());
        
  bddBuilder myBuilder(expList);
        
  myBuilder.setAdapter(adapter);
  myBuilder.setVarFileName(varfilename);
  myBuilder.setStatsFileName(statsfile);
  constraintSorter* cs = newCnstOrder(constReorder);
  myBuilder.setConstraintSorter(cs);
        
  if (reorder)
    myBuilder.setReorderMethod(reorderMethod);
  else
    myBuilder.setReorderMethod("noreorder");
        
  myBuilder.setReorderInterval(reorderInterval);
  myBuilder.setMinNodes(minNodes);
  myBuilder.setLineLength(lineLength);
  myBuilder.setMaxNodes(maxNodes);
  myBuilder.setMinConstraint(minConstraint);
  myBuilder.setReorderEvery(reorderEvery);
  myBuilder.setBlocks(blocks);
              
  myBuilder.build(inflation);

  delete cs;
  remove(varfilename.c_str());
  remove(statsfile.c_str());

  return adapter;
}

constraintSorter* Transformation::newCnstOrder(const std::string & s) {
    constraintSorter* sorter = NULL;
    if (s == "minimax")
        sorter = new minmaxSorter(adapter, expList);
    else
        if (s == "minspan")
            sorter = new minspanSorter(adapter, expList);
        else if (s == "remember")
            sorter = new RememberSorter(adapter, expList);
        else if (s == "none")
            sorter = new nullSorter(adapter, expList);
        else if (s != "none") {
            Rcpp::stop("Unknown constraint sorter %s", s);
        }
    return sorter;
}
