#include "big_integer.h"
using namespace std;


big_integer::big_integer() {
	is_positive = 1;
	number.push_back(0);
}

big_integer::big_integer(big_integer const& other){
	is_positive = other.is_positive;
	number = other.number;
}

big_integer::big_integer(int a) {
	is_positive = (a >= 0);
	a = abs(a);
	if (a == 0) {
		number.push_back(0);
		return;
	}
	while (a > 0) {
		number.push_back(a & 1);
		a >>= 1;
	}
}

big_integer::big_integer(string const& str)
{
	vector<short> d[2];
	is_positive = 1;
	int pos = 0;
	if (str[0] == '-') {
		pos++;
		while (pos < (int) str.size() - 1 && str[pos] == '0')
			pos++;
		if (str[pos] != '0')
			is_positive = 0;
	}
	for (int i = pos; i < (int) str.size(); i++) {
		d[0].push_back(str[i] - '0');
		d[1].push_back(0);
	}
	int i = 0, a, b;
	pos = d[0].size() - 1;
	short carry;
	while (pos || d[i][0] > 1) {
		if (d[i][pos] & 1)
			number.push_back(1);
		else
			number.push_back(0);
		a = 0;
		carry = 0;
		if (d[i][a] < 2) {
			carry *= 10;
			carry += d[i][a];
			a++;
		}
		b = 0;
		while (a <= pos) {
			carry *= 10;
			carry += d[i][a];
			d[1 ^ i][b] = carry >> 1;
			carry -= d[1 ^ i][b] << 1;
			a++;
			b++;
		}
		i ^= 1;
		pos = b - 1;
	}
	if (d[i][0] || !number.size())
		number.push_back(d[i][0]);
}
// сравнение по модулю
short compare(big_integer const &a, big_integer const& b) {
	if (a.number.size() > b.number.size())
		return 1;
	if (a.number.size() < b.number.size())
		return 2;
	if (a.number.size() == b.number.size()) {
		for (int i = a.number.size() - 1; i >= 0; i--) {
			if (a.number[i] > b.number[i]) 
				return 1;
			if (a.number[i] < b.number[i])
				return 2;
		}
	}
	return 0;
}
// сравнение с учётом знака
short comp(big_integer const &a, big_integer const& b) {
	short temp = 0;
	if (a.is_positive && !b.is_positive)
		return 1;
	if (!a.is_positive && b.is_positive)
		return 2;
	if (a.is_positive == b.is_positive) {
		if (a.number.size() > b.number.size())
			temp = 1;
		else if (a.number.size() < b.number.size())
			temp = 2;
		else if (a.number.size() == b.number.size()) {
			for (int i = a.number.size() - 1; i >= 0; i--) {
				if (a.number[i] > b.number[i]) {
					temp = 1;
					break;
				}
				if (a.number[i] < b.number[i]) {
					temp = 2;
					break;
				}
			}
			if (temp == 0)
				return 0;
		}
		if (a.is_positive)
			return temp;
		else
			return (temp == 1) ? 2 : 1;
	}
	return 0;
}

//перевод в дополнительный код
big_integer change(big_integer a, int new_lenght) {
	bool positive = a.is_positive;
	a.is_positive = 1;
	if (!positive) {
		for (int i = 0; i < (int) a.number.size(); i++)
			a.number[i] ^= 1;
		a = a + 1;
	}	
	while((int) a.number.size() != new_lenght)
		a.number.push_back(!positive);
	return a;
}

//перевод в обратный код
big_integer change(big_integer a) { 
	short sign = a.number.back();
	if (sign == 1) {
		for (int i = 0; i < (int) a.number.size(); i++)
			a.number[i] ^= 1;
		a = a + 1;
	}
	while (a.number.size() > 1 && !a.number.back())
		a.number.pop_back();
	if (a != 0)
		a.is_positive = !sign;
	else
		a.is_positive = 1;
	return a;
}

void add(big_integer& ans, big_integer const& b, int shift) {
	short carry = 0;
	int len = ans.number.size() - shift;
	if (ans.number.size() < b.number.size() + shift)
		ans.number.resize(b.number.size() + shift);
	for (int i = 0; i < (int) ans.number.size() - shift; i++) {
		carry = carry + ((i < len) ? ans.number[i + shift] : 0) + ((i < (int) b.number.size()) ? b.number[i] : 0);
		ans.number[i + shift] = carry & 1;
		carry >>= 1;
	}
	if (carry)
		ans.number.push_back(carry);
}

