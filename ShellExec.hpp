#ifndef ASU_SHELLEXEC
#define ASU_SHELLEXEC

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
 * string ans  ----  stdout from the Shell command.
 *
 * Shule Yu
 * Jan 17 2018
 *
 * Key words: Shell, standard output.
 * Notice:    Most command will have a trailing '\n' character
 *            in the return string.
*************************************************/

std::string ShellExec(const std::string &cmd) {
    std::array<char, 128> buffer;
    std::string ans;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            ans+=buffer.data();
    }

    // If all characters are white spaces, return "".
    for (const char &c:ans)
        if (!std::isspace(c))
            return ans;

    return "";
}

#endif
