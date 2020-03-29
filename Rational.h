#pragma once

#include "BigInteger.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Rational
{
private:
	BigInteger _up;
	BigInteger _dn;

private:
	void _reduction();
	int _get_sign() const;
	friend BigInteger _abs(const BigInteger& one);

public:
	Rational();
	Rational(const BigInteger& data);
	Rational(const int& data);

	friend Rational operator - (const Rational& one);

	Rational& operator += (const Rational& other);
	Rational& operator -= (const Rational& other);
	Rational& operator *= (const Rational& other);
	Rational& operator /= (const Rational& other);

	friend Rational operator + (Rational one, const Rational& other);
	friend Rational operator - (Rational one, const Rational& other);
	friend Rational operator * (Rational one, const Rational& other);
	friend Rational operator / (Rational one, const Rational& other);

	friend bool operator == (const Rational& one, const Rational& other);
	friend bool operator != (const Rational& one, const Rational& other);
	friend bool operator < (const Rational& one, const Rational& other);
	friend bool operator > (const Rational& one, const Rational& other);
	friend bool operator <= (const Rational& one, const Rational& other);
	friend bool operator >= (const Rational& one, const Rational& other);

	std::string toString() const;
	std::string asDecimal(size_t precision = 0);

	explicit operator double();
};

//-----------------------------------------------------------------------------------------------------------------
//--------------------------------REALISATION----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

BigInteger _abs(const BigInteger& one);

BigInteger gcd(BigInteger a, BigInteger b)
{
	return (b == 0 ? a : gcd(b, a % b));
}
void Rational::_reduction()
{
	BigInteger C = gcd(_abs(_up), _abs(_dn));

	_up /= C;
	_dn /= C;
}
int Rational::_get_sign() const
{
	BigInteger A = _up;
	BigInteger B = _dn;

	int sign = 1;
	if (A < 0)
		sign *= -1;
	if (B < 0)
		sign *= -1;

	return sign;
}
BigInteger _abs(const BigInteger& one)
{
	BigInteger other = one;

	if (one < 0)
		other *= (-1);
	return other;
}

Rational::Rational()
{
	_up = 1;
	_dn = 1;
}
Rational::Rational(const int& data)
{
	_up = BigInteger(data);
	_dn = 1;
}
Rational::Rational(const BigInteger& data)
{
	_up = data;
	_dn = 1;
}

BigInteger pow(const BigInteger& one, int p)
{
	if (p == 0)
		return BigInteger(1);
	if (p % 2 == 0)
	{
		BigInteger res = pow(one, p / 2);
		return res * res;
	}
	else
	{
		BigInteger res = pow(one, p - 1);
		return res * one;
	}
}

std::string Rational::asDecimal(size_t precision)
{
	std::string integer_part;

	BigInteger A = _up;
	BigInteger B = _dn;

	if (*this < Rational(0))
		integer_part = "-" + integer_part;

	A = _abs(A);
	B = _abs(B);

	BigInteger C = A / B;
	integer_part += C.toString();

	A -= B * C;
	A *= pow(BigInteger(10), precision);

	A /= B;

	std::string fractional_part = A.toString();
	while (fractional_part.size() < precision)
		fractional_part = "0" + fractional_part;
	while (fractional_part.size() > 1 && fractional_part.back() == '0')
		fractional_part.pop_back();

	fractional_part = '.' + fractional_part;

	std::string result = integer_part + fractional_part;
	return result;
	return "";
}


Rational::operator double()
{
	std::string current = asDecimal(324);
	double result = std::stod(current);
	return result;
}

Rational operator - (const Rational& one)
{
	return Rational(-1) * one;
}

std::string Rational::toString() const
{
	std::string result = _up.toString();

	if (_dn != 1)
	{
		result += '/';
		result += _dn.toString();
	}

	return result;
}

//return copies
Rational operator + (Rational one, const Rational& other)
{
	return one += other;
}
Rational operator - (Rational one, const Rational& other)
{
	return one -= other;
}
Rational operator * (Rational one, const Rational& other)
{
	return one *= other;
}
Rational operator / (Rational one, const Rational& other)
{
	return one /= other;
}

//return *this You can use additional memory
Rational& Rational::operator += (const Rational& other)
{
	BigInteger new_up = _up * other._dn + other._up * _dn;
	BigInteger new_dn = _dn * other._dn;

	this->_up = new_up;
	this->_dn = new_dn;

	this->_reduction();

	return  *this;
}
Rational& Rational::operator -= (const Rational& other)
{
	Rational copy = other;
	copy = -copy;
	return (*this += copy);
}
Rational& Rational::operator *= (const Rational& other)
{
	BigInteger new_up = _up * other._up;
	BigInteger new_dn = _dn * other._dn;

	this->_up = new_up;
	this->_dn = new_dn;

	this->_reduction();

	return *this;
}
Rational& Rational::operator /= (const Rational& other)
{
	BigInteger new_up = _abs(_up) * _abs(other._dn);
	BigInteger new_dn = _abs(_dn) * _abs(other._up);

	if (_up * other._up * _dn * other._dn < 0)
		new_up *= BigInteger(-1);

	this->_up = new_up;
	this->_dn = new_dn;

	this->_reduction();

	return *this;
}


bool operator == (const Rational& one, const Rational& other)
{
	return !(one < other) && !(one > other);
}
bool operator != (const Rational& one, const Rational& other)
{
	return !(one == other);
}
bool operator < (const Rational& one, const Rational& other)
{
	return one._up * other._dn < other._up * one._dn;
}
bool operator > (const Rational& one, const Rational& other)
{
	return other < one;
}
bool operator <= (const Rational& one, const Rational& other)
{
	return (one == other || one < other);
}
bool operator >= (const Rational& one, const Rational& other)
{
	return (one > other || one == other);
}

