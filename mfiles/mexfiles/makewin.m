%
% makewin.m
%

clear all;
delete *.mex*
clc;

disp('Building the MATLAB interface for Windows ... ');

% the ported CUDD
cuddProjPath = fullfile('..','..','cudd-3.0.0');
cuddIncPath =  fullfile(cuddProjPath);
cuddLibPath =  fullfile(cuddProjPath, 'lib', 'cudd.lib');


% path of includes
ipath_bdd  = ['-I' fullfile('..','..','bdd')];
ipath_cudd = ['-I' cuddIncPath];

% linking with CUDD. You need to have the library in the path
linkcudd = ['-l' cuddLibPath];

mex('-v',ipath_bdd, ipath_cudd, linkcudd , 'mexSymbolicSet.cc')