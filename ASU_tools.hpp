#ifndef ASU_TOOLS_HPP
#define ASU_TOOLS_HPP

#include<vector>
#include<string>

int Combination(const int &n, const int &k);
std::string Decimal2Binary(int n);
std::vector<std::vector<bool>> GrayCode(const int &n);
std::string ShellExec(const std::string &cmd);
void Wiginterp(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, bool IsEven);

#include<Amplitude.hpp>
#include<AvrStd.hpp>
#include<Bootstrap.hpp>
#include<BottomDepth.hpp>                        // Need TauP Toolkit
#include<BottomLocation.hpp>                     // Need TauP Toolkit
#include<Butterworth.hpp>                        // Need SAC
#include<Cart2Sph.hpp>
#include<CircleBin.hpp>
#include<CMT2Sph.hpp>
#include<CombinePolygons.hpp>
#include<CombineTwoPolygons.hpp>
#include<CompareSignal.hpp>
#include<ComplexDivide.hpp>
#include<ConfidenceInterval.hpp>                 // Need sci-libs/gsl, (uncomment for less compile time)
#include<ConvexPolygon.hpp>
#include<Convolve.hpp>
#include<CrossCorrelation.hpp>
#include<CrossProduct.hpp>
#include<CrossStd2D.hpp>
#include<CrossStd.hpp>
#include<CurveJunction.hpp>
#include<Diff.hpp>
#include<DotDist.hpp>
#include<Envelope.hpp>                           // Need sci-libs/fftw.
#include<FFT.hpp>                                // Need sci-libs/fftw.
#include<FindAz.hpp>
#include<FindPeak.hpp>
#include<FindRayPath.hpp>
#include<GaussianBlur.hpp>
#include<GaussianFunction.hpp>
#include<GaussianSignal.hpp>
#include<GcpDistance.hpp>
#include<GridStretch.hpp>
#include<HannTaper.hpp>
#include<Interpolate.hpp>
#include<Kmeans.hpp>                             // Need mlpack, (uncomment for less compile time), http://www.mlpack.org/
#include<LinearInterp.hpp>
#include<LineJunction.hpp>
#include<LL2TP.hpp>
#include<LocDist.hpp>
#include<Lon2180.hpp>
#include<Lon2360.hpp>
#include<CreateGrid.hpp>
#include<MeshGrid.hpp>
#include<MollifierFunction.hpp>
#include<Normalize.hpp>
#include<PNormErr.hpp>
#include<PNorm.hpp>
#include<PolyFit.hpp>                            // Need dev-libs/boost, (uncomment for compile time)
#include<PointInPolygon.hpp>
#include<PointOnSegment.hpp>
#include<PREM.hpp>
#include<RadiationPattern.hpp>
#include<RampFunction.hpp>
#include<RayP2TakeOff.hpp>
#include<RayPath.hpp>
#include<ReorderX2Y.hpp>
#include<ReorderY2X.hpp>
#include<RemoveTrend.hpp>
#include<SegmentJunction.hpp>
#include<SHCoefficients.hpp>
#include<ShiftPhase.hpp>                         // Need sci-libs/fftw.
#include<ShiftStack.hpp>
#include<SimpsonRule.hpp>
#include<SNR.hpp>
#include<SolvePoly.hpp>
#include<SortWithIndex.hpp>
#include<Sph2Cart.hpp>
#include<SphAngle.hpp>
#include<StretchSignal.hpp>
#include<TP2LL.hpp>
#include<TriangleSignal.hpp>
#include<TstarOperation.hpp>                     // Need sci-libs/fftw.
#include<UnionIntervals.hpp>
#include<VectorAngle.hpp>
#include<WaterLevelDecon.hpp>                    // Need sci-libs/fftw.
#include<WayPoint.hpp>

#endif
