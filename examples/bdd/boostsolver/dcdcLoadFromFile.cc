/*
 * dcdcLoadFromFile.cc
 *
 *  created on: 18.04.2016
 *      author: rungger
 */

/*
 * demonstration how to load a transition relation of a symbolic model from file  
 *
 */

#include <array>
#include <iostream>

#include "cuddObj.hh"

#include "SymbolicSet.hh"
#include "SymbolicModel.hh"

int main() {
  /* there is one unique manager to organize the bdd variables */
  Cudd mgr;
  /* read SymbolicSet containing the transition relation from file */
  scots::SymbolicSet trset(mgr,"dcdc_abs.bdd");
  trset.printInfo(1);
  /* the first two dimensions correspond to the state alphabet */
  std::vector<size_t> ssdim={1,2};
  scots::SymbolicSet ss(trset,ssdim);
  /* the third dimensions corresponds to the input alphabet */
  std::vector<size_t> isdim={3};
  scots::SymbolicSet is(trset,isdim);
  /* the last two dimensions corresponds to the state alphabet 
   * (containing the post variables) */
  std::vector<size_t> sspostdim={4,5};
  scots::SymbolicSet sspost(trset,sspostdim);
  /* create SymbolicModel */
  scots::SymbolicModel sys(&ss,&is,&sspost);
  sys.setTransitionRelation(trset.getSymbolicSet());
  return 1;
}
