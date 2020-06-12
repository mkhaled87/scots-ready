/*
 * onedim.cc
 *
 *  created on: 30.09.2015
 *      author: rungger
 */

/*
 * information about this example is given in the readme file
 *
 */

#include <array>
#include <iostream>

#include "cuddObj.hh"

#include "SymbolicSet.hh"
#include "SymbolicModelGrowthBound.hh"

#include "TicToc.hh"
#include "RungeKutta4.hh"
#include "FixedPoint.hh"


/* state space dim */
#define sDIM 1
#define iDIM 1

/* data types for the ode solver */
typedef std::array<double,1> state_type;
typedef std::array<double,1> input_type;

/* we integrate the vehicle ode by 0.3 sec (the result is stored in x)  */
auto  onedim_post = [](state_type &x, input_type &u) -> void {
  x[0] = 0.5*x[0] + u[0];
};

/* computation of the growth bound (the result is stored in r)  */
auto onedim_radius = [](state_type &r, input_type &u) {
  r[0] = 0.0*r[0];
};

scots::SymbolicSet onedimCreateStateSpace(Cudd &mgr) {

  /* setup the workspace of the synthesis problem and the uniform grid */
  /* lower bounds of the hyper rectangle */
  double lb[sDIM]={-4.1};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={ 4.1}; 
  
  /* grid node distance diameter */
  double eta[sDIM]={0.1};  

  /* grid node distance diameter */
  double err[sDIM]={2.9};     


  scots::SymbolicSet ss(mgr,sDIM,lb,ub,eta,err);

  /* add the grid points to the SymbolicSet ss */
  ss.addGridPoints();

 return ss;
}

scots::SymbolicSet onedimCreateInputSpace(Cudd &mgr) {

  /* lower bounds of the hyper rectangle */
  double lb[sDIM]={-1};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={1}; 
  /* grid node distance diameter */
  double eta[sDIM]={0.1};   

  scots::SymbolicSet is(mgr,iDIM,lb,ub,eta);
  is.addGridPoints();

  return is;
}

void onedimCreateSafeSpace(scots::SymbolicSet &safe_set) {
  safe_set.addGridPoints();
}


int main() {
  /* to measure time */
  TicToc tt;
  /* there is one unique manager to organize the bdd variables */
  Cudd mgr;

  /****************************************************************************/
  /* construct SymbolicSet for the state space */
  /****************************************************************************/
  scots::SymbolicSet ss=onedimCreateStateSpace(mgr);
  ss.writeToFile("onedim_ss.bdd");


  /****************************************************************************/
  /* construct SymbolicSet for the input space */
  /****************************************************************************/
  scots::SymbolicSet is=onedimCreateInputSpace(mgr);
  is.writeToFile("onedim_is.bdd");

  /****************************************************************************/
  /* construct SymbolicSet for the input space */
  /****************************************************************************/
  scots::SymbolicSet safe_set(ss);
  onedimCreateSafeSpace(safe_set);
  safe_set.writeToFile("onedim_safe.bdd");

  /****************************************************************************/
  /* setup class for symbolic model computation */
  /****************************************************************************/
  /* first create SymbolicSet of post variables 
   * by copying the SymbolicSet of the state space and assigning new BDD IDs */
  scots::SymbolicSet sspost(ss,1);
  /* instantiate the SymbolicModel */
  scots::SymbolicModelGrowthBound<state_type,input_type> abstraction(&ss, &is, &sspost);
  /* compute the transition relation */
  tt.tic();
  abstraction.computeTransitionRelation(onedim_post, onedim_radius);
  std::cout << std::endl;
  tt.toc();
  /* get the number of elements in the transition relation */
  std::cout << std::endl << "Number of elements in the transition relation: " << abstraction.getSize() << std::endl;

  /****************************************************************************/
  /* we continue with the controller synthesis */
  /****************************************************************************/
  scots::FixedPoint fp(&abstraction);
  BDD S = safe_set.getSymbolicSet();
  tt.tic();
  /* compute controller */
  BDD C=fp.safe(S,1);
  tt.toc();

  /****************************************************************************/
  /* last we store the controller as a SymbolicSet 
   * the underlying uniform grid is given by the Cartesian product of 
   * the uniform gird of the space and uniform gird of the input space */
  /****************************************************************************/
  scots::SymbolicSet controller(ss,is);
  controller.setSymbolicSet(C);
  controller.writeToFile("onedim_controller.bdd");
  std::cout << std::endl << "Number of elements in the controller: " << controller.getSize() << std::endl;

  return 1;
}
