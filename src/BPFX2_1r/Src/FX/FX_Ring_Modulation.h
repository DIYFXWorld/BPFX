#ifndef FX_Ring_Modulation_H_
#define FX_Ring_Modulation_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>

struct FX_Ring_Modulation : public FX_Interface
{
	Volume<Curve_B>		Frequency;
	Q15T_LFO					LFO;

	FX_Ring_Modulation() :
		LFO( _FS_, int16_t_Sin_Table )
	{
		Frequency.Set_Value( 50 );
	}

	void Destroy()	{ delete this; }

	int Process( int input )
	{
		// Set Rate
		{
			int v = Map( Frequency.Get_Value(), 0, UINT12_MAX, 30, UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 2 ) );
		}

		int	wet = ( LFO.Get_Value() * input ).to_int();

		wet  = LIMIT_INT16( wet ) ;

		return wet - input;
	}

	virtual void Set_Param_0( int v )	{ Frequency.Set_Value( Map( v, 0, UINT12_MAX, 1, UINT12_MAX ) ); }
	virtual int  Get_Param_0() const	{ return Frequency.Initial_Value; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Ring_Modulation; }

	void Reset()
	{
		Frequency.Fast_Forward();
		LFO.Reset();
	}
};

#endif
