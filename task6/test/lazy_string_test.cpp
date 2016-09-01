#include "lazy_string.h"
#include <stdlib.h>
#include <vector>
#include <thread>
#include <mutex>

int main(int argc, char const *argv[]) {
	try {
		std::string str = "abacaba";
		std::string astr = "e";
		lazy_string s(str);
		
		lazy_string t(astr);
		t = s;
		s[4] = 'o';
		std::cout << t << ' ' << s << std::endl;
		// lazy_char lc = s[1];
		t[3] = 'd';
		lazy_string r = t.substr(1, 4);
		std::cout << r << std::endl;
		std::cerr << (std::string) r << std::endl;
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}