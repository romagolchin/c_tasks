#include "lazy_string.h"

class X {
	int a;
	std::shared_ptr<rw_lock> lock;
public:
	int get_a();
	X(int a) : a(a), lock(std::make_shared<rw_lock>())
	{}
};

int X::get_a() {
	std::cout << static_cast<bool>(lock) << std::endl; 
	lock->read_lock();
	int res = a;
	lock->read_unlock();
	return res;
}

int main() {
	X x(42);
	std::cout << x.get_a();
	return 0;
}