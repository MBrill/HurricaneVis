
// ****************************************************************************
// *** Includes
// ****************************************************************************

#include <stdio.h>
#include <math.h>
#include "Position.h"

#define _EPSILON (0.0001)

Position::Position( )
{
	value_[0] = 0.0;
	value_[1] = 0.0;
}

Position::Position(const Position& vector_in)
{
	value_[0] = vector_in.value_[0];
	value_[1] = vector_in.value_[1];
}

Position::Position(const double& a_in,const double& b_in)
{
	value_[0] = a_in;
	value_[1] = b_in;
}

Position&
Position::operator=(const Position& vector_in)
{
	if(&vector_in != this)
	{
		value_[0] = vector_in.value_[0];
		value_[1] = vector_in.value_[1];
	}

	return *this;
}

double&
Position::operator[](const unsigned short index_in)
{
	if(index_in > 1)
		throw;

	return value_[index_in];
}

double
Position::operator[](const unsigned short index_in) const
{
	if(index_in > 1)
		throw;

	return value_[index_in];
}

double&
Position::operator()(const unsigned short index_in)
{
	if(index_in > 1)
		throw;

	return value_[index_in];
}

double
Position::operator()(const unsigned short index_in) const
{
	if(index_in > 1)
		throw;

	return value_[index_in];
}


void
Position::set(const double& a_in, const double& b_in)

{
	value_[0] = a_in;
	value_[1] = b_in;
}

void
Position::get( double& a_out, double& b_out) const

{
	a_out = value_[0];
	b_out = value_[1];
}

bool
Position::operator==( const Position& vector_in ) const

{
	if( (fabs(value_[0] - vector_in.value_[0]) > _EPSILON)
		|| (fabs(value_[1] - vector_in.value_[1]) > _EPSILON) )
		return false;
	return true;
}

bool
Position::operator!=(const Position& vector_in) const

{
	return( !(*this == vector_in) );
}


Position
Position::operator-( ) const

{
	return Position(
		-value_[0],
		-value_[1]
		);
}


Position
operator-(const Position& vector0_in,const Position& vector1_in)

{
	return Position(
		vector0_in.value_[0] - vector1_in.value_[0],
		vector0_in.value_[1] - vector1_in.value_[1]
		);
}


Position
operator+(const Position& vector0_in,const Position& vector1_in)

{
	return Position(
		vector0_in.value_[0] + vector1_in.value_[0],
		vector0_in.value_[1] + vector1_in.value_[1]
		);
}


double
operator*(const Position& vector0_in,const Position& vector1_in)

{
	return(
		vector0_in.value_[0] * vector1_in.value_[0]
		+ vector0_in.value_[1] * vector1_in.value_[1]
		);
}


Position
operator*(const double& scalar_in,const Position& vector_in)

{
	return Position(
		scalar_in * vector_in.value_[0],
		scalar_in * vector_in.value_[1]
		);
}


Position
operator*(const Position& vector_in,const double& scalar_in)

{
	return Position(
		vector_in.value_[0] * scalar_in,
		vector_in.value_[1] * scalar_in
		);
}


Position
operator/(const Position& vector_in,const double& scalar_in)

{
	return Position(
		vector_in.value_[0] / scalar_in,
		vector_in.value_[1] / scalar_in
		);
}


Position&
Position::operator+=(const Position& vector_in)

{
	value_[0] += vector_in.value_[0];
	value_[1] += vector_in.value_[1];

	return *this;
}


Position&
Position::operator-=(const Position& vector_in)

{
	value_[0] -= vector_in.value_[0];
	value_[1] -= vector_in.value_[1];

	return *this;
}


Position&
Position::operator*=(const double& scalar_in)

{
	value_[0] *= scalar_in;
	value_[1] *= scalar_in;

	return *this;
}


Position&
Position::operator/=(const double& scalar_in)

{
	value_[0] /= scalar_in;
	value_[1] /= scalar_in;

	return *this;
}


double
Position::getLength( ) const

{
    return sqrt(
		value_[0] * value_[0]
		+ value_[1] * value_[1]
		);
}


void
Position::getNormalized(Position& vector_out) const

{
	double length;
	length = getLength();
	if( fabs( length ) < _EPSILON )
		throw;

	vector_out = Position( value_[0] / length,value_[1] / length );
}


Position
Position::getNormalized() const

{
	double length;
	length = getLength();
	if( fabs( length ) < _EPSILON )
		throw;

	return Position(
		value_[0] / length,
		value_[1] / length
		);
}


void
Position::normalize()

{
	double length;
	length = getLength();

	if( fabs( length ) < _EPSILON )
		throw;

	value_[0] /= length;
	value_[1] /= length;
}


bool
Position::isNormalized() const

{
	double length;
	length = getLength();

	if(fabs(length - 1.0) > _EPSILON)
		return false;
	return true;
}


bool
Position::isNil() const

{
	double length;
	length = getLength();

	if(fabs(length) > _EPSILON)
		return false;
	return true;
}
