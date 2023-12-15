#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>

#include "my_Vector.h"
#include "my_Allocator.h"

class Point
{



public:
	int x, y;

	Point()
	{
		x = 5;
		y = 8;
		std::cout << "default\n";
	}


	Point(int x, int y) : x(x), y(y)
	{
		std::cout << "tp\n";
	}

	Point(const Point&& other) noexcept : x(other.x), y(other.y)
	{
		std::cout << "move\n";
	}

	Point(const Point& other) : x(other.x), y(other.y)
	{
		std::cout << "copy\n";
	}

	void operator=(const Point& other)
	{
		x = other.x;
		y = other.y;
	}

	void operator=(Point&& other) noexcept
	{
		x = other.x;
		y = other.y;
	}
	friend std::ostream& operator<<(std::ostream& out, const Point& show) 
	{
		out << "x - " << show.x << " y - " << show.y << '\n';
		return out;
	}
};






template<typename T>
void print(T start, T end)
{
	for (auto i = start; i != end; i++)
	{
		std::cout << *(*i) << '\n';
	}
}

//void test_1();
//
//void test_2();

void test_3();

void test_4(int size);

void test_5();

int main()
{
	//int size = 5;
	//my_Allocator<int> al;
	//int* a = al.allocate(size);

	//print(a, a+size);
	//al.construct(a, 0);
	//print(a, a + size);

	std::vector<Point> a(10, Point(0, 0));
	a.reserve(20);
	a.emplace(std::vector<Point>::iterator(a.begin()), 3,3);



	std::cout << "----------------------------\n";
	//auto start = std::chrono::steady_clock::now();
	//test_4(10000);
	//auto end = std::chrono::steady_clock::now();
	//std::cout << "Elapsed time in seconds: "
	//	<< std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
	//	<< " ms";



	my_Vector<Point> arr(10, Point(0,0));
	arr.reserve(20);
	arr.emplace(my_Vector<Point>::Iterator(arr.begin()), 5,5);

}

//void test_1()
//{
//	my_Vector<Point> ar;
//	Point a;
//	ar.push_back(Point());
//	ar.push_back(Point(1, 2));
//	ar.push_front(Point(-1, -2));
//	ar.push_back(Point(a));
//	ar.push_back(a);
//	ar.insert(Point(5, 5), 2);
//	ar.insert(Point(6, 6), 2);
//	ar.push_front(Point(-11, -22));
//	ar.push_front(Point(-111, -222));
//	ar.push_back(a);
//	ar.insert(Point(7, 7), 2);
//	ar.insert(Point(8, 8), 2);
//	ar.push_back(a);
//	ar.insert(Point(0, 0), 0);
//	ar.insert(Point(10, 10), ar.get_size());
//
//	print(ar.begin(), ar.end());
//	std::cout << "\n----------------------------------------------------\n";
//	ar.pop_back();
//	ar.pop_back();
//
//	print(ar.begin(), ar.end());
//	std::cout << "\n----------------------------------------------------\n";
//
//	ar.pop_front();
//	ar.pop_front();
//
//	print(ar.begin(), ar.end());
//	std::cout << "\n----------------------------------------------------\n";
//
//	ar.erase(0);
//	ar.erase(ar.get_size());
//	ar.erase(3);
//	ar.erase(1);
//
//	print(ar.begin(), ar.end());
//	std::cout << "\n----------------------------------------------------\n";
//
//}
//
//void test_2()
//{
//	my_Vector<int> arr = {1,2,3,4,5};
//	arr = {5,4,3,2};
//	arr = { 5,4,3,2 ,6,7,8,8,8,8,8,8};
//	arr = { 1 };
//}

void test_3()
{
	int a = 5;
	int* b = &a;
	my_Vector<int*> arr = {new int(1), new int(2), new int(3), new int(4), new int(5) };
	arr = { new int(5) };
	arr = { new int(1), new int(2), new int(3), new int(4), new int(5),new int(1), new int(2), new int(3), new int(4), new int(5) };
	arr.push_back(b);
	arr.insert(&a, 4);
	arr.push_front(new int(100000));
	print(arr.begin(), arr.end());
}

void test_4(int size)
{
	my_Vector<int> arr(5);
	for (int i = 0; i < size; i++)
	{
		arr.push_back(i);
	}
}


void test_5()
{
	my_Vector<int> a(10,5);




}