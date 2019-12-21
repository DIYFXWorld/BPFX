#ifndef	Fraction_h
#define	Fraction_h

struct Fraction
{
	int		HI, LO;

	inline Fraction( void )	{}
	inline Fraction( const int& hi, const int& lo ) : HI( hi ), LO( lo )	{}
};

inline Q15T operator * ( const Q15T& a, const Fraction& b )
{
	Q15T	Result;
	Result.Value = a.Value * b.HI / b.LO;
	return Result;
}

inline Q15T operator / ( const Q15T& a, const Fraction& b )
{
	Q15T	Result;
	Result.Value = a.Value * b.LO / b.HI;
	return Result;
}

inline int operator * ( int a, const Fraction& b )
{
	return a * b.HI / b.LO;
}

inline int operator / ( int a, const Fraction& b )
{
	return a * b.LO / b.HI;
}

#endif
