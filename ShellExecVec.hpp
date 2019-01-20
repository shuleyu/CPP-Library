#ifndef ASU_SHELLEXECVEC
#define ASU_SHELLEXECVEC

#include<string>
#include<array>
#include<memory>
#include<vector>

/*************************************************
 * This C++ template returns the stdout from the
 * execution of given shell command.
 *
 * input(s):
 * const T &cmd  ----  Input command. Most time T should be a string.
 *
 * return(s):
 * vector<string> ans  ----  stdout from the Shell command as vector of strings.
 *
 * Shule Yu
 * Jan 17 2018
 *
 * Key words: Shell, standard output.
*************************************************/

std::vector<std::string> ShellExecVec(const std::string &cmd) {
    std::vector<std::string> ans;
    std::array<char,128> buffer;

    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            ans.push_back(std::string(buffer.data()));
    }
    return ans;
}

#endif
