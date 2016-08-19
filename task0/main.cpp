#include "lazy_string.h"
#include <stdlib.h>
#include <vector>

void ls_write(lazy_string& ls) {
	ls[0] = 'o';
}

void ls_read(lazy_string& ls) {
	std::cout << ls << std::endl;
}

void fun(lazy_string ls) {
	std::cout << ls.length() << std::endl;
	std::cout << ls << std::endl;
}

void gun(std::string s) {
	std::cout << s.length() << std::endl;
	std::cout << s << std::endl;
}

int main(int argc, char const *argv[]) {
	// try {
		std::string str = "abacaba";
		std::string astr = "e";
		lazy_string s(str);
		// std::thread t1(ls_write, std::ref(s));
		// std::thread t2(ls_read, std::ref(s));
		// t1.join();
		// t2.join();
		// freopen("main.out", "w", stdout);
		// srand(time(NULL));
		// std::vector<std::thread> ths;
		// for(int i = 0; i < 1; ++i) {
		// 	int len = rand() % 10 + 1;
		// 	std::string t;
		// 	for(int j = 0; j < len; ++j)
		// 		t += std::string(1, 'a' + rand() % 26);
		// 	std::cerr << t << std::endl;
		// 	lazy_string ls_t(t);
		// 	std::cout << ls_t.length() << std::endl;
		// 	// std::thread&& th = std::thread(fun, ls_t);
		// 	// ths.push_back(th);
		// 	gun(ls_t);
		// 	std::thread thread(gun, t);
		// 	ths.push_back(std::move(thread));
		// }
		// for(auto& th : ths)
		// 	th.join();
		s[4] = 'o';
		lazy_string t(astr);
		std::cout << t << ' ' << s << std::endl; 
		t[3] = 'd';
		lazy_string r = t.substr(0, 4);
		std::cout << r << std::endl;
		std::cerr << (std::string) r << std::endl;
		lazy_string q("abcdef");
		lazy_string p = q.substr(1, 4);
		lazy_string z = p.substr(1, 2);
		p[1] = 'p';
		std::cout << q << ' ' << p << ' ' << z << std::endl;
	// }
	// catch(const std::exception& e) {
	// 	std::cout << e.what() << std::endl;
	// }
	return 0;
}