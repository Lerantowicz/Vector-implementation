#include "pch.h"

#include "../Vector implementation/my_Vector.h"

#include <string>
#include <vector>
#include <algorithm>

using namespace my_Namespace;

struct string_struct
{
	std::string str;
	my_Vector<int> vec;
	std::pair<int, int> answers;
};

std::vector<int> usable_int = { 1000, 10000, 100000 };

std::vector<string_struct> proper_names
{
	string_struct{"DefaultConstructor", my_Vector<int>(),
	std::pair(0, 4)},
	string_struct{"OneElementConstructor", my_Vector<int>(1),
	std::pair(1, 4)},
	string_struct{"TwoElementConstructor", my_Vector<int>(5, 5),
	std::pair(5, 10)},
	string_struct{"InitializerList", my_Vector<int>{1, 2, 3, 4},
	std::pair(4, 4)},
	string_struct{"MoveConstruction", my_Vector<int>(my_Vector<int>(10, 10)),
	std::pair(10, 20)},
	string_struct{"TwoItersConstruction", my_Vector<int>(usable_int.begin(), usable_int.end()),
	std::pair(3, 4)}

};

template<typename Type>
void check_my_Vector_equality(const my_Vector<Type>& lhs, const my_Vector<Type>& rhs)
{
	EXPECT_EQ(lhs.size(), rhs.size());
	EXPECT_EQ(lhs.capacity(), rhs.capacity());

	EXPECT_NE(lhs.data(), rhs.data());
	EXPECT_TRUE(std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template<typename Iter, typename Type>
bool check_array_equals_to_value(Iter first, Iter last, Type val)
{
	return std::all_of(first, last,
		[&val](std::string& x) { return x == val; });
}

class ConstructedTestFixture : public ::testing::TestWithParam<string_struct>
{
};

TEST_P(ConstructedTestFixture, Construction) {
	// when: vector is constructed
	auto& parameters = GetParam();

	// then: size and capacity are properly initialized
	EXPECT_EQ(parameters.vec.size(), parameters.answers.first);
	EXPECT_EQ(parameters.vec.capacity(), parameters.answers.second);
}

INSTANTIATE_TEST_CASE_P(Case, ConstructedTestFixture,
	::testing::ValuesIn(proper_names),
	[](const ::testing::TestParamInfo<ConstructedTestFixture::ParamType>& info)
	{
		return info.param.str;
	});

TEST(VectorConstructionTest, ShouldBeMoveConstructedProperly) {
	// given: vector is constructed
	my_Vector<int> constructed_from_two_params(2, 10);

	size_t size = constructed_from_two_params.size();
	size_t capacity = constructed_from_two_params.capacity();

	// when: vector is move constructed
	my_Vector<int> move_constructed(std::move(constructed_from_two_params));


	// then: the move constructed vector isn't empty
	EXPECT_EQ(move_constructed.size(), size);
	EXPECT_EQ(move_constructed.capacity(), capacity);

	// then: the moved from vector is empty
	EXPECT_EQ(constructed_from_two_params.size(), 0);
	EXPECT_EQ(constructed_from_two_params.capacity(), 0);
	EXPECT_TRUE(constructed_from_two_params.data() == nullptr);

}

TEST(VectorConstructionTest, ShouldBeCopyConstructedProperly) {
	// given: vector is constructed
	my_Vector<int> constructed_from_two_params(100, 100);

	size_t size = constructed_from_two_params.size();
	size_t capacity = constructed_from_two_params.capacity();

	// when: vector is copy constructed
	my_Vector<int> copy_constructed(constructed_from_two_params);


	// then: the copy constructed vector isn't empty
	EXPECT_EQ(copy_constructed.size(), size);
	EXPECT_EQ(copy_constructed.capacity(), capacity);

	// then: both vectors are equal except their pointers
	check_my_Vector_equality(
		copy_constructed, constructed_from_two_params
	);

}

class VectorFunctionsFixture : public ::testing::Test
{
protected:
	void SetUp() override
	{
		string_vector.resize(100, "a");

	}

	void TearDown() override
	{
	}

	my_Vector<std::string> string_vector;
};

TEST_F(VectorFunctionsFixture, OperatorEqualsFromLvalue) {
	// given: not empty vector
	my_Vector<std::string> temp 
		= {"1val", "2val", "3val", "4val", "5val", "6val", "7val"};
	
	// when: assigning one vector to another
	string_vector = temp;

	// then: both vectors are equal except their pointers
	check_my_Vector_equality(string_vector, temp);
}

TEST_F(VectorFunctionsFixture, OperatorEqualsFromRvalue) {
	// given: previosly modified vector

	// when: assigning one vector to another
	string_vector = my_Vector<std::string>(20, "values");

	// then: both vectors are equal except their pointers
	check_my_Vector_equality(
		string_vector, my_Vector<std::string>(20, "values")
	);
}

TEST_F(VectorFunctionsFixture, OperatorEqualsFromInitializerList) {
	// given: previosly modified vector
	std::initializer_list<std::string> in_list = { " a", " b", "ccc", "nike", "nice" };
	// when: assigning one vector to another
	string_vector = in_list;

	// then: data and size is the same
	EXPECT_EQ(string_vector.size(), in_list.size());

	EXPECT_TRUE(std::equal(string_vector.begin(), string_vector.end(), in_list.begin()));
}

TEST_F(VectorFunctionsFixture, AssignNumberOfAndValue) {
	// given: previosly modified vector
	// when: assigning one vector to another
	string_vector.assign(100, "count, value");

	// then: data and size is the same
	EXPECT_EQ(string_vector.size(), 100);
	EXPECT_EQ(string_vector.capacity(), 200);


	EXPECT_TRUE(check_array_equals_to_value(
		string_vector.begin(), string_vector.end(), string_vector[0]));
}

TEST_F(VectorFunctionsFixture, DataAccess) {
	// given: previosly modified vector
	
	// when: assigning one vector to another
	std::cout << "Size " << string_vector.size() << '\n';
	std::cout << "Capacity " << string_vector.capacity() << '\n';
	//std::cout << string_vector.size();

	EXPECT_EQ(string_vector[0], string_vector.at(0));
	EXPECT_EQ(string_vector[string_vector.size() - 1], 
			  string_vector.at(string_vector.size() - 1));
	EXPECT_EQ(string_vector[0], string_vector.front());
	EXPECT_EQ(string_vector[string_vector.size() - 1],
			  string_vector.back());

	// then: data and size is the same
	EXPECT_EQ(string_vector.size(), 100);
	EXPECT_EQ(string_vector.capacity(), 200);

	EXPECT_TRUE(check_array_equals_to_value(
		string_vector.begin(), string_vector.end(), string_vector[0]));
}