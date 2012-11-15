
#include "vector.h"
#include "sort.h"

#include <sstream>
#include <iostream>

using namespace std;
using namespace rtl;

struct SourceLocation {
    int _line;
    const char* _file;

    SourceLocation(int line, const char* file)
      : _line(line), _file(file)
    {}

    std::string toString()
    {
        std::stringstream strm;
        strm << _file << " line " << _line;
        return strm.str();
    }
};

void test_assert_with_location(bool b, SourceLocation loc)
{
    if (!b)
        throw std::runtime_error(loc.toString() + ": Assert failed");
}

void test_equals_with_location(std::string const& a, std::string const& b, SourceLocation loc)
{
    if (a != b)
        throw std::runtime_error(loc.toString() + ": " + a + " != " + b);
}

void run_test_with_name(void(*func)(), std::string const& name)
{
    try {
        func();
    } catch (std::exception const& e) {
        std::cout << name << " failed: \n  " << e.what() << std::endl;
    }
}

#define test_assert(b) test_assert_with_location((b), SourceLocation(__LINE__, __FILE__))
#define test_equals(a,b) test_equals_with_location((a),(b), SourceLocation(__LINE__, __FILE__))
#define run_test(name) run_test_with_name(name, #name)

std::string to_string(vector<std::string> const& v)
{
    std::stringstream strm;
    strm << "[";
    for (size_t i=0; i < v.size(); i++) {
        if (i != 0)
            strm << ", ";
        strm << v[i];
    }
    strm << "]";
    return strm.str();
}

std::string to_string_using_iterator(vector<std::string> const& v)
{
    std::stringstream strm;
    strm << "[";
    for (vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin())
            strm << ", ";
        strm << *it;
    }
    strm << "]";
    return strm.str();
}

std::string to_string_reversed_iterator(vector<std::string> const& v)
{
    std::stringstream strm;
    strm << "[";
    for (vector<std::string>::const_iterator it = v.rbegin(); it != v.rend(); --it) {
        if (it != v.rbegin())
            strm << ", ";
        strm << *it;
    }
    strm << "]";
    return strm.str();
}


vector<std::string> gSpyLog;
int gNextAnonSpy = 1;

void spy_clear()
{
    gSpyLog.clear();
    gNextAnonSpy = 1;
}

std::string spy_log()
{
    return to_string(gSpyLog);
}

struct Spy
{
    // The Spy class keeps a global log of all calls made to its constructor and destructor.

    std::string _name;
    int _numCopies;

    Spy() {
        _numCopies = 0;

        std::stringstream name;
        name << "Anon" << gNextAnonSpy++;
        _name = name.str();

        gSpyLog.push_back(std::string("ctor:") + _name);
    }

    Spy(std::string const& name) {
        _numCopies = 0;

        _name = name;
        gSpyLog.push_back(std::string("ctor:") + _name);
    }

    Spy(Spy const& copy) {
        _numCopies = 0;

        std::stringstream name;
        name << copy._name << "_copy" << (copy._numCopies + 1);
        _name = name.str();
        gSpyLog.push_back(std::string("ctor:") + _name);

        const_cast<Spy&>(copy)._numCopies++;
    }

    ~Spy() {
        gSpyLog.push_back(std::string("dtor:") + _name);
    }
};

void test_with_to_string()
{
    vector<std::string> v;

    test_equals(to_string(v), "[]");

    v.push_back("apple");
    test_equals(to_string(v), "[apple]");
    test_equals(to_string_using_iterator(v), "[apple]");
    test_equals(to_string_reversed_iterator(v), "[apple]");

    v.push_back("banana");
    test_equals(to_string(v), "[apple, banana]");
    test_equals(to_string_using_iterator(v), "[apple, banana]");
    test_equals(to_string_reversed_iterator(v), "[banana, apple]");

    v.push_back("cherry");
    test_equals(to_string(v), "[apple, banana, cherry]");
    test_equals(to_string_using_iterator(v), "[apple, banana, cherry]");
    test_equals(to_string_reversed_iterator(v), "[cherry, banana, apple]");

    v.pop_back();
    test_equals(to_string(v), "[apple, banana]");
    test_equals(to_string_using_iterator(v), "[apple, banana]");
    test_equals(to_string_reversed_iterator(v), "[banana, apple]");

    v.pop_back();
    test_equals(to_string(v), "[apple]");
    test_equals(to_string_using_iterator(v), "[apple]");
    test_equals(to_string_reversed_iterator(v), "[apple]");

    v.pop_back();
    test_equals(to_string(v), "[]");
    test_equals(to_string_using_iterator(v), "[]");
    test_equals(to_string_reversed_iterator(v), "[]");
}

