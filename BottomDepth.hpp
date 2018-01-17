#ifndef ASU_BOTTOMDEPTH
#define ASU_BOTTOMDEPTH

#include<string>

/**************************************************
 * This C++ template use TauP toolkit to return the
 * turning depth of given parameters.
 *
 * Using TauP toolkit. Model is PREM.
 *
 * const T1     &Dist   ----  gcp distance.
 * const T2     &EVDP   ----  source depth.
 * const string &Phase  ----  seismic phase.
 * T3           &Depth  ----  Bottom depth.
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: bottom depth.
**************************************************/

template<class T1, class T2, class T3>
void BottomDepth(const T1 &Dist,const T2 &EVDP,const std::string &Phase,T3 &Depth){

	std::string command="taup_path -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
	                    +" -evt 0 0 -sta 0 "+std::to_string(Dist)
	                    +" -o stdout | grep -v '>' | sort -g -k2 |  head -n 1 | awk '{print $2}'";

	std::string res=ShellExec(command);

	if (res.empty()) {
		std::cerr <<  __func__ << "; Warning: No such phase for given parameters ..." << std::endl; 
		Depth=0;
		return;
	}

	Depth=6371.0-stod(res);

    return;
}

#endif
