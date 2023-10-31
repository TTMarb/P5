close
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
kv=1/w_1dbn; %factor between 1 dbn frequency and 3 dbn frequency
fs=2000000*2*pi;
kv2=1+(kv-1)/3.45;

s=tf('s');
z=tf('z', 1/fs);


d = designfilt("bandpassiir",FilterOrder=12, ...
    HalfPowerFrequency1=w_p1/kv,HalfPowerFrequency2=w_p2*kv, ...
    SampleRate=fs);
freqz(d.Coefficients,[],fs)

[num, den]=tf(d);%pole and zero coefficients
zplane(num, den)
saveas(gcf,'PolesZeroBandPass.pdf')
%[vz,vp,vk] = zplane(d)
%fvtool(d)
formatSpec='%d & $%d$ & $%f$ \\\\ \n';
for i = 0:length(num)-1 
    %fprintf(formatSpec,i,num(length(num)-i),den(length(den)-i)); 
end


