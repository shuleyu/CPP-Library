clear all
close all
clc

A=textread('GaussianBlur_infile');
B=textread('GaussianBlur_out');

subplot(2,1,1);
plot(A(:,1),A(:,2));

delta=0.025;
mu=0;
sigma=1;
gwidth=100;

NPTS=2*ceil(gwidth/2/delta)+1;
END=floor(NPTS/2);
for count=1:END
    x(count)=mu-delta*(END-count+1);
    x(NPTS+1-count)=2*mu-x(count);
end
x(ceil(NPTS/2))=mu;

gaussFilter=gaussmf(x,[sigma,0]);
C=conv(A(:,2),gaussFilter,'same');
C=C/max(abs(C));

subplot(2,1,2);
hold on
plot(A(:,1),B);
plot(A(:,1),C,'r');
legend('C','Matlab');
