%
% Install.m
%
% created on: 03.07.2018
%     author: M.Khaled
%
% last update: 05.11.2018
%
% This file installs the interface by building the C++ files.
% It calls 'mexfiles/make.m'.
% Then, it adds some folders to matlab paths.
%
% Notes:
% --------
% - You must run this file before using the Matlab interface M-files.
% - You must buld pFaces first.
% - This M-file will not probably work when you build pFaces with 'Debug'
%   configuration. Please build pFaces with 'Release' configuration first.
%   You can then return to 'Debug' configuration of pFaces and you dont
%   need to re run this again unless you change in the libraries/interface
%   source codes.
% - For windows, this file was tested with mex/Visual-Studio-C++-2015 and
%   mex/Visual-Studio-C++-2017.
% - For windows, this file was tested with mex/GCC 4.8 and mex/GCC 5.4.

% check if we are running from the same folder
file_path = strrep(strrep(mfilename('fullpath'),'/Install',''),'\Install','');
curr_path = pwd;
if(~strcmp(file_path, curr_path))
    error('Please run the script from its directory.')
end

% check the compiler is configured
cc = mex.getCompilerConfigurations;
cc_name = cc.Name;
if(isempty(cc_name))
    error('Please configure the Mex-Compiler first.')
end

cd mexfiles
try
    if ispc
        makewin
    else
        makelinux
    end
    disp('MATLAB interface is built succesefully!');
catch ex
    error('Failed to build the interface.')
    cd ..
    return
end

addpath('./');
savepath
cd ..
addpath('./');
savepath

