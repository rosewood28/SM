vn = 2.8:0.1:32;

S = [];
E = [];
R = [];
U = [];
Q = [];

for i = vn
  n = i
  Sn = log(n) * log(log(n)); %accelerarea
  S = [S Sn];
  En = Sn / n; %eficienta
  E = [E En];
  Rn = 1 / (log(n) * log(log(n))); %redundanta
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
  
  set(x, 'XScale', 'log');
  set(x, 'XTick', [1, 2, 4, 8, 16, 32]);
  set(x, 'XTickLabel', [1, 2, 4, 8, 16, 32]);
  set(x,'XScale','log');
  set(x, 'XLim', [3 32]);
  
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