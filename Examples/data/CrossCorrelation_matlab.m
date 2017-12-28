clear all
close all
clc

A=textread('cc_infile1');
B=textread('cc_infile2');
C=textread('cc_out');

dt=0.025;

t1=(0:length(A)-1);
t1=(t1-C)*dt;
t2=(0:length(B)-1)*dt;

plot(t1,A,t2,B)
C

[C,Lags]=xcorr(A,B);
C=C/(sqrt(sum(A(:).^2))*sqrt(sum(B(:).^2)));
[~,I]=max(abs(C));
I-14000
Lags(I)
