/**
 * \file   Matrix.h
 * \author Christof Rezk-Salama
 * \class  Matrix
 * \brief  Stores a 4x4 matrix and provides appropriate operations.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 * Instances of this class are used in several other classes for simple matrix operations.
 */

#ifndef __Matrix_h
#define __Matrix_h

class Vector;

class  Matrix
{
public:

private:
	double value_[4][4];	// [row][column]
	// (0,0) (0,1) (0,2) (0,3)
	// (1,0) (1,1) (1,2) (1,3)
	// (2,0) (2,1) (2,2) (2,3)
	// (3,0) (3,1) (3,2) (3,3)

public:
	Matrix(
		);

	Matrix(
		const Matrix& matrix_in
		);

	Matrix(
		const double& a00_in,
		const double& a01_in,
		const double& a02_in,
		const double& a03_in,
		const double& a10_in,
		const double& a11_in,
		const double& a12_in,
		const double& a13_in,
		const double& a20_in,
		const double& a21_in,
		const double& a22_in,
		const double& a23_in,
		const double& a30_in,
		const double& a31_in,
		const double& a32_in,
		const double& a33_in
		);

	Matrix(
		const double a_in[16]
		);

	Matrix&
	operator=(
		const Matrix& matrix_in
		);

	Vector
	operator[](
		const unsigned short rowIndex_in
		) const;

	double&
	operator()(
		const unsigned short rowIndex_in,
		const unsigned short colIndex_in
		);

	double
	operator()(
		const unsigned short rowIndex_in,
		const unsigned short colIndex_in
		) const;

	void
	set(
		const double& a00_in,
		const double& a01_in,
		const double& a02_in,
		const double& a03_in,
		const double& a10_in,
		const double& a11_in,
		const double& a12_in,
		const double& a13_in,
		const double& a20_in,
		const double& a21_in,
		const double& a22_in,
		const double& a23_in,
		const double& a30_in,
		const double& a31_in,
		const double& a32_in,
		const double& a33_in
		);

	void
	get(
		double& a00_out,
		double& a01_out,
		double& a02_out,
		double& a03_out,
		double& a10_out,
		double& a11_out,
		double& a12_out,
		double& a13_out,
		double& a20_out,
		double& a21_out,
		double& a22_out,
		double& a23_out,
		double& a30_out,
		double& a31_out,
		double& a32_out,
		double& a33_out
		) const;

	void
	setZeroMatrix(
		);

	static Matrix
	zeroMatrix(
		);

	void
	setIdentityMatrix(
		);

	static Matrix
	identityMatrix(
		);


	bool
	operator==(
		const Matrix& matrix_in
		) const;

	bool
	operator!=(
		const Matrix& matrix_in
		) const;

	Matrix
	operator-(
		) const;

	friend  Matrix
	operator-(
		const Matrix& matrix0_in,
		const Matrix& matrix1_in
		);

	friend  Matrix
	operator+(
		const Matrix& matrix0_in,
		const Matrix& matrix1_in
		);

	friend  Matrix
	operator*(
		const Matrix& matrix0_in,
		const Matrix& matrix1_in
		);

	friend  Matrix
	operator*(
		const double& scalar_in,
		const Matrix& matrix_in
		);

	friend  Matrix
	operator*(
		const Matrix& matrix_in,
		const double& scalar_in
		);

	friend  Vector
	operator*(
		const Vector& rowVector_in,
		const Matrix& matrix_in
		);

	friend  Vector
	operator*(
		const Matrix& matrix_in,
		const Vector& colVector_in
		);

	Matrix&
	operator+=(
		const Matrix& matrix_in
		);

	Matrix&
	operator-=(
		const Matrix& matrix_in
		);

	Matrix&
	operator*=(
		const Matrix& matrix_in
		);

	Matrix&
	operator*=(
		const double& scalar_in
		);

	void
	invert(
		);

	Matrix
	getInverse(
		) const;

	void
	transpose(
		);

	Matrix
	getTransposed(
		) const;

	bool
	isIdentityMatrix(
		) const;

	bool
	isZeroMatrix(
		) const;

	double 
	getSubDeterminant(
		const unsigned short rowIndex_in,
		const unsigned short colIndex_in
		) const;

	
	double
	getDeterminant(
		) const;
};

#endif