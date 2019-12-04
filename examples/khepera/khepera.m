%
% khepera.m
%
% originally on: 21.01.2016 by [rungger]
%     author: M. Khaled
%
% you need to run ./khepera binary first 
%
% so that the files: khepera_ss.bdd 
%                    khepera_obst.bdd
%                    khepera_target.bdd
%                    khepera_controller.bdd 
% are created
%

function unicycle
clear set
close all



%% init
L=[2 0 0; 0 1 0; 0 0 .1];
c=[9.5; 0.6; 0];

% colors
colors=get(groot,'DefaultAxesColorOrder');

% initial state
x0=[12.5 10 -pi/2];

% load the symbolic set containig the abstract state space
% set=SymbolicSet('khepera_ss.bdd','projection',[1 2]);
% plotCells(set,'facecolor','none','edgec',[0.8 0.8 0.8],'linew',.1)
hold on
box on
axis([-.5 15.5 -.5 13.5])

% load the symbolic set containig obstacles
set=SymbolicSet('khepera_obst.bdd','projection',[1 2]);
plotCells(set,'facecolor',colors(1,:)*0.5+0.5,'edgec',colors(1,:),'linew',.1)

% load the symbolic set containig target set
set=SymbolicSet('khepera_target.bdd','projection',[1 2]);
plotCells(set,'facecolor',colors(2,:)*0.5+0.5,'edgec',colors(2,:),'linew',.1)


%% simulation
controller=SymbolicSet('khepera_controller.bdd','projection',[1 2 3]);

y=x0;
v=[];
while(1)

  
  if ( (y(end,:)-c')*L'*L*(y(end,:)'-c)<=1 )
    break;
  end 

  u=controller.getInputs(y(end,:));
  v=[v; u(1,:)];
  [t x]=ode45(@khepera_ode,[0 .3], y(end,:),[],u(1,:));

  y=[y; x(end,:)];
end

% plot initial state  and trajectory
plot(y(:,1),y(:,2),'k.-')
plot(y(1,1),y(1,2),'.','color',colors(5,:),'markersize',20)


end

function dxdt = khepera_ode(t,x,u)
  dxdt = zeros(3,1);

  dxdt(1)=u(1)*cos(x(3));
  dxdt(2)=u(1)*sin(x(3));
  dxdt(3)=u(2);
end
