#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

template<unsigned P>
class Finite
{
private:
	unsigned value;
	unsigned module;

public:
	Finite() {
		value = 0;
		module = P;
	}

	Finite(int x) {
		module = P;
		value = x % module;
	}

	Finite<P>& operator += (const Finite<P>& other);
	Finite<P>& operator -= (const Finite<P>& other);
	Finite<P>& operator *= (const Finite<P>& other);
	Finite<P>& operator /= (const Finite<P>& other); //If p - prime return other such that other * this = 1 mod(P) else CE

	bool operator != (const Finite<P>& other);
	bool operator == (const Finite<P>& other);
	bool operator < (const Finite<P>& other);
	bool operator <= (const Finite<P>& other);
	bool operator > (const Finite<P>& other);
	bool operator >= (const Finite<P>& other);

	Finite<P>& operator ++ ();
	Finite<P>& operator -- ();
	Finite<P> operator ++ (int);
	Finite<P> operator -- (int);

	Finite<P> operator + (const Finite<P>& other) const;
	Finite<P> operator - (const Finite<P>& other) const;
	Finite<P> operator * (const Finite<P>& other) const;
	Finite<P> operator / (const Finite<P>& other) const; //If p - prime return other such that other * this = 1 mod(P) else CE

	Finite<P> pow(const Finite<P> a, const unsigned k); //log(k) complexity
	Finite<P> getInverse(); //If p - prime return other such that other * this = 1 mod(P) else CE



private:

	template<int N, int D>
	struct isDivaded {
		static const bool result = (N % D) && isDivaded<N, D - 1>::result;
	};

	template<int N>
	struct isDivaded <N, 1> {
		static const bool result = true;
	};

	struct Prime {
		static const bool result = isDivaded<P, P - 1>::result;
	};

	template<bool M, typename T = int>
	struct cheker {
		void isPrime();
	};

	template<typename T>
	struct cheker<true, T> {
		void isPrime() {};
	};
};


//-----------------------------------------------------------------------------------------------------------------
//--------------------------------REALISATION----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


template<unsigned P>
Finite<P>& Finite<P>::operator += (const Finite<P>& other) {
	value += other.value;
	value = ((value % module) + module) % module;
	return *this;
}

template<unsigned P>
Finite<P>& Finite<P>::operator -= (const Finite<P>& other) {
	value += (module - other.value);
	value %= module;
	return *this;
}

template<unsigned P>
Finite<P>& Finite<P>::operator *= (const Finite<P>& other) {
	value *= other.value;
	value = ((value % module) + module) % module;
	return *this;
}

template<unsigned P>
Finite<P>& Finite<P>::operator /= (const Finite<P>& other) {
	value *= other.getInverse();
	return *this;
}

template<unsigned P>
Finite<P> Finite<P>::pow(const Finite<P> a, const unsigned k) {
	if (k == 0) {
		return Finite<P>();
	}
	if (k % 2 == 0) {
		Finite<P> cur = pow(a, k / 2);
		cur *= cur;

		return cur;
	}
	else {
		Finite<P> cur = pow(a, k - 1);
		cur *= a;

		return cur;
	}
}

template<unsigned P>
Finite<P> Finite<P>::getInverse() {
	cheker<Prime::result> check;
	check.isPrime();

	return pow(*this, module - 2);
}

template<unsigned P>
Finite<P> Finite<P>::operator + (const Finite<P>& other) const {
	Finite<P> copy = *this;
	copy += other;
	return copy;
}
template<unsigned P>
Finite<P> Finite<P>::operator - (const Finite<P>& other) const {
	Finite<P> copy = *this;
	copy -= other;
	return copy;
}
template<unsigned P>
Finite<P> Finite<P>::operator * (const Finite<P>& other) const {
	Finite<P> copy = *this;
	copy *= other;
	return copy;
}
template<unsigned P>
Finite<P> Finite<P>::operator / (const Finite<P>& other) const {
	Finite<P> copy = *this;
	copy /= other;
	return copy;
}


template<unsigned P>
Finite<P> Finite<P>::operator ++ (int) {
	Finite<P> old_this = *this;
	*this += 1;
	return  old_this;
}

template<unsigned P>
Finite<P> Finite<P>::operator -- (int) {
	Finite<P> old_this = *this;
	*this -= 1;
	return  old_this;
}

template<unsigned P>
Finite<P>& Finite<P>::operator ++ () {
	*this += 1;
	return *this;
}

template<unsigned P>
Finite<P>& Finite<P>::operator -- () {
	*this -= 1;
	return *this;
}

template<unsigned P>
bool Finite<P>::operator != (const Finite<P>& other) {
	return value != other.value;
}

template<unsigned P>
bool Finite<P>::operator == (const Finite<P>& other) {
	return value == other.value;
}

template<unsigned P>
bool Finite<P>::operator < (const Finite<P>& other) {
	return value < other.value;
}

template<unsigned P>
bool Finite<P>::operator <= (const Finite<P>& other) {
	return value <= other.value;
}

template<unsigned P>
bool Finite<P>::operator > (const Finite<P>& other) {
	return value > other.value;
}

template<unsigned P>
bool Finite<P>::operator >= (const Finite<P>& other) {
	return value >= other.value;
}


