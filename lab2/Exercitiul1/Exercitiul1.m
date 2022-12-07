vn = 1:0.1:32;

S = [];
E = [];
R = [];
U = [];
Q = [];

for i = vn
  n = i
  Sn = (n + 3) / 4; %accelerarea
  S = [S Sn];
  En = Sn / n; %eficienta
  E = [E En];
  Rn = (n + log2(n)) / n; %redundanta
  R = [R Rn];
  Un = Rn * En; %utilizarea
  U = [U Un];
  Qn = Sn * En / Rn; %factorul de calitate
  Q = [Q Qn];
endfor

[x, y1, y2] = plotyy(vn, [E; U],
                     vn, [S; R; Q], 
                     'plot', 'semilogy');
       
  hold on;

  set(x(1), 'YLim', [-0.26 1.2]);
  set(x(2), 'YLim', [0.4 64]);
  
  set(x, 'XTick', [1, 2, 4, 8, 16, 32]);
  set(x, 'XTickLabel', [1, 2, 4, 8, 16, 32]);
  set(x,'XScale','log');
  set(x, 'XLim', [0.95 34]);
  
  set(x(1), 'YTick', [0, 0.2, 0.4, 0.6, 0.8, 1]);
  set(x(1),'YTickLabel', [0, 0.2, 0.4, 0.6, 0.8, 1]);
  
  set(x(2), 'YTick', [1, 2, 4, 8, 16, 32]);
  set(x(2),'YTickLabel',[1, 2, 4, 8, 16, 32]);
  
  set(x, 'Xgrid', 'on');
  set(x, 'Ygrid', 'on');
  
  set(x, {"ycolor"}, {"b"; "r"});
  
  legend(["E(n)"; "U(n)"; "S(n)"; "R(n)"; "Q(n)"]);
  legend("location", "northwestoutside");
  legend("boxoff");

  title("Speedup, Efficiency, Redundancy, Utility, Quality factor");

  xlabel("n");
  ylabel(x(1), "E(n), U(n)");
  ylabel(x(2), "S(n), R(n), Q(n)");
  
  hold off;