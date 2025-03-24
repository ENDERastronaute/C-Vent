//
// Created by ender on 22.03.25.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "event.h"
#include <string>

/*
 * Tests variables
 */

template<typename T>
T test_result;

struct TestStruct
{
    int value;

    bool operator==(const TestStruct& other) const
    {
        return value == other.value;
    }
};

template<typename T>
struct TestTemplatedStruct
{
    T value;

    bool operator==(const TestTemplatedStruct& other) const
    {
        return value == other.value;
    }
};

template<typename T>
void test_callback(const T data)
{
    test_result<T> = data;
}

template<typename T>
class TestClass
{
public:
    TestClass() = default;

    void test_callback(const T data)
    {
        value = data;
    }

    T value{};
};


/*
 * Tests
 */

TEMPLATE_TEST_CASE("Events call multiple handlers", "[Event]", int, double, float, std::string, TestStruct, TestTemplatedStruct<double>)
{
    Event<TestType> event;
    TestClass<TestType> c1;
    TestClass<TestType> c2;

    event.connect(&c1, &TestClass<TestType>::test_callback);
    event.connect(&c2, &TestClass<TestType>::test_callback);
    event.connect(test_callback<TestType>);

    REQUIRE( event.handlers() == 3 );

    TestType value{};
    event.emit(value);

    REQUIRE( c1.value == value );
    REQUIRE( c2.value == value );
    REQUIRE( test_result<TestType> == value );

    event.disconnect(&c1, &TestClass<TestType>::test_callback);

    REQUIRE( event.handlers() == 2 );

    TestType new_value{};
    event.emit(new_value);

    REQUIRE(c1.value == value);
    REQUIRE(c1.value == new_value);
    REQUIRE(test_result<TestType> == new_value);
}

// TEMPLATE_TEST_CASE("Disconnecting non-existent handler doesn't affect others", "[Event]", int, double, float, std::string, TestStruct, TestTemplatedStruct<double>)
// {
//     Event<TestType> event;
//     TestClass<TestType> c1;
//     TestClass<TestType> c2;
//
//     event.connect(&c1, &TestClass<TestType>::test_callback);
//     REQUIRE( event.handlers() == 1 );
// }