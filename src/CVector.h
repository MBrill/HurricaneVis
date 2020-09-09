/**
 * \file   CVector.h
 * \author Christof Rezk-Salama
 * \class  CVector
 * \brief  Stores a vector including texture coordinates and color and provides appropriate operations.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 * Instances of this class are used in several other classes wherever vector operations
 * simplify the processing such as computing viewing direction, camera position and so on.
 */

#ifndef CVector_H
#define CVector_H

class CVector
{
public:
private:
	double value_[4];
	double texcoord_[3];
	double color_[4];

public:
	CVector(
		);

	CVector(
		const CVector& vector_in
		);

	CVector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in = 1.0
		);
	CVector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in,
		const double& s_in,
		const double& t_in,
		const double& r_in
		);
	CVector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in,
		const double& s_in,
		const double& t_in,
		const double& r_in,
		const double& R_in,
		const double& G_in,
		const double& B_in,
		const double& A_in
		);
	CVector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in,
		const double& R_in,
		const double& G_in,
		const double& B_in,
		const double& A_in
		);

	CVector&
	operator=(
		const CVector& vector_in
		);

	double&
	operator[](
		const unsigned short index_in
		);

	double
	operator[](
		const unsigned short index_in
		) const;

	double&
	operator()(
		const unsigned short index_in
		);

	double
	operator()(
		const unsigned short index_in
		) const;

	void
	set(
		const double& a_in, 
		const double& b_in,
		const double& c_in,
		const double& d_in
		);

	void
	get(
		double& a_out, 
		double& b_out,
		double& c_out,
		double& d_out
		) const;

	bool
	operator==(
		const CVector& vector_in
		) const;

	bool
	operator!=(
		const CVector& vector_in
		) const;

	CVector
	operator-(
		) const;

	friend CVector
	operator-(
		const CVector& vector0_in,
		const CVector& vector1_in
		);

	friend CVector
	operator+(
		const CVector& vector0_in,
		const CVector& vector1_in
		);

	friend double
	operator*(
		const CVector& vector0_in,
		const CVector& vector1_in
		);

	double
	getInnerProduct(
		const CVector& vector_in
		) const;
   
	friend CVector
	operator*(
		const double& scalar_in,
		const CVector& vector_in
		);

	friend CVector
	operator*(
		const CVector& vector_in,
		const double& scalar_in
		);

	friend CVector
	operator/(
		const CVector& vector_in,
		const double& scalar_in
		);

	CVector&
	operator+=(
		const CVector& vector_in
		);

	CVector&
	operator-=(
		const CVector& vector_in
		);

	CVector&
	operator*=(
		const double& scalar_in
		);

	CVector&
	operator/=(
		const double& scalar_in
		);

	double
	getLength(
		) const;

	void
	normalize(
		);

	bool
	isNormalized(
		) const;

	bool
	isNil(
		) const;
};


#endif // CVector_H
