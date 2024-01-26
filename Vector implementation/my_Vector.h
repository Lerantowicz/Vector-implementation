#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <memory>
#include <iterator>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <sys/sysinfo.h>
#endif


namespace my_Namespace{

template <typename Type, typename Allocator = std::allocator<Type>>
class my_VectorStorage
{
protected:

	using value_type			= Type;
	using const_value_type		= const Type;
	using pointer				= typename std::allocator_traits<Allocator>::pointer;
	using const_pointer			= typename std::allocator_traits<Allocator>::const_pointer;
	using reference				= value_type&;
	using const_reference		= const value_type&;
	using move_type				= Type&&;
	using size_type				= size_t;

protected:
	size_type m_capacity = 0;
	pointer m_data = nullptr;
	size_type m_size = 0;
	const size_type m_capacity_modifier = 2;
	const size_type m_min_capacity = 4;

	Allocator al;

private:

	/*template<typename Iter, typename Value, typename Modifier>
void fill(Iter begin, Iter end, Value&& value, Modifier func = [](auto&& value) {return value; })
{
	for (; begin != end; ++begin)
	{
		*begin = func(std::forward<Value>(value));
	}
}*/

	// delete later maybe current uses(0)
	void increase_capacity_if_too_small()
	{
		if (m_size == 0)
		{
			m_capacity = m_min_capacity;
		}
	}

	void throw_if_same(const my_VectorStorage<Type, Allocator>& other)
	{
		if (this == &other)
		{
			throw std::invalid_argument("Argument cannot be the same instance as the calling object.");
		}
	}
protected:

	my_VectorStorage();

	my_VectorStorage(size_type size);

	my_VectorStorage(size_type n_arg, const_reference value);

	my_VectorStorage(const my_VectorStorage<Type, Allocator>& other);
	 
	my_VectorStorage(my_VectorStorage<Type, Allocator>&& other);
	 
	my_VectorStorage(std::initializer_list<Type> list);
	 
	~my_VectorStorage();

