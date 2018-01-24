#ifndef ASU_FINDRAYP
#define ASU_FINDRAYP

#include<string>

/**************************************************
 * This C++ template use TauP toolkit to return the
 * ray path of given source-receiver pair.
 *
 * Using TauP toolkit. Model is PREM.
 *
 * const T1     &EVLO   ----  event longitude.
 * const T2     &EVLA   ----  event latitude.
 * const T3     &EVDP   ----  event depth.
 * const T4     &STLO   ----  station longitude.
 * const T5     &STLA   ----  station latitude.
 * const string &Phase  ----  seismic phase name (TauP name).
 *
 * Shule Yu
 * Jan 24 2018
 *
 * Key words: ray parameter.
**************************************************/

template<class T1, class T2, class T3, class T4, class T5>
double FindRayP(const T1 &EVLO,const T2 &EVLA, const T3 &EVDP,
                const T4 &STLO,const T5 &STLA, const std::string &Phase){

	std::string command="taup_time -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
	                    +" -evt " + std::to_string(EVLA) +" "+ std::to_string(EVLO)
	                    +" -sta " + std::to_string(STLA) +" "+ std::to_string(STLO)
	                    +" -rayp -o stdout";

	std::string res=ShellExec(command);

	if (res.empty()) {
		std::cerr <<  __func__ << "; Warning: No such phase for given parameters ..." << std::endl; 
		return -1;
	}

    return std::stod(res);
}

#endif
