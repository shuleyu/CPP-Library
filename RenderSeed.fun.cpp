#include<chrono>
#include<random>

#include<ASU_tools.hpp>

unsigned RenderSeed(){
	return std::chrono::system_clock::now().time_since_epoch().count();
}
