/*
 * dcdc.cc
 *
 *  created on: 18.04.2016
 *      author: rungger
 */

/*
 * demonstration of the boost ode solver usage
 *
 */

#include <array>
#include <iostream>

#include "cuddObj.hh"

#include "SymbolicSet.hh"
#include "SymbolicModelGrowthBound.hh"

#include "TicToc.hh"
#include <boost/numeric/odeint.hpp>


/* state space dim */
#define sDIM 2
#define iDIM 1

/* data types for the ode solver */
typedef std::array<double,2> state_type;
typedef std::array<double,1> input_type;

typedef boost::numeric::odeint::runge_kutta_dopri5< state_type > stepper_type;


/* we integrate the dcdc ode by 0.5 sec (the result is stored in x)  */
auto  dcdc_post = [](state_type &x, input_type &u) -> void {

  /* the ode describing the system */
  auto  system_ode = [&](const state_type &x, state_type &dxdt, const double) -> void {

    const double r0=1.0 ; 
    const double vs = 1.0 ;
    const double rl = 0.05 ;
    const double rc = rl / 10 ;
    const double xl = 3.0 ;
    const double xc = 70.0 ;

    const double b[2]={vs/xl, 0};

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
  boost::numeric::odeint::integrate_adaptive(make_controlled(1E-12 ,1E-12,stepper_type()),system_ode,x,0.0,0.5,0.5);
};

/* computation of the growth bound (the result is stored in r)  */
auto radius_post = [](state_type &r, input_type &u) -> void {

  /* the ode to determine the radius of the cell which over-approximates the
   * attainable set see: http://arxiv.org/abs/1503.03715v1 */
  auto growth_bound_ode = [&](const state_type &r, state_type &drdt, const double t) {
    /* for the dcdc boost converter the growth bound is simply given by the metzler matrix of the system matrices */ 
    const double r0=1.0 ; 
    const double rl = 0.05 ;
    const double rc = rl / 10 ;
    const double xl = 3.0 ;
    const double xc = 70.0 ;

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

  boost::numeric::odeint::integrate_adaptive(make_controlled(1E-12,1E-12,stepper_type()),growth_bound_ode,r,0.0,0.5,0.5);
};


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
  double lb[sDIM]={1.15,5.45};  
  /* upper bounds of the hyper rectangle */
  double ub[sDIM]={1.55,5.85}; 
  /* grid node distance diameter */
  double eta[sDIM]={2/4e3,2/4e3};   
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
  /* create SymbolicSet for the post domain postX in preX x U x postX
   * by coping preX and assigning new BDD IDs */
  scots::SymbolicSet sspost(ss,1);
  scots::SymbolicModelGrowthBound<state_type,input_type> abs(&ss, &is, &sspost);
  /* compute the transition relation */
  tt.tic();
  abs.computeTransitionRelation(dcdc_post, radius_post);
  std::cout << std::endl;
  tt.toc();
  /* get the number of elements in the transition relation */
  std::cout << std::endl << "Number of elements in the transition relation: " << abs.getSize() << std::endl;

  /* get SymbolicSet containing the transition relation with domain X x U x X */
  scots::SymbolicSet tr=abs.getTransitionRelation();
  /* write SymbolicSet to file */
  tr.writeToFile("dcdc_abs.bdd");

  /* read SymbolicSet containing the transition relation from file */
  scots::SymbolicSet trset(mgr,"dcdc_abs.bdd");



  return 1;
}
