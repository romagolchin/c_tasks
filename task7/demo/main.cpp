#include "lazy_string.h"
#include <vector>
#include <stdlib.h>
#include <sstream>

std::string rand_string() {
	std::string res;
	int len = rand() % 10 + 1;
	for(int j = 0; j < len; ++j)
		res += ('a' + rand() % 26);
	return res;
}

void fun(std::stringstream& is, std::ostream& os) {
	srand(time(0));
	std::vector<lazy_string> lss;
	// std::string str;
	// is >> str;
	// std::cout << str << std::endl;
	std::cout << "gcount " << is.gcount() << std::endl;
	int cnt = 0;
	while(lss.size() <= 50) {
		//0-ins, 1-r/w
		int type = rand() % 2;
		std::cout << type << ' ' << lss.size() << std::endl;
		if(!type || lss.empty()) {
			std::string t = rand_string();
			lss.emplace_back(t);
		}
		else if(type) {
			int rw_type = rand() % 5;
			std::cerr << rw_type << std::endl;
			int index = rand() % (lss.size());
			switch (rw_type) {
				case 0: {
					std::cout <<"size: " << lss[index].size() << std::endl;
					break;
				}
				case 1: {
					int ls_index = rand() % lss[index].length();
					try {
						char res = static_cast<char>(lss[index].at(ls_index));
						std::cout <<"at " << ls_index << ":" << res << std::endl;
					}
					catch(const std::out_of_range& oor) {
						std::cout <<oor.what() << std::endl;
					}
					break;
				}
				case 2: {
					int ls_index = rand() % 15;
					int ls_len = rand() % 15;
					try {
						lazy_string res = (lss[index].substr(ls_index, ls_len));
						std::cout <<"substr " << ls_index << " " << ls_len << " " << res << std::endl;
					}
					catch(const std::out_of_range& oor) {
						std::cout <<oor.what() << std::endl;
					}
					break;
				}
				case 3: {
					++cnt;
					std::cout << "cnt=" << cnt << std::endl;
					lazy_string prev = lss[index];
					// if(is.gcount()) {
					// 	std::cout << "YES" << std::endl;
					// }
					//std::cout << "str=" << is.str() << std::endl;
					is >> lss[index]; 
					std::cout << "gcount " << is.gcount() << std::endl;
					std::cout << "changed by >> :" << prev << "->" << lss[index] << std::endl; 
					break;
				}
				case 4: {
					std::cout << "<< :" << lss[index] << std::endl;
					break;
				}
			}
		}
	}
	std::cout << cnt << std::endl;
}

int main(int argc, char const *argv[]) {
	srand(time(0));
	std::string concat_all;
	for(int i = 0; i < 50; ++i) {
		concat_all += rand_string() + " ";
	}
	std::stringstream is1(concat_all);
	std::cout << concat_all << std::endl;
	// std::string str;
	// is1 >> str;
	// std::cout << str << std::endl;
	// std::cout << "is1.gcount() " << is1.gcount() << std::endl;
	std::stringstream os1;
	std::thread t1(fun, std::ref(is1), std::ref(os1));
	// std::stringstream is2;
	// for(int i = 0; i < 100; ++i)
	// 	is2 << rand_string() << ' ';
	// std::stringstream os2;
	// std::thread t2(fun, std::ref(is2), std::ref(os2));
	t1.join();
	// t2.join();
	char c;
	std::cout << "output of t1:" << std::endl;
	while(os1.get(c))
		std::cout << c;
	// std::cout << "output of t2:" << std::endl;
	// while(os1 >> c)
	// 	std::cout << c;
	return 0;
}