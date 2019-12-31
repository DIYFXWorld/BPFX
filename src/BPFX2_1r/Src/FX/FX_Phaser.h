#ifndef FX_PHASER_H_
#define FX_PHASER_H_

#include <FX_Config.h>
#include <Myutil.h>
#include <Volume.h>
#include <FX_Interface.h>
#include <Sub_Process.h>
#include <Q15T_BQF.h>
#include <Q15T_LFO.h>

constexpr Q15T_BQF_Param FX_Phaser_HPF_Param = BQF_Builder( _FS_ ).HPF(   200.f, 0.75f );
constexpr Q15T_BQF_Param FX_Phaser_LPF_Param = BQF_Builder( _FS_ ).LPF( 10000.f, 0.75f );

struct FX_Phaser : public FX_Interface
{
	class Allpass_Delay
	{
  private:

    	Q15T	_a1, _zm1;

	public:

		Allpass_Delay() : _a1( 0 ), _zm1( 0 ){}

		void Delay( const Q15T& delay )
		{
			_a1 = ( Q15T_1 - delay ) / ( Q15T_1 + delay );
    }

		int operator()( const int& inSamp )
		{
			int	y = ( ( _a1 * -1 ) * inSamp + _zm1 ).to_int();

			_zm1 = ( _a1 * y ) + inSamp;

			return y;
		}
	};

  Allpass_Delay _alps[ 6 ];

	Q15T	_dmin, _dmax;		// range
  Q15T	_fb;						// feedback		0 -> <1.0
  Q15T	_depth;					// 0 -> 1.0
  int		_zm1;

	Volume<Curve_F>		Rate;				// Control_0
	Volume<Curve_C>		Depth;			// Control_2
	Volume<Curve_B>		Feedback;
	Volume<Curve_B>		Mix_Level;
	Q15T_LFO					LFO;
	Q15T_BQF					HPF, LPF;
	uint8_t						num_APF;

	FX_Phaser() :
		_fb( 0.2f ),
    _depth( 1.f ),
    _zm1( 0 ),
		LFO( _FS_, int16_t_Sin_Table ),
		num_APF( 6 )
	{
  	Set_Range( Q15T( 200 ), Q15T( 1600 ) );
		HPF = FX_Phaser_HPF_Param;
		LPF = FX_Phaser_LPF_Param;
  	Mix_Level.Set_Value( UINT12_MAX*7/10 );
		Reset();
  }

  void Set_Range( const Q15T& min, const Q15T& max )	 // Hz
	{
   	_dmin = min / ( _FS_ / 2 );
    _dmax = max / ( _FS_ / 2  );
  }

	int Process( int inSamp )
	{
		inSamp = HPF( inSamp );

		// Set Rate
		{
			int v = Map( Rate.Get_Value(), 0, UINT12_MAX, 2, UINT12_MAX );
			LFO.Set_Rate( Fraction( v, 300 ) );
		}
		// Set Depth
		{
			int v = Map( Depth.Get_Value(), 0, UINT12_MAX, 0, INT16_MAX );
			_depth.Value = v;
		}
		// Set Feedback
		{
			int v = Map( Feedback.Get_Value(),0, UINT12_MAX, 0,INT16_MAX*9/10 );
			_fb.Value = v;
		}

    Q15T d  = _dmin + ( _dmax -_dmin ) * ( ( LFO.Get_Value() * _depth + Q15T_1 ) / 2 );

    int	y = 0;

    if( num_APF == 2 )
    {
    	for( int i( 0 ); i < 2; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ](	_alps[ 1 ]( ( ( _fb * _zm1 ) + inSamp ).to_int() ) );
    }
    else if( num_APF == 4 )
    {
    	for( int i( 0 ); i < 4; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ](	_alps[ 1 ](	_alps[ 2 ](	_alps[ 3 ]( ( ( _fb * _zm1 ) + inSamp ).to_int() ) ) ) );
    }
    else if( num_APF == 6 )
    {
    	for( int i( 0 ); i < 6; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ](	_alps[ 1 ](	_alps[ 2 ](	_alps[ 3 ](	_alps[ 4 ](	_alps[ 5 ](	( ( _fb * _zm1 ) + inSamp ).to_int() ) ) ) ) ) );
    }
  	_zm1 = y;

		int	wet = Mix_Level.Per( y );

		wet = LIMIT_INT16( wet );

		wet = LPF( wet );

		return wet;
	}

	virtual void Set_Param_0( int v )	{ Rate.Set_Value( v ); }
	virtual int  Get_Param_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Param_1( int v )	{ Mix_Level.Set_Value( v ); }
	virtual int  Get_Param_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Param_2( int v )	{ Depth.Set_Value( v ); }
	virtual int  Get_Param_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Param_3( int v )	{ Feedback.Set_Value( v ); }
	virtual int  Get_Param_3() const	{ return Feedback.Initial_Value; }

	virtual void Set_Param_4( int v )	{ num_APF = v; }
	virtual int  Get_Param_4() const	{ return num_APF; }

	virtual FX_ID Get_FX_ID() const	{ return FX_ID_Phaser; }

	void Reset()
	{
	}
};

#endif /* FX_PHASER_II_H_ */
