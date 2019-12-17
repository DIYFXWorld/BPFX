/*
 * FX_Phaser.h
 *
 *  Created on: 2019/03/19
 *      Author: gamenyge
 */

#ifndef FX_PHASER_II_H_
#define FX_PHASER_II_H_

#include	"Q15T_LFO.h"
#include	"Q15T_BQF.h"
#include	"Audio.h"
#include	"Volume.h"
#include	"Potentiometer_Curve.h"
#include	"FX_ID.h"
#include	"FX_Interface.h"
#include	"Common.h"

//const	Q15T CONST_1( 1 );

struct FX_Phaser_II : public FX_Interface
{
	bool Reset;

	class Allpass_Delay
	{
  private:

    	Q15T	_a1, _zm1;

	public:

		Allpass_Delay() : _a1( 0 ), _zm1( 0 ){}

		void Delay( const Q15T& delay )
		{
			_a1 = ( CONST_1 - delay ) / ( CONST_1 + delay );
    }

		int Update( const int& inSamp )
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

	Type_Volume<Curve_F>		Rate;				// Control_0
	Type_Volume<Curve_C>		Depth;			// Control_2
	Type_Volume<Curve_B>		Feedback;
	Type_Volume<Curve_B>		Mix_Level;
	Q15T_LFO_Sin						LFO;
	Q15T_BQF								HPF, LPF;
	uint8_t									num_APF;

	FX_Phaser_II() :
		Reset( false ),
		_fb( 0.2f ),
    _depth( 1.f ),
    _zm1( 0 ),
		LFO( Audio::Sampling_Rate ),
		num_APF( 6 )
	{
  	Set_Range( Q15T( 200 ), Q15T( 1600 ) );
		HPF = BQF_Builder( Audio::Sampling_Rate ).HPF(   200.f, 0.75f );
		LPF = BQF_Builder( Audio::Sampling_Rate ).LPF( 15000.f, 0.75f );
  	Mix_Level.Set( UINT12_MAX*7/10 );
  }

	void Destroy() { delete this; }

  void Set_Range( const Q15T& min, const Q15T& max )	 // Hz
	{
   	_dmin = min / ( Audio::Sampling_Rate / 2 );
    _dmax = max / ( Audio::Sampling_Rate / 2  );
  }

	int Process( int inSamp )
	{
		inSamp = HPF.Process( inSamp );

		// Set Rate
		{
			int v = Map( Rate.Get(), 0, UINT12_MAX, 2, UINT12_MAX );
			LFO.Rate( Fraction( v, 300 ) );
		}
		// Set Depth
		{
			int v = Map( Depth.Get(), 0, UINT12_MAX, 0, INT16_MAX );
			_depth.Value = v;
		}
		// Set Feedback
		{
			int v = Map( Feedback.Get(), 0, UINT12_MAX, 0, INT16_MAX*9/10 );
			_fb.Value = v;
		}

    Q15T d  = _dmin + ( _dmax -_dmin ) * ( ( LFO.Get() * _depth + CONST_1 ) / 2 );

    int	y = 0;

    if( num_APF == 2 )
    {
    	for( int i( 0 ); i < 2; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ].Update(
    			_alps[ 1 ].Update( ( ( _fb * _zm1 ) + inSamp ).to_int() ) );
    }
    else if( num_APF == 4 )
    {
    	for( int i( 0 ); i < 4; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ].Update(
    			_alps[ 1 ].Update(
    			_alps[ 2 ].Update(
    			_alps[ 3 ].Update( ( ( _fb * _zm1 ) + inSamp ).to_int() ) ) ) );
    }
    else if( num_APF == 6 )
    {
    	for( int i( 0 ); i < 6; ++i )
    	{
    		_alps[ i ].Delay( d );
    	}

    	y =	_alps[ 0 ].Update(
    			_alps[ 1 ].Update(
    			_alps[ 2 ].Update(
    			_alps[ 3 ].Update(
    			_alps[ 4 ].Update(
    			_alps[ 5 ].Update( ( ( _fb * _zm1 ) + inSamp ).to_int() ) ) ) ) ) );
    }
  	_zm1 = y;

		int	wet = Mix_Level.Per( y );

		wet = Limit( INT16_MIN, wet, INT16_MAX );

		wet = LPF.Process( wet );

		return wet;
	}

	virtual void Set_Control_0( int v )	{ Rate.Set( v ); }
	virtual int  Get_Control_0() const	{ return Rate.Initial_Value; }

	virtual void Set_Control_1( int v )	{ Mix_Level.Set( v ); }
	virtual int  Get_Control_1() const	{ return Mix_Level.Initial_Value; }

	virtual void Set_Control_2( int v )	{ Depth.Set( v ); }
	virtual int  Get_Control_2() const	{	return Depth.Initial_Value; }

	virtual void Set_Control_3( int v )	{ Feedback.Set( v ); }
	virtual int  Get_Control_3() const	{ return Feedback.Initial_Value; }

	virtual int Get_Memory_Length() const	{ return 0; }

	virtual enum_FX_ID Get_ID() const	{ return FX_ID_Phaser; }

	virtual void Set_Control_4( int v )	{ num_APF = v; }
	virtual int  Get_Control_4() const	{ return num_APF; }
};

#endif /* FX_PHASER_II_H_ */
