/*
 * dcdc.cc
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
#define sDIM 2
#define iDIM 1

/* data types for the ode solver */
typedef std::array<double,2> state_type;
typedef std::array<double,1> input_type;

/* some model constants */
const double r0=1.0 ; 
const double vs = 1.0 ;
const double rl = 0.05 ;
const double rc = rl / 10 ;
const double xl = 3.0 ;
const double xc = 70.0 ;
const double b[2]={vs/xl, 0};

/* sampling time */
const double tau = 0.5;
/* number of intermediate steps in the ode solver */
const int nint=5;
OdeSolver ode_solver(sDIM,nint,tau);

/* we integrate the dcdc ode by 0.5 sec (the result is stored in x)  */
auto  dcdc_post = [](state_type &x, input_type &u) -> void {
  /* the ode describing the system */
  auto  system_ode = [](state_type &dxdt, const state_type &x, const input_type &u) -> void {
    double a[2][2];
    if(u[0]==1) {
      a[0][0] = -rl / xl;
      a[0][1] = 0;
      a[1][0] = 0;
      a[1][1] = (-1 / xc) * (1 / (r0 + rc));
    } else {
      a[0][0] = (-1 / xl) * (rl + ((r0 * rc) / (r0 + rc))) ;
      a[0][1] =  ((-1 / xl) * (r0 / (r0 + rc))) / 5 ;
      a[1][0] = 5 * (r0 / (r0 + rc)) * (1 / xc);
      a[1][1] =(-1 / xc) * (1 / (r0 + rc)) ;
    }
    dxdt[0] = a[0][0]*x[0]+a[0][1]*x[1] + b[0];
    dxdt[1] = a[1][0]*x[0]+a[1][1]*x[1] + b[1];
  };
  ode_solver(system_ode,x,u);
};

/* computation of the growth bound (the result is stored in r)  */
auto radius_post = [](state_type &r, input_type &u) -> void {

  /* the ode to determine the radius of the cell which over-approximates the
   * attainable set see: http://arxiv.org/abs/1503.03715v1 */
  auto growth_bound_ode = [](state_type &drdt,  const state_type &r, const input_type &u) {
    /* for the dcdc boost converter the growth bound is simply given by the metzler matrix of the system matrices */ 
    double a[2][2];
    if(u[0]==1) {
      a[0][0] = -rl / xl;
      a[0][1] = 0;
      a[1][0] = 0;
      a[1][1] = (-1 / xc) * (1 / (r0 + rc));
    } else {
      a[0][0] = (-1 / xl) * (rl + ((r0 * rc) / (r0 + rc))) ;
      a[0][1] =  ((1 / xl) * (r0 / (r0 + rc))) / 5 ;
      a[1][0] = 5 * (r0 / (r0 + rc)) * (1 / xc);
      a[1][1] =(-1 / xc) * (1 / (r0 + rc)) ;
    }

    drdt[0] = a[0][0]*r[0]+a[0][1]*r[1];
    drdt[1] = a[1][0]*r[0]+a[1][1]*r[1];
  };
  ode_solver(growth_bound_ode,r,u);
};


/****************************************************************************/
/* main computation */
/****************************************************************************/
int main() {
  /* to measure time */
  TicToc tt;
  /* there is one unique manager to organize the bdd variables */
  Cudd mgr;

  /****************************************************************************/
  /* construct SymbolicSet for the state space */
  /****************************************************************************/
  /* setup the workspace of the synthesis problem and the uniform grid */
  /* lower bounds of the hyper rectangle */
  double lb[sDIM]={0.65,4.95};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={1.65,5.95};  
  /* grid node distance diameter */
  double eta[sDIM]={10*2/4e3,10*2/4e3};   
  scots::SymbolicSet ss(mgr,sDIM,lb,ub,eta);
  ss.addGridPoints();
  /****************************************************************************/
  /* construct SymbolicSet for the input space */
  /****************************************************************************/
  double ilb[iDIM]={1};  
  double iub[iDIM]={2}; 
  double ieta[iDIM]={1};   
  scots::SymbolicSet is(mgr,iDIM,ilb,iub,ieta);
  is.addGridPoints();

  /****************************************************************************/
  /* setup class for symbolic model computation */
  /****************************************************************************/
  scots::SymbolicSet sspost(ss,1); /* create state space for post variables */ 
  scots::SymbolicModelGrowthBound<state_type,input_type> abstraction(&ss, &is, &sspost);
  /* compute the transition relation */
  tt.tic();
  abstraction.computeTransitionRelation(dcdc_post, radius_post);
  std::cout << std::endl;
  tt.toc();
  /* get the number of elements in the transition relation */
  std::cout << std::endl << "Number of elements in the transition relation: " << abstraction.getSize() << std::endl;

  /****************************************************************************/
  /* we continue with the controller synthesis for FG (target) */
  /****************************************************************************/
  /* construct SymbolicSet for target (it is a subset of the state space)  */
  scots::SymbolicSet target(ss);
  /* add inner approximation of P={ x | H x<= h } form state space */
  double H[4*sDIM]={-1, 0,
                     1, 0,
                     0,-1,
                     0, 1};
  double h[4] = {-1.1,1.6,-5.4, 5.9};
  target.addPolytope(4,H,h, scots::INNER);
  std::cout << "Target set details:" << std::endl;
  target.writeToFile("dcdc_target.bdd");

  /* we setup a fixed point object to compute reach and stay controller */
  scots::FixedPoint fp(&abstraction);
  /* the fixed point algorithm operates on the BDD directly */
  BDD T = target.getSymbolicSet();

  /* we implement the nested fixed point algorithm
   *
   * mu X. nu Y. ( pre(Y) & T ) | pre(X)
   *
   */
  tt.tic();
  size_t i,j;
  /* outer fp*/
  BDD X=mgr.bddOne();
  BDD XX=mgr.bddZero();
  /* inner fp*/
  BDD Y=mgr.bddZero();
  BDD YY=mgr.bddOne();
  /* the controller */
  BDD C=mgr.bddZero();
  BDD U=is.getCube();
  /* as long as not converged */
  for(i=1; XX != X; i++) {
    X=XX;
    BDD preX=fp.pre(X);
    /* init inner fp */
    YY = mgr.bddOne();
    for(j=1; YY != Y; j++) {
      Y=YY;
      YY= ( fp.pre(Y) & T ) | preX;
    }
    XX=YY;
    std::cout << "Iterations inner: " << j << std::endl;
    /* remove all (state/input) pairs that have been added
     * to the controller already in the previous iteration * */
    BDD N = XX & (!(C.ExistAbstract(U)));
    /* add the remaining pairs to the controller */
    C=C | N;
    //std::cout << C.CountMinterm(17) << std::endl;
  }
  std::cout << "Iterations outer: " << i << std::endl;
  tt.toc();

  /****************************************************************************/
  /* last we store the controller as a SymbolicSet 
   * the underlying uniform grid is given by the Cartesian product of 
   * the uniform gird of the space and uniform gird of the input space */
  /****************************************************************************/
  scots::SymbolicSet controller(ss,is);
  controller.setSymbolicSet(C);
  std::cout << "Domain size: " << controller.getSize() << std::endl;
  controller.writeToFile("dcdc_controller.bdd");

  return 1;
}

