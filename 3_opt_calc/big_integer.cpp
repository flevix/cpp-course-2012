#include "big_integer.h"
using namespace std;

big_integer::big_integer()
{
    container = new copy_on_write();
    container->count = 1;
}

big_integer::big_integer(int x)
{
    container = new copy_on_write();
    container->count = 1;
    if (!x)
    {
        container->data.push_back(0);
        return;
    }
    container->data.push_back(x);
}

big_integer::big_integer(big_integer const& x)
{
    container = x.container;
    container->count++;
}

big_integer::big_integer(string const& s)
{
    big_integer value = 0;
    for (int i = 0 + (s[0] == '-'); i < (int) s.size(); i++)
        value = value * 10 + (s[i] - '0');
    if (s[0] == '-')
        value = -value;
    value.fix();
    *this = value;
}

big_integer& big_integer::operator=(big_integer x)
{
    removeLink();
    container = x.container;
    container->count++;
    return *this;
}

bool operator< (big_integer const& a, big_integer const& b)
{
	if (a.container->data.size() != b.container->data.size())
        return (int) a.container->data.size() * a.sign() < (int) b.container->data.size() * b.sign();

    for (int i = a.container->data.size() - 1; i >= 0; i--)
        if (a.container->data[i] != b.container->data[i])
            return a.container->data[i] * a.sign() < b.container->data[i] * b.sign();

    return false;
}

bool operator> (big_integer const& a, big_integer const& b){ return b < a; }
bool operator>=(big_integer const& a, big_integer const& b){ return !(a < b); }
bool operator<=(big_integer const& a, big_integer const& b){ return !(b < a); }
bool operator!=(big_integer const& a, big_integer const& b){ return a < b || b < a; }
bool operator==(big_integer const& a, big_integer const& b){ return !(a < b) && !(b < a); }

big_integer operator>>(big_integer const& a, int x) {
    big_integer res;
    res.container->data.clear();
    int before = 0, after;
    int k = x % a.bits;
    int t = x / a.bits;
    res.container->data.resize(a.container->data.size() - (t));
    for (int i = a.container->data.size() - 1; i >= 0; i--)
    {
        after = before;
        before = (( (unsigned) (-1) >> (a.bits - k)) & a.container->data[i]) << (a.bits - k);
        res.container->data[t - i] = (a.container->data[i] >> k) | after;
    }
    if (k)
        res.container->data[a.container->data.size() - t] |= before;
    if (a.sign() == -1)
        res.container->data.back() |= ((unsigned) (-1) << (a.bits - k));
    res.fix();
    return res;
}

big_integer operator<<(big_integer const& a, int x)
{
    big_integer res;
    res.container->data.clear();
    int before = 0, after;
    int k = x % a.bits;
    int t = x / a.bits;
    res.container->data.resize(a.container->data.size() + (t) + 1);
    for (int i = 0; i < (int) a.container->data.size(); i++)
    {
        after = before;
        before = (( (unsigned) (-1) << (a.bits - k)) & a.container->data[i]) >> (a.bits - k);
        res.container->data[t + i] = (a.container->data[i] << k) | after;
    }
    if (k)
        res.container->data[a.container->data.size() + t] |= before;
    if (a.sign() == -1)
        res.container->data.back() |= ((unsigned) (-1) << (k));
    res.fix();
    return res;
}

big_integer and_or_xor(big_integer const& a, big_integer const& b, char v)
{
    big_integer res;
    for (int i = 0; i < (int) max(a.container->data.size(), b.container->data.size()); i++)
    {
        int m, n;
        if (i >= (int) a.container->data.size())
        {
            if ((a.container->data.size()) == 1)
            	m = 0;
            else
            	m = -1;
        }else m = a.container->data[i];
        if (i >= (int) b.container->data.size())
        {
            if ((b.container->data.size()) == 1)
                n = 0;
            else
                n = -1;
        }else n = b.container->data[i];
        if (v == '&')
	        res.container->data.push_back (m & n);
		else if (v == '|')
	        res.container->data.push_back (m | n);
		else
			res.container->data.push_back (m ^ n);

    }
    res.fix();
    return res;
}

big_integer operator^(big_integer const& a, big_integer const& b){ return and_or_xor(a, b, '^'); }
big_integer operator|(big_integer const& a, big_integer const& b){ return and_or_xor(a, b, '|'); }
big_integer operator&(big_integer const& a, big_integer const& b){ return and_or_xor(a, b, '&'); }
big_integer operator%(big_integer const& a, big_integer const& b){ return divmod(a, b).second; }
big_integer operator/(big_integer const& a, big_integer const& b){ return divmod(a, b).first; }

