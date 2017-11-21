#pragma once

namespace nm
{
	class matrix4 
	{
	public:
		inline void zero();
		inline void identity();

		inline matrix4() {}
		inline matrix4(float* d);
		inline matrix4(const matrix4& m);


		// Access
		inline float& operator [] ( int index );
		inline float operator [] ( int index ) const;
		inline float& operator () ( int r, int c );
		inline float operator () ( int r, int c ) const;

		// Scalar
		inline matrix4& operator += ( float scalar );
		inline matrix4& operator -= ( float scalar );
		inline matrix4& operator *= ( float scalar );
		inline matrix4& operator /= ( float scalar );

		inline matrix4 operator + ( float scalar ) const;
		inline matrix4 operator - ( float scalar ) const;
		inline matrix4 operator * ( float scalar ) const;
		inline matrix4 operator / ( float scalar ) const;

		// Infix
		inline matrix4& operator += ( const matrix4& matrix );
		inline matrix4& operator -= ( const matrix4& matrix );
		inline matrix4& operator *= ( const matrix4& matrix );

		inline matrix4 operator + ( const matrix4& matrix ) const;
		inline matrix4 operator - ( const matrix4& matrix ) const;
		inline matrix4 operator * ( const matrix4& matrix ) const;

		operator float* () { return &data[0]; }
		operator const float* () const { return &data[0]; }

		union {
			struct {
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;

			};
			float m[4][4];
			float data[16];
		};		
	};

	matrix4::matrix4(float* d)
	{
		data[0] = d[0];
		data[1] = d[1];
		data[2] = d[2];
		data[3] = d[3];
		data[4] = d[4];
		data[5] = d[5];
		data[6] = d[6];
		data[7] = d[7];
		data[8] = d[8];
		data[9] = d[9];
		data[10] = d[10];
		data[11] = d[11];
		data[12] = d[12];
		data[13] = d[13];
		data[14] = d[14];
		data[15] = d[15];
	}

	matrix4::matrix4(const matrix4& m)
	{
		data[0] = m.data[0];
		data[1] = m.data[1];
		data[2] = m.data[2];
		data[3] = m.data[3];
		data[4] = m.data[4];
		data[5] = m.data[5];
		data[6] = m.data[6];
		data[7] = m.data[7];
		data[8] = m.data[8];
		data[9] = m.data[9];
		data[10] = m.data[10];
		data[11] = m.data[11];
		data[12] = m.data[12];
		data[13] = m.data[13];
		data[14] = m.data[14];
		data[15] = m.data[15];

	}


	void matrix4::zero()
	{
		data[0]		= 0.0f;
		data[1]		= 0.0f;
		data[2]		= 0.0f;
		data[3]		= 0.0f;
		data[4]		= 0.0f;
		data[5]		= 0.0f;
		data[6]		= 0.0f;
		data[7]		= 0.0f;
		data[8]		= 0.0f;
		data[9]		= 0.0f;
		data[10]	= 0.0f;
		data[11]	= 0.0f;
		data[12]	= 0.0f;
		data[13]	= 0.0f;
		data[14]	= 0.0f;
		data[15]	= 0.0f;
	}

	void matrix4::identity()
	{
		data[0]		= 1.0f;
		data[1]		= 0.0f;
		data[2]		= 0.0f;
		data[3]		= 0.0f;
		data[4]		= 0.0f;
		data[5]		= 1.0f;
		data[6]		= 0.0f;
		data[7]		= 0.0f;
		data[8]		= 0.0f;
		data[9]		= 0.0f;
		data[10]	= 1.0f;
		data[11]	= 0.0f;
		data[12]	= 0.0f;
		data[13]	= 0.0f;
		data[14]	= 0.0f;
		data[15]	= 1.0f;
	}

	// Access
	float& matrix4::operator [] ( int index )
	{
		return data[ index ];
	}

	float matrix4::operator [] ( int index ) const
	{
		return data[ index ];
	}

	float& matrix4::operator () ( int r, int c )
	{
		return data[ c * 4 + r ];
	}

	float matrix4::operator () ( int r, int c ) const
	{
		return data[ c * 4 + r ];
	}

	// Scalar
	matrix4& matrix4::operator += ( float scalar )
	{
		data[0]		+= scalar;
		data[1]		+= scalar;
		data[2]		+= scalar;
		data[3]		+= scalar;
		data[4]		+= scalar;
		data[5]		+= scalar;
		data[6]		+= scalar;
		data[7]		+= scalar;
		data[8]		+= scalar;
		data[9]		+= scalar;
		data[10]	+= scalar;
		data[11]	+= scalar;
		data[12]	+= scalar;
		data[13]	+= scalar;
		data[14]	+= scalar;
		data[15]	+= scalar;
		return *this;
	}

