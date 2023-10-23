#include <iostream>
#include "_allocator_.h"
/*
template <typename T>
class Allocator {
public:
	void* allocate(size_t n) {
		return ::operator new(n);
	}
	void deallocate(T* ptr, size_t) {
		::operator delete(ptr);
	}

	template <typename... Args>
	void construct(T* ptr, const Args&... args) {
		new(ptr) T(args...);
	}

	void destroy(T* ptr) {
		ptr->~T();
	}
};
*/