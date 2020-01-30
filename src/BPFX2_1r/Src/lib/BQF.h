#ifndef BQF_h_
#define BQF_h_

#include	<math.h>

struct BQF_Params
{
	float	a0, a1, a2;
	float	b0, b1, b2;

	constexpr BQF_Params():
		a0(0.f), a1(0.f), a2(0.f), b0(0.f), b1(0.f), b2(0.f)
	{
	}

	constexpr BQF_Params( float A0, float A1, float A2, float B0, float B1, float B2 ):
		a0(A0), a1(A1), a2(A2), b0(B0), b1(B1), b2(B2)
	{
	}
};

struct BQF
{
	float		b0_a0, b1_a0, b2_a0;
	float		a1_a0, a2_a0;

	float		in1, in2, out1, out2;
	float		output;

public:

	constexpr BQF():
		b0_a0(0.f), b1_a0(0.f), b2_a0(0.f),
		a1_a0(0.f), a2_a0(0.f),
		in1(0.f), in2(0.f), out1(0.f), out2(0.f), output(0.f)
	{
	}

	constexpr BQF( const BQF_Params& v ):
		b0_a0(0.f), b1_a0(0.f), b2_a0(0.f),
		a1_a0(0.f), a2_a0(0.f),
		in1(0.f), in2(0.f), out1(0.f), out2(0.f), output(0.f)
	{
		*this = v;
	}

	float operator()( float input )
	{
		output	=	b0_a0 * input
						+ b1_a0 * in1
						+ b2_a0 * in2
						- a1_a0 * out1
						- a2_a0 * out2;

		in2  = in1;
		in1  = input;

		out2 = out1;
		out1 = output;

		return output;
	}

	BQF& operator = ( const BQF_Params& v )
	{
		b0_a0 = v.b0/v.a0;
		b1_a0 = v.b1/v.a0;
		b2_a0 = v.b2/v.a0;
		a1_a0 = v.a1/v.a0;
		a2_a0 = v.a2/v.a0;

		return *this;
	};

	void Clear()	{ in1 = in2 = out1 = out2 = output = 0.f; }
};

struct BQF_Builder
{
	const int	samplerate;

	constexpr BQF_Builder( int fs = 40000 ) : samplerate( fs ){}

	inline constexpr BQF_Params LPF( float Frequency, float Q ) const;
	inline constexpr BQF_Params HPF( float Frequency, float Q ) const;
	inline constexpr BQF_Params BPF( float Frequency, float Band_Width ) const;
	inline constexpr BQF_Params Notch( float Frequency, float Band_Width ) const;
	inline constexpr BQF_Params LSF( float Frequency, float Q, float Gain ) const;
	inline constexpr BQF_Params HSF( float Frequency, float Q, float Gain ) const;
	inline constexpr BQF_Params Peak( float Frequency, float Band_Width, float Gain ) const;
	inline constexpr BQF_Params APF( float Frequency, float Q ) const;
};

constexpr BQF_Params BQF_Builder::LPF( float freq, float q ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		(1-cos(omega)) / 2,
		 1-cos(omega),
		(1-cos(omega)) / 2
	);
}

constexpr BQF_Params BQF_Builder::HPF( float freq, float q ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		  1 + alpha,
		 -2 * cos(omega),
		  1 - alpha,
		 (1+cos(omega)) / 2,
		-(1+cos(omega)),
		 (1+cos(omega)) / 2
	);
}

constexpr BQF_Params BQF_Builder::BPF( float freq, float bw ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		     alpha,
		 0,
		   - alpha
	);
}

constexpr BQF_Params BQF_Builder::Notch( float freq, float bw ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		 1,
		-2 * cos(omega),
		 1
	);
}

constexpr BQF_Params BQF_Builder::LSF( float freq, float q, float gain ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float A = pow( 10, ( gain / 40 ) );
	float beta = sqrt( A ) / q;

	return BQF_Params
	(
		        (A+1) + (A-1) * cos(omega) + beta * sin(omega),
		-2   * ((A-1) + (A+1) * cos(omega)),
		        (A+1) + (A-1) * cos(omega) - beta * sin(omega),
		 A   * ((A+1) - (A-1) * cos(omega) + beta * sin(omega)),
		 2*A * ((A-1) - (A+1) * cos(omega)),
		 A   * ((A+1) - (A-1) * cos(omega) - beta * sin(omega))
	);
}