void sub(big_integer& ans, big_integer const& b) {
	short carry = 0;
	for (int i = 0; i < (int) b.number.size() || carry; ++i) {
		ans.number[i] -= carry + (i < (int) b.number.size() ? b.number[i] : 0);
		carry = ans.number[i] < 0;
		if (carry)
			ans.number[i] += 2;
	}
}

void divide(big_integer& a, big_integer const& b, big_integer& mod) {
	if (compare(b, 0) == 0)
		throw runtime_error("divide by zero =(");
	if (compare(a , b) == 2) {
		mod = a;
		a = 0;
		return;
	}
	int left, right;
	big_integer ans;
	ans.number.clear();
	right = a.number.size() - 1;
	left = right - b.number.size() + 1;
	big_integer x;
	x.number.clear();
	for(int i = left; i <= right; i++)
		x.number.push_back(a.number[i]);
	x.number.assign(a.number.begin() + left, a.number.begin() + right + 1);
	
	if (compare(x, b) == 2)
		--left;
	while (left >= 0) {
		x.number.clear();
		for(int i = left; i <= right; i++)
			x.number.push_back(a.number[i]);
		
		if (!a.number[right] || compare(x, b) == 2) {
			ans.number.push_back(0);
			if (!a.number[right]) {
				a.number.pop_back();
				--right;
			}
			--left;
		} else {
			ans.number.push_back(1);
			sub(x, b);
			for (int i = 0; i < (int) x.number.size(); i++)
				a.number[i + left] = x.number[i];
			--left;
			while (right >= 0 && right > left && !a.number[right]) {
				a.number.pop_back();
				--right;
			}
		}
	}
	mod = a;
	a = ans;
	if (!mod.number.size())
		mod.number.push_back(0);
	reverse(a.number.begin(), a.number.end());
}

big_integer operator+(big_integer const& a, big_integer const& b) {
	big_integer ans(0);
	if (a.is_positive == b.is_positive) {
		ans = a;
		add(ans, b, 0);
	} else {
		if (compare(a, b) < 2) {
			ans = a;
			sub(ans, b);
		} else {
			ans = b;
			sub(ans, a);
		}
		while (ans.number.size() > 1 && ans.number.back() == 0)
			ans.number.pop_back();
	}
	return ans;
}

big_integer operator-(big_integer const& a, big_integer const& b) {
	big_integer ans(0);
	if (a.is_positive != b.is_positive) {
		ans = a;
		add(ans, b, 0);
	} else {
		if (compare(a, b) < 2) {
			ans = a;
			sub(ans, b);
		} else {
			ans = b;
			ans.is_positive = !ans.is_positive;
			sub(ans, a);
		}
	}
	while (ans.number.size() > 1 && ans.number.back() == 0)
		ans.number.pop_back();
	return ans;
}

big_integer operator*(big_integer const& a, big_integer const& b) {
	big_integer ans(0);
	if (compare(a, 0) != 0 && compare(b, 0) != 0) {
		for (int i = 0; i < (int) b.number.size(); i++)
			if (b.number[i])
				add(ans, a, i);
		ans.is_positive = !(a.is_positive ^ b.is_positive);
	}
	return ans;
}

big_integer operator/(big_integer const& a, big_integer const& b) {
	if (compare(a, 0) == 0)
		return 0;
	big_integer ans(a), mod(0);
	ans.is_positive = 1;
	divide(ans, b, mod);
	ans.is_positive = !(a.is_positive ^ b.is_positive);
	return ans;
}

big_integer operator%(big_integer const& a, big_integer const& b) {
	if (compare(a, 0) == 0)
		return 0;
	big_integer ans(a), mod(0);
	divide(ans, b, mod);
	mod.is_positive = compare(mod, 0) == 0 ? 1 : a.is_positive;
	return mod;
}

big_integer operator&(big_integer const& a, big_integer const& b) {
	int maxl = (max(a.number.size(), b.number.size()) + 1);
	big_integer a2 = change(a, maxl);
	big_integer b2 = change(b, maxl);
	for (int i = 0; i < maxl; i++)
		a2.number[i] &= b2.number[i];
	return change(a2);
}

