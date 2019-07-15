#ifndef ASU_STREAMADDDELIMITER
#define ASU_STREAMADDDELIMITER

#include<istream>
#include<locale>
#include<string>

/*************************************************
 * This C++ function add customized delimiter to
 * istreams such as cin, fpin.
 *
 * input(s):
 * istream &is      ----  Input stream.
 * const string &s  ----  customized delimiters (no order)
 *
 * Shule Yu
 * Jul 15 2019
 *
 * Key words: delimiter, istream
*************************************************/

struct cuStomIzedSpAce: std::ctype<char> {
    cuStomIzedSpAce(const std::string &s) : std::ctype<char>(get_table(s)) {}
    static mask const* get_table(const std::string &s){
        static mask rc[table_size];
        for (auto c: s) rc[int(c)] = std::ctype_base::space;
        return &rc[0];
    }
};

void StreamAddDelimiter(std::istream &is,const std::string &s=""){
    is.imbue(std::locale(is.getloc(), new cuStomIzedSpAce (s)));
    return;
}

void StreamDefaultDelimiter(std::istream &is){
    is.imbue(std::locale());
    return;
}


#endif
