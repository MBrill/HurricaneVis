/**
 * \file   Vector.h
 * \author Christof Rezk-Salama
 * \class  Vector
 * \brief  Stores a vector including texture coordinates and color and provides appropriate operations.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 * Instances of this class are used in several other classes wherever vector operations
 * simplify the processing such as computing viewing direction, camera position and so on.
 */

#ifndef __Vector_h
#define __Vector_h

#include <math.h>

#define _EPSILON (0.0001)

class Vector
{
public:
private:
	double value_[4];
	double texcoord_[3];
	double color_[4];

public:
	Vector(
		);

	Vector(
		const Vector& vector_in
		);

	Vector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in = 1.0
		);
	Vector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in,
		const double& s_in,
		const double& t_in,
		const double& r_in
		);
	Vector(
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
	Vector(
		const double& a_in,
		const double& b_in,
		const double& c_in,
		const double& d_in,
		const double& R_in,
		const double& G_in,
		const double& B_in,
		const double& A_in
		);

	Vector&
	operator=(
		const Vector& vector_in
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
		const Vector& vector_in
		) const;

	bool
	operator!=(
		const Vector& vector_in
		) const;

	Vector
	operator-(
		) const;

	friend Vector
	operator-(
		const Vector& vector0_in,
		const Vector& vector1_in
		);

	friend Vector
	operator+(
		const Vector& vector0_in,
		const Vector& vector1_in
		);

	friend double
	operator*(
		const Vector& vector0_in,
		const Vector& vector1_in
		);

	double
	getInnerProduct(
		const Vector& vector_in
		) const;
   
	friend Vector
	operator*(
		const double& scalar_in,
		const Vector& vector_in
		);

	friend Vector
	operator*(
		const Vector& vector_in,
		const double& scalar_in
		);

	friend Vector
	operator/(
		const Vector& vector_in,
		const double& scalar_in
		);

	Vector&
	operator+=(
		const Vector& vector_in
		);

	Vector&
	operator-=(
		const Vector& vector_in
		);

	Vector&
	operator*=(
		const double& scalar_in
		);

	Vector&
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

#ifdef USE_GLVERTEX
	inline void
	glVertex(bool bColor = true, bool bTexCoord = true) const{
	if (bColor) glColor4d(color_[0], color_[1], color_[2], color_[3]);
	if (bTexCoord) glTexCoord3d(texcoord_[0], texcoord_[1], texcoord_[2]);
	glVertex4d(value_[0],value_[1],value_[2],value_[3]);
}
#endif
};

#endif