big_integer operator|(big_integer const& a, big_integer const& b) {
	int maxl = (max(a.number.size(), b.number.size()) + 1);
	big_integer a2 = change(a, maxl);
	big_integer b2 = change(b, maxl);
	for (int i = 0; i < maxl; i++)
		a2.number[i] |= b2.number[i];
	return change(a2);
}



big_integer operator^(big_integer const& a, big_integer const& b) {
	int maxl = (max(a.number.size(), b.number.size()) + 1);
	big_integer a2 = change(a, maxl);
	big_integer b2 = change(b, maxl);
	for (int i = 0; i < maxl; i++)
		a2.number[i] ^= b2.number[i];
	return change(a2);
}

big_integer operator<<(big_integer const& a, int b) { // '*2
	if (b < 0)
		throw runtime_error("negative shift");
	big_integer ans(0);
	for (int i = 1; i < b; i ++)
		ans.number.push_back(0);
	for (int i = 0; i < (int) a.number.size(); i++)
		ans.number.push_back(a.number[i]);
	ans.is_positive = a.is_positive;
	return ans;
}
big_integer operator>>(big_integer const& a, int b) { // '/2
	if (b < 0)
		throw runtime_error("negative shift");
	if (b > (int) a.number.size())
		return 0;
	big_integer ans(0);
	ans.number[0] = a.number[b++];
	for (int i = b; i < (int) a.number.size(); i++)
		ans.number.push_back(a.number[i]);
	ans.is_positive = a.is_positive;
	return ans;
}

bool operator==(big_integer const& a, big_integer const& b) {
	return comp(a, b) == 0;
}

bool operator!=(big_integer const& a, big_integer const& b) {
	return comp(a, b) != 0;
}

bool operator< (big_integer const& a, big_integer const& b) {
	return comp(a, b) == 2;
}

bool operator> (big_integer const& a, big_integer const& b) {
	return comp(a, b) == 1;
}

bool operator<=(big_integer const& a, big_integer const& b) {
	short tmp = comp(a, b);
	return (tmp == 2 || tmp == 0);
}

bool operator>=(big_integer const& a, big_integer const& b) {
	short tmp = comp(a, b);
	return (tmp == 1 || tmp == 0);
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
	*this = *this + rhs;
	return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
	*this = *this - rhs;
	return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
	*this = *this * rhs;
	return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
	*this = *this / rhs;
	return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
	*this = *this % rhs;
	return *this;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
	*this = *this & rhs;
	return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
	*this = *this | rhs;
	return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
	*this = *this ^ rhs;
	return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
	*this = *this >> rhs;
	return *this;
}

big_integer& big_integer::operator<<=(int rhs) {
	*this = *this << rhs;
	return *this;
}

big_integer big_integer::operator-() const {
	big_integer tmp = *this;
	if (compare(*this, 0) != 0)
		tmp.is_positive ^= 1;
	return tmp;
}

big_integer big_integer::operator+() const {
	return *this;
}

big_integer big_integer::operator~() const {
	return -(*this) - 1;
}

big_integer big_integer::operator++() {
	operator+=(1);
	return *this;
}

big_integer big_integer::operator++(int) {
	return operator++();
}

big_integer big_integer::operator--() {
	operator-=(1);
	return *this;
}

big_integer big_integer::operator--(int) {
	return operator--();
}

string to_string(big_integer const& a)
{
	vector<short> extent, data;
	extent.push_back(1);
	data.push_back(a.number[0]);
	for (int i = 1; i < (int) a.number.size(); i++) {
		short carry = 0;
		for (int j = 0; j < (int) extent.size(); j++) {
			extent[j] = (extent[j] << 1) + carry;
			carry = extent[j] / 10;
			extent[j] %= 10;
		}
		if (carry > 0)
			extent.push_back(carry);
		if (a.number[i]) {
			carry = 0;
			data.resize(extent.size());
			for (int j = 0; j < (int) extent.size(); j++) {
				data[j] += carry + extent[j];
				carry = data[j] / 10;
				data[j] %= 10;
			}
			if (carry > 0)
				data.push_back(carry);
		}
	}
	
	string s;
	if (a.is_positive)
		s.resize(data.size());
	else {
		s.resize(data.size() + 1);
		s[0] = '-';
	}
	for (int i = 0; i < (int) data.size(); i++) {
		s[s.size() - i - 1] = data[i] + '0';
	}
	return s;
}