pair<big_integer, big_integer> divmod(const big_integer &a1, const big_integer &b1)
{
    big_integer div = 0, mod = 0;
    big_integer a, b, x = 0;
    a = a1, b = b1;
    if (a1.sign() < 0) a = -a;
    if (b1.sign() < 0) b = -b;
    for (int i = a.container->data.size() - 1; i >= 0; i--)
        for (int j = a.bits - 1; j >= 0; j--)
        {
            div <<= 1, x <<= 1;
            x += ((a.container->data[i] & (1 << j))) >> j;
            if (x >= b)
                div += 1, x -= b;
        }
    mod = x;
    if ((a1.sign() * b1.sign()) == -1)
    {
        div = -div;
        if (a1.sign() < 0) mod = -mod;
    }
    div.fix();
    mod.fix();
    return make_pair(div, mod);
}

big_integer operator+(big_integer const& a, big_integer const& b)
{
    big_integer res;
    long long carry = 0;
    int len = (int) max(a.container->data.size(), b.container->data.size());
    for (int i = 0; i < len; i++)
    {
        unsigned m = 0, n = 0;
        if (i < (int) a.container->data.size() && i < (int) b.container->data.size())
        {
            m = a.container->data[i];
            n = b.container->data[i];
        }
        else
        {
            if (i >= (int) a.container->data.size())
            {
                m = b.container->data[i];
                if (a.sign() == -1) n = (unsigned) -1;
            }
            else
            {
                m = a.container->data[i];
                if (b.sign() == -1) n = (unsigned) -1;
            }
        }
        res.container->data.push_back((m + n + carry) & (unsigned) (-1));
        carry = (((long long)m + n + carry) >> (a.bits));
    }
    if ((a.sign() == b.sign()))
    {
        if (res.sign() != a.sign())
            if (a.sign() == -1)
                res.container->data.push_back((unsigned) (-1));
            else
                res.container->data.push_back((unsigned) (0));
    }
    res.fix();
    return res;
}

big_integer operator*(big_integer const& a, big_integer const& b)
{
    big_integer x = a, y = b, res = 0;
    if (x.sign() == -1)
        x = -x;
    if (y.sign() == -1)
        y = -y;
    for (int i = 0; i < (int) x.container->data.size(); i++)
        for (int j = 0; j < a.bits; j++)
        {
            if (x.container->data[i] & (1 << j))
                res += y;
            y <<= 1;
        }
    if ((a.sign() * b.sign()) == -1)
        res = -res;
    res.fix();
    return res;
}


big_integer big_integer::operator-() const
{
    big_integer x = *this;
    if (*this != 0)
    	x = ~(x) + 1;
    x.fix();
    return x;
}

big_integer big_integer::operator~() const
{
    big_integer t;
    t.container->data.resize(container->data.size());
    for (int i = 0; i < (int) container->data.size(); i++)
        t.container->data[i] = ~container->data[i];
    t.fix();
    return t;
}

int big_integer::sign() const { return ((container->data.back() >> (bits - 1)) == 1) ? -1 : 1; }

big_integer big_integer::operator+() const { return *this; }
big_integer operator-(big_integer const& a, big_integer const& v){ return (a + -(v)); }

big_integer& big_integer::operator+=(big_integer const& x)
{
	return *this = *this + x;
}

big_integer& big_integer::operator-=(big_integer const& x)
{
	return *this = *this - x;
}

big_integer& big_integer::operator*=(big_integer const& x)
{
	return *this = *this * x;
}

big_integer& big_integer::operator/=(big_integer const& x)
{
	return *this = *this / x;
}

big_integer& big_integer::operator%=(big_integer const& x)
{
	return *this = *this % x;
}

big_integer& big_integer::operator&=(big_integer const& x)
{
	return *this = *this & x;
}

big_integer& big_integer::operator|=(big_integer const& x)
{
	return *this = *this | x;
}

big_integer& big_integer::operator^=(big_integer const& x)
{
	return *this = *this ^ x;
}

big_integer& big_integer::operator>>=(int x)
{
	return *this = *this >> x;
}

big_integer& big_integer::operator<<=(int x)
{
	return *this = *this << x;
}

big_integer big_integer::operator++()
{
	return *this += 1;
}

big_integer big_integer::operator--()
{
	return *this -= 1;
}

big_integer big_integer::operator++(int)
{
	return operator++();
}

big_integer big_integer::operator--(int)
{
	return operator--();
}

void big_integer::fix()
{
    while (container->data.size() > 1 && container->data.back() == 0 && (((container->data[container->data.size() - 2]) >> (bits - 1)) == 0))
        container->data.pop_back();

    while (container->data.size() > 1 && container->data.back() == (unsigned)(-1) && ((container->data[container->data.size() - 2] >> (bits - 1)) == 1))
        container->data.pop_back();
}

string to_string(big_integer const& a)
{
    if (a == 0)
    	return "0";

    int x, f = 0;
    string s = "";
    big_integer temp = a;
    if (a.sign() == -1)
        f = 1, temp = -temp;
    while (temp > 0)
    {
        big_integer m = (temp % 10);
        x = m.container->data[0];
        temp /= 10;
        stringstream ss;
        ss << x;
        s += ss.str();
    }
    if (f)
        s += "-";
    reverse(s.begin(), s.end());
    return s;
}

int main() {

}
