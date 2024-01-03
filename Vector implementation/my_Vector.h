#pragma once

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <memory>

template <typename Type, typename Allocator = std::allocator<Type>>
class my_VectorStorage
{
protected:
	using ValueType = Type;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;
	using CReferenceType = const ValueType&;
	using MoveType = Type&&;
	using SizeType = size_t;

protected:
	PointerType m_data = nullptr;
	SizeType m_size = 0;
	SizeType m_capacity = 0;

	Allocator al;

protected:

	template<typename Iter, typename Value, typename Modifier>
	void fill(Iter begin, Iter end, Value&& value, Modifier func = [](auto&& value) {return value; })
	{
		for (; begin != end; ++begin)
		{
			*begin = func(std::forward<Value>(value));
		}
	}

	my_VectorStorage();

	my_VectorStorage(my_VectorStorage<Type, Allocator>&& other);

	my_VectorStorage(const my_VectorStorage<Type, Allocator>& other);

	inline void swap(my_VectorStorage<Type, Allocator>& other) noexcept;

	my_VectorStorage<Type, Allocator> operator=(const my_VectorStorage<Type, Allocator>& other);

	my_VectorStorage<Type, Allocator> operator=(my_VectorStorage<Type, Allocator>&& other) noexcept;

	~my_VectorStorage();
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
	for (SizeType i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], other[i]);
	}
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator> my_VectorStorage<Type, Allocator>::operator=(const my_VectorStorage<Type, Allocator>& other)
{
	my_Vector temp(other);
	swap(temp);
	return *this;
}

template<typename Type, typename Allocator>
my_VectorStorage<Type, Allocator>::my_VectorStorage(my_VectorStorage<Type, Allocator>&& other)
{
	swap(other);
}

