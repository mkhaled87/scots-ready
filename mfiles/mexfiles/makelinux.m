%
% make.m
%
% created on: 08.01.2018
%     author: M.Khaled

clear all;
delete *.mex*
clc;

disp('Building the MATLAB interface for Linux/Mac ... ');


% the CUDD
cuddProjPath = fullfile('..','..','cudd-3.0.0');
cuddIncPath =  fullfile(cuddProjPath);
cuddLibPath =  fullfile(cuddProjPath, 'lib');
linkCuddPath = ['-L' cuddLibPath];
linkCudd = '-lcudd';

% path of includes
ipath_bdd  = ['-I' fullfile('..','..','bdd')];
ipath_cudd = ['-I' cuddIncPath];

	
mex('-v',ipath_bdd, ipath_cudd, linkCuddPath,  linkCudd, 'mexSymbolicSet.cc')	


clear all;


