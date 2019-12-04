/*
 * unicycle.cc
 *
 *  created on: 21.03.2018
 *      author: m. khaled
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
#define sDIM 3
#define iDIM 2

/* data types for the ode solver */
typedef std::array<double,sDIM> state_type;
typedef std::array<double,iDIM> input_type;

/* sampling time */
const double tau = 0.3;


/* number of intermediate steps in the ode solver */
const int nint=5;
OdeSolver ode_solver(sDIM,nint,tau);


/* we integrate the unicycle ode by 0.3 sec (the result is stored in x)  */
auto  khepera_post = [](state_type &x, input_type &u) -> void {

  /* the ode describing the unicycle */
  auto rhs =[](state_type& xx,  const state_type &x, input_type &u) -> void {
      xx[0] = u[0]*std::cos(x[2]);
      xx[1] = u[0]*std::sin(x[2]);
      xx[2] = u[1];
  };
  ode_solver(rhs,x,u);
};

/* computation of the growth bound (the result is stored in r)  */
auto radius_post = [](state_type &r, input_type &u) -> void {
    r[0] = r[0]+r[2]*std::abs(u[0])*0.3;
    r[1] = r[1]+r[2]*std::abs(u[0])*0.3;
};

scots::SymbolicSet kheperaCreateStateSpace(Cudd &mgr) {

  /* setup the workspace of the synthesis problem and the uniform grid */
  /* lower bounds of the hyper rectangle */
  double lb[sDIM]={0,0,-M_PI-0.4};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={15,13,M_PI+0.4}; 
  /* grid node distance diameter */
  double eta[sDIM]={.1,.1,.1};   


  scots::SymbolicSet ss(mgr,sDIM,lb,ub,eta);

  /* add the grid points to the SymbolicSet ss */
  ss.addGridPoints();

  double H[4*sDIM]={-1, 0, 0,
                    1, 0, 0,
                    0,-1, 0,
                    0, 1, 0};
  
  #define num_obstacles 34
  double h[num_obstacles][4] = {
	{-0.0, 0.5, - 8.2, 13},
	{-0.5, 1.5, -10.5, 13},
	{-1.5, 2.2, - 8.2, 13},

	{-1.5, 2.2,  -6.3, 7.0},

	{-0.0, 0.5,  -1.2, 5},
	{-0.5, 1.5,  -1.2, 3},
	{-1.5, 2.2,  -1.2, 5},			

/////////////////////////////////////////

	{-3.0, 3.3,  - 0.0, 1.5},

	{-3.0, 3.3,  - 3.7, 5.2},

	{-3.0, 3.3,  - 7.5, 8.9},

	{-3.0, 3.3,  -10.8, 12.2},

/////////////////////////////////////////

	{-4.2, 6.8,  -10.8, 11.3},
	{-6.2, 6.8,  -11.3, 13},

	{-4.2, 6.8,  - 9.6, 10.0},

	{-4.2, 6.8,  - 7.9,  8.5},
	{-4.2, 4.8,  - 4.9,  7.9},
	{-4.2, 6.8,  - 3.5,  6.0},

	{-5.1, 6.8,  - 2.4,  2.8},

	{-4.2, 6.8,  - 0.0,  1.5},

/////////////////////////////////////////

	{-7.7, 8.0,  - 11 ,   13},

	{-7.7, 8.0,  - 3.8,  8.1},

	{-7.7, 8.0,  - 0.0,  1.3},

/////////////////////////////////////////

	{-9.1,  11.2,  -10.8, 13},

	{-9.1,  11.2,  - 9.6, 10.0},

	{-9.1,  11.2,  - 6.8,  8.5},

	{-9.1,  11.2,  - 3.8,  4.4},

	{-9.1,  10.2,  - 2.4,  2.8},

	{-8.9,  15.0,  - 0.0,  1.5},

/////////////////////////////////////////

	{-12.0, 12.3,  - 11 ,   13},

	{-12.0, 12.3,  - 6.2,  8.1},

	{-12.0, 12.3,  - 3.8,  4.9},

/////////////////////////////////////////


	{-13.2,   15,  - 6.8,  11.5},

	{-13.2,   14,  - 5.6,   5.9},

	{-13.2,   15,  - 3.6,  4.7},

  };

  for(size_t i=0; i<num_obstacles; i++){
	ss.remPolytope(4,H,h[i], scots::OUTER);  
  }  

  return ss;
}

scots::SymbolicSet kheperaCreateInputSpace(Cudd &mgr) {

  /* lower bounds of the hyper rectangle */
  double lb[sDIM]={-1,-1.5};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={1 , 1.5}; 
  /* grid node distance diameter */
  double eta[sDIM]={.3,.2};   

  scots::SymbolicSet is(mgr,iDIM,lb,ub,eta);
  is.addGridPoints();

  return is;
}


scots::SymbolicSet kheperaCreateTargetSet(scots::SymbolicSet ss) {

  scots::SymbolicSet ts = ss;
  ts.clear();

  double H[4*sDIM]={-1, 0, 0,
                    1, 0, 0,
                    0,-1, 0,
                    0, 1, 0};
  

  double h[4] = {-3.0, 3.3,  - 2.2, 2.6};
  ts.addPolytope(4, H,h, scots::OUTER);

  return ts;
}

int main() {
  /* to measure time */
  TicToc tt;
  /* there is one unique manager to organize the bdd variables */
  Cudd mgr;

  /****************************************************************************/
  /* construct SymbolicSet for the state space */
  /****************************************************************************/
  scots::SymbolicSet ss=kheperaCreateStateSpace(mgr);
  ss.writeToFile("khepera_ss.bdd");
    
  ss.complement();
  ss.writeToFile("khepera_obst.bdd");
  ss.complement();
  std::cout << "Unfiorm grid details:" << std::endl;
  ss.printInfo(1);


  /****************************************************************************/
  /* the target set */
  /****************************************************************************/
  /* first make a copy of the state space so that we obtain the grid
   * information in the new symbolic set */
  scots::SymbolicSet ts = kheperaCreateTargetSet(ss);
  ts.writeToFile("khepera_target.bdd");


  /****************************************************************************/
  /* construct SymbolicSet for the input space */
  /****************************************************************************/
  scots::SymbolicSet is=kheperaCreateInputSpace(mgr);
  std::cout << std::endl << "Input space details:" << std::endl;
  is.printInfo(1);

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
  abstraction.computeTransitionRelation(khepera_post, khepera_post);
  std::cout << std::endl;
  tt.toc();
  /* get the number of elements in the transition relation */
  std::cout << std::endl << "Number of elements in the transition relation: " << abstraction.getSize() << std::endl;


  /****************************************************************************/
  /* we continue with the controller synthesis */
  /****************************************************************************/
  int verbose=1;
  /* we setup a fixed point object to compute reachabilty controller */
  scots::FixedPoint fp(&abstraction);
  /* the fixed point algorithm operates on the BDD directly */
  BDD T = ts.getSymbolicSet();
  tt.tic();
  BDD C = fp.reach(T,verbose);
  tt.toc();

  /****************************************************************************/
  /* last we store the controller as a SymbolicSet 
   * the underlying uniform grid is given by the Cartesian product of 
   * the uniform gird of the space and uniform gird of the input space */
  /****************************************************************************/
  scots::SymbolicSet controller(ss,is);
  controller.setSymbolicSet(C);
  controller.writeToFile("khepera_controller.bdd");

  scots::SymbolicSet tr = abstraction.getTransitionRelation();
  tr.writeToFile("khepera_transition_relation.bdd");


  return 1;
}

