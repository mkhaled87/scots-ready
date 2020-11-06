%
% make.m
%
% created on: 08.01.2018
%     author: M.Khaled

clear all;
delete *.mex*
clc;

disp('Building the MATLAB interface for Linux/Mac ... ');


% paths of includes
cudd_path = fullfile('..','..','cudd-3.0.0');
ipath_bdd  = ['-I' fullfile('..','..','bdd')];
ipath_cudd = ['-I' cudd_path];

% build all (including the CUDD) and link inside one file
mex('-v', ipath_bdd, ipath_cudd, 'mexSymbolicSet.cc', fullfile(cudd_path, '*.cc'), fullfile(cudd_path, '*.c'))

clear all;


