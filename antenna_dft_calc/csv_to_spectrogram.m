% Set up the Import Options
clear
opts = delimitedTextImportOptions("NumVariables", 2);
opts.DataLines = [1, 5000000]; %Specify range
opts.Delimiter = ","; %Specify delimeter
opts.VariableNames = ["Ch1", "Ch2"]; %Specify col names
opts.VariableTypes = ["single", "single"]; %Specify var type

%Open file and save to two column vectors
tbl = readtable("C:/Users/chrel/Documents/test9m.csv", opts); %Select file location
Ch1 = tbl.Ch1; %Set array to contain first column from the table
Ch2 = tbl.Ch2; %Set array to contain second column from the table
clear tbl; %Clear table to free up memory

% Specify variable properties
opts = setvaropts(opts, ["Ch1", "Ch2"], "TrimNonNumeric", true);
opts = setvaropts(opts, ["Ch1", "Ch2"], "ThousandsSeparator", ",");

tbl = readtable("C:\Users\chrel\Documents\GitHub\P5\antenna_dft_calc\8192_9meter_diller.csv", opts);
RE = tbl.Ch1; %Set array to contain the real part from the table
IM = tbl.Ch2; %Set array to contain the imag part from the table
clear tbl; %Clear table to free up memory


%% Create both Spectrogram and graph from DFT program for comparison

%Spectrogram
w = hamming(2048); %Create window to apply to each FFT within the STFT
spectrogram(Ch1 ,w,1024,2048,1*10^6, 'yaxis','power'); %Column vector, window, overlap, FFT sample size, Sample freq, Freq on yaxis, Power colorscale.

%DFT graph
com_vec = complex(RE,IM);
Y = abs(com_vec);
t = 5; %Insert the time over which the signal was measured
X = linspace(0,t,length(com_vec));

figure;
plot(X,Y)
xlabel("Time (s)")
ylabel("Amplitude of signal (*)")
title("DFT result over time")





