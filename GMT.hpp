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
#include<ShellExec.hpp>

/*************************************************
 * This is a c++ wrapper for GMT C API.
 *
 *
 * Shule Yu
 * Aug 13 2018
 *
 * Key words: gmt plotting c++
*************************************************/

namespace GMT { // the order of the function definition matters: dependencies should appear first.

    namespace Hidden {

        // Related to grids.
        template<typename T>
        void GridJob(const std::string &outfile, const std::vector<std::vector<T>> &G,
                     const double &xinc, const double &yinc, const std::string &cmd, 
                     const std::string &WhichJob){

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

            // South to North. Row numbers. Y direction.
            auto res=CreateGrid(wesn[2],wesn[3],inc[1],-2);
            std::size_t m=(std::size_t)res[0]+4;
            inc[1]=res[1]; // Y increment.

            // West to East. Column numbers. X direction.
            res=CreateGrid(wesn[0],wesn[1],inc[0],-2);
            std::size_t n=(std::size_t)res[0]+4;
            inc[0]=res[1]; // X increment.


            // Create plot data.
            GMT_GRID *grid=(GMT_GRID *)GMT_Create_Data(API,GMT_IS_GRID,GMT_IS_SURFACE,
                                                       GMT_CONTAINER_ONLY,NULL,wesn,inc,
                                                       GMT_GRID_NODE_REG,-1,NULL);

            // Inject data.
            float *aux_data = new float [m*n];
            for (std::size_t i=0;i<m*n;++i) aux_data[i]=0.0/0.0;
            for (const auto &item:G) {
                std::size_t X=(std::size_t)round((item[0]-wesn[0])/inc[0]);
                std::size_t Y=(std::size_t)round((item[1]-wesn[2])/inc[1]);

                // swap X,Y position and flip along y-axis.
                aux_data[(m-3-Y)*n+X+2]=item[2];
            }
            grid->data=aux_data;


            // Adjust some value in the header. (magic happens here)
            grid->header->z_min=MinVal;
            grid->header->z_max=MaxVal;

            grid->header->grdtype=3;
            grid->header->gn=1;
            grid->header->gs=1;

            grid->header->BC[0]=2;
            grid->header->BC[1]=2;
            grid->header->BC[2]=2;
            grid->header->BC[3]=2;


            // periodic along longitude (x) direction.
            for (std::size_t Y=0;Y<m;++Y) {
                aux_data[(m-1-Y)*n+0]=aux_data[(m-1-Y)*n+n-4];
                aux_data[(m-1-Y)*n+1]=aux_data[(m-1-Y)*n+n-3];
                aux_data[(m-1-Y)*n+n-1]=aux_data[(m-1-Y)*n+3];
                aux_data[(m-1-Y)*n+n-2]=aux_data[(m-1-Y)*n+2];
            }

            // Get the virtual file.
            char filename[20];
            GMT_Open_VirtualFile(API,GMT_IS_GRID,GMT_IS_SURFACE,GMT_IN,grid,filename);

/*
std::cout << "1. " << m << " " << n << std::endl;
std::cout << "2. " << grid->header->n_rows << " " << grid->header->n_columns << " " << grid->header->registration << std::endl;
std::cout << "3. " << grid->header->wesn[0] << " " << grid->header->wesn[1] << " " << grid->header->wesn[2] << " " << grid->header->wesn[3] << " " << std::endl;
std::cout << "4. " << grid->header->inc[0] << " " << grid->header->inc[1] << std::endl;
std::cout << "5. " << grid->header->z_scale_factor << " " << grid->header->z_add_offset << std::endl;
std::cout << "6. " << std::string(grid->header->x_units) << " " << std::string(grid->header->y_units) << " " << std::string(grid->header->z_units) << " " << std::string(grid->header->title) << std::endl;
std::cout << "7. " << std::string(grid->header->command) << std::endl;
std::cout << "8. " << std::string(grid->header->remark) << std::endl;
std::cout << "9.z_minmax " << grid->header->z_min << " " << grid->header->z_max << std::endl;
std::cout << "10. " << grid->header->type << " " << grid->header->bits << " " << grid->header->complex_mode << " " << grid->header->n_alloc << std::endl;
std::cout << "11. " << grid->header->mx << " " << grid->header->my << " " << grid->header->nm << " " << grid->header->size << std::endl;
std::cout << "12. " << grid->header->trendmode << " " << grid->header->arrangement << " " << grid->header->n_bands << " " << grid->header->grdtype<< std::endl;
std::cout << "13.pad " << grid->header->pad[0] << " " << grid->header->pad[1] << " " << grid->header->pad[2] << " " << grid->header->pad[3] << " " << std::endl;
std::cout << "14.BC " << grid->header->BC[0] << " " << grid->header->BC[1] << " " << grid->header->BC[2] << " " << grid->header->BC[3] << " " << std::endl;
std::cout << "15. " << std::string(grid->header->name) << " " << std::endl;
std::cout << "16. " << std::string(grid->header->varname) << " " << std::endl;
// std::cout << "17. " << std::string(grid->header->ProjRefPROJ4) << " " << std::endl;
// std::cout << "18. " << std::string(grid->header->ProjRefWKT) << " " << std::endl;
std::cout << "19.netcdf " << grid->header->row_order << " " << grid->header->z_id << " " << grid->header->ncid << " " << std::endl;
std::cout << "20.netcdf_dim " << grid->header->xy_dim[0] << " " << grid->header->xy_dim[1] << std::endl;
std::cout << "21.netcdf_index " << grid->header->t_index[0] << " " << grid->header->t_index[1] << " " << grid->header->t_index[2] << std::endl;
std::cout << "22.netcdf " << grid->header->data_offset << " " << grid->header->stride << std::endl;
std::cout << "23. " << grid->header->nan_value << " " << grid->header->xy_off << std::endl;
std::cout << "24. " << grid->header->r_inc[0] << " " << grid->header->r_inc[1] << std::endl;
std::cout << "25.ESRI "<< grid->header->flags[0] << " " << grid->header->flags[1] << " " << grid->header->flags[2] << " " << grid->header->flags[3] << std::endl;
// std::cout << "26. GDAL"<< std::string(grid->header->pocket) << std::endl;
std::cout << "27. "<< grid->header->mem_layout[0] << " " << grid->header->mem_layout[1] << " " << grid->header->mem_layout[2] << " " << grid->header->mem_layout[3] << std::endl;
std::cout << "28. " << grid->header->bcr_threshold << " " << grid->header->bcr_interpolant << " " << grid->header->bcr_n << std::endl;
std::cout << "29. " << grid->header->nxp << " " << grid->header->nyp << " " << grid->header->no_BC << std::endl;
std::cout << "30. " << grid->header->gn << " " << grid->header->gs << std::endl;
std::cout << "31. " << grid->header->is_netcdf4 << std::endl;
std::cout << "32. " << grid->header->z_chunksize[0] << " " << grid->header->z_chunksize[1] << std::endl;
std::cout << "33. " << grid->header->z_shuffle << " " << grid->header->z_deflate_level << " " << grid->header-> z_scale_autoadjust << " " << grid->header->z_offset_autoadjust << std::endl;
std::cout << "34. " << grid->header->xy_adjust[0] << " " << grid->header->xy_adjust[1] << std::endl;
std::cout << "35. " << grid->header->xy_mode[0] << " " << grid->header->xy_mode[1] << std::endl;
std::cout << "36. " << grid->header->xy_unit[0] << " " << grid->header->xy_unit[1] << std::endl;
std::cout << "37. " << grid->header->xy_unit_to_meter[0] << " " << grid->header->xy_unit_to_meter[1] << std::endl;
std::cout << std::endl;
*/


            // Plot.
            char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
            if (WhichJob=="grdimage")
                GMT_Call_Module(API,"grdimage",GMT_MODULE_CMD,command);
            else if (WhichJob=="grdcontour")
                GMT_Call_Module(API,"grdcontour",GMT_MODULE_CMD,command);

            // Free spaces (aux_data seem to be deleted by GMT_Destroy_Session?).
            delete [] command;
            GMT_Close_VirtualFile(API,filename);
            GMT_Destroy_Data(API,grid);
            GMT_Destroy_Session(API);

            return;
        }

    }

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
    void pstext(const std::string &outfile, const std::vector<Text> &texts,
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

        // Free spaces (txt seem to be deleted by GMT_Destroy_Session?).
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

        // Free spaces (X,Y seem to be deleted by GMT_Destroy_Session?).
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
        std::size_t m=Data.size(),n=Data[0].size(); // m rows, n columns
        for (std::size_t i=0;i<m;++i)
            if (Data[i].size()!=n)
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
            double *X = new double [m];
            for (std::size_t j=0;j<m;++j)
                X[j]=Data[j][i];
            GMT_Put_Vector(API,vec,i,GMT_DOUBLE,X);
        }

