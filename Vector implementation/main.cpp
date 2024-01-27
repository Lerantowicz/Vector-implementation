#include "my_Vector.h"
#include <vector>
#include <string>
#include <iostream>

using namespace my_Namespace;

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
	std::vector<std::string> a{ "a", "b", "c", "d" };
	std::allocator<std::string> al;
	auto b = al.allocate(4);
	std::uninitialized_copy(a.begin(), a.end(), b);
	print(b, b + 4);
	return 0;
}