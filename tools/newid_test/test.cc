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

/* state space dim */
#define sDIM 2
#define iDIM 1

void getPoints(const char* filename, double* points, size_t newIds){
  Cudd mgr;
  scots::SymbolicSet set(mgr, filename, newIds);
  set.copyGridPoints(points);
}

int main() {
  Cudd mgr;

  double lb[sDIM]={0,0};
  double ub[sDIM]={15.5,15.5};
  double eta[sDIM]={1.0/2.0,1.0/2.0};
  scots::SymbolicSet ss(mgr,sDIM,lb,ub,eta);
  ss.addGridPoints();

  std::cout << "---------------" << std::endl;
  std::cout << "SS Info:" << std::endl;
  std::cout << "---------------" << std::endl;
  ss.printInfo(1);
  std::cout << std::endl;

  ss.writeToFile("X.bdd");
  std::cout << std::endl;


  std::cout << "---------------" << std::endl;
  std::cout << "Manually:" << std::endl;;
  std::cout << "---------------" << std::endl;
  size_t dim = ss.getDimension();
  std::cout << "dim: "  << dim << std::endl;
  size_t size = ss.getSize();
  std::cout << "size: "  << size << std::endl;
  double* points = new double[1024*2];
  ss.copyGridPoints(points);
  std::cout << "points: ";
  for(size_t i=0; i<1024; i++)
      std::cout << "(" << points[2*i] << "," << points[2*i + 1] << "), ";
  delete[] points;
  std::cout << std::endl;

  std::cout << "---------------" << std::endl;
  std::cout << "Load Differece Test:" << std::endl;;
  std::cout << "---------------" << std::endl;
  double* points0 = new double[1024*2];
  double* points1 = new double[1024*2];
  std::string targefile = "X.bdd";
  getPoints(targefile.c_str(), points0, 0);
  getPoints(targefile.c_str(), points1, 1);
  bool failed = false;
  for(size_t i=0; i<1024; i++){
    if(points0[i] != points1[i]){
      std::cout << "Test Failed !" << std::endl;
      failed = true;
      break;
    }
  }
  if(!failed)
    std::cout << "Test OK !" << std::endl;
  delete[] points0;
  delete[] points1;

  return 1;
}
