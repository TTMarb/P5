
w_p1=457000*2*pi-80*2*pi; %pass band frequency 1
w_p2=457000*2*pi+80*2*pi; %pass band frequency 2
w_s2=1000000*2*pi;   %stop frequency 2
w_s1=w_p1/(w_s2/w_p2);   %stop frequency 1
w_0=457000*2*pi;
scaleFactor=w_s2/w_p2;

alpha_s=60;    %min attenuation in the band stop in dB
alpha_p=1;     %max attenuation in the band pass in dB

n=ceil(1/(2*log10(w_s2/w_p2))*log10((10^(alpha_s/10)-1)/(10^(alpha_p/10)-1)));%butterworth order

w_1dbn=(10^(alpha_p/10)-1)^(1/(2*n)); %normalized frequency at 1dB attenuation
kf=w_p2/w_1dbn; %frequency scaled factor

%find the poles for normalized and scaled
syms k
p_k=zeros(1, n);
p_ks=zeros(1,n);
p(k)=exp(1i*(pi*((2*k)-1)/(2*n)+(pi)/(2)));
for x=1:n
    p_k(x)=p(x);
    p_ks(x)=p(x)*kf;
end

%plot the poles and zeros (everything scaled by kf)
theta = linspace(0,2*pi,100);

hold on
figure
plot(kf*cos(theta),kf*sin(theta),':', Color='black')
plot(p_ks, 'x', color='red')
ylabel('Im')
xlabel('Re')
axis equal
saveas(gcf,'PolesScaled.pdf')
hold off
close
hold on
figure
plot(cos(theta),sin(theta),':', Color='black')
plot(p_k, 'x', color='blue')
axis equal
hold off
saveas(gcf,'PolesNormalized.pdf')
close


%creating the transferfunction
s=tf('s');
H_lp=1;
for x=1:n/2
    H_lp=H_lp*1/((s/kf)^2-2*real(p_k(x))*s/kf+1);
end
%highpass
close

w_1dbn_hp=(10^(alpha_p/10)-1)^(1/(2*n));
kf_hp=w_p1/w_1dbn;

H_hp=1;
S=w_1dbn_hp/(s/kf_hp);
for x=1:n/2
    H_hp=H_hp*1/((S)^2-2*real(p_k(x))*S+1);
end

bode(H_hp)
saveas(gcf,'bode_HighPass.pdf')


H_bp=1.4454*H_lp*H_hp;
bode(H_bp)
saveas(gcf,'bode_BandPass.pdf')


