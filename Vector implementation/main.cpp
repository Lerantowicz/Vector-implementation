#include "my_Vector.h"
#include <vector>

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
	my_Vector<std::string> b;
	b.assign(a.begin(), a.end());
	print(b.begin(), b.end());
	return 0;
}