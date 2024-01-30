#include "my_Vector.h"
#include <vector>
#include <string>
#include <iostream>

using namespace my_Namespace;

struct string_vector
{
	std::string str;
	my_Vector<int> vec;
	std::pair<int, int> answers;
};

std::vector<string_vector> proper_names
{
	string_vector{"DefaultConstructor", my_Vector<int>(), std::pair(0, 4)},
	string_vector{"OneElementConstructor", my_Vector<int>(1), std::pair(1, 4)},
	string_vector{"TwoElementConstructor", my_Vector<int>(5, 5), std::pair(5, 10)},
	string_vector{"InitializerList", my_Vector<int>{1, 2, 3, 4}, std::pair(4, 4)},
	string_vector{"MoveConstruction", my_Vector<int>(my_Vector<int>(10, 10)), std::pair(10, 20)}
};


template <typename Iter>
void print(Iter begin, Iter end)
{
	for (; begin != end; ++begin)
	{
		std::cout << *begin << '\n';
	}
}

int main()
{
	my_Vector<std::string> string_vector;
	std::initializer_list<std::string> ilist = { "a", "b", "c", "d", "e", "f" };
	my_Vector<std::string> temp
		= { "1val", "2val", "3val", "4val", "5val", "6val", "7val" };

	// when: assigning one vector to another
	string_vector = temp;

	auto a = string_vector[0];
	auto a1 = string_vector.at(0);
	

	
	auto a2 = string_vector[string_vector.size() - 1];
	auto a3 = string_vector.at(string_vector.size() - 1);
	

	
	auto a4 = string_vector[0];
	auto a5 = string_vector.front();
	

	
	auto a6 = string_vector[string_vector.size() - 1];
	auto a7 = string_vector.back();
	

	

	print(string_vector.begin(), string_vector.end());
	return 0;
}