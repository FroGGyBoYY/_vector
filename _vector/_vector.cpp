#include<iostream>
#include<vector>
#include<iterator>

//-------------------------------
template <typename Iterator>
void my_advance(Iterator& iter, int n) {
	if constexpr (std::is_same<typename std::iterator_traits<Iterator>::iterator_category, typename std::random_access_iterator_tag>::value) {
		iter += n;
	}
	else {
		if (n < 0) { for (int i = 0; i < n; ++i, --iter); }
		else { for (int i = 0; i < n; ++i, ++iter); }
	}
}

template<typename Iterator>
size_t my_distance(Iterator& first_it, const Iterator& second_it) {
	if constexpr (std::is_same<typename std::iterator_traits<Iterator>::iterator_category, typename std::random_access_iterator_tag>::value) {
		return second_it - first_it;
	}
	else {
		size_t result = 0;
		for (; first_it != second_it; ++result, ++first_it);
		return result;
	}
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

//OUTPUT iterartors
template<typename Container>
class Back_Insert_Iterator {
private:
	Container& container;
public:
	explicit Back_Insert_Iterator(Container& _container) :container(_container) {}
	Back_Insert_Iterator<Container>& operator++() noexcept {
		return *this;
	}
	Back_Insert_Iterator<Container> operator++(int) noexcept {
		Back_Insert_Iterator<Container> copy = *this;
		return copy;
	}
	Back_Insert_Iterator<Container>& operator*() noexcept {
		return *this;
	}
	Back_Insert_Iterator<Container>& operator=(const typename Container::value_type& item) {
		container.push_back(item);
		return *this;
	}
	Back_Insert_Iterator<Container>& operator=(const typename Container::value_type&& item) {
		container.push_back(std::move(item));
		return *this;
	}
	Back_Insert_Iterator<Container>& operator=(const Back_Insert_Iterator<Container>& b_iter) {
		container = b_iter.container;
		return *this;
	}
};
template<typename Container>
Back_Insert_Iterator<Container> Back_Inserter(Container& container) {
	return Back_Insert_Iterator<Container>(container);
}


template <typename T>
class _vector {
private:
	size_t v_size;
	size_t v_cap;
	T* arr;
public:
	_vector() :v_size(0),v_cap(0), arr(nullptr) {}
	_vector(size_t size) :v_size(size), v_cap(size), arr(reinterpret_cast<T*>(new uint8_t[v_cap * sizeof(T)])) {
		for (size_t i = 0; i < v_size; ++i) {
			arr[i] = T();
		}
	}
	_vector(const std::initializer_list<T>& lst) :v_size(lst.size()),v_cap(lst.size()), 
													arr(reinterpret_cast<T*>(new uint8_t[v_cap*sizeof(T)])) {
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
	Iterator end() const noexcept { return (arr + v_size); }
	Const_Iterator cbegin() const noexcept { return arr; }
	Const_Iterator cend() const noexcept { return (arr + v_size); }

	using Reverse_Iterator = _Reverse_Iterator<Iterator>;
	using Const_Reverse_Iterator = _Reverse_Iterator<Const_Iterator>;

	Reverse_Iterator rbegin() const noexcept { return Reverse_Iterator((arr + v_size - 1u)); }
	Reverse_Iterator rend() const noexcept { return Reverse_Iterator(std::prev(arr, 1u)); }

	size_t size() const {
		return v_size;
	}
	size_t v_capasity() {
		return v_cap;
	}

	void resize(size_t n, const T& value) {
		if (n > v_cap)reserve(n);
		if (n < v_size) {
			for (size_t i = n; i < v_size; ++i) {
				arr[i]->~T();
			}
		}
		else {
			for (size_t i = v_size; i < n; ++i) {
				new(arr + i)T(value);
			}
		}
	}

	void reserve(size_t n) {
		if (n <= v_cap)return;

		T* newarr = reinterpret_cast<T*>(new uint8_t[n * sizeof(T)]);
		size_t i = 0;
		try {
			std::uninitialized_copy(arr, arr + v_size, newarr);
		}
		catch (...) {
			delete[]	reinterpret_cast<uint8_t*>(newarr);
			throw;
		}

		for (size_t i = 0; i < v_size; ++i) {
			(arr + i)->~T();
		}
		delete[]	reinterpret_cast<uint8_t*>(arr);
		arr = newarr;
	}

	void push_back(const T& value) {
		if (v_size == v_cap)resize(2 * v_cap);
		new(arr + v_size)T(value);
		++v_size;
	}
	void pop_back() {
		(arr + v_size - 1u)->~T();
		--v_size;
	}

	T& operator[](const size_t n) {
		return *(arr + n);
	}
	const T& operator[](const size_t n)const {
		return *(arr + n);
	}

	~_vector() { 
		for (size_t i = 0; i < v_size; ++i) {
			(arr + i)->~T();
		}
		delete[]arr; 
	}
};

template <>
class _vector<bool> {

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