#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include "vector.cpp"

struct big_integer
{
	static const int bits = 32;
	struct copy_on_write
	{
	    myvector data;
	    int count;
	};

	copy_on_write * container;
	void removeLink()
	{
	    if (container)
	    {
	        --container->count;
	        if (container->count <= 0)
	            delete container;
	    }
	}

	void fix();
	int sign() const;
	
	big_integer();
	big_integer(int );
	big_integer(big_integer const& );
	explicit big_integer(std::string const& );
	big_integer& operator = (big_integer);
	~big_integer() { removeLink(); }
	
	big_integer& operator+=(big_integer const& );
	big_integer& operator-=(big_integer const& );
	big_integer& operator*=(big_integer const& );
	big_integer& operator/=(big_integer const& );
	big_integer& operator%=(big_integer const& );
	
	big_integer& operator&=(big_integer const& );
	big_integer& operator|=(big_integer const& );
	big_integer& operator^=(big_integer const& );
	
	big_integer& operator<<=(int );
	big_integer& operator>>=(int );
	
	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;
	
	friend big_integer operator+(big_integer const&, big_integer const&);
	friend big_integer operator-(big_integer const&, big_integer const&);
	friend big_integer operator*(big_integer const&, big_integer const&);
	friend big_integer operator/(big_integer const&, big_integer const&);
	friend big_integer operator%(big_integer const&, big_integer const&);

	friend big_integer operator&(big_integer const&, big_integer const&);
	friend big_integer operator|(big_integer const&, big_integer const&);
	friend big_integer operator^(big_integer const&, big_integer const&);
	
	friend big_integer operator<<(big_integer const&, int );
	friend big_integer operator>>(big_integer const&, int );

	big_integer operator--();
	big_integer operator++();
	
	big_integer operator--(int);
	big_integer operator++(int);

	friend bool operator==(big_integer const&, big_integer const&);
	friend bool operator!=(big_integer const&, big_integer const&);
	friend bool operator< (big_integer const&, big_integer const&);
	friend bool operator> (big_integer const&, big_integer const&);
	friend bool operator<=(big_integer const&, big_integer const&);
	friend bool operator>=(big_integer const&, big_integer const&);
	friend std::pair<big_integer, big_integer> divmod(const big_integer &, const big_integer &);
	friend std::string to_string(big_integer const& );
};

#endif // BIG_INTEGER_H
