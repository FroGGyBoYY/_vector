#include<iostream>
#include<vector>
#include<iterator>

template<typename Iterator, typename IteratorCategory>
void my_advance_helper(Iterator& iter, int n, IteratorCategory) {
	for (int i = 0; i < n; ++i, ++iter);
}
template<typename Iterator>
void my_advance_helper(Iterator& iter, int n, std::random_access_iterator_tag) {
	iter += n;
}

template<typename Iterator>
void my_advance(Iterator& iter, int n) {
	my_advance_helper(iter, n, std::iterator_traits<Iterator>::iterator_category());
}



//	4 Const iterator
template<typename Iterator>
class _Const_Iterator {};



//	5 Reverse iterators
template<typename Iterator>
class _Reverse_Iterator {
private:
	Iterator iter;
public:
	_Reverse_Iterator(const Iterator& iter) :iter(iter) {}
	_Reverse_Iterator<Iterator>& operator++() {
		--iter;
		return *this;
	}
	_Reverse_Iterator<Iterator> operator++(int) {
		_Reverse_Iterator<Iterator> copy = *this;
		--* this;
		return copy;
	}
	_Reverse_Iterator<Iterator>& operator--() {
		++iter;
		return *this;
	}
	_Reverse_Iterator<Iterator> operator--(int) {
		_Reverse_Iterator<Iterator> copy = *this;
		++* this;
		return copy;
	}
	auto operator*() -> decltype(*iter) {
		return *iter;
	}
	bool operator==(const _Reverse_Iterator<Iterator>& it) {
		return (iter == it.iter);
	}
	bool operator!=(const _Reverse_Iterator<Iterator>& it) {
		return !(*this == it);
	}
	Iterator base() const { return iter; }
};

template <typename T>
class _vector {
private:
	size_t size;
	T* arr;
public:
	_vector() :size(0), arr(nullptr) {}
	_vector(size_t size) :size(size), arr(new T[size]) {
		for (int i = 0; i < size; ++i) {
			arr[i] = T();
		}
	}
	_vector(const std::initializer_list<T>& lst) :size(lst.size()), arr(new T[size]) {
		std::copy(lst.begin(), lst.end(), arr);
	}

	template <bool IsConst>
	struct Common_Iterator {
	private:
		std::conditional_t<IsConst, const T*, T*> ptr_elem;
	public:
		Common_Iterator() :ptr_elem(nullptr) {}
		Common_Iterator(T* ptr) :ptr_elem(ptr) {}
		Common_Iterator& operator++() {
			++ptr_elem;
			return *this;
		}
		Common_Iterator operator++(int) {
			Common_Iterator copy = *this;
			++* this;
			return copy;
		}
		Common_Iterator& operator--() {
			--ptr_elem;
			return *this;
		}
		Common_Iterator operator--(int) {
			Common_Iterator copy = *this;
			++* this;
			return copy;
		}
		bool operator ==(const Common_Iterator& it) const noexcept {
			return ptr_elem == it.ptr_elem;
		}
		bool operator !=(const Common_Iterator& it)const noexcept {
			return !(*this == it);
		}
		std::conditional_t<IsConst, const T&, T&> operator*() { return *ptr_elem; }
		std::conditional_t<IsConst, const T*, T*> operator->() { return ptr_elem; }
	};

	using Iterator = Common_Iterator<false>;
	using Const_Iterator = Common_Iterator<true>;

	Iterator begin() const noexcept { return arr; }
	Iterator end() const noexcept { return (arr + size); }
	Const_Iterator cbegin() const noexcept { return arr; }
	Const_Iterator cend() const noexcept { return (arr + size); }

	using Reverse_Iterator = _Reverse_Iterator<Iterator>;
	using Const_Reverse_Iterator = _Reverse_Iterator<Const_Iterator>;

	Reverse_Iterator rbegin() const noexcept { return Reverse_Iterator((arr + size - 1)); }
	Reverse_Iterator rend() const noexcept { return Reverse_Iterator(std::prev(arr, 1)); }



	~_vector() { delete[]arr; }
};

int main() {
	
	std::vector<int>v = { 1,2,3,4,5 };
	for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
	for (auto it : v) {
		std::cout << it << ' ';
	}
	

	// 3.2 Trying and testing my 
	// container with iterators

	std::cout << "\n\n\t Try show my_vec with Iterators\n" << std::endl;
	_vector<int> my_vec{ 1,2,3,4,5,6,7,8,9,10,11,12 };
	for (_vector<int>::Iterator it = my_vec.begin(); it != my_vec.end(); ++it) {
		std::cout << *it << ' ';
	}

	std::cout << "\n\n\t Try show my_vec with Reverse_Iterators\n" << std::endl;
	for (_vector<int>::Reverse_Iterator r_it = my_vec.rbegin(); r_it != my_vec.rend(); ++r_it) {
		std::cout << *r_it << ' ';
	}
	return 0;
}