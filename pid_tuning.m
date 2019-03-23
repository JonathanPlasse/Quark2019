k1 = 9750;
tau1 = 0.148;
k2 = 9523;
tau2 = 0.164;

nbStep = 1633;
pwm = 220;

Ts = 0.01;

g1 = tf(k1, [tau1 1]) / nbStep / pwm;
g2 = tf(k2, [tau2 1]) / nbStep / pwm;

g1d = c2d(g1, Ts);
g2d = c2d(g2, Ts);

pidTuner(g1d);

%  Kp + Ki * ------
%              z-1 
%  with Kp = 46.2, Ki = 752, Ts = 0.01

%   0.08185 z - 0.06851
%  ----------------------
%  z^2 - 1.853 z + 0.8662