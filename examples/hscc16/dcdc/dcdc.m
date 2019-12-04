%
% dcdc.m
%
% created on: 09.10.2015
%     author: rungger
%
% see readme file for more information on the dcdc example
%
% you need to run ./dcdc binary first 
%
% so that the file: dcdc_controller.bdd is created
%

function dcdc
clear set
close all

%% simulation
% initial state
x0=[1.2 5.6];

% load the symbolic set containing the controller
controller=SymbolicSet('dcdc_controller.bdd');

y=x0;
v=[];
T=400; 
for t=1:T/0.5
  
  u=controller.getInputs(y(end,:));
  v=[v; u(1,:)];
  [t x]=ode45(@unicycle_ode,[0 .5], y(end,:), odeset('abstol',1e-4,'reltol',1e-4),u(1,:));

  y=[y; x(end,:)];
end


%% plot the dcdc safe set
% colors
colors=get(groot,'DefaultAxesColorOrder');

% plot the domain of the controller
x=controller.points;
plot(x(:,1),x(:,2),'.','color',[0.8 0.8 0.8])
hold on


% plot initial state  and trajectory
plot(y(:,1),y(:,2),'.-','color',colors(1,:))
plot(y(1,1),y(1,1),'.','color',colors(5,:),'markersize',20)

% plot safe set
v=[1.15 5.45; 1.55 5.45; 1.15 5.85; 1.55 5.85 ];
patch('vertices',v,'faces',[1 2 4 3],'facecolor','none','edgec',colors(2,:),'linew',1)
hold on


box on
grid on
axis([1.1 1.6 5.4 5.9])

end

function dxdt = unicycle_ode(t,x,u)

  r0=1.0;
  vs = 1.0 ;
  rl = 0.05 ;
  rc = rl / 10 ;
  xl = 3.0 ;
  xc = 70.0 ;

  if(u==1)
    A = [ -rl / xl  0 ;  0  (-1 / xc) * (1 / (r0 + rc)) ] ;
  else
    A = [ (-1 / xl) * (rl + ((r0 * rc) / (r0 + rc)))  ((-1 / xl) * (r0 / (r0 + rc))) / 5 ;
          5 * (r0 / (r0 + rc)) * (1 / xc)  (-1 / xc) * (1 / (r0 + rc)) ];
  end
  b = [(vs / xl) ; 0 ] ;

  dxdt= A*x+b;
end

