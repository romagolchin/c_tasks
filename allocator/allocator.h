#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <cstddef>
#include <vector>
using namespace std;

namespace my_alloc {

	class allocator {
		void* ptr_unit;
		size_t sz_unit; // size of block
		std::set<void*> empty;
		std::multimap<size_t, void*> empty_sz;
		std::map<void*, void*> left_empty_neigh;
	public:
		allocator(void* ptr, size_t size);
		void* alloc(size_t size);
		void free(void* ptr);
		void out();
	};
}
