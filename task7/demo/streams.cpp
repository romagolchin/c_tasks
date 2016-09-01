#include <vector>
#include <stdlib.h>
#include <sstream>
#include <iostream>

std::string rand_string() {
	std::string res;
	int len = rand() % 10 + 1;
	for(int j = 0; j < len; ++j)
		res += ('a' + rand() % 26);
	return res;
}


int main(int argc, char const *argv[])
{
	std::stringstream ss;
	std::string concat;
	for(int i = 0; i < 100; ++i) {
		std::string str = rand_string();
		ss << str << ' ';
		concat += str + ' ';
	}
	std::stringstream ss1(concat);
	std::cout << (ss1.str() == ss.str());
	char c;
	std::cout << "one" << std::endl;
	std::istream ss2 = static_cast<std::istream>(ss);
	while(ss2.get(c))
		std::cout << c;
	std::cout << "another" << std::endl;
	while(ss1.get(c)) 
		std::cout << c;
	return 0;
}