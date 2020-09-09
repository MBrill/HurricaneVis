/**
 * \file   CPosition.h
 * \author Christof Rezk-Salama
 * \class  CPosition
 * \brief  Stores the position of trapezoid points and provides appropriate operations.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 * This class is used to store the position of trapezoid points in the transfer function
 * representation. Thus instances of this class are used in vtkTrapezoid.
 */

#ifndef CPosition_H
#define CPosition_H

class  CPosition
{

private:
	double value_[2];

public:
	CPosition(
		);

	CPosition(
		const CPosition& vector_in
		);

	CPosition(
		const double& a_in,
		const double& b_in
		);

	CPosition&
	operator=(
		const CPosition& vector_in
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
		const double& b_in
		);

	void
	get(
		double& a_out, 
		double& b_out
		) const;

	bool
	operator==(
		const CPosition& vector_in
		) const;

	bool
	operator!=(
		const CPosition& vector_in
		) const;

	CPosition
	operator-(
		) const;

	friend  CPosition
	operator-(
		const CPosition& vector0_in,
		const CPosition& vector1_in
		);

	friend  CPosition
	operator+(
		const CPosition& vector0_in,
		const CPosition& vector1_in
		);

	friend  double
	operator*(
		const CPosition& vector0_in,
		const CPosition& vector1_in
		);

	friend  CPosition
	operator*(
		const double& scalar_in,
		const CPosition& vector_in
		);

	friend  CPosition
	operator*(
		const CPosition& vector_in,
		const double& scalar_in
		);

	friend  CPosition
	operator/(
		const CPosition& vector_in,
		const double& scalar_in
		);

	CPosition&
	operator+=(
		const CPosition& vector_in
		);

	CPosition&
	operator-=(
		const CPosition& vector_in
		);

	CPosition&
	operator*=(
		const double& scalar_in
		);

	CPosition&
	operator/=(
		const double& scalar_in
		);

	double
	getLength(
		) const;

	void
	getNormalized(
		CPosition& vector_out
		) const;

	CPosition
	getNormalized(
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

#endif // CPosition_H
