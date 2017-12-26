clear all
close all
clc

delta=0.01;
npts=1000;
t=(0:npts-1)*delta;

A=textread('Butterworth_infile');

SAC_LP=textread('Butterworth_lp_SAC');
SAC_BP=textread('Butterworth_bp_SAC');
SAC_HP=textread('Butterworth_hp_SAC');

C_LP=textread('Butterworth_lp_CPP');
C_HP=textread('Butterworth_hp_CPP');
C_BP=textread('Butterworth_bp_CPP');

subplot(3,1,1);
scatter(t,SAC_LP,'bo');
hold on;
scatter(t,C_LP,'g*');
legend('LP, SAC','LP, C');

subplot(3,1,2);
scatter(t,SAC_BP,'bo');
hold on;
scatter(t,C_BP,'g*');
legend('BP, SAC','BP, C');

subplot(3,1,3);
scatter(t,SAC_HP,'bo');
hold on;
scatter(t,C_HP,'g*');
legend('HP, SAC','HP, C');
