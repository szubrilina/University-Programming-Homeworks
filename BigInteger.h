#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class BigInteger
{
private:
	int _sign = 0;
	std::vector<int> _numbers;

	static const int NEGATIVE = -1;
	static const int POSSITIVE = 1;

public:

	BigInteger();
	BigInteger(const std::string& s);
	BigInteger(const int& data);
	BigInteger(const BigInteger& data);

	std::string toString() const;
	explicit operator bool();

	BigInteger& operator += (const BigInteger& other);
	BigInteger& operator -= (const BigInteger& other);
	BigInteger& operator *= (const BigInteger& other);
	BigInteger& operator /= (const BigInteger& other);
	BigInteger& operator %= (const BigInteger& mode);

	friend bool operator == (const BigInteger& one, const BigInteger& other);
	friend bool operator != (const BigInteger& one, const BigInteger& other);
	friend bool operator < (const BigInteger& one, const BigInteger& other);
	friend bool operator > (const BigInteger& one, const BigInteger& other);
	friend bool operator <= (const BigInteger& one, const BigInteger& other);
	friend bool operator >= (const BigInteger& one, const BigInteger& other);

	friend BigInteger operator + (BigInteger one, const BigInteger& other);
	friend BigInteger operator - (BigInteger one, const BigInteger& other);
	friend BigInteger operator * (BigInteger one, const BigInteger& other);
	friend BigInteger operator / (BigInteger one, const BigInteger& other);
	friend BigInteger operator % (BigInteger one, const BigInteger& mod);

	friend BigInteger operator - (const BigInteger& one);
	friend BigInteger operator + (const BigInteger& one);

	BigInteger& operator ++ ();
	BigInteger& operator -- ();
	BigInteger operator ++ (int);
	BigInteger operator -- (int);

private:
	void _deleteLeadingZeroes();
	BigInteger _abs(const BigInteger& current);
};

//-----------------------------------------------------------------------------------------------------------------
//--------------------------------REALISATION----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

BigInteger::BigInteger()
{
	_sign = POSSITIVE;
	_numbers.push_back(0);
}
BigInteger::BigInteger(const std::string& s)
{
	int start = 0;

	(s[0] == '-' ? this->_sign = NEGATIVE, start = 1 : this->_sign = POSSITIVE);

	for (int i = start; i < (int)s.size(); ++i)
	{
		this->_numbers.push_back(s[i] - '0');
	}
	this->_deleteLeadingZeroes();
}
BigInteger::BigInteger(const int& data)
{
	int value = data;

	_sign = (value < 0 ? NEGATIVE : POSSITIVE);
	value = abs(value);

	if (value == 0)
		_numbers.push_back(0);

	while (value > 0)
	{
		_numbers.push_back(value % 10);
		value /= 10;
	}

	reverse(_numbers.begin(), _numbers.end());
	_deleteLeadingZeroes();
}
BigInteger::BigInteger(const BigInteger& data)
{
	_sign = data._sign;
	_numbers = data._numbers;
}

BigInteger::operator bool()
{
	if (_numbers.size() == 1 && _numbers[0] == 0)
		return 0;
	return 1;
}

//return copies
BigInteger operator + (BigInteger one, const BigInteger& other)
{
	return one += other;
}
BigInteger operator - (BigInteger one, const BigInteger& other)
{
	return one -= other;
}
BigInteger operator * (BigInteger one, const BigInteger& other)
{
	return one *= other;
}
BigInteger operator / (BigInteger one, const BigInteger& other)
{
	return one /= other;
}
BigInteger operator % (BigInteger one, const BigInteger& mod)
{
	return one %= mod;
}

