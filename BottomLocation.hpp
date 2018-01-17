#ifndef ASU_BOTTOMLOCATION
#define ASU_BOTTOMLOCATION

#include<string>

/**************************************************
 * This C++ template use TauP toolkit to return the
 * turning depth of given parameters.
 *
 * Using TauP toolkit. Model is PREM.
 *
 * const T1     &evtlon  ----  event lon
 * const T2     &evtlat  ----  event lat
 * const T3     &EVDP    ----  event depth
 * const T4     &stalon  ----  station lon
 * const T5     &stalat  ----  station lat
 * const string &Phase   ----  seismic phase
 * T6           &Depth   ----  Bottom depth
 *
 * Shule Yu
 * Dec 28 2017
 *
 * Key words: bottom location.
**************************************************/

template<class T1, class T2, class T3, class T4, class T5, class T6>
std::pair<double,double> BottomLocation(const T1 &evtlon, const T2 &evtlat, const T3 &EVDP,
										const T4 &stalon, const T5 &stalat,
                                        const std::string &Phase,T6 &Depth){

	std::string command="taup_path -mod prem -h "+std::to_string(EVDP)+" -ph "+Phase
	                    +" -evt "+std::to_string(evtlat)+" "+std::to_string(evtlon)+
	                    +" -sta "+std::to_string(stalat)+" "+std::to_string(stalon)+
	                    +" -o stdout | grep -v '>' | sort -g -k2 |  head -n 1 | awk '{print $2,$3,$4}'";

	std::string res=ShellExec(command);
	if (res.empty()) {
		std::cerr <<  __func__ << "; Warning: No such phase for given parameters ..." << std::endl;
		Depth=0;
		return {};
	}

	// Parse the string.
	size_t pb=0,p=0;
	while (std::isspace(res[p])) ++p;
	pb=p;
	while (!std::isspace(res[p])) ++p;
	Depth=6371.0-stod(res.substr(pb,p));

	while (std::isspace(res[p])) ++p;
	pb=p;
	while (!std::isspace(res[p])) ++p;
	double plat=stod(res.substr(pb,p));

	while (std::isspace(res[p])) ++p;
	pb=p;
	while (p<res.size() && !std::isspace(res[p])) ++p;
	double plon=stod(res.substr(pb,p));

    return {plon,plat};
}

#endif
