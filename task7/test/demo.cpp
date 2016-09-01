#define N 1000
#define IO "iostream"
#include IO
#include <string.h>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "lazy_string.h"
//#include "bar.cpp"

using namespace std;

std::vector<lazy_string> v;

void f() {
	for(int i = 0; i < 100; ++i) {
		lazy_string tmp("a");
		v.push_back(tmp);	
	}
}

int main() {
	v.reserve(1000);
	thread t1(f);
	thread t2(f);
	t1.join();
	t2.join();
	return 0;
}