#include "allocator.cpp"

int main() {
	const int N = 1024;
	int* ptr = (int*) malloc(sizeof(int) * N);
	my_alloc::allocator A(ptr, sizeof(int) * N);
	A.out();
	const int M = 64; const int Len = 64;
	int i;
	std::vector<int*> alloc_ptrs;
	for(i = 0; i < M; ++i) {
		int* p = (int*) A.alloc(Len);
		if(!p) {
			std::cout << "null" << std::endl;
			break;
		}
		alloc_ptrs.push_back(p);
	}
	A.out();
	fflush(stdout);
	for(i = 0; i < (int) alloc_ptrs.size(); i += 2)
		A.free(alloc_ptrs[i]);
	A.free(alloc_ptrs[M - 1]);
	A.out();
	return 0;
}
