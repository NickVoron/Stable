/*********************************************************************

	SiGMan / iO UpG  -  Copyright (C) 2000-2001

	Author: SiGMan
	  Date: 22.07.2001
	  Time: 5:09:59

	Implemented:	Matrix4 class.

*********************************************************************/

#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

// Graphics gems (IV)
// Gauss-Jordan elimination with partial pivoting
// data[ c * 4 + r ]
float
Matrix4::GetDeterminant() const
{	
	float ans;
	float a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
	
	a1 = data[0];	b1 = data[4];
	c1 = data[8];	d1 = data[12];

	a2 = data[1];	b2 = data[5];
	c2 = data[9];	d2 = data[13];

	a3 = data[2];	b3 = data[6];
	c3 = data[10];	d3 = data[14];

	a4 = data[3];	b4 = data[7];
	c4 = data[11];	d4 = data[15];

	ans = a1 * Matrix3( b2, c2, d2, b3, c3, d3, b4, c4, d4 ).GetDeterminant()
		- b1 * Matrix3( a2, c2, d2, a3, c3, d3, a4, c4, d4 ).GetDeterminant()
		+ c1 * Matrix3( a2, b2, d2, a3, b3, d3, a4, b4, d4 ).GetDeterminant()
		- d1 * Matrix3( a2, b2, c2, a3, b3, c3, a4, b4, c4 ).GetDeterminant();
	return ans;	
}

Matrix4&
Matrix4::Invert()
{
	Matrix4 b = GetIdentity();
	int i, j, i1;
	// Loop over cols of a from left to right, eliminatin above and below diag
	for( j = 0; j < 4; j++ ) {	// Find a largest pivot in col 'j' among rows j..3
		i1	= j;				// Row with a largest pivot candidate
		for( i = j + 1; i < 4; i++ ) {
			if ( fabsf( data[ j * 4 + i ] ) > fabsf( data[ j * 4 + i1 ] ) ) 
				i1 = i;
		}
		// Swap rows i1 and j in a and b to put pivot onto diagonal
		float ftmp;
		// swap rows in data
		// effectively row gets compiled into:
		// fld float1;
		// mov edx, float2
		// mov float1, edx
		// fstp float2
		ftmp = data[i1   ];	data[i1   ]	= data[j   ]; data[j   ] = ftmp;
		ftmp = data[i1+4 ];	data[i1+4 ]	= data[j+4 ]; data[j+4 ] = ftmp;
		ftmp = data[i1+8 ];	data[i1+8 ]	= data[j+8 ]; data[j+8 ] = ftmp;
		ftmp = data[i1+12];	data[i1+12]	= data[j+12]; data[j+12] = ftmp;
		// swap rows in b
		ftmp = b.data[i1   ];	b.data[i1   ]	= b.data[j   ]; b.data[j   ] = ftmp;
		ftmp = b.data[i1+4 ];	b.data[i1+4 ]	= b.data[j+4 ]; b.data[j+4 ] = ftmp;
		ftmp = b.data[i1+8 ];	b.data[i1+8 ]	= b.data[j+8 ]; b.data[j+8 ] = ftmp;
		ftmp = b.data[i1+12];	b.data[i1+12]	= b.data[j+12]; b.data[j+12] = ftmp;
		// Scale row j to have a unit diagonal
		// Singular matrix !
		float scale = data[ j *4 + j ];
		// Trick! Trick!
		scale = 1.0f / scale;
		b.data[j   ] *= scale;
		b.data[j+4 ] *= scale;
		b.data[j+8 ] *= scale;
		b.data[j+12] *= scale;
		data[j   ] *= scale;
		data[j+4 ] *= scale;
		data[j+8 ] *= scale;
		data[j+12] *= scale;
		// Eliminate off-diagonal elems in col j of data, doing identical on b
		for( i = 0; i < 4; i++ ) {
			if ( i != j ) {
				float c1, c2, c3, c4, c;
				c = data[j*4+i];
				c1 = c * b.data[j   ];
				c2 = c * b.data[j+4 ];
				c3 = c * b.data[j+8 ];
				c4 = c * b.data[j+12];
				b.data[i   ] -= c1;
				b.data[i+4 ] -= c2;
				b.data[i+8 ] -= c3;
				b.data[i+12] -= c4;

				c1 = c * data[j   ];
				c2 = c * data[j+4 ];
				c3 = c * data[j+8 ];
				c4 = c * data[j+12];
				data[i   ] -= c1;
				data[i+4 ] -= c2;
				data[i+8 ] -= c3;
				data[i+12] -= c4;
			}
		}
	}
	Load( b.data );
	return *this;
}


const Vector3& Matrix4::getTranslate() const
{
	return *(Vector3*)&data[12];
}
