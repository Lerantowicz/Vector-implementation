#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <memory>
#include <iostream>

template <typename Type, typename Allocator = std::allocator<Type>>
class my_VectorStorage
{
protected:

	using value_type			= Type;
	using pointer				= typename std::allocator_traits<Allocator>::pointer;
	using const_pointer			= typename std::allocator_traits<Allocator>::const_pointer;
	using reference				= value_type&;
	using const_reference		= const value_type&;
	using move_type				= Type&&;
	using size_type				= size_t;

protected:
	size_type m_capacity;
	pointer m_data = nullptr;
	size_type m_size;

	Allocator al;

protected:

	/*template<typename Iter, typename Value, typename Modifier>
	void fill(Iter begin, Iter end, Value&& value, Modifier func = [](auto&& value) {return value; })
	{
		for (; begin != end; ++begin)
		{
			*begin = func(std::forward<Value>(value));
		}
	}*/

	my_VectorStorage();

	my_VectorStorage(size_type size);

	my_VectorStorage(size_type n_arg, const_reference value);

	my_VectorStorage(const my_VectorStorage<Type, Allocator>& other);
	
	my_VectorStorage(my_VectorStorage<Type, Allocator>&& other);

	my_VectorStorage(std::initializer_list<Type> list);

	~my_VectorStorage();