        // Get the virtual file.
        char filename[20];
        GMT_Open_VirtualFile(API,GMT_IS_VECTOR,GMT_IS_POINT,GMT_IN,vec,filename);

        // Plot.
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psxy",GMT_MODULE_CMD,command);

        // Free spaces (Xs seem to be deleted by GMT_Destroy_Session?).
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

        // Free spaces (X seem to be deleted by GMT_Destroy_Session?).
        delete [] command;
        GMT_Close_VirtualFile(API,filename);
        GMT_Destroy_Data(API,vec);
        GMT_Destroy_Session(API);

        return;
    }

    // gmt grdcontour.
    template<typename T>
    void grdcontour(const std::string &outfile, const std::vector<std::vector<T>> &G,
                    const double &xinc, const double &yinc, const std::string &cmd){
        Hidden::GridJob(outfile, G, xinc, yinc, cmd,"grdcontour");
        return;
    }

    // gmt grdimage.
    // Tip: -nn is a good flag.
    template<typename T>
    void grdimage(const std::string &outfile, const std::vector<std::vector<T>> &G,
                    const double &xinc, const double &yinc, const std::string &cmd){
        Hidden::GridJob(outfile, G, xinc, yinc, cmd,"grdimage");
        return;
    }

    // Functions(templates) dependent on others.

    template<typename T>
    void psxy(const std::string &outfile, const std::vector<T> &X,
              const std::vector<T> &Y, const std::string &cmd){
        psxy(outfile,X.begin(),X.end(),Y.begin(),Y.end(),cmd);
        return;
    }

    template<typename T>
    void psxy(const std::string &outfile, const T &X, const T &Y, const std::string &cmd){
        psxy(outfile, std::vector<double> {X}, std::vector<double> {Y},cmd);
        return;
    }

    // Move reference point.
    void MoveReferencePoint(const std::string &outfile, const std::string &cmd){
        psxy(outfile,std::vector<double> {0},std::vector<double> {0},"-J -R -O -K "+cmd);
        return;
    }


    // Plot a time stamp at left bottom corner.
    void timestamp(const std::string &outfile){
        auto it=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string date(ctime(&it));
        std::vector<Text> texts;
        texts.push_back(Text(0,0,date,4,"LB"));
        MoveReferencePoint(outfile,"-Xf0i -Yf0i");
        pstext(outfile,texts,"-JX1i/1i -R0/1/0/1 -N -O -K");
        return;
    }

    template<typename T>
    void pshistogram(const std::string &outfile, const std::vector<T> &X, const std::string &cmd){
        pshistogram(outfile,X.begin(),X.end(),cmd);
        return;
    }

    void BeginPlot(const std::string &outfile, const std::string cmd="-P"){
        remove(outfile.c_str());
        psxy(outfile,std::vector<double> {0},std::vector<double> {0},"-JX1i/1i -R-1/1/-1/1 -K "+cmd);
        timestamp(outfile);
        return;
    }

    void SealPlot(std::string &outfile, const bool &CleanUp=true, const double &XSIZE=-1, const double YSIZE=-1){
        // new sizes are in inch, will converted to pts by multipling it by 72.

        psxy(outfile,std::vector<double> {0},std::vector<double> {0},"-JX1i/1i -R-1/1/-1/1 -O");
        if (CleanUp) {
            remove("gmt.conf");
            remove("gmt.history");
        }
        if (XSIZE>0 && YSIZE>0) {
            int newXSIZE=(int)(XSIZE*72),newYSIZE=(int)(YSIZE*72);
            char a[]="tmpfile_XXXXXX";
            mkstemp(a);
            std::string newfile(a),oneline;

            std::ifstream fpin(outfile);
            std::ofstream fpout(newfile);

            while (getline(fpin,oneline)){
                if (oneline.find("%%BoundingBox:")!=std::string::npos)
                    fpout << "%%BoundingBox: 0 0 " << newXSIZE << " " << newYSIZE << '\n';
                else if (oneline.find("%%HiResBoundingBox:")!=std::string::npos)
                    fpout << "%%HiResBoundingBox: 0 0 " << newXSIZE << " " << newYSIZE << '\n';
                else if (oneline.find("PageSize")!=std::string::npos) {

                    auto it=oneline.find("PageSize");
                    it+=oneline.substr(it).find("[");
                    auto it2=it+oneline.substr(it).find("]");
                    oneline=oneline.substr(0,it+1)+std::to_string(newXSIZE)+" "+std::to_string(newYSIZE)+oneline.substr(it2);

                    fpout << oneline << '\n';
                }
                else fpout << oneline << '\n';
            }

            fpin.close();
            fpout.close();
            swap(outfile,newfile);
            remove(newfile.c_str());
        }
        return;
    }

    std::string BeginEasyPlot(const double &XSIZE=15, const double &YSIZE=15) {

        char a[]="tmpfile_XXXXXX";
        mkstemp(a);
        std::string outfile(a);

        set("PS_MEDIA "+std::to_string(XSIZE)+"ix"+std::to_string(YSIZE)+"i");
        set("MAP_FRAME_PEN 0.4p,black");
        set("MAP_GRID_PEN_PRIMARY 0.4p,gray");
        set("FONT_ANNOT 8p");
        set("MAP_ANNOT_OFFSET_PRIMARY 6p");
        set("FONT_LABEL 12p");
        set("MAP_TICK_PEN_PRIMARY 0.4p,black");
        set("MAP_TICK_LENGTH_PRIMARY 0.01i");
        set("MAP_TICK_LENGTH_SECONDARY 0.005i");

        BeginPlot(outfile);

        return outfile;
    }

}

#endif
