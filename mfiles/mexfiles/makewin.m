%
% makewin.m
%
% created on: 08.01.2018
%     author: M.Khaled
%
% This file builds the mex executables for the interface
% the supplied proted CUDD library is buyilt for Release/x64
% If your Matlab is 32bits, this will not work. Contact me to
% supply the 32bit version.
%
% You have to have he file CUDD.lib availoble in the .\cuddwin64\lib
% You will find it in a compressed file. Uncompress it.
%
clear all;
delete mexSymbolicSet.mex*

% the ported CUDD
cuddProjPath = fullfile('.','cuddwin64');
cuddIncPath =  fullfile(cuddProjPath, 'include');
cuddLibPath =  fullfile(cuddProjPath, 'lib', 'CUDD.lib');


% path of includes
ipath_bdd  = ['-I' fullfile('..','..','bdd')];
ipath_cudd = ['-I' cuddIncPath];

% linking with CUDD. You need to have the library in the path
linkcudd = ['-l' cuddLibPath];

mex('-v',ipath_bdd, ipath_cudd, linkcudd , 'mexSymbolicSet.cc')