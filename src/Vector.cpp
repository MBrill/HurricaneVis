#include "Vector.h"

Vector::Vector(
	)
{
	value_[0] = 0.0;
	value_[1] = 0.0;
	value_[2] = 0.0;
	value_[3] = 0.0;
	
	texcoord_[0] = 0.0;
	texcoord_[1] = 0.0;
	texcoord_[2] = 0.0;

	color_[0] = 1.0;
	color_[1] = 1.0;
	color_[2] = 1.0;
	color_[3] = 1.0;

}

Vector::Vector(
	const Vector& vector_in
	)
{
	value_[0] = vector_in.value_[0];
	value_[1] = vector_in.value_[1];
	value_[2] = vector_in.value_[2];
	value_[3] = vector_in.value_[3];

	texcoord_[0] = vector_in.texcoord_[0];
	texcoord_[1] = vector_in.texcoord_[1];
	texcoord_[2] = vector_in.texcoord_[2];

	color_[0] = vector_in.color_[0];
	color_[1] = vector_in.color_[1];
	color_[2] = vector_in.color_[2];
	color_[3] = vector_in.color_[3];
}

Vector::Vector(
	const double& a_in,
	const double& b_in,
	const double& c_in,
	const double& d_in
	)
{
	value_[0] = a_in;
	value_[1] = b_in;
	value_[2] = c_in;
	value_[3] = d_in;
}

Vector::Vector(
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
	)
{
	value_[0] = a_in;
	value_[1] = b_in;
	value_[2] = c_in;
	value_[3] = d_in;
	
	texcoord_[0] = s_in;
	texcoord_[1] = t_in;
	texcoord_[2] = r_in;

	color_[0] = R_in;
	color_[1] = G_in;
	color_[2] = B_in;
	color_[3] = A_in;
}

Vector::Vector(
	const double& a_in,
	const double& b_in,
	const double& c_in,
	const double& d_in,
	const double& s_in,
	const double& t_in,
	const double& r_in
	)
{
	value_[0] = a_in;
	value_[1] = b_in;
	value_[2] = c_in;
	value_[3] = d_in;
	
	texcoord_[0] = s_in;
	texcoord_[1] = t_in;
	texcoord_[2] = r_in;

	color_[0] = 1.0;
	color_[1] = 1.0;
	color_[2] = 1.0;
	color_[3] = 1.0;
}

Vector::Vector(
	const double& a_in,
	const double& b_in,
	const double& c_in,
	const double& d_in,
	const double& R_in,
	const double& G_in,
	const double& B_in,
	const double& A_in
	)
{
	value_[0] = a_in;
	value_[1] = b_in;
	value_[2] = c_in;
	value_[3] = d_in;

	texcoord_[0] = 0.0;
	texcoord_[1] = 0.0;
	texcoord_[2] = 0.0;

	color_[0] = R_in;
	color_[1] = G_in;
	color_[2] = B_in;
	color_[3] = A_in;
}


Vector&
Vector::operator=(
	const Vector& vector_in
	)
{
	if(&vector_in != this)
	{
		value_[0] = vector_in.value_[0];
		value_[1] = vector_in.value_[1];
		value_[2] = vector_in.value_[2];
		value_[3] = vector_in.value_[3];
		
		texcoord_[0] = vector_in.texcoord_[0];
		texcoord_[1] = vector_in.texcoord_[1];
		texcoord_[2] = vector_in.texcoord_[2];

		color_[0] = vector_in.color_[0];
		color_[1] = vector_in.color_[1];
		color_[2] = vector_in.color_[2];
		color_[3] = vector_in.color_[3];
	}

	return *this;
}

double&
Vector::operator[](
	const unsigned short index_in
	)
{
	if(index_in > 3)
		throw;

	return value_[index_in];
}

double
Vector::operator[](
	const unsigned short index_in
	) const
{
	if(index_in > 3)
		throw;

	return value_[index_in];
}

double&
Vector::operator()(
	const unsigned short index_in
	)
{
	if(index_in > 3)
		throw;

	return value_[index_in];
}

double
Vector::operator()(
	const unsigned short index_in
	) const
{
	if(index_in > 3)
		throw;

	return value_[index_in];
}

void
Vector::set(
	const double& a_in, 
	const double& b_in,
	const double& c_in,
	const double& d_in
	)
{
	value_[0] = a_in;
	value_[1] = b_in;
	value_[2] = c_in;
	value_[3] = d_in;
}

