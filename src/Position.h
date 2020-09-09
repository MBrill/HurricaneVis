/**
 * \file   Position.h
 * \author Christof Rezk-Salama
 * \class  Position
 * \brief  Stores the position of trapezoid points and provides appropriate operations.
 *
 * This class is taken from the example for the "Real-Time Volume Graphics" book.
 * The example can be downloaded at http://www.real-time-volume-graphics.org/
 * [accessed January 2009].
 * This class is used to store the position of trapezoid points in the transfer function
 * representation. Thus instances of this class are used in vtkTrapezoid.
 */

#ifndef __Position_h
#define __Position_h

class  Position
{

private:
	double value_[2];

public:
	Position(
		);

	Position(
		const Position& vector_in
		);

	Position(
		const double& a_in,
		const double& b_in
		);

	Position&
	operator=(
		const Position& vector_in
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
		const Position& vector_in
		) const;

	bool
	operator!=(
		const Position& vector_in
		) const;

	Position
	operator-(
		) const;

	friend  Position
	operator-(
		const Position& vector0_in,
		const Position& vector1_in
		);

	friend  Position
	operator+(
		const Position& vector0_in,
		const Position& vector1_in
		);

	friend  double
	operator*(
		const Position& vector0_in,
		const Position& vector1_in
		);

	friend  Position
	operator*(
		const double& scalar_in,
		const Position& vector_in
		);

	friend  Position
	operator*(
		const Position& vector_in,
		const double& scalar_in
		);

	friend  Position
	operator/(
		const Position& vector_in,
		const double& scalar_in
		);

	Position&
	operator+=(
		const Position& vector_in
		);

	Position&
	operator-=(
		const Position& vector_in
		);

	Position&
	operator*=(
		const double& scalar_in
		);

	Position&
	operator/=(
		const double& scalar_in
		);

	double
	getLength(
		) const;

	void
	getNormalized(
		Position& vector_out
		) const;

	Position
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

#endif