#ifndef	Potentiometer_Curve_h
#define	Potentiometer_Curve_h

int To_Curve_A( int );	// log
int To_Curve_B( int );	// liner
int To_Curve_C( int );	// exp
int To_Curve_D( int );	// log
int To_Curve_E( int );	// exp
int To_Curve_F( int );	// log
int To_Curve_G( int );	// exp
int To_Curve_H( int );	// log
int To_Curve_I( int );	// exp

struct Curve_A { int operator()( int v ) const { return To_Curve_A( v ); } };
struct Curve_B { int operator()( int v ) const { return To_Curve_B( v ); } };
struct Curve_C { int operator()( int v ) const { return To_Curve_C( v ); } };
struct Curve_D { int operator()( int v ) const { return To_Curve_D( v ); } };
struct Curve_E { int operator()( int v ) const { return To_Curve_E( v ); } };
struct Curve_F { int operator()( int v ) const { return To_Curve_F( v ); } };
struct Curve_G { int operator()( int v ) const { return To_Curve_G( v ); } };
struct Curve_H { int operator()( int v ) const { return To_Curve_H( v ); } };
struct Curve_I { int operator()( int v ) const { return To_Curve_I( v ); } };

#endif
