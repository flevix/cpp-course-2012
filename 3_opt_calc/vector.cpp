#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "vector.h"

myvector::myvector () : length(0), in_vector(0) { }

void myvector::clear()
{
    if (in_vector)
        data.clear();
    else
        small_data = 0;
    length = 0;
}

void myvector::pop_back()
{
    if (!length) return;
    if (in_vector)
    {
        data.pop_back();
        length--;
        if (length == 1)
        {
            small_data = data[0];
            in_vector = 0;
            data.clear();
        }
    }
    else
    {
        small_data = 0;
        length--;
    }
}

void myvector::push_back(unsigned x)
{
    if (in_vector)
        data.push_back(x);
    else
    {
        if (length == 1)
        {
            data.clear();
            data.push_back(small_data);
            data.push_back(x);
            in_vector = 1;
            small_data = 0;
        }
        else
        {
            small_data = x;
            in_vector = 0;
        }
    }
    length++;
}

unsigned & myvector::front()
{
	return (in_vector) ? data.front() : small_data;
}

unsigned & myvector::back()
{
	return (in_vector) ? data.back() : small_data;
}

unsigned const & myvector::front() const
{
	return (in_vector) ? data.front() : small_data;
}

unsigned const & myvector::back() const
{
	return (in_vector) ? data.back() : small_data;
}

unsigned & myvector::operator [] (int x)
{
	return (in_vector) ? data[x] : small_data;
}

unsigned const & myvector::operator [] (int x) const
{
	return (in_vector) ? data[x] : small_data;
}

bool myvector::empty()
{
	return length > 0;
}

void myvector::resize(int x)
{
    if (in_vector)
        data.resize(x);
    else if (x > 1)
    {
        data.resize(x);
        in_vector = 1;
    }
    length = x;
}

int myvector::size() const
{
	return length;
}
