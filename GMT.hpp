#ifndef ASU_GMT
#define ASU_GMT

#include<iostream>
#include<iterator>
#include<vector>
#include<cstdlib>
#include<cstring>

extern "C" {
// Current version: GMT-5.4.3
#include<gmt/gmt.h>
}

/*************************************************
 * This C++ template returns the amplitude and the
 * positions of that amplitude.
 *
 * input(s):
 * const vector<T> &p  ----  Input array.
 *
 * output(s):
 * pair<T,vector<size_t>> ans
 *                     ----  ans.first is the ampiltude.
 *                           ans.second is the position(s)
 *                           this amplitude occurs in p.
 *
 * Shule Yu
 * Dec 19 2017
 *
 * Key words: amplitude
*************************************************/

namespace GMT {

    // gmt set.
    void set(const std::string &cmd){
        void *API=GMT_Create_Session("My session",2,0,NULL);
        char *command=strdup(cmd.c_str());
        GMT_Call_Module(API,"set",GMT_MODULE_CMD,command);
        GMT_Destroy_Session(API);
        delete [] command;
    }

    // gmt pscoast.
    void pscoast(const std::string &outfile, const std::string &cmd){
        void *API=GMT_Create_Session("My session",2,0,NULL);
        char *command=strdup((cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"pscoast",GMT_MODULE_CMD,command);
        GMT_Destroy_Session(API);
        delete [] command;
    }

    // gmt psbasemap.
    void psbasemap(const std::string &outfile, const std::string &cmd){
        void *API=GMT_Create_Session("My session",2,0,NULL);
        char *command=strdup((cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psbasemap",GMT_MODULE_CMD,command);
        GMT_Destroy_Session(API);
        delete [] command;
    }

    // gmt pstext.
    class Text{
        public:
        std::string Justify,Font,Content;
        double X,Y;
        int Size;
        Text(const double &x, const double &y, const std::string &c)               : Text(x,y,c,12) {}
        Text(const double &x, const double &y, const std::string &c, const int &s) : Text(x,y,c,s,"CB") {}
        Text(const double &x, const double &y, const std::string &c, const int &s,
             const std::string &j)                                                 : Text(x,y,c,s,j,"Helvetica") {}
        Text(const double &x, const double &y, const std::string &c, const int &s,
             const std::string &j, const std::string &f)                           {X=x;Y=y;Content=c;Size=s;Justify=j;Font=f;}
    };

    void pstext(const std::string &outfile, const std::vector<GMT::Text> &texts, const std::string &cmd){

        void *API=GMT_Create_Session("My session",2,0,NULL);
        size_t n=texts.size();
        if (n==0) return;

        uint64_t par[]={1,1,1};
        GMT_TEXTSET *txt=(GMT_TEXTSET *)GMT_Create_Data(API,GMT_IS_TEXTSET,GMT_IS_NONE,0,par,NULL,NULL,0,-1,NULL);

        char filename[20];
        for (const auto &item:texts) {
            std::string s=std::to_string(item.X)+" "+std::to_string(item.Y)+" "+item.Content;

            txt->table[0]->segment[0]->n_rows = 1;
            txt->table[0]->segment[0]->data[0] = new char [s.size()+1];
            strcpy(txt->table[0]->segment[0]->data[0],s.c_str());

            GMT_Open_VirtualFile(API,GMT_IS_TEXTSET,GMT_IS_NONE,GMT_IN,txt,filename);
            char *command=strdup(("-<"+std::string(filename)+" "+cmd+" -F+j"+item.Justify
                                    +"+f"+std::to_string(item.Size)+"p,"+item.Font+" ->>"+outfile).c_str());
            GMT_Call_Module(API,"pstext",GMT_MODULE_CMD,command);
            delete [] command;
        }
        GMT_Destroy_Session(API);
        return;
    }

    // gmt psxy.
    template <class T>
    void psxy(const std::string &outfile, const T XBegin, const T XEnd, const T YBegin, const T YEnd, const std::string &cmd){

        // Check array size.
        size_t n=std::distance(XBegin,XEnd),m=std::distance(YBegin,YEnd);
        if (m!=n) throw std::runtime_error("In __func__, input x,y size don't match.");

        void *API=GMT_Create_Session("My session",2,0,NULL);
        
        if (n==0) {
            char *command=strdup(("-</dev/null "+cmd+" ->>"+outfile).c_str());
            GMT_Call_Module(API,"psxy",GMT_MODULE_CMD,command);
            GMT_Destroy_Session(API);
            delete [] command;
            return;
        }

        uint64_t par[2];
        par[0]=2;  // x,y value (2 columns).
        par[1]=n;  // npts (n rows).

        double *X = new double [n], *Y = new double [n];
        size_t i=0;
        for (auto it=XBegin;it!=XEnd;++it) X[i++]=*it;
        i=0;
        for (auto it=YBegin;it!=YEnd;++it) Y[i++]=*it;

        GMT_VECTOR *vec=(GMT_VECTOR *)GMT_Create_Data(API,GMT_IS_VECTOR,GMT_IS_PLP,0,par,NULL,NULL,0,-1,NULL);
        GMT_Put_Vector(API,vec,0,GMT_DOUBLE,X);
        GMT_Put_Vector(API,vec,1,GMT_DOUBLE,Y);

        char filename[20];

        GMT_Open_VirtualFile(API,GMT_IS_VECTOR,GMT_IS_PLP,GMT_IN,vec,filename);
        char *command=strdup(("-<"+std::string(filename)+" "+cmd+" ->>"+outfile).c_str());
        GMT_Call_Module(API,"psxy",GMT_MODULE_CMD,command);
        delete [] command;

        // free spaces (X,Y seem to be deleted by GMT_Destroy_Session? very confusing).
        GMT_Destroy_Data(API,vec);
        GMT_Destroy_Session(API);

        return;
    }

    template<class T>
    void psxy(const std::string &outfile, std::vector<T> X, std::vector<T> Y, const std::string &cmd){
        GMT::psxy(outfile,X.begin(),X.end(),Y.begin(),Y.end(),cmd);
        return;
    }

    void beginplot(const std::string &outfile){
        GMT::psxy(outfile,std::vector<double> (),std::vector<double> (),"-JX1i/1i -R-1/1/-1/1 -Xf0i -Yf0i -P -K");
        return;
    }
    void sealplot(const std::string &outfile){
        GMT::psxy(outfile,std::vector<double> (),std::vector<double> (),"-J -R -O");
        return;
    }
    void movereference(const std::string &outfile, const std::string &cmd){
        GMT::psxy(outfile,std::vector<double> (),std::vector<double> (),"-J -R -O -K "+cmd);
        return;
    }
}

#endif
