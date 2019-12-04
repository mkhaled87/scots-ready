classdef SymbolicSet < handle 
% some matlab commands to load points from a file stored by
% SCS:SymbolicSet::writeToFile() 
% 
% Please have a look at ./manual/manual.pdf for usage details
%
  properties (SetAccess=private)
    filename  % name of file which contains the SymbolicSet
    dim       % dimension of the real space
    z         % measurement error bound
    points    % the gird points contained in the symbolic set
    eta       % grid point distance parameter if SymbolicSet is SymbolicSetFromUniformGrid 
    first     % first grid point of the uniform grid if SymbolicSet is SymbolicSetFromUniformGrid 
    last      % last grid point of the uniform grid if SymbolicSet is SymbolicSetFromUniformGrid 
    project   % optional: project the SymbolicSet onto the dimensions specified in the array project
  end
  methods 
    function obj=SymbolicSet(filename, varargin)
    % the constructor opens the file and reads all the information about
    % the SymbolicSet

      if(isstr(filename))
        obj.filename=filename;
      else
        error('filname is not a string');
      end

      [dim eta first last z]=mexSymbolicSet(filename,'parameters');
      obj.filename=filename;
      obj.dim=dim;
      obj.eta=eta;
      obj.z=z;
      obj.last=last;
      obj.first=first;
      obj.points=[];

      if(nargin>1 && strcmp(varargin{1},'projection'))
        obj.project=varargin{2};
        obj.dim=length(varargin{2});
      elseif(nargin>1)
        error('could not read input arguments');
      else
        obj.project=0;
      end
    end
    function disp(obj)
      disp(['Matlab object to access the points cointained in the Symbolic Set stored in ', obj.filename])
      disp(' ')
    end
    function points=get.points(obj)
    % read the grid points from file
    % optional projection indices
    % get.point('project',[1 2 3]) loads the grid points in the symbolic set
    % projected onto the dimensions [1 2 3]

      if(obj.project)
        project=obj.project-1;
        points=mexSymbolicSet(obj.filename,'gridpoints',project);
        obj.points=points;
        if(isempty(points))
          error(['there are not points in the Symbolic Set ',obj.filename]);
        end
      else
        points=mexSymbolicSet(obj.filename,'gridpoints');
        obj.points=points;
        if(isempty(points))
          error(['there are not points in the Symbolic Set ',obj.filename]);
        end
      end
    end
    function eta=get.eta(obj)
      if obj.eta~=0
        eta=obj.eta;
      else
        error(['eta is not associated with the Symbolic Set stored in ',obj.filename]);
      end
    end
    function first=get.first(obj)
      if ~isempty(obj.first)
        first=obj.first;
      else
        error(['No firstGridPoint associated with the symbolic set stored in ',obj.filename]);
      end
    end
    function last=get.last(obj)
      if ~isempty(obj.last)
        last=obj.last;
      else
        error(['No firstGridPoint associated with the symbolic set stored in ',obj.filename]);
      end
    end
    function u=getInputs(obj,x)
      x=x(:);
      idx=1:length(x);
      u=mexSymbolicSet(obj.filename,'setvaluedmap',x,idx);
      if(isempty(u))
        error(['The state ', mat2str(x(:)') ,' is not in the domain of the controller stored in:',obj.filename]);
      end
    end
    function out = isElement(obj,x)
      x=x(:);
      if (mexSymbolicSet(obj.filename,'checkstate',x))
        out=1;
      else
        out=0;
      end
    end
    function u=setValuedMap(obj,x,varargin)
      x=x(:);
      if(isempty(varargin))
        idx=1:length(x);
      else
        idx=varargin{1};
        if(length(idx)~=length(x))
          error(['The number of indices in the index list must be equal the number of elements in x']);
        end
        if(max(idx(:))>obj.dim)
          error(['One of the specified indices exceeds the domain dimension']);
        end
      end

      u=mexSymbolicSet(obj.filename,'setvaluedmap',x,idx);
      if(isempty(u))
        error(['The state is not in the domain of the SymbolicSet stored in:',obj.filename]);
      end
    end
    function plot(obj,varargin)
      if(obj.points==0)
        error(['No grid points stored to be plotted. Did you run set.points ?']);
      end
      switch(obj.dim)
      case 1
        n=length(obj.points(:,1));
        plot(obj.points(:,1),zeros(n,1),varargin{:})
      case 2
        plot(obj.points(:,1),obj.points(:,2),varargin{:})
      case 3
        plot3(obj.points(:,1),obj.points(:,2),obj.points(:,3),varargin{:})
      otherwise 
        disp('plot function is supported for 1, 2 and 3 dimensions only');
      end
    end
    function plotCells(obj,varargin)
      if(obj.points==0)
        error(['No grid points stored to be plotted. Did you run set.points ?']);
      end
      switch(obj.dim)
      case 2
        n=length(obj.points(:,1));
        eh=obj.eta./2;
        for i=1:n
          x=obj.points(i,1);
          xdata=x+[-1 1 1 -1]*eh(1);
          y=obj.points(i,2);
          ydata=y+[-1 -1 1 1]*eh(2);
          v=[xdata(:) ydata(:)];
          f=[1 2 3 4];
          patch('faces',f,'vertices',v,varargin{:})
        end
      case 3
        n=length(obj.points(:,1));
        eh=obj.eta./2;
        for i=1:n
          x=obj.points(i,1);
          xdata=x+[-1 1 1 -1]*eh(1);
          xdata=[xdata xdata];
          y=obj.points(i,2);
          ydata=y+[-1 -1 1 1]*eh(2);
          ydata=[ydata ydata];
          z=obj.points(i,3);
          zdata=z+[-1 -1 -1 -1]*eh(3);
          zdata=[zdata zdata+2*eh(3)];
          v=[xdata(:) ydata(:) zdata(:)];
          f=[1 2 3 4;
             1 2 6 5;
             2 3 7 6;
             3 4 8 7;
             4 1 5 8;
             5 6 7 8];
          patch('faces',f,'vertices',v,varargin{:})
        end
      case 3
      otherwise 
        disp('plot cell function is supported for 2 and 3 dimensions only');
      end
    end
  end
end
