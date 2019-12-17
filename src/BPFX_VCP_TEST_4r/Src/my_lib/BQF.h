/*
 * BQF.h
 *
 *  Created on: 2019/06/12
 *      Author: gamenyge
 */

#ifndef BQF_H_
#define BQF_H_

struct BQF_Param
{
	float		a0, a1, a2;
	float		b0, b1, b2;
};

struct BQF_Builder
{
	const int	samplerate;

	BQF_Builder( int fs ) : samplerate( fs ){}

	BQF_Param LPF( float Frequency, float Q ) const;
	BQF_Param HPF( float Frequency, float Q ) const;
	BQF_Param BPF( float Frequency, float Band_Width ) const;
	BQF_Param NTC( float Frequency, float Band_Width ) const;
	BQF_Param LSF( float Frequency, float Q, float Gain ) const;
	BQF_Param HSF( float Frequency, float Q, float Gain ) const;
	BQF_Param PK ( float Frequency, float Band_Width, float Gain ) const;
	BQF_Param APF( float Frequency, float Q ) const;
};

#endif /* BQF_H_ */
