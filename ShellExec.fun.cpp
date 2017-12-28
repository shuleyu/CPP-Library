#include<string>
#include<array>
#include<memory>

#include<ASU_tools.hpp>

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
