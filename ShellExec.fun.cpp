#include<string>
#include<array>
#include<memory>

#include<ASU_tools.hpp>

/*************************************************
 * This C++ function returns the stdout from the
 * execution of given shell command.
 *
 * input(s):
 * const string &cmd  ----  Input command.
 *
 * return(s):
 * string ans  ----  stdout from the Shell command.
 *
 * Shule Yu
 * Jan 17 2018
 *
 * Key words: Shell, standard output.
*************************************************/

std::string ShellExec(const std::string &cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
        result += buffer.data();
    }
    return result;
}
