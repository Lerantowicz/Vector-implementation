#pragma once

#include <memory>

template<typename my_Vector>
class my_Iterator
{

public:

	using ValueType = typename my_Vector::ValueType;
	using Pointer_Type = ValueType*;
	using ReferenceType = ValueType&;

	my_Iterator(Pointer_Type ptr) : m_ptr(ptr)
	{

	}

	my_Iterator& operator++()
	{
		m_ptr++;
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

		m_ptr--;
		return *this;
	}

	my_Iterator operator--(int)
	{
		my_Iterator temp = *this;
		--(*this);
		return temp;
	}

	my_Iterator& operator=(my_Iterator& other)
	{
		other.m_ptr = m_ptr;
		return *this;
	}

	ReferenceType operator[](size_t index)
	{
		return *(m_ptr + index);
	}

	Pointer_Type operator->()
	{
		return m_ptr;
	}

	ReferenceType operator*()
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


	
	my_Iterator operator+(size_t index)
	{
		my_Iterator temp = *this;
		this += index;
		return temp;
	}


	my_Iterator operator-(size_t index)
	{
		my_Iterator temp = *this;
		this -= index;
		return temp;
	}

	my_Iterator& operator+=(size_t index)
	{
		m_ptr += index;
		return *this;
	}


	my_Iterator& operator-=(size_t index)
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
	Pointer_Type m_ptr;
};



template <typename Type, typename Allocator = std::allocator<Type>>
class my_Vector
{
public:
	using ValueType = Type;
	using Pointer_Type = Type*;
	using ReferenceType = Type&;
	using MoveType = Type&&;
	using Iterator = my_Iterator<my_Vector<Type, Allocator>>;


public:
	my_Vector() : m_size(0), m_capacity(4)
	{
		m_data = al.allocate(m_capacity);
	}

	my_Vector(size_t size);

	my_Vector(size_t n_arg, ReferenceType value);

	my_Vector(const my_Vector<Type, Allocator>& other);
	
	my_Vector(my_Vector<Type, Allocator>&& other);

	my_Vector(std::initializer_list<Type> list);

	~my_Vector();

	void operator=(const my_Vector<Type, Allocator>& other);

	void operator=(my_Vector<Type, Allocator>&& other);


	void operator=(std::initializer_list<Type> list);


	void swap(my_Vector<Type, Allocator>& other);


	void reserve(unsigned capacity_new);


	//void push_back(Type value);

	//void push_back(Pointer_Type value);

	void push_back(ReferenceType value);

	void push_back(MoveType value);

	void pop_back();



	void push_front(ReferenceType value);

	void push_front(MoveType value);

	void pop_front();

	//void insert(Type value, size_t place);

	//void insert(Pointer_Type value, size_t place);

	void insert(ReferenceType value, size_t place);

	void insert(MoveType value, size_t place);

	void erase(size_t place);
	
	template <class... Args>
	void emplace(my_Vector::Iterator Iter, Args&&... args);


	void clear();


	size_t get_size() const
	{
		return m_size;
	}

	size_t get_capacity() const
	{
		return m_capacity;
	}

	Pointer_Type data() const
	{
		return m_data;
	}

	
	Iterator begin()
	{
		return Iterator(m_data);
	}

	Iterator end()
	{
		return Iterator(m_data+m_size);
	}


private:

	Pointer_Type m_data = nullptr;
	size_t m_size;
	size_t m_capacity;