constexpr BQF_Params BQF_Builder::HSF( float freq, float q, float gain ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float A = pow( 10, ( gain/40 ) );
	float beta = sqrt(A) / q;

	return BQF_Params
	(
		        (A+1) - (A-1) * cos(omega) + beta * sin(omega),
		   2 * ((A-1) - (A+1) * cos(omega)),
		        (A+1) - (A-1) * cos(omega) - beta * sin(omega),
		   A * ((A+1) + (A-1) * cos(omega) + beta * sin(omega)),
		-2*A * ((A-1) + (A+1) * cos(omega)),
		   A * ((A+1) + (A-1) * cos(omega) - beta * sin(omega))
	);
}

constexpr BQF_Params BQF_Builder::Peak( float freq, float bw, float gain ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );
	float A = pow( 10, ( gain / 40 ) );

	return BQF_Params
	(
		 1 + (alpha/A),
		-2 * cos(omega),
		 1 - (alpha/A),
		 1 + (alpha*A),
		-2 * cos(omega),
		 1 - (alpha*A)
	);
}

constexpr BQF_Params BQF_Builder::APF( float freq, float q ) const
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		 1 - alpha,
		-2 * cos(omega),
		 1 + alpha
	);
}

//////////////////////////////////

constexpr BQF_Params BQF_LPF( float freq, float q, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		(1-cos(omega)) / 2,
		 1-cos(omega),
		(1-cos(omega)) / 2
	);
}

constexpr BQF_Params BQF_HPF( float freq, float q, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		  1 + alpha,
		 -2 * cos(omega),
		  1 - alpha,
		 (1+cos(omega)) / 2,
		-(1+cos(omega)),
		 (1+cos(omega)) / 2
	);
}

constexpr BQF_Params BQF_BPF( float freq, float bw, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		     alpha,
		 0,
		   - alpha
	);
}

constexpr BQF_Params BQF_Notch( float freq, float bw, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		 1,
		-2 * cos(omega),
		 1
	);
}

constexpr BQF_Params BQF_LSF( float freq, float q, float gain, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float A = pow( 10, ( gain / 40 ) );
	float beta = sqrt( A ) / q;

	return BQF_Params
	(
		        (A+1) + (A-1) * cos(omega) + beta * sin(omega),
		-2   * ((A-1) + (A+1) * cos(omega)),
		        (A+1) + (A-1) * cos(omega) - beta * sin(omega),
		 A   * ((A+1) - (A-1) * cos(omega) + beta * sin(omega)),
		 2*A * ((A-1) - (A+1) * cos(omega)),
		 A   * ((A+1) - (A-1) * cos(omega) - beta * sin(omega))
	);
}

constexpr BQF_Params BQF_HSF( float freq, float q, float gain, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float A = pow( 10, ( gain/40 ) );
	float beta = sqrt(A) / q;

	return BQF_Params
	(
		        (A+1) - (A-1) * cos(omega) + beta * sin(omega),
		   2 * ((A-1) - (A+1) * cos(omega)),
		        (A+1) - (A-1) * cos(omega) - beta * sin(omega),
		   A * ((A+1) + (A-1) * cos(omega) + beta * sin(omega)),
		-2*A * ((A-1) + (A+1) * cos(omega)),
		   A * ((A+1) + (A-1) * cos(omega) - beta * sin(omega))
	);
}

constexpr BQF_Params BQF_Peak( float freq, float bw, float gain, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) * sinh( log( 2 ) / 2 * bw * omega / sin(omega) );
	float A = pow( 10, ( gain / 40 ) );

	return BQF_Params
	(
		 1 + (alpha/A),
		-2 * cos(omega),
		 1 - (alpha/A),
		 1 + (alpha*A),
		-2 * cos(omega),
		 1 - (alpha*A)
	);
}

constexpr BQF_Params BQF_APF( float freq, float q, int samplerate = _FS_ )
{
	float omega = 2 * M_PI * freq / samplerate;
	float alpha = sin(omega) / ( 2 * q );

	return BQF_Params
	(
		 1 + alpha,
		-2 * cos(omega),
		 1 - alpha,
		 1 - alpha,
		-2 * cos(omega),
		 1 + alpha
	);
}

#endif
