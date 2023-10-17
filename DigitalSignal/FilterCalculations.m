syms a

w_p1=457000-80; %pass band frequency 1
w_p2=457000+80; %pass band frequency 2
w_s2=1000000;   %stop frequency 2
w_s1=w_p1/(w_s2/w_p2);   %stop frequency 1

Bw=w_p2-w_p1;   %bandwidth