	inline void swap(my_VectorStorage<Type, Allocator>& other) noexcept;

};

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator>::my_VectorStorage() : m_size(0), m_capacity(4)
{
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator>::my_VectorStorage(const my_VectorStorage<Type, Allocator>& other) : m_size(other.m_size), m_capacity(other.m_capacity)
{
	if (this == &other)
	{
		return;
	}
	m_data = al.allocate(m_capacity);
	for (size_type i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], other.m_data[i]);
	}
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(my_VectorStorage<Type, Allocator>&& other)
{
	swap(other);
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(size_type size) : m_size(size), m_capacity(size > 4 ? size * 2 : 4)
{
	m_data = al.allocate(m_capacity);
	for (size_type i = 0; i < m_size; i++)
	{
		for (size_type i = 0; i < m_size; i++)
		{
			al.construct(&m_data[i], value_type());
		}
	}
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator>::my_VectorStorage(size_type n_arg, const_reference value) : m_size(n_arg), m_capacity(n_arg * 2)
{
	if (m_size == 0)
	{
		m_capacity = 4;
	}
	m_data = al.allocate(m_capacity);
	for (size_type i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], value);
	}
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator>::my_VectorStorage(std::initializer_list<Type> list) : m_size(list.size()), m_capacity(list.size() > 4 ? list.size() * 2 : 4)
{
	if (m_size == 0)
	{
		m_capacity = 4;
	}
	m_data = al.allocate(m_capacity);
	int add = 0;
	for (auto i = list.begin(); i != list.end(); i++, add++)
	{
		al.construct(m_data + add, *i);
	}
}

template<typename Type, typename Allocator>
inline void my_VectorStorage<Type, Allocator>::swap(my_VectorStorage<Type, Allocator>& other) noexcept
{
	if (this == &other)
	{
		return;
	}
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator>::~my_VectorStorage()
{
	for (size_type i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	al.deallocate(m_data, m_capacity);
}

template <typename Type, typename Allocator = std::allocator<Type>>
class my_Vector : public my_VectorStorage<Type, Allocator>
{
	using typename my_VectorStorage<Type, Allocator>::value_type;
	using typename my_VectorStorage<Type, Allocator>::pointer;
	using typename my_VectorStorage<Type, Allocator>::const_pointer;
	using typename my_VectorStorage<Type, Allocator>::reference;
	using typename my_VectorStorage<Type, Allocator>::const_reference;
	using typename my_VectorStorage<Type, Allocator>::move_type;
	using typename my_VectorStorage<Type, Allocator>::size_type;
	using allocator_type		= Allocator;
	using difference_type		= std::ptrdiff_t;
	using my_VectorStorage<Type, Allocator>::m_capacity;
	using my_VectorStorage<Type, Allocator>::m_size;
	using my_VectorStorage<Type, Allocator>::m_data;
	using my_VectorStorage<Type, Allocator>::al;


public:

	template<typename Type = my_VectorStorage<Type, Allocator>::value_type>
	class my_Iterator
	{
	public:

		using iterator_category = std::forward_iterator_tag;
		using difference_type	= std::ptrdiff_t;
		using value_type		= Type;
		using pointer			= Type*;
		using reference			= Type&;
		using size_type			= size_t;
		my_Iterator(pointer ptr) : m_ptr(ptr)
		{

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
			*this += index;
			return temp;
		}
		my_Iterator operator-(size_type index)
		{
			my_Iterator temp = *this;
			*this -= index;
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
		/*my_Iterator operator+(const my_Iterator& other)
		{
			my_Iterator temp = *this;
			this += index;
			return temp;
		}
		my_Iterator operator-(const my_Iterator& other)
		{
			my_Iterator temp = *this;
			this -= index;
			return temp;
		}*/
	private:
		pointer m_ptr;
	};

	using iterator = my_Iterator<my_Vector<Type, Allocator>::value_type>;

public:

	/*template<typename = std::void_t<decltype(Iterator{}++), decltype(*Iterator{}) >>
		my_Vector(Iterator start, Iterator end);*/

	my_Vector() : my_VectorStorage<Type, Allocator>(){}

	my_Vector(size_type size) : my_VectorStorage<Type, Allocator>(size){}

	my_Vector(size_type n_arg, const_reference value) : my_VectorStorage<Type, Allocator>(n_arg, value){}

	my_Vector(const my_Vector<Type, Allocator>& other) : my_VectorStorage<Type, Allocator>(other){}

	my_Vector(my_Vector<Type, Allocator>&& other) noexcept : my_VectorStorage<Type, Allocator>(other) {}

	my_Vector(std::initializer_list<Type> list) : my_VectorStorage<Type, Allocator>(list) {}

	void operator=(const my_Vector<Type, Allocator>& other);

	void operator=(my_Vector<Type, Allocator>&& other) noexcept;

	void operator=(std::initializer_list<Type> list);

	void swap(my_Vector<Type, Allocator>& other) noexcept;
	
	reference operator[](size_type position);

	const_reference operator[](size_type position) const;

	void reserve(size_type capacity_new);

	/*void push_back(const_reference value);
	void push_front(const_reference value);
	void insert(const_reference value, size_type place);*/

	//void push_back(move_type value);

	void push_back(const_reference value);

	void pop_back();

	void push_front(move_type value);

	//void push_front(const_reference value);

	void pop_front();

	void insert(move_type value, size_type place);

	//void insert(const_reference value, size_type place);

	void erase(size_type place);

	template <class... Args>
	void emplace(iterator iter, Args&&...args);

	void clear();

	void shrink_to_fit();

	size_type size() const
	{
		return m_size;
	}

	size_type capacity() const
	{
		return m_capacity;
	}

	pointer data() 
	{
		return m_data;
	}

	iterator begin()
	{
		return iterator(m_data);
	}

	iterator end()
	{
		return iterator(m_data + m_size + 1);
	}

	private:
		void reallocate_if_small();
};

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(const my_Vector<Type, Allocator>& other)
{
	my_Vector temp(other);
	swap(temp);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(std::initializer_list<Type> list)
{
	clear();
	reserve(list.size() * 2);

	size_type add = 0;
	for (auto i = list.begin(); i != list.end(); i++, ++add)
	{
		al.construct(m_data + add, *i);
	}
	m_size = list.size();
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(my_Vector<Type, Allocator>&& other) noexcept
{
	if (this == &other)
	{
		return;
	}
	swap(*this, other);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::swap(my_Vector<Type, Allocator>& other) noexcept
{
	my_VectorStorage<Type, Allocator>::swap(other);
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::reference my_Vector<Type, Allocator>::operator[](typename size_type position)
{
	return m_data[position];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::const_reference my_Vector<Type, Allocator>::operator[](size_type position) const
{
	return m_data[position];
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::reserve(size_type capacity_new)
{
	if (capacity_new > m_capacity)
	{
		pointer temp = al.allocate(capacity_new);
		for (size_type i = 0; i < m_size; i++)
		{
			temp[i] = std::move(m_data[i]);
		}
		m_capacity = capacity_new;
	}
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_back(move_type value)
//{
//	reallocate_if_small();
//	al.construct(m_data + m_size, std::move(value));
//	++m_size;
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_back(const_reference value)
{
	reallocate_if_small();
	al.construct(m_data + m_size, value);
	++m_size;
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::pop_back()
{
	if (m_size > 0)
	{
		al.destroy(m_data + m_size - 1);
		--m_size;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_front(move_type value)
{
	insert(std::move(value), 0);
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_front(const_reference value)
//{
//	insert(value, 0);
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::pop_front()
{
	erase(0);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::insert(move_type value, size_type place)
{
	reallocate_if_small();
	for (size_type i = m_size; i > place; i--)
	{
		*(m_data + i) = std::move(*(m_data + i - 1));
	}
	al.construct(m_data + place, std::move(value));
	++m_size;
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::insert(const_reference value, size_type place)
//{
//	reallocate_if_small();
//	for (size_type i = m_size; i > place; i--)
//	{
//		*(m_data + i) = std::move(*(m_data + i - 1));
//	}
//	al.construct(m_data + place, value);
//	++m_size;
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::erase(size_type place)
{
	if (m_size > 0)
	{
		al.destroy(m_data + place);
		for (size_type i = place; i < m_size; i++)
		{
			*(m_data + i) = std::move(*(m_data + i + 1));
		}
		--m_size;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::clear()
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
inline void my_Vector<Type, Allocator>::shrink_to_fit()
{
	if (m_capacity > m_size > 0)
	{
		al.deallocate(m_data + m_size, m_capacity - m_size);
		m_capacity = m_size;
	}
}

//template<typename Type, typename Allocator>
//template<typename>
//inline my_Vector<Type, Allocator>::my_Vector(Iterator start, Iterator end)
//{
//	int size = std::distance(start, end);
//	reserve(size);
//	std::memcpy(m_data, start, size);
//}

//rewrite with std:forward
template<typename Type, typename Allocator>
template<typename ...Args>
inline void my_Vector<Type, Allocator>::emplace(iterator iter, Args&&...args)
{
	int place = std::distance(begin(), iter);
	reallocate_if_small();
	iterator emplaced = begin();
	std::advance(emplaced, place);
	std::move_backward(emplaced, end(), end() + 1);
	*emplaced = Type(std::forward<Args>(args)...);
	++m_size;
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::reallocate_if_small()
{
	if (m_size >= m_capacity)
	{
		m_capacity *= 2;
		reserve(m_capacity);
	}
}

template <typename Allocator>
class my_Vector<bool, Allocator>{};