% Set up the Import Options
clear
opts = delimitedTextImportOptions("NumVariables", 2);
opts.DataLines = [1, 5000000]; %Specify range
opts.Delimiter = ","; %Specify delimeter
opts.VariableNames = ["Ch1", "Ch2"]; %Specify col names
opts.VariableTypes = ["single", "single"]; %Specify var type

%Open file and save to two column vectors
tbl = readtable("C:\Users\chrel\OneDrive - Aalborg Universitet\P5\Project\Plots and calculation\Scripts for tests\test_2.csv", opts); %Select file location
Ch1 = tbl.Ch1; %Set array to contain first column from the table
Ch2 = tbl.Ch2; %Set array to contain second column from the table
clear tbl; %Clear table to free up memory

%Spectrogram
w = hamming(2048); %Create window to apply to each FFT within the STFT
spectrogram(Ch1 ,w,1024,2048,1*10^6, 'yaxis','power'); %Column vector, window, overlap, FFT sample size, Sample freq, Freq on yaxis, Power colorscale.








