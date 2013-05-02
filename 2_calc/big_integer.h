#pragma once
#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>

struct big_integer
{
	std::vector<short> number;
	bool is_positive;

    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    explicit big_integer(std::string const& str);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;
	
    big_integer operator++();
	big_integer operator++(int);
    
	big_integer operator--();
	big_integer operator--(int);
    
};

big_integer operator-(big_integer const& a, big_integer const& b); 
big_integer operator+(big_integer const& a, big_integer const& b); 
big_integer operator*(big_integer const& a, big_integer const& b); 
big_integer operator/(big_integer const& a, big_integer const& b); 
big_integer operator%(big_integer const& a, big_integer const& b); 

big_integer operator&(big_integer const& a, big_integer const& b); 
big_integer operator|(big_integer const& a, big_integer const& b); 
big_integer operator^(big_integer const& a, big_integer const& b); 

big_integer operator<<(big_integer const& a, int b); 
big_integer operator>>(big_integer const& a, int b); 

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator< (big_integer const& a, big_integer const& b);
bool operator> (big_integer const& a, big_integer const& b); 
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);

#endif // BIG_INTEGER_H