//return *this You can use additional memory
BigInteger& BigInteger::operator += (const BigInteger& other)
{
	if (this->_sign == other._sign)
	{
		std::vector<int> ans;

		int index_This = (int)_numbers.size() - 1;
		int index_Other = (int)other._numbers.size() - 1;

		int ost = 0;
		while (index_This >= 0 && index_Other >= 0)
		{
			ost += _numbers[index_This--] + other._numbers[index_Other--];
			ans.push_back(ost % 10);
			ost /= 10;
		}

		while (index_This >= 0)
		{
			ost += _numbers[index_This--];
			ans.push_back(ost % 10);
			ost /= 10;
		}

		while (index_Other >= 0)
		{
			ost += other._numbers[index_Other--];
			ans.push_back(ost % 10);
			ost /= 10;
		}

		if (ost > 0)
			ans.push_back(ost);

		reverse(ans.begin(), ans.end());

		_numbers = ans;
	}
	else
	{
		BigInteger A = *this;
		A._sign = POSSITIVE;

		BigInteger B = other;
		B._sign = POSSITIVE;

		int ans_sign = (A > B ? this->_sign : other._sign);

		if (A < B)
			std::swap(A, B);

		reverse(A._numbers.begin(), A._numbers.end());
		reverse(B._numbers.begin(), B._numbers.end());

		while (B._numbers.size() < A._numbers.size())
			B._numbers.push_back(0);

		for (int i = 0; i < (int)A._numbers.size(); ++i)
		{
			if (A._numbers[i] < B._numbers[i])
			{
				A._numbers[i] += 10;
				A._numbers[i + 1] -= 1;
			}

			A._numbers[i] -= B._numbers[i];
		}

		reverse(A._numbers.begin(), A._numbers.end());
		A._sign = ans_sign;

		*this = A;
	}

	this->_deleteLeadingZeroes();
	return *this;
}
BigInteger& BigInteger::operator -= (const BigInteger& other)
{
	BigInteger copy = other;
	copy = -copy;
	return (*this += copy);
}
BigInteger& BigInteger::operator *= (const BigInteger& other)
{
	BigInteger mult;
	std::string delta = "";
	for (int i = (int)other._numbers.size() - 1; i >= 0; --i)
	{
		std::string res = delta;
		int ost = 0;

		for (int j = (int)this->_numbers.size() - 1; j >= 0; --j)
		{
			ost += this->_numbers[j] * other._numbers[i];
			res += ('0' + ost % 10);
			ost /= 10;
		}

		res += ('0' + ost);

		reverse(res.begin(), res.end());
		BigInteger current = BigInteger(res);

		mult += current;
		delta += '0';
	}
	mult._sign = this->_sign * other._sign;
	*this = mult;
	this->_deleteLeadingZeroes();
	return *this;
}
BigInteger& BigInteger::operator /= (const BigInteger& other)
{
	BigInteger ans = BigInteger(1);

	for (int i = 0; i < (int)this->_numbers.size(); ++i)
	{
		BigInteger copy = ans;
		copy *= BigInteger(10);

		if (_abs(copy) * _abs(other) <= _abs(*this))
		{
			ans = copy;
		}
		else
		{
			break;
		}
	}
	for (int i = 0; i < (int)ans._numbers.size(); ++i)
	{
		BigInteger copy = ans;

		int l = 0, r = 10;
		while (l < r - 1)
		{
			int integer = (l + r) / 2;

			copy._numbers[i] = integer;

			if (_abs(copy) * _abs(other) <= _abs(*this))
				l = integer;
			else
				r = integer;
		}
		ans._numbers[i] = l;
	}
	this->_numbers = ans._numbers;
	this->_sign *= other._sign;
	this->_deleteLeadingZeroes();
	return *this;
}
BigInteger& BigInteger::operator %= (const BigInteger& mode)
{
	BigInteger div = *this / mode;
	BigInteger result = *this - (mode * div);
	*this = result;
	return *this;
}

//return copies
BigInteger operator - (const BigInteger& one)
{
	return BigInteger(-1) * one;
}
BigInteger operator + (const BigInteger& one)
{
	return one;
}

//return copies of old
BigInteger BigInteger::operator ++ (int)
{
	BigInteger old_this = *this;
	*this += 1;
	return  old_this;
}
BigInteger BigInteger::operator -- (int)
{
	BigInteger old_this = *this;
	*this -= 1;
	return  old_this;
}

//return *this
BigInteger& BigInteger::operator ++ ()
{
	*this += 1;
	return *this;
}
BigInteger& BigInteger::operator -- ()
{
	*this -= 1;
	return *this;
}

bool operator == (const BigInteger& one, const BigInteger& other)
{
	return !(one < other) && !(one > other);
}
bool operator != (const BigInteger& one, const BigInteger& other)
{
	return !(one == other);
}
bool operator < (const BigInteger& one, const BigInteger& other)
{
	if (one._sign != other._sign)
		return one._sign < other._sign;

	bool isNegative = 0;
	if (one._sign == -1) //&& other == NEGATIVE
	{
		isNegative = 1;
	}

	if (one._numbers.size() != other._numbers.size())
		return ((one._numbers.size() < other._numbers.size()) ^ isNegative);

	for (int i = 0; i < (int)one._numbers.size(); ++i)
	{
		if (one._numbers[i] != other._numbers[i])
		{
			return (one._numbers[i] < other._numbers[i]) ^ isNegative;
		}
	}
	return false;
}
bool operator > (const BigInteger& one, const BigInteger& other)
{
	return other < one;
}
bool operator <= (const BigInteger& one, const BigInteger& other)
{
	return (one == other || one < other);
}
bool operator >= (const BigInteger& one, const BigInteger& other)
{
	return (one > other || one == other);
}

std::ostream& operator << (std::ostream& out, BigInteger bigint)
{
	out << bigint.toString();
	return out;
}
std::istream& operator >> (std::istream& in, BigInteger& bigint)
{
	std::string s;
	in >> s;
	bigint = BigInteger(s); //is it correct?
	return in;
}

std::string BigInteger::toString() const
{
	std::string s = "";

	if (this->_sign == NEGATIVE)
		s += '-';

	for (int i = 0; i < (int)this->_numbers.size(); ++i)
		s += (this->_numbers[i] + '0');

	return s;
}
void BigInteger::_deleteLeadingZeroes()
{
	reverse(this->_numbers.begin(), this->_numbers.end());
	while ((int)this->_numbers.size() > 1 && this->_numbers.back() == 0)
		this->_numbers.pop_back();

	if ((int)this->_numbers.size() == 1 && this->_numbers[0] == 0)
		this->_sign = POSSITIVE;

	reverse(this->_numbers.begin(), this->_numbers.end());
}
BigInteger BigInteger::_abs(const BigInteger& current)
{
	BigInteger copy = current;
	copy._sign = 1;
	return copy;
}

