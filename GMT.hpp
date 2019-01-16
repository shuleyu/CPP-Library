#ifndef ASU_GMT
#define ASU_GMT

#include<iostream>
#include<fstream>
#include<iterator>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<string>
#include<chrono>
#include<algorithm>

extern "C" {
// Current version: GMT-5.4.4
#include<gmt/gmt.h>
}

#include<CreateGrid.hpp>
#include<SACSignals.hpp>
#include<EvenSampledSignal.hpp>
#include<DigitalSignal.hpp>

/*************************************************
 * This is a wrapper for GMT API.
 *
 *
 * Shule Yu
 * Aug 13 2018
 *
 * Key words: gmt plotting c++
*************************************************/

namespace GMT { // the order of the function definition matters: dependencies should appear first.

    // basic operations (no dependencies needed)

    // gmt set.
    void set(const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup(cmd.c_str());
        GMT_Call_Module(API,"set",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }


    // gmt pscoast.
    void pscoast(const std::string &outfile, const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup((cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"pscoast",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }


    // gmt makecpt.
    void makecpt(const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup(cmd.c_str());
        GMT_Call_Module(API,"makecpt",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }


    // gmt psscale.
    void psscale(const std::string &outfile, const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup((cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psscale",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }


    // gmt psbasemap.
    void psbasemap(const std::string &outfile, const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup((cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psbasemap",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }

    // gmt psimage.
    void psimage(const std::string &outfile, const std::string &image, const std::string &cmd){
        void *API=GMT_Create_Session(__func__,2,0,NULL);
        char *command=strdup((image+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psimage",GMT_MODULE_CMD,command);

        delete [] command;
        GMT_Destroy_Session(API);
    }


    // gmt pstext.
    class Text {
        public:
        std::string Justify,Font,Content;
        double X,Y;
        int Size;
        Text(const double &x, const double &y, const std::string &c) : Text(x,y,c,12) {}
        Text(const double &x, const double &y, const std::string &c, const int &s) :
             Text(x,y,c,s,"CB") {}
        Text(const double &x, const double &y, const std::string &c, const int &s,
             const std::string &j) : Text(x,y,c,s,j,"Helvetica") {}
        Text(const double &x, const double &y, const std::string &c, const int &s,
             const std::string &j, const std::string &f) {
                 X=x;Y=y;Content=c;Size=s;Justify=j;Font=f;
             }
    };
    void pstext(const std::string &outfile, const std::vector<GMT::Text> &texts,
                const std::string &cmd){

        std::size_t n=texts.size();
        if (n==0) return;

        void *API=GMT_Create_Session(__func__,2,0,NULL);

        uint64_t par[]={1,1,1};
        GMT_TEXTSET *txt=(GMT_TEXTSET *)GMT_Create_Data(API,GMT_IS_TEXTSET,GMT_IS_NONE,
                                                        0,par,NULL,NULL,0,-1,NULL);

        char filename[20];
        for (const auto &item:texts) {
            std::string s=std::to_string(item.X)+" "+std::to_string(item.Y)+" "+item.Content;

            txt->table[0]->segment[0]->n_rows = 1;
            txt->table[0]->segment[0]->data[0] = new char [s.size()+1];
            strcpy(txt->table[0]->segment[0]->data[0],s.c_str());

            GMT_Open_VirtualFile(API,GMT_IS_TEXTSET,GMT_IS_NONE,GMT_IN,txt,filename);
            char *command=strdup(("-<"+std::string(filename)+" "+cmd+" -F+j"+item.Justify+"+f"+
                                  std::to_string(item.Size)+"p,"+item.Font+" ->>"+outfile).c_str());
            GMT_Call_Module(API,"pstext",GMT_MODULE_CMD,command);

            delete [] command;
            delete [] txt->table[0]->segment[0]->data[0];
            txt->table[0]->segment[0]->data[0]=nullptr;
            GMT_Close_VirtualFile(API,filename);
        }

        GMT_Destroy_Data(API,txt);
        GMT_Destroy_Session(API);
        return;
    }


    // gmt psxy.
    template <typename T>
    void psxy(const std::string &outfile, const T XBegin, const T XEnd,
              const T YBegin, const T YEnd, const std::string &cmd){

        // Check array size.
        std::size_t n=std::distance(XBegin,XEnd),m=std::distance(YBegin,YEnd);
        if (n==0) return;

        if (m!=n) throw std::runtime_error("In "+std::string(__func__)+", input x,y size don't match.");

        void *API=GMT_Create_Session(__func__,2,0,NULL);


        // Set vector dimensions..
        uint64_t par[2];
        par[0]=2;  // x,y value (2 columns).
        par[1]=n;  // npts (n rows).

        // Create plot data.
        GMT_VECTOR *vec=(GMT_VECTOR *)GMT_Create_Data(API,GMT_IS_VECTOR,GMT_IS_POINT,
                                                      0,par,NULL,NULL,0,-1,NULL);

        // Inject data.
        double *X = new double [n], *Y = new double [n];
        std::size_t i=0;
        for (auto it=XBegin;it!=XEnd;++it) X[i++]=*it;
        i=0;
        for (auto it=YBegin;it!=YEnd;++it) Y[i++]=*it;

        GMT_Put_Vector(API,vec,0,GMT_DOUBLE,X);
        GMT_Put_Vector(API,vec,1,GMT_DOUBLE,Y);

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_VECTOR,GMT_IS_POINT,GMT_IN,vec,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psxy",GMT_MODULE_CMD,command);

        // free spaces (X,Y seem to be deleted by GMT_Destroy_Session? very confusing).
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,vec);
        GMT_Destroy_Session(API);

        return;
    }

    // psxy with multiple coloms.
    template <typename T>
    void psxy(const std::string &outfile, const std::vector<std::vector<T>> &Data,
              const std::string &cmd){


        // Check array size.
        if (Data.empty()) return;
        std::size_t n=Data.size(),m=Data[0].size(); // m rows, n columns
        for (std::size_t i=0;i<n;++i)
            if (Data[i].size()!=m)
                throw std::runtime_error("In "+std::string(__func__)
                                         +", input data column size don't match.");

        void *API=GMT_Create_Session(__func__,2,0,NULL);

        // Set vector dimensions..
        uint64_t par[2];
        par[0]=n;  // x,y,z,... value (n columns).
        par[1]=m;  // (m of rows).

        // Create plot data.
        GMT_VECTOR *vec=(GMT_VECTOR *)GMT_Create_Data(API,GMT_IS_VECTOR,GMT_IS_POINT,
                                                      0,par,NULL,NULL,0,-1,NULL);

        // Inject data.
        for (std::size_t i=0;i<n;++i) {
            double *X = (double *)malloc(m*sizeof(double));
            for (std::size_t j=0;j<m;++j)
                X[j]=Data[i][j];
            GMT_Put_Vector(API,vec,i,GMT_DOUBLE,X);
        }

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_VECTOR,GMT_IS_POINT,GMT_IN,vec,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psxy",GMT_MODULE_CMD,command);

        // free spaces (X's seem to be deleted by GMT_Destroy_Session? very confusing).
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,vec);
        GMT_Destroy_Session(API);

        return;
    }

    // gmt pshistogram.
    template <typename T>
    void pshistogram(const std::string &outfile, const T XBegin, const T XEnd,
                     const std::string &cmd){

        // Check array size.
        std::size_t n=std::distance(XBegin,XEnd);
        if (n==0) return;

        void *API=GMT_Create_Session(__func__,2,0,NULL);


        // Set vector dimensions..
        uint64_t par[2];
        par[0]=1;  // value (1 columns).
        par[1]=n;  // npts (n rows).

        // Create plot data.
        GMT_VECTOR *vec=(GMT_VECTOR *)GMT_Create_Data(API,GMT_IS_VECTOR,GMT_IS_POINT,
                                                      0,par,NULL,NULL,0,-1,NULL);

        // Inject data.
        double *X = new double [n];
        std::size_t i=0;
        for (auto it=XBegin;it!=XEnd;++it) X[i++]=*it;

        GMT_Put_Vector(API,vec,0,GMT_DOUBLE,X);

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_VECTOR,GMT_IS_POINT,GMT_IN,vec,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"pshistogram",GMT_MODULE_CMD,command);

        // free spaces.
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,vec);
        GMT_Destroy_Session(API);

        return;
    }


    // gmt grdimage.

    template<typename T>
    void grdimage(const std::string &outfile, const std::vector<std::vector<T>> &G,
                  const double &xinc, const double &yinc, const std::string &cmd){

        if (G.empty()) return;

        // Check array size.
        for (const auto &item:G)
            if (item.size()!=3)
                throw std::runtime_error("In "+std::string(__func__)+", input is not x,y,z data");

        void *API=GMT_Create_Session(__func__,2,0,NULL);

        // Set grid limits.
        double MinVal=std::numeric_limits<double>::max(),MaxVal=-MinVal;
        double wesn[]={MinVal,MaxVal,MinVal,MaxVal};
        for (const auto &item:G) {
            wesn[0]=std::min(wesn[0],item[0]);
            wesn[1]=std::max(wesn[1],item[0]);
            wesn[2]=std::min(wesn[2],item[1]);
            wesn[3]=std::max(wesn[3],item[1]);
            MinVal=std::min(MinVal,item[2]);
            MaxVal=std::max(MaxVal,item[2]);
        }


        // Set grid increments.
        double inc[]={xinc,yinc};

        // Set up grid size.
        auto res=CreateGrid(wesn[0],wesn[1],inc[0],-1);
        std::size_t m=(std::size_t)res[0]+4;
        res=CreateGrid(wesn[2],wesn[3],inc[1],-1);
        std::size_t n=(std::size_t)res[0]+4;


        // Create plot data.
        GMT_GRID *grid=(GMT_GRID *)GMT_Create_Data(API,GMT_IS_GRID,GMT_IS_SURFACE,
                                                   GMT_CONTAINER_ONLY,NULL,wesn,inc,
                                                   GMT_GRID_NODE_REG,-1,NULL);

        // Inject data.
        float *aux_data = new float [m*n];
        for (std::size_t i=0;i<m*n;++i) aux_data[i]=0.0/0.0;
        for (const auto &item:G) {
            std::size_t X=(std::size_t)round((item[0]-wesn[0])/xinc);
            std::size_t Y=(std::size_t)round((item[1]-wesn[2])/yinc);
            // swap X,Y position and flip along y-axis.
            aux_data[(n-3-Y)*m+X+2]=item[2];
        }
        grid->data=aux_data;


        // Adjust something in the header. (magic)
        grid->header->z_min=MinVal;
        grid->header->z_max=MaxVal;

        grid->header->grdtype=3;
        grid->header->gn=1;
        grid->header->gs=1;
        grid->header->BC[2]=3;
        grid->header->BC[3]=3;

        // periodic along longitude (x) direction.
        grid->header->BC[0]=2;
        grid->header->BC[1]=2;
        for (std::size_t Y=0;Y<n;++Y) {
            aux_data[(n-1-Y)*m+0]=aux_data[(n-1-Y)*m+m-4];
            aux_data[(n-1-Y)*m+1]=aux_data[(n-1-Y)*m+m-3];
            aux_data[(n-1-Y)*m+m-1]=aux_data[(n-1-Y)*m+3];
            aux_data[(n-1-Y)*m+m-2]=aux_data[(n-1-Y)*m+2];
        }

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_GRID,GMT_IS_SURFACE,GMT_IN,grid,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"grdimage",GMT_MODULE_CMD,command);

        // Free spaces.
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,grid);
        GMT_Destroy_Session(API);

        return;
    }

    // gmt grdcontour.

    template<typename T>
    void grdcontour(const std::string &outfile, const std::vector<std::vector<T>> &G,
                    const double &xinc, const double &yinc, const std::string &cmd){

        if (G.empty()) return;

        // Check array size.
        for (const auto &item:G)
            if (item.size()!=3)
                throw std::runtime_error("In "+std::string(__func__)+", input is not x,y,z data");

        void *API=GMT_Create_Session(__func__,2,0,NULL);

        // Set grid limits.
        double MinVal=std::numeric_limits<double>::max(),MaxVal=-MinVal;
        double wesn[]={MinVal,MaxVal,MinVal,MaxVal};
        for (const auto &item:G) {
            wesn[0]=std::min(wesn[0],item[0]);
            wesn[1]=std::max(wesn[1],item[0]);
            wesn[2]=std::min(wesn[2],item[1]);
            wesn[3]=std::max(wesn[3],item[1]);
            MinVal=std::min(MinVal,item[2]);
            MaxVal=std::max(MaxVal,item[2]);
        }


        // Set grid increments.
        double inc[]={xinc,yinc};

        // Set up grid size.
        auto res=CreateGrid(wesn[0],wesn[1],inc[0],-1);
        std::size_t m=(std::size_t)res[0]+4;
        res=CreateGrid(wesn[2],wesn[3],inc[1],-1);
        std::size_t n=(std::size_t)res[0]+4;


        // Create plot data.
        GMT_GRID *grid=(GMT_GRID *)GMT_Create_Data(API,GMT_IS_GRID,GMT_IS_SURFACE,
                                                   GMT_CONTAINER_ONLY,NULL,wesn,inc,
                                                   GMT_GRID_NODE_REG,-1,NULL);

        // Inject data.
        float *aux_data = new float [m*n];
        for (std::size_t i=0;i<m*n;++i) aux_data[i]=0.0/0.0;
        for (const auto &item:G) {
            std::size_t X=(std::size_t)round((item[0]-wesn[0])/xinc);
            std::size_t Y=(std::size_t)round((item[1]-wesn[2])/yinc);
            // swap X,Y position and flip along y-axis.
            aux_data[(n-3-Y)*m+X+2]=item[2];
        }
        grid->data=aux_data;


        // Adjust something in the header. (magic)
        grid->header->z_min=MinVal;
        grid->header->z_max=MaxVal;

        grid->header->grdtype=3;
        grid->header->gn=1;
        grid->header->gs=1;
        grid->header->BC[2]=3;
        grid->header->BC[3]=3;

        // periodic along longitude (x) direction.
        grid->header->BC[0]=2;
        grid->header->BC[1]=2;
        for (std::size_t Y=0;Y<n;++Y) {
            aux_data[(n-1-Y)*m+0]=aux_data[(n-1-Y)*m+m-4];
            aux_data[(n-1-Y)*m+1]=aux_data[(n-1-Y)*m+m-3];
            aux_data[(n-1-Y)*m+m-1]=aux_data[(n-1-Y)*m+3];
            aux_data[(n-1-Y)*m+m-2]=aux_data[(n-1-Y)*m+2];
        }

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_GRID,GMT_IS_SURFACE,GMT_IN,grid,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"grdcontour",GMT_MODULE_CMD,command);

        // Free spaces.
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,grid);
        GMT_Destroy_Session(API);

        return;
    }

    // Functions(templates) dependent on others.

    // Move reference point.
    void MoveReferencePoint(const std::string &outfile, const std::string &cmd){
        GMT::psbasemap(outfile,"-J -R -Bwens -O -K "+cmd);
        return;
    }

    // Plot a time stamp at left bottom corner.
    void timestamp(const std::string &outfile){
        auto it=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string date(ctime(&it));
        std::vector<GMT::Text> texts;
        texts.push_back(GMT::Text(0,0,date,4,"LB"));
        GMT::MoveReferencePoint(outfile,"-Xf0i -Yf0i");
        GMT::pstext(outfile,texts,"-JX1i/1i -R0/1/0/1 -N -O -K");
        return;
    }


    template<typename T>
    void psxy(const std::string &outfile, const std::vector<T> &X,
              const std::vector<T> &Y, const std::string &cmd){
        GMT::psxy(outfile,X.begin(),X.end(),Y.begin(),Y.end(),cmd);
        return;
    }

    void psxy(const std::string &outfile,
              const SACSignals &item, const std::size_t index,
              const std::string &cmd){
        if (item.Size()==0 || item.Size()<=index) return;

        auto data=item.GetTimeAndWaveforms(std::vector<std::size_t> {index});
        psxy(outfile,data[0].first,data[0].second,cmd);
        return;
    }
    void psxy(const std::string &outfile,
              const DigitalSignal &item, const std::string &cmd){
        psxy(outfile,item.GetTime(),item.GetAmp(),cmd);
        return;
    }

    template<typename T>
    void pshistogram(const std::string &outfile, const std::vector<T> &X, const std::string &cmd){
        GMT::pshistogram(outfile,X.begin(),X.end(),cmd);
        return;
    }

    void BeginPlot(const std::string &outfile){
        remove(outfile.c_str());
        GMT::psbasemap(outfile,"-JX1i/1i -R-1/1/-1/1 -Bwens -P -K");
        GMT::timestamp(outfile);
        return;
    }

    void SealPlot(const std::string &outfile){
        GMT::psbasemap(outfile,"-JX1i/1i -R-1/1/-1/1 -Bwens -O");
        remove("gmt.conf");
        remove("gmt.history");
        return;
    }

    std::string BeginEasyPlot() {

        char a[]="tmpfile_XXXXXX";
        mkstemp(a);
        std::string outfile(a);

        GMT::set("PS_MEDIA 15ix15i");
        GMT::set("FONT_ANNOT 8p");
        GMT::set("FONT_LABEL 12p");
        GMT::set("MAP_ANNOT_OFFSET_PRIMARY 6p");
        GMT::set("MAP_FRAME_PEN 0.4p,black");
        GMT::set("MAP_TICK_PEN_PRIMARY 0.4p,black");

        GMT::BeginPlot(outfile);

        return outfile;
    }

}

#endif
