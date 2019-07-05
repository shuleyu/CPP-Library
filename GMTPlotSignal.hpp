#ifndef ASU_GMTPLOTSIGNAL
#define ASU_GMTPLOTSIGNAL

#include<GMT.hpp>
#include<SACSignals.hpp>
#include<EvenSampledSignal.hpp>
#include<DigitalSignal.hpp>

/*************************************************
 * Add support for signal plotting.
 *
 * Shule Yu
 * Aug 13 2018
 *
 * Key words: gmt plotting c++, signal
*************************************************/

namespace GMT {

    void psxy(const std::string &outfile,
              const DigitalSignal &item, const std::string &cmd){
        if (item.Size()==0) return;
        psxy(outfile,item.GetTime(),item.GetAmp(),cmd);
        return;
    }

    void psxy(const std::string &outfile,
              const SACSignals &item, const std::size_t index,
              const std::string &cmd){
        if (item.Size()==0 || item.Size()<=index) return;
        psxy(outfile,item.GetData()[index],cmd);
        return;
    }
}

#endif
