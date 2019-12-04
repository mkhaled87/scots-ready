/*
 * mexSymbolicSet.cc
 *
 *  created on: 30.09.2015
 *      author: rungger
 */



#include <iostream>
#include <math.h>
#include <cstring>

#include "mex.h"

#include "SymbolicSet.hh"



using namespace std;

void mexFunction(
     int          nlhs,
     mxArray      *plhs[],
     int          nrhs,
     const mxArray *prhs[]
     )
{
  /* Check for proper number of arguments */
  if (nrhs < 2) 
    mexErrMsgIdAndTxt("MATLAB:mexSymbolicSet:","mexSymbolicSet requires at least two input arguments.");
  /* input must be a string */
  if ( mxIsChar(prhs[0]) != 1)
    mexErrMsgIdAndTxt("MATLAB:mexSymbolicSet:", "First input must be the filename.");
  /* input must be a string */
  if ( mxIsChar(prhs[1]) != 1)
    mexErrMsgIdAndTxt("MATLAB:mexSymbolicSet:", "Second input must a string out of {parameters,gridPoints,setValuedMap}.");

  /* get pointer to the the filename .*/
  char* filename=mxArrayToString(prhs[0]);
  /* get pointer to the paramter */
  char* param = mxArrayToString(prhs[1]);

  int recognized=0;
  if (!std::strcmp(param,"parameters")) {
    recognized=1;

    Cudd mgr;
    scots::SymbolicSet set(mgr,filename);
    /* dim */
    size_t dim=set.getDimension();
    plhs[0]=mxCreateDoubleScalar((double)dim);
    /* eta */
    if(nlhs>=1) {
      plhs[1]=mxCreateDoubleMatrix((mwSize)dim,1,mxREAL);
      double *eta=mxGetPr(plhs[1]);
      set.copyEta(eta);
    }
    /* first */
    if(nlhs>=2) {
      plhs[2]=mxCreateDoubleMatrix((mwSize)dim,1,mxREAL);
      double *first=mxGetPr(plhs[2]);
      set.copyFirstGridPoint(first);
    }
    /* last */
    if(nlhs>=3) {
      plhs[3]=mxCreateDoubleMatrix((mwSize)dim,1,mxREAL);
      double *last=mxGetPr(plhs[3]);
      set.copyLastGridPoint(last);
    }
    /* z - measurement error bound */
    if(nlhs>=4) {
      plhs[4]=mxCreateDoubleMatrix((mwSize)dim,1,mxREAL);
      double *z=mxGetPr(plhs[4]);
      set.copyZ(z);
    }
  }

  if (!std::strcmp(param,"gridpoints")) {
    recognized=1;

    /* get grid points projected onto dimensions specified by the input arguments */
    if(nrhs>2 && mxIsNumeric(prhs[2])) {
      mwSize n    = mxGetN(prhs[2]);
      double* ptr = mxGetPr(prhs[2]);
      std::vector<size_t> projectdim;
      for(size_t i=0; i<n; i++) 
        projectdim.push_back((size_t)ptr[i]);
      /* laod set */
      Cudd mgr;
      scots::SymbolicSet set(mgr,filename);
      size_t dim=set.getDimension();
      /* check sanity of project dimensions */
      if(n> dim) 
        mexErrMsgIdAndTxt( "MATLAB:mexSymbolicSet:", "Number of project dimensions cannot exceed the number of dimensions.");
      for(size_t i=0; i<n; i++) {
        if(projectdim[i]>=dim)
          mexErrMsgIdAndTxt( "MATLAB:mexSymbolicSet:", "Cannot project SymbolicSet onto given dimensions.");
      }

      size_t size = set.getSize(projectdim);
      /* grid points */
      if(size) {
        plhs[0]=mxCreateDoubleMatrix((double)(n*size),1,mxREAL);
        double *points=mxGetPr(plhs[0]);
        set.copyGridPoints(points,projectdim);
        mwSize ndim[2]={size, n};
        mxSetDimensions(plhs[0],ndim, 2);
      } else
        plhs[0] = mxCreateDoubleMatrix( 0, 0, mxREAL );

    } else { /* copy all grid points */
      Cudd mgr;
      scots::SymbolicSet set(mgr,filename);
      size_t dim=set.getDimension();
      size_t size = set.getSize();
      /* grid points */
      if(size) {
        plhs[0]=mxCreateDoubleMatrix((double)(dim*size),1,mxREAL);
        double *points=mxGetPr(plhs[0]);
        set.copyGridPoints(points);
        mwSize ndim[2]={size, dim};
        mxSetDimensions(plhs[0],ndim, 2);
      } else
        plhs[0] = mxCreateDoubleMatrix( 0, 0, mxREAL );
    }
  }
  if (!std::strcmp(param,"checkstate")) {
    recognized=1;
    mwSize nx    = mxGetM(prhs[2]);
    double* ptr_x = mxGetPr(prhs[2]);

    plhs[0]=mxCreateDoubleMatrix(1,1,mxREAL);
    double *inside = mxGetPr(plhs[0]);

    std::vector<double> x;
    std::vector<size_t> idx;
    for(size_t i=0; i<nx; i++) {
      x.push_back(ptr_x[i]);
      idx.push_back(i);
    }
    Cudd mgr;
    scots::SymbolicSet set(mgr,filename);

    if(set.isElement(x)) 
      inside[0]=1;
    else
      inside[0]=0;
  }
  if (!std::strcmp(param,"setvaluedmap")) {
    recognized=1;
    if (nrhs < 3) 
      mexErrMsgIdAndTxt("MATLAB:mexSymbolicSet:setvaluedmap","mexSymbolicSet with setValuedMap option requires at least three input arguments.");
 
    
    mwSize nx    = mxGetM(prhs[2]);
    double* ptr_x = mxGetPr(prhs[2]);
    double* ptr_idx = mxGetPr(prhs[3]);

    std::vector<double> x;
    std::vector<size_t> idx;
    for(size_t i=0; i<nx; i++) {
      x.push_back(ptr_x[i]);
      idx.push_back(ptr_idx[i]-1);
    }
    Cudd mgr;
    scots::SymbolicSet set(mgr,filename);
    std::vector<std::vector<double>> inputs = set.setValuedMap(x,idx);

    size_t length = inputs.size();
    if(length) {
      size_t idim =  inputs[0].size();
      plhs[0]=mxCreateDoubleMatrix(length,idim,mxREAL);
      double *u = mxGetPr(plhs[0]);
      for(size_t i=0; i<idim; i++) {
        for(size_t j=0; j<length; j++)
          u[i*length+j] = inputs[j][i];
      }
    } else {
      plhs[0]= mxCreateDoubleMatrix( 0, 0, mxREAL );
    }
    
  }

  if(!recognized)
    mexErrMsgIdAndTxt("MATLAB:mexSymbolicSet","Could not read parameter string.");

  return;
}