void
Vector::get(
	double& a_out, 
	double& b_out,
	double& c_out,
	double& d_out
	) const
{
	a_out = value_[0];
	b_out = value_[1];
	c_out = value_[2];
	d_out = value_[3];
}

bool
Vector::operator==(
	const Vector& vector_in
	) const
{
	if( (fabs(value_[0] - vector_in.value_[0]) >    _EPSILON)
		|| (fabs(value_[1] - vector_in.value_[1]) > _EPSILON)
		|| (fabs(value_[2] - vector_in.value_[2]) > _EPSILON)
		|| (fabs(value_[3] - vector_in.value_[3]) > _EPSILON) )
		return false;
	return true;
}

bool
Vector::operator!=(
	const Vector& vector_in
	) const
{
	return( !(*this == vector_in) );
}

Vector
Vector::operator-(
	) const
{
	return Vector(
		-value_[0],
		-value_[1],
		-value_[2],
		-value_[3],
		-texcoord_[0],
		-texcoord_[1],
		-texcoord_[2]
		);
}

Vector
operator-(
	const Vector& vector0_in,
	const Vector& vector1_in
	)
{
	return Vector(
		vector0_in.value_[0] - vector1_in.value_[0],
		vector0_in.value_[1] - vector1_in.value_[1],
		vector0_in.value_[2] - vector1_in.value_[2],
		vector0_in.value_[3] - vector1_in.value_[3],
		vector0_in.texcoord_[0] - vector1_in.texcoord_[0],
		vector0_in.texcoord_[1] - vector1_in.texcoord_[1],
		vector0_in.texcoord_[2] - vector1_in.texcoord_[2],
		vector0_in.color_[0] - vector1_in.color_[0],
		vector0_in.color_[1] - vector1_in.color_[1],
		vector0_in.color_[2] - vector1_in.color_[2],
		vector0_in.color_[3] - vector1_in.color_[3]
		);
}

Vector
operator+(
	const Vector& vector0_in,
	const Vector& vector1_in
	)
{
	return Vector(
		vector0_in.value_[0] + vector1_in.value_[0],
		vector0_in.value_[1] + vector1_in.value_[1],
		vector0_in.value_[2] + vector1_in.value_[2],
		vector0_in.value_[3] + vector1_in.value_[3],
		vector0_in.texcoord_[0] + vector1_in.texcoord_[0],
		vector0_in.texcoord_[1] + vector1_in.texcoord_[1],
		vector0_in.texcoord_[2] + vector1_in.texcoord_[2],
		vector0_in.color_[0] + vector1_in.color_[0],
		vector0_in.color_[1] + vector1_in.color_[1],
		vector0_in.color_[2] + vector1_in.color_[2],
		vector0_in.color_[3] + vector1_in.color_[3]
		);
}

double
operator*(
	const Vector& vector0_in,
	const Vector& vector1_in
	)
{
	return(
		  vector0_in.value_[0] * vector1_in.value_[0]
		+ vector0_in.value_[1] * vector1_in.value_[1]
		+ vector0_in.value_[2] * vector1_in.value_[2]
		+ vector0_in.value_[3] * vector1_in.value_[3]
		);
}

double
Vector::getInnerProduct(
	const Vector& vector_in
	) const
{
	return(
		value_[0] * vector_in.value_[0]
		+ value_[1] * vector_in.value_[1]
		+ value_[2] * vector_in.value_[2]
		+ value_[3] * vector_in.value_[3]
		);
}
  
Vector
operator*(
	const double& scalar_in,
	const Vector& vector_in
	)
{
	return Vector(
		scalar_in * vector_in.value_[0],
		scalar_in * vector_in.value_[1],
		scalar_in * vector_in.value_[2],
		scalar_in * vector_in.value_[3],
		scalar_in * vector_in.texcoord_[0],
		scalar_in * vector_in.texcoord_[1],
		scalar_in * vector_in.texcoord_[2],
		scalar_in * vector_in.color_[0],
		scalar_in * vector_in.color_[1],
		scalar_in * vector_in.color_[2],
		scalar_in * vector_in.color_[3]
		);
}

