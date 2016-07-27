#include "allocator.h"
#define DEBUG 1
// #undef DEBUG

#include <string>

const size_t SZ = sizeof(size_t); //size of the size cell prepended to each block

my_alloc::allocator::allocator(void* ptr, size_t size) {

	if(!ptr || size <= SZ) {
		std::cout << "Failed to allocate" << std::endl;
	}	
	else {
		sz_unit = size;
		ptr_unit = ptr;
		empty.insert((char*) ptr_unit + SZ);
		empty_sz.insert(make_pair(sz_unit - SZ, (char*) ptr_unit + SZ));
		*((size_t*) ptr_unit) = sz_unit - SZ;
	}
} 

void* my_alloc::allocator::alloc(size_t size_to_alloc) {
	std::multimap<size_t, void*>::iterator iter;
	
	iter = empty_sz.lower_bound(size_to_alloc);
	if(iter == empty_sz.end()) {
		std::cout << "null" << std::endl;
		return nullptr;
	}
	void* alloc_ptr = iter->second;
	*((size_t*) alloc_ptr - 1) = size_to_alloc;
	size_t sz = iter->first;
	empty_sz.erase(iter);
	set<void*>::iterator it = empty.find(alloc_ptr);
	empty.erase(it);
	if(size_to_alloc + SZ < sz) {
		empty_sz.insert(make_pair(sz - size_to_alloc - SZ, 
			(char *) alloc_ptr + size_to_alloc + SZ));
		empty.insert((char*) alloc_ptr + size_to_alloc + SZ);
		*((size_t*) ((char *) alloc_ptr + size_to_alloc)) = sz - size_to_alloc - SZ;
	}	
	return alloc_ptr;
}

void my_alloc::allocator::free(void* ptr) {
	
	if(!ptr)
		return;
	cout << __func__ << ' ' << ptr << endl;
	size_t sz = *((size_t *) ptr - 1);
	size_t old_sz = sz;
	void* old_ptr = ptr;
	set<void*>::iterator it;
	multimap<size_t, void*>::iterator it_mm;
	//merge with right empty neighbour
	it = empty.find((char *) ptr + sz + SZ);
	multimap<size_t, void*>::iterator next_it;
	if(it != empty.end()) {
		size_t sz_right = *((size_t *) ((char*) ptr + sz));
		sz += sz_right + SZ;
		
		empty.erase(it);
		
		printf("sz_right %lld\n", sz_right);
		fflush(stdout);

		pair<multimap<size_t, void*>::iterator, multimap<size_t, void*>::iterator> res =
		 empty_sz.equal_range(sz_right);
		 		
		printf("%i %i\n", res.first == empty_sz.end(), res.second == empty_sz.end());
		fflush(stdout);
		it_mm = res.first;
		while(it_mm != res.second) {
			next_it = next(it_mm, 1);
			if((char *) it_mm->second == (char*) ptr + old_sz + SZ) 
				empty_sz.erase(it_mm);
			it_mm = next_it;
		}	
	} 
	//left
	std::map<void*, void*>::iterator it_m = left_empty_neigh.find(ptr);
	if(it_m != left_empty_neigh.end()) {
		ptr = it_m->second;
		size_t sz_left = *((size_t*) ptr - 1);
		sz += sz_left + SZ;
		pair<multimap<size_t, void*>::iterator, multimap<size_t, void*>::iterator> res =
		 empty_sz.equal_range(sz_left);
 		

		it_mm = res.first;
		while(it_mm != res.second) {
			next_it = next(it_mm, 1);
			if(it_mm->second == ptr) 
				empty_sz.erase(it_mm);
			it_mm = next_it;
		}

	}

	empty.insert(ptr);
	empty_sz.insert(make_pair(sz, ptr));
	void* right_neigh = (char*) ptr + sz + SZ;
	*((size_t*) ptr - 1) = sz;
	if(((char*) right_neigh - (char*) ptr_unit >= sz_unit) ||
		(empty.find(right_neigh) != empty.end()))
		return;
	

	it_m = left_empty_neigh.find(right_neigh);
	if(it_m != left_empty_neigh.end())
		left_empty_neigh.erase(it_m);
	// left_empty_neigh.erase(right_neigh);
	left_empty_neigh.insert(make_pair(right_neigh, ptr));
	printf("%lld\n", left_empty_neigh.size());
}

void my_alloc::allocator::out() {

	printf("%s\n", __func__);
	std::multimap<size_t, void*>::iterator iter;
	set<void*>::iterator it;
	map<void*, void*>::iterator i;
	printf("%s\n", "empty_sz and sizes");
	for(iter = empty_sz.begin(); iter != empty_sz.end(); ++iter){
		printf("%lld %p\n", iter->first, iter->second);
		size_t* ptr = (size_t *) iter->second;
		printf("%lld\n", *(ptr - 1));
	}
	printf("%s\n", "empty");
	for(it = empty.begin(); it != empty.end(); ++it)
		printf("%p\n", *it);
	printf("%s\n", "left_empty_neigh");
	for(i = left_empty_neigh.begin(); i != left_empty_neigh.end(); ++i)
		printf("%lld %lld\n", (char*) i->first - (char*) ptr_unit,
		 (char*) i->second - (char*) ptr_unit);
}