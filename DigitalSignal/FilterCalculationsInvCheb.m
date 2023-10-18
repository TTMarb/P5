syms a

w_p1=457000*2*pi-80*2*pi; %pass band frequency 1
w_p2=457000*2*pi+80*2*pi; %pass band frequency 2
w_s2=1000000*2*pi;   %stop frequency 2
w_s1=w_p1/(w_s2/w_p2);   %stop frequency 1

Bw=w_p2-w_p1;   %bandwidth

alpha_s=60;    %min attenuation in the band stop in dB
alpha_p=1;     %max attenuation in the band pass in dB

epsilon=1/(sqrt(10^(alpha_s/10)-1));    %=0.001

n=ceil(acosh(sqrt((10^(alpha_s/10) - 1) / (10^(alpha_p/10) - 1)) / acosh(w_s2/w_p2)));%order required

n_butterworth=ceil(1/(2*log10(w_s2/w_p1))*log10((10^(alpha_s/10)-1)/(10^(alpha_p/10)-1)));%butterworth order for refrence

%finding the poles of the system
syms k
p_k=zeros(1, n);
p(k)=1/(sin((2*k-1)*pi/(2*n))*sinh(1/n*asinh(1/epsilon))+1i*cos((2*k-1)*pi/(2*n))*cosh(1/n*acosh(1/epsilon))); %poles
for x=n+1:2*n
    p_k(x-n)=p(x);
%vpa(p(x),4)
end

%finding the zeros of the system
%needs an even number of poles rounded down to an even number

Num_Zero=floor(n/2)*2;
z_k=zeros(1, Num_Zero);
z(k)=1i/(cos((2*k-1)*pi/(2*Num_Zero)));
for x=1:Num_Zero/2
    z_k(x*2-1)=z(x);
    z_k(x*2)=-z(x);
end

%plot the poles and zeros
theta = linspace(0,2*pi,100);
hold on
%plot(cos(theta),sin(theta),'-', Color='black')
plot(z_k, 'o', Color='red')
plot(p_k, 'x', color='blue')
axis equal
ylabel('Im')
xlabel('Re')
legend({'Poles','Zeros'});
hold off

%creating the transferfunction
s=tf('s');
H=1;
for x=1:n
    H=H*1/(s-p_k(x));
end
for x=1:Num_Zero
    H=H*(s-z_k(x));
end

bode(H)