	void swap(my_VectorStorage<Type, Allocator>& other) noexcept;

};

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage() : m_size(0), m_capacity(m_min_capacity)
{
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(const my_VectorStorage<Type, Allocator>& other) : m_size(other.m_size), m_capacity(other.m_capacity)
{
	throw_if_same(other);
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(my_VectorStorage<Type, Allocator>&& other)
{
	swap(other);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(size_type size) : m_size(size)
{
	m_capacity = size > m_min_capacity ? size * m_capacity_modifier : m_min_capacity;
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(size_type n_arg, const_reference value) : m_size(n_arg)
{
	m_capacity = n_arg > 4 ? n_arg * m_capacity_modifier : m_min_capacity;
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(std::initializer_list<Type> list) : m_size(list.size())
{
	m_capacity = list.size() > m_min_capacity ? list.size() * m_capacity_modifier : m_min_capacity;
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
void my_VectorStorage<Type, Allocator>::swap(my_VectorStorage<Type, Allocator>& other) noexcept
{
	throw_if_same(other);
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::~my_VectorStorage()
{
	al.deallocate(m_data, m_capacity);
}

template <typename Type, typename Allocator = std::allocator<Type>>
class my_Vector : public my_VectorStorage<Type, Allocator>
{
	using my_VectorStorage<Type, Allocator>::m_capacity;
	using my_VectorStorage<Type, Allocator>::m_capacity_modifier;
	using my_VectorStorage<Type, Allocator>::m_size;
	using my_VectorStorage<Type, Allocator>::m_data;
	using my_VectorStorage<Type, Allocator>::al;


public:

	using typename my_VectorStorage<Type, Allocator>::value_type;
	using typename my_VectorStorage<Type, Allocator>::const_value_type;
	using typename my_VectorStorage<Type, Allocator>::pointer;
	using typename my_VectorStorage<Type, Allocator>::const_pointer;
	using typename my_VectorStorage<Type, Allocator>::reference;
	using typename my_VectorStorage<Type, Allocator>::const_reference;
	using typename my_VectorStorage<Type, Allocator>::move_type;
	using typename my_VectorStorage<Type, Allocator>::size_type;
	using allocator_type = Allocator;
	using difference_type = std::ptrdiff_t;

	template<typename Iter_Type = typename my_VectorStorage<Type, Allocator>::value_type>
	class my_Iterator
	{
	public:

		using iterator_category = std::forward_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= Iter_Type;
		using pointer			= Iter_Type*;
		using reference			= Iter_Type&;
		using size_type			= size_t;
		my_Iterator(pointer ptr) : m_ptr(ptr)
		{

		}
		// it's awfull but it works
		template<typename U, typename = void,
			typename = std::enable_if_t<!std::is_const_v<Type>
			&& !std::is_const_v<Iter_Type>  
			&& std::is_const_v<U>>>
		my_Iterator(my_Iterator<U> other)
		{
			m_ptr = const_cast<pointer>(other.operator->());
		}
		template <typename U,
			typename = std::enable_if_t<!std::is_const<U>::value || std::is_const<Iter_Type>::value>>
		my_Iterator(my_Iterator<U> other)
		{
			m_ptr = other.operator->();
		}
		my_Iterator& operator++()
		{
			++m_ptr;
			return *this;
		}
		my_Iterator operator++(int)
		{
			my_Iterator temp = *this;
			++(*this);
			return temp;
		}
		my_Iterator& operator--()
		{
			--m_ptr;
			return *this;
		}
		my_Iterator operator--(int)
		{
			my_Iterator temp = *this;
			--(*this);
			return temp;
		}
		my_Iterator& operator=(const my_Iterator& other)
		{
			m_ptr = other.m_ptr;
			return *this;
		}
		reference operator[](size_type index)
		{
			return *(m_ptr + index);
		}
		pointer operator->()
		{
			return m_ptr;
		}
		reference operator*()
		{
			return *m_ptr;
		}
		bool operator==(const my_Iterator& other) const
		{
			return m_ptr == other.m_ptr;
		}
		bool operator!=(const my_Iterator& other) const
		{
			return m_ptr != other.m_ptr;
		}
		my_Iterator operator+(size_type index)
		{
			my_Iterator temp = *this;
			temp += index;
			return temp;
		}
		my_Iterator operator-(size_type index)
		{
			my_Iterator temp = *this;
			temp -= index;
			return temp;
		}
		my_Iterator& operator+=(size_type index)
		{
			m_ptr += index;
			return *this;
		}
		my_Iterator& operator-=(size_type index)
		{
			m_ptr -= index;
			return *this;
		}
		difference_type operator-(const my_Iterator& other) const
		{
			return m_ptr - other.m_ptr;
		}
	private:
		pointer m_ptr;
	};

	using iterator = my_Iterator<my_Vector<Type, Allocator>::value_type>;
	using const_iterator = my_Iterator<my_Vector<Type, Allocator>::const_value_type>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:

	

	my_Vector(); /* {}*/
	my_Vector(size_type size); /* {}*/
	my_Vector(size_type n_arg, const_reference value); /* {}*/
	my_Vector(const my_Vector<Type, Allocator>& other); /* {}*/
	my_Vector(my_Vector<Type, Allocator>&& other) noexcept; /*  {}*/
	my_Vector(std::initializer_list<Type> list); /* {}*/
	template<typename Iterator, typename = std::void_t<decltype(Iterator{}++), decltype(*Iterator{}) >>
	my_Vector(Iterator start, Iterator end);
	
	~my_Vector();

	void operator=(const my_Vector<Type, Allocator>& other);
	void operator=(my_Vector<Type, Allocator>&& other) noexcept;
	void operator=(std::initializer_list<Type> list);

	void assign(std::initializer_list<Type> list);
	void assign(size_type n_arg, const_reference value);
	// here check if iterator
	template<typename Iterator, typename = std::void_t<decltype(Iterator{}++), decltype(*Iterator{}) >>
	void assign(Iterator first, Iterator last);

	allocator_type get_allocator() const noexcept;

	reference at(size_type position);
	const_reference at(size_type position) const;

	reference operator[](size_type position);
	const_reference operator[](size_type position) const;

	reference front();
	const_reference front() const;

	reference back();
	const_reference back() const;

	pointer data() noexcept;
	const_pointer data() const;

	bool empty() const noexcept
	{
		return m_capacity == 0;
	}
	
	size_type size() const noexcept
	{
		return m_size;
	}

	size_type max_size() const noexcept; //	

	void swap(my_Vector<Type, Allocator>& other) noexcept;
	
	void reserve(size_type capacity_new);

	void push_back(const_reference value);
	void push_back(move_type value);

	void clear();

	iterator insert(const_iterator pos, const_reference value);
	iterator insert(const_iterator pos, move_type value);
	iterator insert(const_iterator pos, size_type count, const_reference value);

	// here check if pointer because initializer_list returns pointers and not iterators
	template<typename Iterator, typename = std::void_t<decltype(Iterator{}+1), decltype(*Iterator{}) >>
	iterator insert(const_iterator pos, Iterator first, Iterator last);

	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);

	//void insert(const_reference value, size_type place);

	iterator erase(const_iterator place);
	iterator erase(const_iterator first, const_iterator last);

	template <class... Args>
	void pretty_emplace(iterator iter, Args&&...args);

	template <class... Args>
	iterator emplace(iterator iter, Args&&...args);

	template <class... Args>
	iterator emplace_back(Args&&...args);

	void pop_back();

	void resize(size_type count);
	void resize(size_type count, const_reference value);

	void shrink_to_fit();

	size_type capacity() const
	{
		return m_capacity;
	}

	iterator begin() noexcept { return iterator(m_data);}
	iterator end() noexcept { return iterator(m_data + m_size);}

	/*const_iterator begin() const noexcept { return const_iterator(m_data); }
	const_iterator end() const noexcept { return const_iterator(m_data + m_size); }*/

	const_iterator cbegin() const noexcept { return const_iterator(m_data); }
	const_iterator cend() const noexcept { return const_iterator(m_data + m_size); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(m_data); }
	reverse_iterator rend() noexcept { return reverse_iterator(m_data + m_size); }

	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(m_data + m_size); }

	private:
		void reallocate_if_small(size_type size);

		void increase_capacity();

		void clear_old_memory();

};

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector() 
	: my_VectorStorage<Type, Allocator>()
{
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(size_type size) 
	: my_VectorStorage<Type, Allocator>(size)
{
	for (size_type i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], value_type());
	}
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(size_type n_arg, const_reference value) 
	: my_VectorStorage<Type, Allocator>(n_arg, value)
{
	for (size_type i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], value);
	}
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(const my_Vector<Type, Allocator>& other) 
	: my_VectorStorage<Type, Allocator>(other)
{
	std::uninitialized_copy(other.m_data, other.m_data + other.m_size, m_data);
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(my_Vector<Type, Allocator>&& other) noexcept 
	: my_VectorStorage<Type, Allocator>(std::move(other))
{
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(std::initializer_list<Type> list) 
	: my_VectorStorage<Type, Allocator>(list)
{
	int add = 0;
	for (auto i = list.begin(); i != list.end(); i++, add++)
	{
		al.construct(m_data + add, *i);
	}
}

template<typename Type, typename Allocator>
template<typename Iterator, typename>
inline my_Vector<Type, Allocator>::my_Vector(Iterator start, Iterator end)
{
	if (start == end)
	{
		throw std::range_error("Iterators are equal");
	}
	size_type new_size = std::distance(start, end);
	reallocate_if_small(new_size);
	std::uninitialized_copy(start, end, begin());
	m_size = new_size;
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::~my_Vector()
{
	for (size_type i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::operator=(const my_Vector<Type, Allocator>& other)
{
	my_Vector temp(other);
	swap(temp);
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::operator=(std::initializer_list<Type> list)
{
	clear();
	size_type new_size = list.size();
	reallocate_if_small(new_size);
	m_size = new_size;
	auto it = begin();
	for (auto i = list.begin(); i != list.end(); i++)
	{
		*it = std::move(*i);
	}
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::operator=(my_Vector<Type, Allocator>&& other) noexcept
{
	swap(*this, other);
}


template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::assign(std::initializer_list<Type> list)
{
	operator=(list);
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::assign(size_type n_arg, const_reference value)
{
	clear();
	size_type new_size = n_arg;
	reallocate_if_small(new_size);
	std::fill(begin(), end(), value);
	m_size = new_size;
}

template<typename Type, typename Allocator>
template<typename Iterator, typename>
void my_Vector<Type, Allocator>::assign(Iterator first, Iterator last)
{
	clear();
	size_type new_size = std::distance(first, last);
	reallocate_if_small(new_size);
	std::uninitialized_copy(first, last, begin());
	m_size = new_size;
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::allocator_type 
my_Vector<Type, Allocator>::get_allocator() const noexcept
{
	return al;
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::reference 
my_Vector<Type, Allocator>::at(size_type position)
{
	if (position >= m_size)
	{
		throw std::out_of_range("Out of range exception");
	}
	return m_data[position];
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::const_reference 
my_Vector<Type, Allocator>::at(size_type position) const
{
	if (position >= m_size)
	{
		throw std::out_of_range("Out of range exception");
	}
	return m_data[position];
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::swap(my_Vector<Type, Allocator>& other) noexcept
{
	my_VectorStorage<Type, Allocator>::swap(other);
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::reference 
my_Vector<Type, Allocator>::operator[](typename size_type position)
{
	return m_data[position];
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::const_reference 
my_Vector<Type, Allocator>::operator[](size_type position) const
{
	return m_data[position];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::reference 
my_Vector<Type, Allocator>::front()
{
	return m_data[m_size - 1];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::const_reference 
my_Vector<Type, Allocator>::front() const
{
	return m_data[m_size - 1];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::reference 
my_Vector<Type, Allocator>::back()
{
	return m_data[0];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::const_reference 
my_Vector<Type, Allocator>::back() const
{
	return m_data[0];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::pointer 
my_Vector<Type, Allocator>::data() noexcept
{
	return m_data;
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::const_pointer 
my_Vector<Type, Allocator>::data() const
{
	return m_data;
}

#ifdef _WIN32
template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::size_type my_Vector<Type, Allocator>::max_size() const noexcept
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	return statex.ullAvailPhys / sizeof(Type);
}
#elif __linux__
template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::size_type my_Vector<Type, Allocator>::max_size() const noexcept
{
	struct sysinfo memInfo;
	sysinfo(&memInfo);
	long long totalMemoryUsed = memInfo.totalram - memInfo.freeram;
	totalMemoryUsed += memInfo.totalswap - memInfo.freeswap;
	totalMemoryUsed *= memInfo.mem_unit;
	return totalMemoryUsed / sizeof(Type);

}
#endif
template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::reserve(size_type capacity_new)
{
	if (capacity_new > m_capacity)
	{
		pointer temp = al.allocate(capacity_new);
		for (size_type i = 0; i < m_size; i++)
		{
			//temp[i] = m_data[i];
			al.construct(temp + i, m_data[i]);
		}
		for (size_type i = 0; i < m_size; i++)
		{
			al.destroy(m_data + i);
		}
		al.deallocate(m_data, m_capacity);
		m_capacity = capacity_new;
		m_data = temp;
	}
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::push_back(const_reference value)
{
	reallocate_if_small(m_size);
	al.construct(&*end(), value);
	++m_size;
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::push_back(move_type value)
{
	reallocate_if_small(m_size);
	al.construct(&*end(), std::move(value));
	++m_size;
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::pop_back()
{
	if (m_size > 0)
	{
		al.destroy(m_data + m_size - 1);
		--m_size;
	}
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::insert(const_iterator pos, const_reference value)
{
	return emplace(pos, value);
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::insert(const_iterator pos, move_type value)
{
	return emplace(pos, value);
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::insert(const_iterator pos, size_type count, const_reference value)
{
	size_type home_dist = std::distance(cbegin(), pos);
	iterator it = begin();
	std::advance(it, home_dist);

	if ((count + m_size) > m_capacity)
	{
		size_type new_capacity = m_size + count * m_capacity_modifier;

		pointer new_data = al.allocate(new_capacity);

		pointer new_data_for_insert = std::uninitialized_copy(begin(), it, new_data);
		for (size_type i = 0; i < count; ++i, ++new_data_for_insert)
		{
			al.construct(new_data_for_insert, value);
		}
		std::uninitialized_copy(it, end(), new_data_for_insert);

		clear_old_memory();
		m_data = new_data;
		m_capacity = new_capacity;
		m_size += count;
	}
	else if (it == end())
	{
		for (size_type i = 0; i < count; ++i, ++m_size)
		{
			al.construct(&*end(), value);
		}
	}
	else
	{
		iterator current = end(), last = end() + count;
		size_type diff = m_size - home_dist;
		for (; current != last; ++m_size, ++current)
		{
			al.construct(&*current, value);
			// here end points to previously created data
			std::swap(*current, *(current - diff));
		} // it will point to the element after the last only here
	}
	return it;
}

// fix return of iterator
template<typename Type, typename Allocator>
template<typename Iterator, typename>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::insert(const_iterator pos, Iterator first, Iterator last)
{
	size_type outsider_size = std::distance(first, last);
	size_type home_dist = std::distance(cbegin(), pos);

	iterator it = begin();
	std::advance(it, home_dist);

	if ((outsider_size + m_size) > m_capacity)
	{
		size_type new_capacity = m_size + outsider_size * m_capacity_modifier;
		
		pointer new_data = al.allocate(new_capacity);
		
		pointer new_data_for_insert = std::uninitialized_copy(begin(), it, new_data);
		for (;first != last; ++first, ++new_data_for_insert)
		{
			al.construct(new_data_for_insert, *first);
		}
		std::uninitialized_copy(it, end(), new_data_for_insert);
		
		clear_old_memory();
		m_data = new_data;
		m_capacity = new_capacity;
		m_size += outsider_size;
	}
	else if (it == end())
	{
		for (; first != last; ++first, ++m_size)
		{
			al.construct(&*end(), *first);
		}
	}
	else
	{
		iterator current = end();
		size_type diff = m_size - home_dist;
		for (; first != last; ++first, ++m_size, ++current)
		{
			al.construct(&*current, *first);
			// here end points to previously created data
			std::swap(*current, *(current - diff));
		} // it will point to the element after the last only here
	}
	return it;
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::insert(const_iterator pos, std::initializer_list<value_type> ilist)
{
	return insert(pos, ilist.begin(), ilist.end());
}

template<typename Type, typename Allocator>
typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::erase(const_iterator place)
{
	iterator it = place;
	al.destroy(&*it);

	std::move(it + 1, end(), it);
	--m_size;
	return it;
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::erase(const_iterator first, const_iterator last)
{
	if ((first == cbegin()) && (last == cend()))
	{
		clear();
		return begin();
	}

	iterator beg = first;
	iterator fin = last;
	for (; first != last; ++first)
	{
		al.destroy(&*first);
	}
	std::move(fin, end(), beg);
	m_size -= std::distance(beg, fin);
	return beg;
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::clear()
{
	if (begin() == end())
	{
		return;
	}

	for (size_type i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	m_size = 0;
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::shrink_to_fit()
{
	if (m_capacity > m_size >= 0)
	{
		pointer temp = al.allocate(m_size);
		for (size_type i = 0; i < m_size; i++)
		{
			al.construct(temp + i, m_data[i]);
		}
		for (size_type i = 0; i < m_size; i++)
		{
			al.destroy(m_data + i);
		}
		al.deallocate(m_data, m_capacity);
		m_capacity = m_size;
		m_data = temp;
	}
}

//template<typename Type, typename Allocator>
//template<typename>
//my_Vector<Type, Allocator>::my_Vector(Iterator start, Iterator end)
//{
//	int size = std::distance(start, end);
//	reserve(size);
//	std::
//  (m_data, start, size);
//}

//not efficient
template<typename Type, typename Allocator>
template<typename ...Args>
void my_Vector<Type, Allocator>::pretty_emplace(iterator iter, Args&&...args)
{
	int dist = std::distance(begin(), iter);
	// don't forget that reserve(if m_size >= m_capacity) invalidates all iterators
	reallocate_if_small();
	++m_size;

	iterator emplaced = begin();
	std::advance(emplaced, dist);

	std::move_backward(emplaced, --end(), end());
	*emplaced = Type(std::forward<Args>(args)...);
}

template<typename Type, typename Allocator>
template<typename ...Args>
inline typename my_Vector<Type, Allocator>::iterator my_Vector<Type, Allocator>::emplace(iterator iter, Args&&...args)
{

	if (m_size >= m_capacity)
	{
		size_type dist = std::distance(begin(), iter);
		size_type new_capacity = m_capacity * m_capacity_modifier;
		pointer new_data = al.allocate(new_capacity);

		pointer new_data_before_emplace = std::uninitialized_copy(begin(), iter, new_data);

		al.construct(new_data_before_emplace, value_type());

		*new_data_before_emplace = Type(std::forward<Args>(args)...);

		pointer new_data_after_emplace = ++new_data_before_emplace;

		std::uninitialized_copy(iter, end(), new_data_after_emplace);

		clear_old_memory();
		m_data = new_data;
		m_capacity = new_capacity;
		++m_size;
		iter = begin();
		std::advance(iter, dist);
	}
	else if (iter == end())
	{
		al.construct(&*iter, std::forward<Args>(args)...);
		++m_size;
	}
	else
	{
		al.construct(&*end(), value_type());
		++m_size;

		std::move_backward(iter, --end(), end());

		*iter = Type(std::forward<Args>(args)...);
	}
	return iter;
}

template<typename Type, typename Allocator>
template<class ...Args>
inline typename my_Vector<Type, Allocator>::iterator 
my_Vector<Type, Allocator>::emplace_back(Args&& ...args)
{
	if (m_size >= m_capacity)
	{
		size_type new_capacity = m_capacity * m_capacity_modifier;
		pointer new_data = al.allocate(new_capacity);

		pointer new_data_before_emplace = std::uninitialized_copy(begin(), end(), new_data);

		clear_old_memory();
		m_data = new_data;
		m_capacity = new_capacity;
	}
	pointer object = &*end();
	al.construct(object, std::forward<Args>(args)...);
	++m_size;
	return object;
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::resize(size_type count)
{
	resize(count, value_type());
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::resize(size_type count, const_reference value)
{
	if (count > m_size)
	{
		if (count > m_capacity)
		{
			size_type new_capacity = count * m_capacity_modifier;
			pointer new_data = al.allocate(new_capacity);
			std::uninitialized_copy(begin(), end(), new_data);
			m_data = new_data;
			m_capacity = new_capacity;
		}
		iterator iter = end();
		size_type new_elems = count - m_size;
		for (size_type i = 0; i < new_elems; i++)
		{
			al.construct(&*iter, value);
			++m_size;
			++iter;
		}
	}
	else
	{
		pointer new_data = al.allocate(m_capacity);
		std::uninitialized_copy(begin(), begin() + count, new_data);
		erase(begin() + count, end());
		m_data = new_data;
		m_size = count;
	}
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::reallocate_if_small(size_type size)
{
	if (size >= m_capacity)
	{
		reserve(size * 2);
	}
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::increase_capacity()
{
	m_capacity *= 2;
}

template<typename Type, typename Allocator>
void my_Vector<Type, Allocator>::clear_old_memory()
{
	for (size_type i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	al.deallocate(m_data, m_capacity);
}

template<typename T, typename Alloc> 
bool operator==(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	if (&lhs = &rhs)
	{
		return true;
	}
	if (lhs.size() == rhs.size())
	{
		return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
	}
	return false;
}

template<typename T, typename Alloc>
bool operator!=(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	return !(lhs == rhs);
}

template<typename T,typename Alloc>
bool operator<(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	return std::lexicographical_compare(lhs.cbegin(), lhs.cend(),
								 rhs.cbegin(), rhs.cend());
}

template<typename T, typename Alloc>
bool operator>(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	return std::lexicographical_compare(rhs.cbegin(), rhs.cend(),
		lhs.cbegin(), lhs.cend());
}

template<typename T, typename Alloc>
bool operator<=(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	return !(lhs > rhs);
}

template<typename T, typename Alloc>
bool operator>=(const my_Vector<T, Alloc>& lhs, const my_Vector<T, Alloc>& rhs)
{
	return !(lhs < rhs);
}

template <typename Allocator>
class my_Vector<bool, Allocator>{};
}