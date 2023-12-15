//#pragma once
//
//
//template <typename T>
//class my_Allocator
//{
//public:
//	using Type = T;
//	using Pointer_Type = T*;
//
//
//public:
//
//
//	Pointer_Type allocate(size_t size)
//	{
//		return  static_cast<T*>(::operator new(size * sizeof(T));
//	}
//
//	void construct(Pointer_Type memory, T value)
//	{
//		::operator new (memory) T(value);
//	}
//
//	void deallocate(Pointer_Type memory, size_t size)
//	{
//		::operator delete[] memory[size];
//	}
//
//	void destroy(Pointer_Type memory)
//	{
//		::operator delete memory;
//	}
//
//};