	Allocator al;
	



};

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(size_t size) : m_size(size), m_capacity(size * 2)
{
	m_data = al.allocate(m_capacity);
	for (size_t i = 0; i < m_size; i++)
	{
		for (size_t i = 0; i < m_size; i++)
		{
			al.construct(&m_data[i], 0);
		}
	}

}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(size_t n_arg, ReferenceType value) : m_size(n_arg), m_capacity(n_arg * 2)
{
	m_data = al.allocate(m_capacity);
	for (size_t i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], value);
	}

}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(const my_Vector<Type, Allocator>& other) : m_size(other.m_size), m_capacity(other.m_capacity)
{
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(my_Vector<Type, Allocator>&& other)
{
	m_data = other.m_data;
	m_size = other.m_size;
	m_capacity = other.m_capacity;

	other.m_data = nullptr;
	other.m_size = 0;
	other.m_capacity = 0;
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(std::initializer_list<Type> list) : m_size(list.size()), m_capacity(m_size*2)
{
	m_data = al.allocate(m_capacity);
	size_t add = 0;
	for (auto i = list.begin(); i != list.end(); i++, add++)
	{
		al.construct(m_data + add, *i);
	}
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::~my_Vector()
{
	for (size_t i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	al.deallocate(m_data, m_capacity);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(const my_Vector<Type, Allocator>& other)
{
	my_Vector temp(other);
	swap(temp);
	return *this;
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(my_Vector<Type, Allocator>&& other)
{

	if (this == &other)
	{
		return;
	}
	al.deallocate(m_data, m_capacity);

	m_data = other.m_data;
	m_size = other.m_size;
	m_capacity = other.m_capacity;

	other.m_data = nullptr;
	other.m_size = 0;
	other.m_capacity = 0;
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::operator=(std::initializer_list<Type> list)
{
	clear();
	reserve(list.size() * 2);
	
	size_t add = 0;
	for (auto i = list.begin(); i != list.end(); i++, ++add)
	{
		al.construct(m_data + add, *i);
	}
	m_size = list.size();
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::swap(my_Vector<Type, Allocator>& other)
{
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::reserve(unsigned capacity_new)
{
	Pointer_Type temp = al.allocate(capacity_new);
	if (capacity_new > m_capacity)
	{
		for (size_t i = 0; i < m_size; i++)
		{
			temp[i] = std::move(m_data[i]);
		}
	}
	else
	{
		for (size_t i = 0; i < (m_size > capacity_new ? capacity_new : m_size); i++)
		{
			temp[i] = std::move(m_data[i]);
		}
	}
	if (m_data != nullptr)
	{
		al.deallocate(m_data, m_size);
	}
	m_size = (m_size > capacity_new ? capacity_new : m_size);
	m_capacity = capacity_new;
	m_data = temp;

}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_back(Type value)
//{
//	if (m_size < m_capacity)
//	{
//		al.construct(m_data + m_size, value);
//		m_size++;
//	}
//	else
//	{
//		m_capacity *= 2;
//		reserve(m_capacity);
//		al.construct(m_data + m_size, value);
//		m_size++;
//	}
//}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_back(Pointer_Type value)
//{
//	size_t i = 8 * 7;
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_back(ReferenceType value)
{
	if (m_size < m_capacity)
	{
		al.construct(m_data + m_size, value);
		m_size++;
	}
	else
	{
		m_capacity += 2;
		reserve(m_capacity);
		al.construct(m_data + m_size, value);
		m_size++;
	}
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_back(ReferenceType value)
//{
//	if (m_size < m_capacity)
//	{
//		al.construct(m_data + m_size, value);
//		m_size++;
//	}
//	else
//	{
//		my_Vector<Type, Allocator> temp(m_size);
//		while (temp.get_size() < m_size)
//		{
//			temp.push_back(m_data[temp.get_size()]);
//		}
//		temp.push_back(value);
//		std::swap(*this, temp);
//	}
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_back(MoveType value)
{
	if (m_size < m_capacity)
	{
		//al.construct(m_data + m_size, value);
		*(m_data + m_size) = std::move(value);
		m_size++;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);
		*(m_data + m_size) = std::move(value);
		//al.construct(m_data + m_size, value);
		m_size++;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::pop_back()
{
	if (m_size > 0)
	{
		al.destroy(m_data + m_size);
		m_size--;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_front(ReferenceType value)
{
	insert(value, 0);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_front(MoveType value)
{
	insert(value, 0);
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::pop_front()
{
	erase(0);
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::insert(Type value, size_t place)
//{
//	
//}
//
//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::insert(Pointer_Type value, size_t place)
//{
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::insert(ReferenceType value, size_t place)
{

	/*if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		Pointer_Type temp_data = al.allocate(m_capacity);
		for (size_t i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		al.construct(m_data + m_size, value);
		for (size_t i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		m_size++;
	}
	else
	{
		size_t old_capacity = m_capacity;
		m_capacity *= 2;
		Pointer_Type temp_data = al.allocate(m_capacity);
		for (size_t i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		al.construct(temp_data + m_size, value);
		for (size_t i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		m_size++;
		al.deallocate(m_data, old_capacity);
	}*/

	if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		for (size_t i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i-1));
		}
		al.construct(m_data + place, value);
		m_size++;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);
		for (size_t i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i-1));
		}
		al.construct(m_data + place, value);
		m_size++;
	}

}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::insert(MoveType value, size_t place)
{
	/*if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		Pointer_Type temp_data = al.allocate(m_capacity);
		for (size_t i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		*(m_data + place) = std::move(value);
		for (size_t i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		m_size++;
	}
	else
	{
		size_t old_capacity = m_capacity;
		m_capacity *= 2;
		Pointer_Type temp_data = al.allocate(m_capacity);
		for (size_t i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		*(temp_data + place) = std::move(value);
		for (size_t i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		m_size++;
		al.deallocate(m_data, old_capacity);
	}*/
	if (place < 0 || place > m_size)
	{
		return;
	}
	if (m_size < m_capacity)
	{
		for (size_t i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i-1));
		}
		*(m_data + place) = std::move(value);
		m_size++;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);
		
		for (size_t i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i-1));
		}
		*(m_data + place) = std::move(value);
		m_size++;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::erase(size_t place)
{
	if (place < 0 || place > m_size)
	{
		throw "Out of range";
	}
	if (m_size > 0)
	{
		al.destroy(m_data + place);
		for (size_t i = place; i < m_size; i++)
		{
			*(m_data + i) = std::move(*(m_data + i+1));
		}
		m_size--;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::clear()
{
	if (begin() == end())
	{
		return;
	}

	for (size_t i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	m_size = 0;
}


//rewrite with std:forward
template<typename Type, typename Allocator>
template<class ...Args>
inline void my_Vector<Type, Allocator>::emplace(my_Vector::Iterator Iter, Args && ...args)
{

}
