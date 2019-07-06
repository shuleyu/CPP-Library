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
 * const string &cmd            ----  Input command. Most time T should be a string.
 * const bool  &removeTrailing  ----  (Optional, default is false)
 *                                    true:  remove the trailing '\n'.
 *                                    false: don't remove the trailing '\n'.
 *
 * return(s):
 * string ans  ----  stdout from the Shell command.
 *
 * Shule Yu
 * Jan 17 2018
 *
 * Key words: Shell, standard output.
*************************************************/

std::string ShellExec(const std::string &cmd, const bool& removeTrailing=false) {
    std::array<char, 128> buffer;
    std::string ans;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("ShellExec popen() failed! Command is: "+cmd);
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            ans+=buffer.data();
    }

    if (removeTrailing) {
        while (!ans.empty() && ans.back()=='\n') ans.pop_back();
    }

    // If all characters are white spaces, return "".
    for (const char &c:ans)
        if (!std::isspace(c))
            return ans;

    return "";
}

#endif
