#ifndef	Volume_h
#define	Volume_h

#define	VOLUME_RADIX	4095		// 10 bits

template <typename CurveT>
struct Type_Volume
{
	int16_t		Initial_Value;
	int16_t		Expected_Value;
	int16_t		Value;
	CurveT		Curve;

	explicit Type_Volume( const int16_t& v = 0 ) :
		Initial_Value( v ), Expected_Value( Curve( Initial_Value ) ), Value( Expected_Value )
	{}

	inline void Set( const int16_t& v )
	{
		Initial_Value = v;
		Expected_Value = Curve( Initial_Value );
	}

	inline int16_t Get()
	{
		if( Value < Expected_Value ){	++Value; }
		if( Value > Expected_Value ){	--Value; }

		return Value;
	}

	inline void Reset( const int16_t& v )
	{
		Expected_Value = Value = v;
	}

	inline void Reset( const int16_t& value, const int16_t& expected_value )
	{
		Expected_Value = expected_value;
		Value = value;
	}

	inline int Per /*centage*/ ( int v )
	{
		return v * Get() / VOLUME_RADIX;
	}
};

#endif
