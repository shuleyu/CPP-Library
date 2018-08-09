#include<iostream>

#include<GMT.hpp>

using namespace std;

int main(){

    bool PlotGrid=false;

    string outfile="GMT.ps";
    size_t NRow=1,NCol=3;
    size_t row,col;
    double Len=5,SpaceRatio=0.1,XSIZE=(NCol+2*SpaceRatio)*Len,YSIZE=(NRow+SpaceRatio)*Len+1,xp,yp;

    // config, define media ("gmt set PS_MEDIA 8.5ix8.5i").
    GMT::set("MAP_FRAME_PEN 0.25p,black");
    GMT::set("FONT_ANNOT 8p");
    GMT::set("PS_MEDIA "+to_string(XSIZE)+"ix"+to_string(YSIZE)+"i");

    // begin plot. (Calling "psxy /dev/null -J -R -P -K >> outfile").
    remove(outfile.c_str());
    GMT::beginplot(outfile);

    // pstitle.
    GMT::movereference(outfile,"-Xc -Yf"+to_string(YSIZE-1)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-J -R -Bxa10f5 -Bya10f5 -BWSne -O -K");

    vector<GMT::Text> texts;
    texts.push_back(GMT::Text(0,0,"GMT API Examples",24,"CB"));
    GMT::pstext(outfile,texts,"-JX1i/1i -R-10/10/-10/10 -N -O -K");


    // pscoast.
    row=1,col=1;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::movereference(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa10f5 -Bya10f5 -BWSne -O -K");

    GMT::pscoast(outfile,"-JR30/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -Glightgray -A10000 -O -K");

    // psxy.
    row=1,col=2;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::movereference(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-10/10/-10/10 -Bxa10f5 -Bya10f5 -BWSne -O -K");
    GMT::pscoast(outfile,"-JG-10/20/"+to_string(Len*(1-SpaceRatio))+"i -Rg -Bxa60g60 -Bya60g60 -BWSne -Glightgray -A10000 -O -K");

    vector<double> lon{-60,40},lat{-20,30};
    GMT::psxy(outfile,lon,lat,"-J -R -W0.5p,black,- -O -K");
    GMT::psxy(outfile,lon,lat,"-J -R -Gblue -Sc0.05i -W1p -O -K");

    texts.clear();
    texts.push_back(GMT::Text(lon[0],lat[0],"P1",10,"RM"));
    texts.push_back(GMT::Text(lon[1],lat[1],"P2",10,"LM"));
    GMT::pstext(outfile,texts,"-J -R -N -O -K");

    // pstext.
    row=1,col=3;
    xp=(col-1+SpaceRatio)*Len,yp=YSIZE-1-row*Len;
    GMT::movereference(outfile,"-Xf"+to_string(xp)+"i -Yf"+to_string(yp)+"i");
    if (PlotGrid) GMT::psbasemap(outfile,"-JX"+to_string(Len)+"i -R-5/5/-5/5 -Bxa5g1 -Bya10g1 -BWSne -O -K");

    texts.clear();
    texts.push_back(GMT::Text(0,0,"BigTitle",24,"CB"));
    texts.push_back(GMT::Text(0,-1,"@;red;SmallTitle@;;",12,"CB","Times-Bold"));
    GMT::pstext(outfile,texts,"-JX"+to_string(Len)+"i -R-5/5/-5/5 -N -O -K");

    // Seal the page (Calling "psxy /dev/null -J -R -O >> outfile").
    GMT::sealplot(outfile);

    // remove configure files.
    remove("gmt.conf");
    remove("gmt.history");

    return 0;
}
