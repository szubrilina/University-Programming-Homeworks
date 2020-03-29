#pragma once

#include "Row.h"

template<unsigned N, unsigned M, typename Field = Rational>
class Matrix {
private:
	std::vector<Row<M, Field>> mx;

public:
	Matrix();
	Matrix(const Matrix<N, M, Field>& other);
	Matrix(const std::vector<std::vector<Field>>& other);
	Matrix(std::initializer_list<std::initializer_list<Field>> l);

	Matrix<N, M, Field> operator + (const Matrix<N, M, Field>& other) const;
	Matrix<N, M, Field> operator - (const Matrix<N, M, Field>& other) const;

	Matrix<N, M, Field>& operator += (const Matrix<N, M, Field>& other);
	Matrix<N, M, Field>& operator -= (const Matrix<N, M, Field>& other);
	Row<M, Field>& operator [] (const unsigned ind);

	Matrix<N, M, Field>& operator *= (const Field value);
	Matrix<N, M, Field> operator * (const Field value) const;

	bool operator == (const Matrix<N, M, Field>& other) const;
	bool operator != (const Matrix<N, M, Field>& other) const;

	template<unsigned K>
	Matrix<N, K, Field> operator * (const Matrix<M, K, Field>& other) const;

	Matrix<M, N, Field> transposed() const;

	Field det() const;
	Field trace() const;
	Field rank() const;
	std::vector<Field> getRow(unsigned) const;
	std::vector<Field> getColumn(unsigned) const;
	void invert();
	Matrix<N, M, Field> inverted() const;

	std::vector<std::vector<Field>> convert_to_vector() const;

	const Row<M, Field>& operator [] (const unsigned ind) const;

private:
	int difference(const unsigned& A, const unsigned& B) const {
		if (A != B)
			return 1;
		else
			return 0;
	}

	template<bool T, typename TT = int>
	struct checker {
		bool is_N_and_M_equal();
	};

	template<typename TT>
	struct checker<false, TT> {
		bool is_N_and_M_equal() {
			return 1;
		};
	};


public:
	struct response {
		std::vector<std::vector<Field>> vec_elementary_matrix;
		Matrix<N, M, Field> matx_elementary_matrix;
		int rank;
	};

public:
	response calc_Gauss() const;
};


//-----------------------------------------------------------------------------------------------------------------
//--------------------------------REALISATION----------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------

template < unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> operator * (const Field& val, const Matrix<N, M, Field>& other) {
	return other * val;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(const std::vector<std::vector<Field>>& other) {
	this->mx.resize(other.size());

	for (int i = 0; i < other.size(); ++i) {
		this->mx[i] = Row<M, Field>(other[i]);
	}
}


template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(const Matrix<N, M, Field>& other) {
	*this = other;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix() {
	mx.resize(N);
	for (int i = 0; i < N; ++i) {
		mx[i] = Row<M, Field>();
	}
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(std::initializer_list<std::initializer_list<Field>> l) {
	mx.insert(mx.end(), l.begin(), l.end());
}


template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>  Matrix<N, M, Field>::operator + (const Matrix<N, M, Field>& other) const {
	Matrix result = *this;
	result += other;
	return result;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>  Matrix<N, M, Field>::operator - (const Matrix<N, M, Field>& other) const {
	Matrix result = *this;
	result -= other;
	return result;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator += (const Matrix<N, M, Field>& other) {
	for (int i = 0; i < N; ++i) {
		mx[i] += other.mx[i];
	}
	return *this;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator -= (const Matrix<N, M, Field>& other) {
	for (int i = 0; i < N; ++i) {
		mx[i] -= other.mx[i];
	}
	return *this;
}

template<unsigned N, unsigned M, typename Field>
Row<M, Field>& Matrix<N, M, Field>::operator [] (const unsigned ind) {
	return mx[ind];
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator * (const Field value) const {
	Matrix<N, M, Field> new_this = Matrix(*this);

	for (int i = 0; i < N; ++i) {
		new_this.mx[i] *= value;
	}

	return new_this;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator *= (const Field value) {
	for (int i = 0; i < N; ++i) {
		mx[i] *= value;
	}

	return *this;
}

template<unsigned N, unsigned M, typename Field>
template<unsigned K>
Matrix<N, K, Field> Matrix<N, M, Field>::operator * (const Matrix<M, K, Field>& other) const {
	Matrix<N, K, Field>result;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {

			Field sum = Field();
			for (int l = 0; l < K; ++l) {
				sum += mx[i][l] * other.mx[l][j];
			}

			result[i][j] = sum;
		}
	}
	return result;
}


template<unsigned N, unsigned M, typename Field>
bool Matrix<N, M, Field>::operator == (const Matrix<N, M, Field>& other) const {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			if (this->mx[i][j] != other.mx[i][j]) {
				return 0;
			}
		}
	}
	return 1;
}

template<unsigned N, unsigned M, typename Field>
bool Matrix<N, M, Field>::operator != (const Matrix<N, M, Field>& other) const {

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			if (mx[i][j] != other.mx[i][j]) {
				return 1;
			}
		}
	}
	return 0;
}



//const Row<M, Field>& operator [] (const unsigned ind) const;

template<unsigned N, unsigned M, typename Field>
const Row<M, Field>& Matrix<N, M, Field>::operator[](const unsigned ind) const {
	return mx[ind];
}


template<unsigned N, unsigned M, typename Field>
Field Matrix<N, M, Field>::det() const {

	checker<N - M> check;
	if (!check.is_N_and_M_equal()) {
		//CE here!
	}

	Matrix<N, M, Field> new_this = calc_Gauss();
	return new_this.trace();
}

template<unsigned N, unsigned M, typename Field>
Field Matrix<N, M, Field>::rank() const {
	checker <N - M> check;

	if (!check.is_N_and_M_equal()) {
		//CE here!
	}

	response ans = calc_Gauss();
	return ans.rank;
}

template<unsigned N, unsigned M, typename Field>
Field Matrix<N, M, Field>::trace() const {
	checker <N - M> check;

	if (!check.is_N_and_M_equal()) {
		//CE here!
	}

	Field sum = Field();

	for (int i = 0; i < N; ++i) {
		sum += mx[i][i];
	}
	return sum;
}

template<unsigned N, unsigned M, typename Field>
Matrix<M, N, Field> Matrix<N, M, Field>::transposed() const {
	Matrix<M, N, Field> other = Matrix<M, N, Field>();

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			other[j][i] = mx[i][j];
		}
	}
	return other;
}

template<unsigned N, unsigned M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getRow(unsigned ind) const {
	return mx[ind].toVector();
}

template<unsigned N, unsigned M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getColumn(unsigned ind) const {
	Matrix<N, M, Field> new_this = this->transposed();
	return new_this.mx[ind].toVector();
}


template<unsigned N, unsigned M, typename Field>
typename Matrix<N, M, Field>::response Matrix<N, M, Field>::calc_Gauss() const {
	Matrix<N, M, Field> mtrx = *this;
	std::vector<std::vector<Field>> vectr = mtrx.convert_to_vector();

	Matrix<N, M, Field>::response ans;
	ans.rank = Gauss(vectr);
	ans.vec_elementary_matrix = vectr;

	Matrix<N, M, Field> new_mtrx;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			new_mtrx[i][j] = vectr[i][j];
		}
	}

	ans.matx_elementary_matrix = new_mtrx;

	return ans;
}

