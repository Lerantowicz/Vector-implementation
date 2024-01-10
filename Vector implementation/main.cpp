#include "my_Vector.h"
#include <vector>
#include <string>
#include <iostream>

template<typename Iter, typename Modifier>
void fill(Iter begin, Iter end, Iter other, Modifier func = [](auto&& value) {return value; })
{
	for (; begin != end; ++begin, ++other)
	{
		*other = func(*begin);
	}
}

struct string_vector
{
	std::string str;
	my_Vector<int> vec;
	std::pair<int, int> answers;

};

std::vector<string_vector> proper_names
{
	string_vector{"default constructor", my_Vector<int>(), std::pair(0, 4)},
	string_vector{"one element constructor", my_Vector<int>(1), std::pair(1, 4)},
	string_vector{"two element constructor", my_Vector<int>(5, 5), std::pair(5, 10)},
	string_vector{"initializer list", my_Vector<int>{1, 2, 3, 4}, std::pair(4, 4)}
};

int main()
{
	std::vector<int> vec_int(10, 10);

	//my_VectorStorage<int> vec_store(10, 10);

//	fill(vec_int.begin(), vec_int.end(), )
	my_Vector<int> int_vector(my_Vector<int>{1, 2, 3});
	int_vector.emplace(int_vector.begin(), 10);

	size_t size = 5;

	/*for (size_t i = 0; i < size; i++)
	{
		string_vector.push_back(1);
		string_vector.push_front(2);
	}

	for (size_t i = 0; i < size; i++)
	{
		string_vector.pop_back();
		string_vector.pop_front();
	}
	string_vector.shrink_to_fit();*/


	/*std::cout << sizeof(string_vector);
	std::cout << sizeof(std::string);
	std::cout << sizeof(my_Vector<int>);
	std::cout << sizeof(std::pair<int, int>);*/

	/*my_Vector<int> a{1, 2, 3, 4};
	my_Vector<int> b(1);
	my_Vector<int> c(10000);
	my_Vector<int> d(10, 10);
	my_Vector<int> e(my_Vector<int>(1));
	my_Vector<int> f;
	my_Vector<int> move_constructed = my_Vector<int>(1, 2);*/

	return 0;
}