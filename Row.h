#pragma once

#include "Rational.h"

template<unsigned N, typename Field = Rational>
class Row
{
private:
	std::vector<Field> row;

public:
	Row();
	Row(const std::vector<Field>& other);
	Row(std::initializer_list<Field> l);

	Field& operator [](const unsigned& ind);
	Row<N, Field>& operator = (const Row<N, Field>& other); ////&??????????????????Why did you create it?

	Row<N, Field> operator + (const Row<N, Field>& other) const;
	Row<N, Field> operator - (const Row<N, Field>& other) const;

	Row<N, Field>& operator *= (const Field& value);
	Row<N, Field> operator * (const Field& value);

	std::vector<Field> toVector();

	//private:
	const Field& operator [](const unsigned& ind) const;
};


//-----------------------------------------------------------------------------------------------------------------
//--------------------------------REALISATION----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------


template<unsigned N, typename Field>
Row<N, Field>::Row() {
	row.assign(N, Field());
}

template<unsigned N, typename Field>
Row<N, Field>::Row(const std::vector<Field>& other) {
	row = other;
}

template<unsigned N, typename Field>
Row<N, Field>::Row(std::initializer_list<Field> l) {
	row.insert(row.end(), l.begin(), l.end());
}

template<unsigned N, typename Field>
Field& Row<N, Field>::operator[](const unsigned& ind) {
	return row[ind];
}

template<unsigned N, typename Field>
const Field& Row<N, Field>::operator[](const unsigned& ind) const {
	return row[ind];
}


template<unsigned N, typename Field>
Row<N, Field>& Row<N, Field>::operator = (const Row<N, Field>& other) {
	this->row = other.row;
	return *this;
}

template<unsigned N, typename Field>
Row<N, Field> Row<N, Field>::operator + (const Row<N, Field>& other) const {
	Row<N, Field> new_row = *this;

	for (int i = 0; i < other.row.size(); ++i) {
		new_row.row[i] += other.row[i];
	}
	return new_row;
}

template<unsigned N, typename Field>
Row<N, Field> Row<N, Field>::operator - (const Row<N, Field>& other) const {
	Row<N, Field> new_row = *this;

	for (int i = 0; i < other.row.size(); ++i) {
		new_row.row[i] -= other.row[i];
	}
	return new_row;
}

template<unsigned N, typename Field>
Row<N, Field>& Row<N, Field>::operator *= (const Field& value) {
	for (int i = 0; i < (int)row.size(); ++i) {
		row[i] *= value;
	}
	return *this;
}

template<unsigned N, typename Field>
Row<N, Field> Row<N, Field>::operator * (const Field& value) {
	Row<N, Field> new_this = *this;

	new_this *= value;

	return new_this;
}

template<unsigned N, typename Field>
std::vector<Field> Row<N, Field>::toVector() {
	return row;
}