void test_the_spy()
{
    // Just test that the Spy helper class is behaving correctly.
    spy_clear();

    test_equals(spy_log(), "[]");

    {
        Spy spy1;
        test_equals(spy_log(), "[ctor:Anon1]");

        Spy spy2;
        test_equals(spy_log(), "[ctor:Anon1, ctor:Anon2]");
    }

    test_equals(spy_log(), "[ctor:Anon1, ctor:Anon2, dtor:Anon2, dtor:Anon1]");

    spy_clear();

    {
        // Multiple copies of one value.
        Spy spy1("spy1");
        test_equals(spy_log(), "[ctor:spy1]");
        Spy spy2(spy1);
        test_equals(spy_log(), "[ctor:spy1, ctor:spy1_copy1]");
        Spy spy3(spy1);
        test_equals(spy_log(), "[ctor:spy1, ctor:spy1_copy1, ctor:spy1_copy2]");
    }

    spy_clear();

    {
        // Copy of a copy.
        Spy spy1("spy1");
        test_equals(spy_log(), "[ctor:spy1]");
        Spy spy2(spy1);
        test_equals(spy_log(), "[ctor:spy1, ctor:spy1_copy1]");
        Spy spy3(spy2);
        test_equals(spy_log(), "[ctor:spy1, ctor:spy1_copy1, ctor:spy1_copy1_copy1]");
    }

    spy_clear();
}

void test_copy()
{
    spy_clear();

    vector<Spy> v;
    test_equals(spy_log(), "[]");

    v.push_back(Spy("a"));
    test_equals(spy_log(), "[ctor:a, ctor:a_copy1, dtor:a]");
    spy_clear();

    v.push_back(Spy("b"));
    test_equals(spy_log(), "[ctor:b, ctor:b_copy1, dtor:b]");
    spy_clear();

    // Make the copy, ensure the values are each copy constructed.
    vector<Spy> v_copy(v);
    test_equals(spy_log(), "[ctor:a_copy1_copy1, ctor:b_copy1_copy1]");
}

void test_clear()
{
    spy_clear();

    vector<Spy> v;

    v.push_back(Spy("a"));
    v.push_back(Spy("b"));
    test_equals(spy_log(), "[ctor:a, ctor:a_copy1, dtor:a, ctor:b, ctor:b_copy1, dtor:b]");

    spy_clear();
    v.clear();
    test_equals(spy_log(), "[dtor:a_copy1, dtor:b_copy1]");
}

void test_accessors()
{
    vector<std::string> v;
    v.push_back("0");
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");

    test_equals(v[0], "0");
    test_equals(v[1], "1");
    test_equals(v[2], "2");
    test_equals(v[3], "3");

    test_equals(v.front(), "0");
    test_equals(v.back(), "3");
    test_assert(v.size() == 4);
}

void test_swap()
{
    vector<Spy> v1;
    vector<Spy> v2;

    v1.push_back(Spy("a"));
    v1.push_back(Spy("b"));
    v1.push_back(Spy("c"));
    v2.push_back(Spy("d"));
    v2.push_back(Spy("e"));

    spy_clear();

    // Do the swap. Shouldn't invoke any constructors.
    v1.swap(v2);
    test_equals(spy_log(), "[]");

    test_equals(v1[0]._name, "d_copy1");
    test_equals(v1[1]._name, "e_copy1");
    test_equals(v2[0]._name, "a_copy1");
    test_equals(v2[1]._name, "b_copy1");
    test_equals(v2[2]._name, "c_copy1");
}

void test_big_list()
{
    const size_t big_size = 10000000;

    vector<int> v;

    for (int i=0; i < big_size; i++)
        v.push_back(i);

    test_assert(v.size() == big_size);
    test_assert(v.capacity() >= big_size);

    for (int i=0; i < big_size; i++)
        test_assert(v[i] == i);
}

void test_insert()
{
    vector<std::string> v;
    v.push_back("0");
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");

    v.insert(v.begin() + 3, "2.5");
    test_equals(to_string(v), "[0, 1, 2, 2.5, 3]");

    v.insert(v.end(), "4");
    test_equals(to_string(v), "[0, 1, 2, 2.5, 3, 4]");

    v.insert(v.begin(), "-1");
    test_equals(to_string(v), "[-1, 0, 1, 2, 2.5, 3, 4]");

    v.clear();
    v.push_back("0");
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");

    v.insert(v.begin() + 1, 3, "0.5");
    test_equals(to_string(v), "[0, 0.5, 0.5, 0.5, 1, 2, 3]");

    v.insert(v.end(), 3, "4");
    test_equals(to_string(v), "[0, 0.5, 0.5, 0.5, 1, 2, 3, 4, 4, 4]");
}

void test_erase()
{
    vector<std::string> v;
    v.push_back("0");
    v.push_back("1");
    v.push_back("2");
    v.push_back("3");

    v.erase(v.begin());
    test_equals(to_string(v), "[1, 2, 3]");

    v.erase(v.end());
    test_equals(to_string(v), "[1, 2]");

    v.push_back("3");
    v.push_back("4");
    v.push_back("5");
    v.push_back("6");

    v.erase(v.begin(), v.begin() + 2);
    test_equals(to_string(v), "[3, 4, 5, 6]");

    v.erase(v.end() - 2, v.end());
    test_equals(to_string(v), "[3, 4]");
}

void apf_run_tests()
{
    run_test(test_with_to_string);
    run_test(test_the_spy);
    run_test(test_copy);
    run_test(test_clear);
    run_test(test_accessors);
    run_test(test_swap);
    run_test(test_big_list);
    run_test(test_insert);
    run_test(test_erase);
}

