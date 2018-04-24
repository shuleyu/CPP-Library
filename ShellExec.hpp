#ifndef ASU_SHELLEXEC
#define ASU_SHELLEXEC

#include<string>
#include<array>
#include<memory>

/*************************************************
 * This C++ template returns the stdout from the
 * execution of given shell command.
 *
 * input(s):
 * const T &cmd  ----  Input command. Most time T should be a C-string.
 *
 * return(s):
 * string ans  ----  stdout from the Shell command.
 *
 * Shule Yu
 * Jan 17 2018
 *
 * Key words: Shell, standard output.
*************************************************/

template <class T>
std::string ShellExec(const T &cmd) {
    std::array<char, 128> buffer;
    std::string ans;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
        ans += buffer.data();
    }

    return ans;
}

#endif