	matrix4& matrix4::operator -= ( float scalar )
	{
		data[0]		-= scalar;
		data[1]		-= scalar;
		data[2]		-= scalar;
		data[3]		-= scalar;
		data[4]		-= scalar;
		data[5]		-= scalar;
		data[6]		-= scalar;
		data[7]		-= scalar;
		data[8]		-= scalar;
		data[9]		-= scalar;
		data[10]	-= scalar;
		data[11]	-= scalar;
		data[12]	-= scalar;
		data[13]	-= scalar;
		data[14]	-= scalar;
		data[15]	-= scalar;
		return *this;
	}

	matrix4& matrix4::operator *= ( float scalar )
	{
		data[0]		*= scalar;
		data[1]		*= scalar;
		data[2]		*= scalar;
		data[3]		*= scalar;
		data[4]		*= scalar;
		data[5]		*= scalar;
		data[6]		*= scalar;
		data[7]		*= scalar;
		data[8]		*= scalar;
		data[9]		*= scalar;
		data[10]	*= scalar;
		data[11]	*= scalar;
		data[12]	*= scalar;
		data[13]	*= scalar;
		data[14]	*= scalar;
		data[15]	*= scalar;
		return *this;
	}

	matrix4& matrix4::operator /= ( float scalar )	{	return operator *= ( 1.0f / scalar );	}

	matrix4 matrix4::operator + ( float scalar ) const	{		return matrix4( *this ) += scalar;	}
	matrix4 matrix4::operator - ( float scalar ) const	{		return matrix4( *this ) -= scalar;	}
	matrix4 matrix4::operator * ( float scalar ) const	{		return matrix4( *this ) *= scalar;	}
	matrix4 matrix4::operator / ( float scalar ) const	{		return matrix4( *this ) /= scalar;	}


	matrix4& matrix4::operator += ( const matrix4& matrix )
	{
		data[0]		+= matrix.data[0];
		data[1]		+= matrix.data[1];
		data[2]		+= matrix.data[2];
		data[3]		+= matrix.data[3];
		data[4]		+= matrix.data[4];
		data[5]		+= matrix.data[5];
		data[6]		+= matrix.data[6];
		data[7]		+= matrix.data[7];
		data[8]		+= matrix.data[8];
		data[9]		+= matrix.data[9];
		data[10]	+= matrix.data[10];
		data[11]	+= matrix.data[11];
		data[12]	+= matrix.data[12];
		data[13]	+= matrix.data[13];
		data[14]	+= matrix.data[14];
		data[15]	+= matrix.data[15];
		return *this;
	}

	matrix4& matrix4::operator -= ( const matrix4& matrix )
	{
		data[0]		-= matrix.data[0];
		data[1]		-= matrix.data[1];
		data[2]		-= matrix.data[2];
		data[3]		-= matrix.data[3];
		data[4]		-= matrix.data[4];
		data[5]		-= matrix.data[5];
		data[6]		-= matrix.data[6];
		data[7]		-= matrix.data[7];
		data[8]		-= matrix.data[8];
		data[9]		-= matrix.data[9];
		data[10]	-= matrix.data[10];
		data[11]	-= matrix.data[11];
		data[12]	-= matrix.data[12];
		data[13]	-= matrix.data[13];
		data[14]	-= matrix.data[14];
		data[15]	-= matrix.data[15];
		return *this;
	}

	matrix4& matrix4::operator *= ( const matrix4& matrix )
	{
		matrix4 temp(*this);
	
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m[i][j] = temp.m[i][0] * matrix.m[0][j] + temp.m[i][1] * matrix.m[1][j] + temp.m[i][2] * matrix.m[2][j] + temp.m[i][3] * matrix.m[3][j];
			}
		}

		return *this;
	}

	matrix4 matrix4::operator + ( const matrix4& matrix ) const
	{
		return matrix4( *this ) += matrix;
	}

	matrix4 matrix4::operator - ( const matrix4& matrix ) const
	{
		return matrix4( *this ) -= matrix;
	}

	matrix4 matrix4::operator * ( const matrix4& matrix ) const
	{
		return matrix4( *this ) *= matrix;
	}
}