template<typename T>
std::vector<T> operator * (const std::vector<T>& a, T val) {
	std::vector<T> b = a;
	for (int i = 0; i < (int)b.size(); ++i) {
		b[i] *= val;
	}

	return b;
}

template<typename T>
std::vector<T> operator + (const std::vector<T>& a, const std::vector<T>& b)
{
	std::vector<T> c = a;
	for (int i = 0; i < (int)a.size(); ++i) {
		c[i] = a[i] + b[i];
	}

	return c;
}

template<typename T>
unsigned Gauss(std::vector<std::vector<T>>& mx, unsigned ind = 0)
{
	int n = mx.size();
	int m = mx[0].size();

	bool fl = 0;
	for (int i = ind; i < std::min(n, m); ++i) {
		if (mx[i][ind] != 0) {
			fl = 1;
			std::swap(mx[i], mx[ind]);
			break;
		}
	}

	if (!fl)
		return ind;

	for (int i = ind + 1; i < std::min(n, m); ++i) {
		mx[i] = mx[i] * mx[ind][ind];

		std::vector<T> delta = mx[ind] * mx[i][ind];

		//delta = delta * (-1);
		for (int i = 0; i < (int)delta.size(); ++i)
			delta[i] *= (-1);

		mx[i] = mx[i] + delta;
	}

	return Gauss(mx, ind + 1);
}

template<unsigned N, unsigned M, typename Field>
void Matrix<N, M, Field>::invert() {

	checker<N - M> check;
	if (!check.is_N_and_M_equal()) {
		//CE here!
	}

	std::vector<std::vector<Field>> arr = (*this).convert_to_vector();
	std::vector<std::vector<Field>> new_arr(N, std::vector<Field>(2 * M));

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			new_arr[i][j] = arr[i][j];
		}
	}

	for (int i = 0; i < N; ++i) {
		new_arr[i][M + i] = 1;
	}

	Gauss(new_arr);

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			this->mx[i][j] = new_arr[i][M + j];
		}
	}
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::inverted() const {
	Matrix<N, M, Field> new_this = *this;
	new_this.invert();
	return new_this;
}


template<unsigned N, unsigned M, typename Field>
typename std::vector<std::vector<Field>> Matrix<N, M, Field>::convert_to_vector() const {

	std::vector<std::vector<Field>> vec(N, std::vector<Field>(M));

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			vec[i][j] = this->mx[i][j];
		}
	}

	return vec;
}