Vector
operator*(
	const Vector& vector_in,
	const double& scalar_in
	)
{
	return Vector(
		vector_in.value_[0] * scalar_in,
		vector_in.value_[1] * scalar_in,
		vector_in.value_[2] * scalar_in,
		vector_in.value_[3] * scalar_in,
		vector_in.texcoord_[0] * scalar_in,
		vector_in.texcoord_[1] * scalar_in,
		vector_in.texcoord_[2] * scalar_in,
		vector_in.color_[0] * scalar_in,
		vector_in.color_[1] * scalar_in,
		vector_in.color_[2] * scalar_in,
		vector_in.color_[3] * scalar_in
		);
}

Vector
operator/(
	const Vector& vector_in,
	const double& scalar_in
	)
{
	return Vector(
		vector_in.value_[0] / scalar_in,
		vector_in.value_[1] / scalar_in,
		vector_in.value_[2] / scalar_in,
		vector_in.value_[3] / scalar_in,
		vector_in.texcoord_[0] / scalar_in,
		vector_in.texcoord_[1] / scalar_in,
		vector_in.texcoord_[2] / scalar_in,
		vector_in.color_[0] / scalar_in,
		vector_in.color_[1] / scalar_in,
		vector_in.color_[2] / scalar_in,
		vector_in.color_[3] / scalar_in
		);
}

Vector&
Vector::operator+=(
	const Vector& vector_in
	)
{
	value_[0]    += vector_in.value_[0];
	value_[1]    += vector_in.value_[1];
	value_[2]    += vector_in.value_[2];
	value_[3]    += vector_in.value_[3];
	texcoord_[0] += vector_in.texcoord_[0];
	texcoord_[1] += vector_in.texcoord_[1];
	texcoord_[2] += vector_in.texcoord_[2];
	color_[0]    += vector_in.color_[0] ;  
	color_[1]    += vector_in.color_[1] ;  
	color_[2]    += vector_in.color_[2] ;  
	color_[3]    += vector_in.color_[3] ;
	return *this;
}

Vector&
Vector::operator-=(
	const Vector& vector_in
	)
{
	value_[0]    -= vector_in.value_[0];
	value_[1]    -= vector_in.value_[1];
	value_[2]    -= vector_in.value_[2];
	value_[3]    -= vector_in.value_[3];
	texcoord_[0] -= vector_in.texcoord_[0];
	texcoord_[1] -= vector_in.texcoord_[1];
	texcoord_[2] -= vector_in.texcoord_[2];
	color_[0]    -= vector_in.color_[0] ;  
	color_[1]    -= vector_in.color_[1] ;  
	color_[2]    -= vector_in.color_[2] ;  
	color_[3]    -= vector_in.color_[3] ;

	return *this;
}

Vector&
Vector::operator*=(
	const double& scalar_in
	)
{
	value_[0]    *= scalar_in;
	value_[1]    *= scalar_in;
	value_[2]    *= scalar_in;
	value_[3]    *= scalar_in;
	texcoord_[0] *= scalar_in;
	texcoord_[1] *= scalar_in;
	texcoord_[2] *= scalar_in;
	color_[0]    *= scalar_in;
	color_[1]    *= scalar_in;
	color_[2]    *= scalar_in;
	color_[3]    *= scalar_in;

	return *this;
}

Vector&
Vector::operator/=(
	const double& scalar_in
	)
{
	value_[0]    /= scalar_in;
	value_[1]    /= scalar_in;
	value_[2]    /= scalar_in;
	value_[3]    /= scalar_in;
	texcoord_[0] /= scalar_in;
	texcoord_[1] /= scalar_in;
	texcoord_[2] /= scalar_in;
	color_[0]    /= scalar_in;
	color_[1]    /= scalar_in;
	color_[2]    /= scalar_in;
	color_[3]    /= scalar_in;

	return *this;
}

double
Vector::getLength(
	) const
{
    return sqrt(
		value_[0] * value_[0] 
		+ value_[1] * value_[1]
		+ value_[2] * value_[2]
		+ value_[3] * value_[3]
		);
}

void
Vector::normalize(
	)
{
	double length;
	length = getLength();

	if( fabs( length ) < _EPSILON )
		return;

	value_[0] /= length;
	value_[1] /= length;
	value_[2] /= length;
	value_[3] /= length;
}

bool
Vector::isNormalized(
	) const
{
	double length;
	length = getLength();

	if(fabs(length - 1.0) > _EPSILON)
		return false;
	return true;
}

bool
Vector::isNil(
	) const
{
	double length;
	length = getLength();

	if(fabs(length) > _EPSILON)
		return false;
	return true;
}
