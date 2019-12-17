#include	"Common.h"

static const int PER_100	= UINT12_MAX;
static const int PER_10 	= PER_100 * 10 / 100;
static const int PER_20 	= PER_100 * 20 / 100;
static const int PER_35 	= PER_100 * 35 / 100;
static const int PER_50		= PER_100 * 50 / 100;
static const int PER_65		= PER_100 * 65 / 100;
static const int PER_80 	= PER_100 * 80 / 100;
static const int PER_90 	= PER_100 * 90 / 100;

static int To_Curve_X( const int& vv, const int& width, const int& height )
{
	int	Result = 0;

	int v = Limit( 0, vv, PER_100 );

       if(     0 <= v && v <  width   )	Result = Map( v,     0,   width,      0,  height );
	else if( width <= v && v <= PER_100 )	Result = Map( v, width, PER_100, height, PER_100 );

	return Result;
}

int To_Curve_A( int v ) { return To_Curve_X( v, PER_50, PER_20 ); }
int To_Curve_B( int v ) { return v; }
int To_Curve_C( int v ) { return To_Curve_X( v, PER_50, PER_80 ); }
int To_Curve_D( int v ) { return To_Curve_X( v, PER_65, PER_20 ); }
int To_Curve_E( int v ) { return To_Curve_X( v, PER_35, PER_80 ); }
int To_Curve_F( int v ) { return To_Curve_X( v, PER_80, PER_20 ); }
int To_Curve_G( int v ) { return To_Curve_X( v, PER_20, PER_80 ); }
int To_Curve_H( int v ) { return To_Curve_X( v, PER_90, PER_10 ); }
int To_Curve_I( int v ) { return To_Curve_X( v, PER_10, PER_90 ); }
