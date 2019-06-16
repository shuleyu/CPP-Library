#include<iostream>
#include<fstream>
#include<random>

#include<GMT.hpp>
#include<CreateGlobeGrid.hpp>
#include<Lon2360.hpp>
#include<Lon2180.hpp>
#include<ShellExec.hpp>

using namespace std;

int main(){

    bool PlotGrid=true;

    string outfile="GMT.ps";
    size_t NRow=3,NCol=3;
    size_t row,col;
    double Len=5,SpaceRatio=0.1,XSIZE=(NCol+2*SpaceRatio)*Len,YSIZE=(NRow+SpaceRatio)*Len+1,xp,yp;

    // config, define media ("gmt set PS_MEDIA 8.5ix8.5i").
    GMT::set("PS_MEDIA "+to_string(XSIZE)+"ix"+to_string(YSIZE)+"i");

    GMT::set("MAP_FRAME_PEN 0.4p,black");
//     GMT::set("MAP_TICK_PEN_PRIMARY 0.4p,black");
//     GMT::set("MAP_TICK_LENGTH_PRIMARY 0.007i");
//     GMT::set("MAP_TICK_LENGTH_SECONDARY 0.004i");
//     GMT::set("MAP_ANNOT_OFFSET_PRIMARY 0.02i");
//     GMT::set("FONT_LABEL 2.5p");
//     GMT::set("MAP_GRID_PEN_PRIMARY 0.25p,gray,.");
//     GMT::set("MAP_TICK_PEN_PRIMARY 0.25p,black");
    GMT::set("FONT_ANNOT 8p");

    // begin plot.
    GMT::BeginPlot(outfile,"-P",ShellExec("pwd",true)+"/"+string(__FILE__));

    // pstitle.
    GMT::MoveReferencePoint(outfile,"-Xc -Yf"+to_string(YSIZE-1)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa10f5 -Bya10f5 -BWSne -O -K");

    vector<GMT::Text> texts;
    texts.push_back(GMT::Text(0,0,"GMT API Examples",24,"CB"));
    GMT::pstext(outfile,texts,"-JX1i/1i -R-10/10/-10/10 -N -O -K");

    // pscoast.
    row=1,col=1;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa10f5 -Bya10f5 -BWSne -O -K");

    GMT::pscoast(outfile,"-JR30/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -Glightgray -A10000 -O -K");

    // psxy.
    row=1,col=2;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa10f5 -Bya10f5 -BWSne -O -K");
    GMT::pscoast(outfile,"-JG-10/20/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -Glightgray -A10000 -O -K");

    vector<double> lon{-60,0,40},lat{-20,10,30};
    GMT::psxy(outfile,lon,lat,"-J -R -W0.5p,black,- -L -O -K");
    GMT::psxy(outfile,lon,lat,"-J -R -W0.5p,red -O -K");
    GMT::psxy(outfile,lon,lat,"-J -R -Gblue -Sc0.05i -W1p -O -K");

    texts.clear();
    texts.push_back(GMT::Text(lon[0],lat[0],"P1",10,"RM"));
    texts.push_back(GMT::Text(lon[1],lat[1],"P2",10,"LM"));
    GMT::pstext(outfile,texts,"-J -R -N -O -K");

    // pstext.
    row=1,col=3;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-5/5/-5/5 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    texts.clear();
    texts.push_back(GMT::Text(0,0,"BigTitle",24,"CB"));
    texts.push_back(GMT::Text(0,-1,"@;red;SmallTitle@;;",12,"CB","Times-Bold"));
    GMT::pstext(outfile,texts,"-JX"+to_string(Len)+"i -R-5/5/-5/5 -N -O -K");

    // grdimage 1.
    row=2,col=1;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len+1.1;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    vector<vector<double>> grid;

    ifstream fpin("/home/shule/Research/Fun.Bash.c001/ritsema.2880");
    double la,lo,val,xinc=1,yinc=1,minval=numeric_limits<double>::max(),maxval=-minval;
    while (fpin >> la >> lo >> val) {
        minval=min(minval,val);
        maxval=max(maxval,val);
        grid.push_back({Lon2360(lo),la,val});
    }
    fpin.close();
    minval=-2.5;maxval=2.5;
    GMT::makecpt("-Cpolar -T"+to_string(minval)+"/"+to_string(maxval)+"/0.5 -I -Z > tmp.cpt");
    GMT::grdimage(outfile,grid,xinc,yinc,"-JR180/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Ctmp.cpt -O -K");
    // -D scale center position relative to last reference piont.
    GMT::psscale(outfile,"-Ctmp.cpt -D"+to_string(Len*(1-SpaceRatio)/2)+"i/-0.5i/2i/0.1ih -O -K -B0.5:dVs(%):");
    GMT::pscoast(outfile,"-JR180/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -W0.5p,black -A10000 -O -K");


    // Contour of previous grid.
    row=2,col=2;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len+1.1;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    double ContourVal=1.0;

    GMT::grdimage(outfile,grid,xinc,yinc,"-JR180/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Ctmp.cpt -O -K");
    GMT::grdcontour(outfile,grid,xinc,yinc,"-J -R -C+"+to_string(ContourVal)+" -W2p,orange -O -K");
    GMT::psscale(outfile,"-Ctmp.cpt -D"+to_string(Len*(1-SpaceRatio)/2)+"i/-0.5i/2i/0.1ih -O -K -B0.5:dVs(%):");
    GMT::pscoast(outfile,"-JR180/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -W0.5p,black -A10000 -O -K");

    // grdimage 2.
    row=2,col=3;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    grid.clear();
    xinc=1,yinc=2,val=0;
    for (double i=0;i<3;i+=xinc)
        for (double j=0;j<3;j+=yinc)
            grid.push_back({i,j,val+=1});

    GMT::grdimage(outfile,grid,xinc,yinc,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Chot -O -K");


    // Try plot grids.
    row=3,col=1;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len+1.1;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    xinc=13.232323,yinc=18.23213445;
    auto res=CreateGlobeGrid(xinc,yinc,true,true);
    for (size_t i=0;i<res.first.size();++i)
        res.first[i][2]=Lon2180(res.first[i][0])+res.first[i][1];

    GMT::makecpt("-Cpolar -T-270/270/0.5 -Z > tmp.cpt");
    GMT::grdimage(outfile,res.first,res.second[2],res.second[3],"-JR0/"+to_string(Len*(1-SpaceRatio))+"i -Rg -nn -E300 -Ctmp.cpt -O -K");
    // -D scale center position relative to last reference piont.
    GMT::psscale(outfile,"-Ctmp.cpt -D"+to_string(Len*(1-SpaceRatio)/2)+"i/-0.5i/2i/0.1ih -O -K -Bxa30 -Bx+lValue");
    GMT::pscoast(outfile,"-J -R -Bxa60g60 -Bya60g60 -BWSne -W0.5p,black -A10000 -O -K");

    // pshistogram.
    row=3,col=2;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0,2.0);
    vector<double> gaussian_val;
    for (size_t i=0;i<3000;++i) gaussian_val.push_back(distribution(generator));

    GMT::pshistogram(outfile,gaussian_val,"-JX"+to_string(Len*(1-SpaceRatio))+"i -R-10/10/0/40 "
                                          "-Z1 -W1 -L0.5p -G50/50/250 -Bxa5f1 -Bya10f2+u\"%\" -BWS -O -K");

    // psxy with different colors.
    row=3,col=3;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::MoveReferencePoint(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa5g1 -Bya10g1 -BWSne -O -K");

//     vector<vector<double>> data={{1.0,1.0},{3,-6},{0.5,0.5},{2,1}};
//     GMT::psxy(outfile,data,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Sc0.15i -W1p,black -N -O -K");
    GMT::makecpt("-Cgray -T0/1 -I > tmp.cpt");
    vector<vector<double>> data={{1.0,1.0,0.2,0.25},{3,-6,0.5,0.2},{0.3,0.6,0.7,0.15},{2,1,1.0,0.1}};
    GMT::psxy(outfile,data,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Sc -Ctmp.cpt -W1p,black -N -O -K");
    remove("tmp.cpt");


    // Seal the page.
    GMT::SealPlot(outfile,XSIZE,YSIZE);

    string pdffile=__FILE__;
    pdffile=pdffile.substr(0,pdffile.find(".cpp"))+".pdf";
    ShellExec("ps2pdf "+outfile+" "+pdffile);
    remove(outfile.c_str());

    return 0;
}
