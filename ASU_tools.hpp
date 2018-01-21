#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

#include<vector>
#include<string>

int Combination(const int &n, const int &k);
std::string Decimal2Binary(int n);
std::vector<std::vector<bool>> GrayCode(const int &n);
std::string ShellExec(const std::string &cmd);
void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven);

// #include<Amplitude.hpp>
// #include<AvrStd.hpp>
// #include<Bootstrap.hpp>
// #include<BottomDepth.hpp>                        // Need TauP Toolkit
// #include<BottomLocation.hpp>                     // Need TauP Toolkit
// #include<Butterworth.hpp>                        // Need SAC
// #include<Cart2Sph.hpp>
// #include<CircleBin.hpp>
// #include<CMT2Sph.hpp>
// #include<CombinePolygons.hpp>
// #include<CombineTwoPolygons.hpp>
// #include<ComplexDivide.hpp>
// #include<ConfidenceInterval.hpp>                 // Need sci-libs/gsl, (uncomment for faster compile)
// #include<Convolve.hpp>
// #include<CrossCorrelation.hpp>
// #include<CrossProduct.hpp>
// #include<CrossStd2D.hpp>
// #include<CrossStd.hpp>
// #include<Diff.hpp>
// #include<DotDist.hpp>
// #include<Envelope.hpp>
// #include<FFT.hpp>
// #include<FindPeak.hpp>
// #include<GaussianBlur.hpp>
// #include<GaussianFunction.hpp>
// #include<GaussianSignal.hpp>
// #include<GcpDistance.hpp>
// #include<GridStretch.hpp>
// #include<Interpolate.hpp>
// #include<Kmeans.hpp>                             // Need mlpack, (uncomment for faster compile), http://www.mlpack.org/
// #include<LineJunction.hpp>
// #include<LL2TP.hpp>
// #include<LocDist.hpp>
// #include<Lon2180.hpp>
// #include<Lon2360.hpp>
// #include<Mollifier.hpp>
// #include<Normalize.hpp>
// #include<PNormErr.hpp>
// #include<PNorm.hpp>
// #include<PolyFit.hpp>                            // Need dev-libs/boost, (uncomment for faster compile)
// #include<PointsInPolygon.hpp>
// #include<PREM.hpp>
// #include<RampFunction.hpp>
// #include<RayP2TakeOff.hpp>
// #include<SHCoefficients.hpp>
// #include<ShiftPhase.hpp>                         // Need sci-libs/fftw.
// #include<ShiftStack.hpp>
#include<SNR.hpp>
// #include<SolvePoly.hpp>
// #include<SortWithIndex.hpp>
// #include<Sph2Cart.hpp>
// #include<SphAngle.hpp>
// #include<TP2LL.hpp>
// #include<TriangleSignal.hpp>
// #include<TstarOperation.hpp>

#endif