template<typename Type, typename Allocator>
inline my_VectorStorage<Type, Allocator> my_VectorStorage<Type, Allocator>::operator=(my_VectorStorage<Type, Allocator>&& other) noexcept
{
	if (this == &other)
	{
		return;
	}
	swap(*this, other);
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
	for (SizeType i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	al.deallocate(m_data, m_capacity);
}

template <typename Type, typename Allocator = std::allocator<Type>>
class my_Vector : public my_VectorStorage<Type, Allocator>
{
	using ValueType = my_VectorStorage<Type, Allocator>::ValueType;
	using PointerType = my_VectorStorage<Type, Allocator>::PointerType;
	using ReferenceType = my_VectorStorage<Type, Allocator>::ReferenceType;
	using CReferenceType = my_VectorStorage<Type, Allocator>::CReferenceType;
	using MoveType = my_VectorStorage<Type, Allocator>::MoveType;
	using SizeType = my_VectorStorage<Type, Allocator>::SizeType;

public:

	template<typename my_Vector>
	class my_Iterator
	{
	public:

		my_Iterator(PointerType ptr) : m_ptr(ptr)
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

		ReferenceType operator[](SizeType index)
		{
			return *(m_ptr + index);
		}

		PointerType operator->()
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

		my_Iterator operator+(SizeType index)
		{
			my_Iterator temp = *this;
			this += index;
			return temp;
		}

		my_Iterator operator-(SizeType index)
		{
			my_Iterator temp = *this;
			this -= index;
			return temp;
		}

		my_Iterator& operator+=(SizeType index)
		{
			m_ptr += index;
			return *this;
		}

		my_Iterator& operator-=(SizeType index)
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
		PointerType m_ptr;
	};

	using Iterator = my_Iterator<my_Vector<Type, Allocator>>;

public:

	my_Vector();

	my_Vector(SizeType size);

	my_Vector(my_Vector<Type, Allocator>&& other);

	my_Vector(const my_Vector<Type, Allocator>& other);

	my_Vector(SizeType n_arg, CReferenceType value);

	my_Vector(std::initializer_list<Type> list);

	/*template<typename = std::void_t<decltype(Iterator{}++), decltype(*Iterator{}) >>
		my_Vector(Iterator start, Iterator end);*/

	~my_Vector();

	inline void swap(my_Vector<Type, Allocator>& other) noexcept;
	
	my_Vector<Type, Allocator> operator=(const my_Vector<Type, Allocator>& other);

	my_Vector<Type, Allocator> operator=(my_Vector<Type, Allocator>&& other) noexcept;

	my_Vector<Type, Allocator> operator=(std::initializer_list<Type> list);

	ReferenceType operator[](SizeType position);

	CReferenceType operator[](SizeType position) const;

	void reserve(SizeType capacity_new);

	void push_back(ReferenceType value);

	void push_back(MoveType value);

	void pop_back();

	void push_front(ReferenceType value);

	void push_front(MoveType value);

	void pop_front();

	void insert(ReferenceType value, SizeType place);

	void insert(MoveType value, SizeType place);

	void erase(SizeType place);

	template <class... Args>
	void emplace(my_Vector::Iterator Iter, Args&&...args);

	void clear();

	void shrink_to_fit();

	SizeType size() const
	{
		return m_size;
	}

	SizeType capacity() const
	{
		return m_capacity;
	}

	PointerType data() 
	{
		return m_data;
	}

	Iterator begin()
	{
		return Iterator(m_data);
	}

	Iterator end()
	{
		return Iterator(m_data + m_size);
	}

};

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector() : m_size(0), m_capacity(4)
{
	m_data = al.allocate(m_capacity);
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(SizeType size) : m_size(size), m_capacity(size > 4 ? size * 2 : 4)
{
	m_data = al.allocate(m_capacity);
	for (SizeType i = 0; i < m_size; i++)
	{
		for (SizeType i = 0; i < m_size; i++)
		{
			al.construct(&m_data[i], 0);
		}
	}

}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(SizeType n_arg, CReferenceType value) : m_size(n_arg), m_capacity(m_size * 2)
{
	if (m_size == 0)
	{
		m_capacity = 4;
	}
	m_data = al.allocate(m_capacity);
	for (SizeType i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], value);
	}

}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(const my_Vector<Type, Allocator>& other) : m_size(other.m_size), m_capacity(other.m_capacity)
{
	if (this == &other)
	{
		return;
	}
	m_data = al.allocate(m_capacity);
	for (SizeType i = 0; i < m_size; i++)
	{
		al.construct(&m_data[i], other[i]);
	}
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::my_Vector(std::initializer_list<Type> list) : m_size(list.size()), m_capacity(list.size() > 4 ? list.size() * 2 : 4)
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
inline my_Vector<Type, Allocator> my_Vector<Type, Allocator>::operator=(const my_Vector<Type, Allocator>& other)
{
	my_Vector temp(other);
	swap(temp);
	return *this;
}

template<typename Type, typename Allocator>
my_Vector<Type, Allocator>::my_Vector(my_Vector<Type, Allocator>&& other)
{
	swap(other);
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator> my_Vector<Type, Allocator>::operator=(my_Vector<Type, Allocator>&& other) noexcept
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
	if (this == &other)
	{
		return;
	}
	std::swap(m_data, other.m_data);
	std::swap(m_size, other.m_size);
	std::swap(m_capacity, other.m_capacity);
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator>::~my_Vector()
{
	for (SizeType i = 0; i < m_size; i++)
	{
		al.destroy(m_data + i);
	}
	al.deallocate(m_data, m_capacity);
}

template<typename Type, typename Allocator>
inline my_Vector<Type, Allocator> my_Vector<Type, Allocator>::operator=(std::initializer_list<Type> list)
{
	clear();
	reserve(list.size() * 2);

	SizeType add = 0;
	for (auto i = list.begin(); i != list.end(); i++, ++add)
	{
		al.construct(m_data + add, *i);
	}
	m_size = list.size();
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::ReferenceType my_Vector<Type, Allocator>::operator[](SizeType position)
{
	return m_data[position];
}

template<typename Type, typename Allocator>
inline typename my_Vector<Type, Allocator>::CReferenceType my_Vector<Type, Allocator>::operator[](SizeType position) const
{
	return m_data[position];
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::reserve(SizeType capacity_new)
{

	PointerType temp = al.allocate(capacity_new);
	if (capacity_new > m_capacity)
	{
		for (SizeType i = 0; i < m_size; i++)
		{
			temp[i] = std::move(m_data[i]);
		}
	}
	else
	{
		for (SizeType i = 0; i < (m_size > capacity_new ? capacity_new : m_size); i++)
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
//		++m_size;
//	}
//	else
//	{
//		m_capacity *= 2;
//		reserve(m_capacity);
//		al.construct(m_data + m_size, value);
//		++m_size;
//	}
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::push_back(ReferenceType value)
{
	if (m_size < m_capacity)
	{
		al.construct(m_data + m_size, value);
		++m_size;
	}
	else
	{
		m_capacity += 2;
		reserve(m_capacity);
		al.construct(m_data + m_size, value);
		++m_size;
	}
}

//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::push_back(ReferenceType value)
//{
//	if (m_size < m_capacity)
//	{
//		al.construct(m_data + m_size, value);
//		++m_size;
//	}
//	else
//	{
//		my_Vector<Type, Allocator> temp(m_size);
//		while (temp.size() < m_size)
//		{
//			temp.push_back(m_data[temp.size()]);
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
		++m_size;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);
		*(m_data + m_size) = std::move(value);
		//al.construct(m_data + m_size, value);
		++m_size;
	}
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
//inline void my_Vector<Type, Allocator>::insert(Type value, SizeType place)
//{
//	
//}
//
//template<typename Type, typename Allocator>
//inline void my_Vector<Type, Allocator>::insert(PointerType value, SizeType place)
//{
//}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::insert(ReferenceType value, SizeType place)
{

	/*if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		PointerType temp_data = al.allocate(m_capacity);
		for (SizeType i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		al.construct(m_data + m_size, value);
		for (SizeType i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		++m_size;
	}
	else
	{
		SizeType old_capacity = m_capacity;
		m_capacity *= 2;
		PointerType temp_data = al.allocate(m_capacity);
		for (SizeType i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		al.construct(temp_data + m_size, value);
		for (SizeType i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		++m_size;
		al.deallocate(m_data, old_capacity);
	}*/

	if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		for (SizeType i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i - 1));
		}
		al.construct(m_data + place, value);
		++m_size;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);
		for (SizeType i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i - 1));
		}
		al.construct(m_data + place, value);
		++m_size;
	}

}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::insert(MoveType value, SizeType place)
{
	/*if (place < 0 || place > m_size)
	{
		return;
	}

	if (m_size < m_capacity)
	{
		PointerType temp_data = al.allocate(m_capacity);
		for (SizeType i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		*(m_data + place) = std::move(value);
		for (SizeType i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		++m_size;
	}
	else
	{
		SizeType old_capacity = m_capacity;
		m_capacity *= 2;
		PointerType temp_data = al.allocate(m_capacity);
		for (SizeType i = 0; i < place; i++)
		{
			temp_data[i] = m_data[i];
		}
		*(temp_data + place) = std::move(value);
		for (SizeType i = place; i < m_size - place; i++)
		{
			temp_data[i] = m_data[i];
		}
		++m_size;
		al.deallocate(m_data, old_capacity);
	}*/
	if (place < 0 || place > m_size)
	{
		return;
	}
	if (m_size < m_capacity)
	{
		for (SizeType i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i - 1));
		}
		*(m_data + place) = std::move(value);
		++m_size;
	}
	else
	{
		m_capacity *= 2;
		reserve(m_capacity);

		for (SizeType i = m_size; i > place; i--)
		{
			*(m_data + i) = std::move(*(m_data + i - 1));
		}
		*(m_data + place) = std::move(value);
		++m_size;
	}
}

template<typename Type, typename Allocator>
inline void my_Vector<Type, Allocator>::erase(SizeType place)
{
	if (place < 0 || place > m_size)
	{
		throw "Out of range";
	}
	if (m_size > 0)
	{
		al.destroy(m_data + place);
		for (SizeType i = place; i < m_size; i++)
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

	for (SizeType i = 0; i < m_size; i++)
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
inline void my_Vector<Type, Allocator>::emplace(my_Vector::Iterator Iter, Args&&...args)
{
	if (m_capacity >= m_size)
	{
		if (m_capacity == m_size)
		{
			reserve(m_size * 2);
		}
		int place = 0;
		auto finish = end();
		auto temp = finish;
		for (auto i = finish; i != Iter; --i)
		{
			place++;
			finish = --temp;
		}
		erase(m_data + (m_size - place));
		al.construct(m_data + (m_size - place), Type(std::forward<Args>(args)...));
		++m_size;
	}

}

template <typename Allocator>
class my_Vector<bool, Allocator>{};