#ifndef	Sub_Process_h
#define	Sub_Process_h

template <typename T>
struct Sub_Process_2
{
	T&				Owner;
	int				Output, Count, Last, Sum;

	Sub_Process_2( T* owner ):
		Owner( *owner ),
		Output( 0 ), Count( 0 ), Last( 0 ), Sum( 0 )
	{
	}

	int operator()( int input )
	{
		if( Count == 0 )
		{
			Owner.SUB_PROCESS_0( Last );
			Sum = input;
			++Count;
		}
		else
		if( Count == 1 )
		{
			Output = Owner.SUB_PROCESS_1();
			Sum += input;
			Last = Sum / 2;
			Count = 0;
		}

		return Output;
	}

	void Reset()
	{
		Output = Count = Last = Sum = 0;
	}
};

template <typename T>
struct Sub_Process_4
{
	T&				Owner;
	int				Output, Count, Last, Sum;

	Sub_Process_4( T& owner ):
		Owner( owner ),
		Output( 0 ), Count( 0 ), Last( 0 ), Sum( 0 )
	{
	}

	int operator()( int input )
	{
		if( Count == 0 )
		{
			Owner.SUB_PROCESS_0( Last );
			Sum = input;
			++Count;
		}
		else
		if( Count == 1 )
		{
			Owner.SUB_PROCESS_1();
			Sum += input;
			++Count;
		}
		else
		if( Count == 2 )
		{
			Owner.SUB_PROCESS_2();
			Sum += input;
			++Count;
		}
		else
		if( Count == 3 )
		{
			Output = Owner.SUB_PROCESS_3();
			Sum += input;
			Last = Sum / 4;
			Count = 0;
		}

		return Output;
	}

	void Reset()
	{
		Output = Count = Last = Sum = 0;
	}
};

#endif
