/*
 * BQF.h
 *
 *  Created on: 2019/06/12
 *      Author: gamenyge
 */

#include	"BQF.h"
#include	<math.h>

BQF_Param BQF_Builder::LPF( float freq, float q ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha;
	v.b0 = (1.0f - cos(omega)) / 2.0f;
	v.b1 = 1.0f - cos(omega);
	v.b2 = (1.0f - cos(omega)) / 2.0f;
	return v;
}

BQF_Param BQF_Builder::HPF( float freq, float q ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha;
	v.b0 = (1.0f + cos(omega)) / 2.0f;
	v.b1 = -(1.0f + cos(omega));
	v.b2 = (1.0f + cos(omega)) / 2.0f;
	return v;
}

BQF_Param BQF_Builder::BPF( float freq, float bw ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) * sinh(log(2.0f) / 2.0 * bw * omega / sin(omega));

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha;
	v.b0 = alpha;
	v.b1 = 0.0f;
	v.b2 = -alpha;
	return v;
}

BQF_Param BQF_Builder::NTC(float freq, float bw ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) * sinh(log(2.0f) / 2.0 * bw * omega / sin(omega));

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha;
	v.b0 = 1.0f;
	v.b1 = -2.0f * cos(omega);
	v.b2 = 1.0f;
	return v;
}

BQF_Param BQF_Builder::LSF( float freq, float q, float gain ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
//	float alpha = sin(omega) / (2.0f * q);
	float A = pow(10.0f, (gain / 40.0f));
	float beta = sqrt(A) / q;

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = (A + 1.0f) + (A - 1.0f) * cos(omega) + beta * sin(omega);
	v.a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cos(omega));
	v.a2 = (A + 1.0f) + (A - 1.0f) * cos(omega) - beta * sin(omega);
	v.b0 = A * ((A + 1.0f) - (A - 1.0f) * cos(omega) + beta * sin(omega));
	v.b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cos(omega));
	v.b2 = A * ((A + 1.0f) - (A - 1.0f) * cos(omega) - beta * sin(omega));
	return v;
}

BQF_Param BQF_Builder::HSF( float freq, float q, float gain ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
//	float alpha = sin(omega) / (2.0f * q);
	float A = pow(10.0f, (gain / 40.0f));
	float beta = sqrt(A) / q;

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = (A + 1.0f) - (A - 1.0f) * cos(omega) + beta * sin(omega);
	v.a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cos(omega));
	v.a2 = (A + 1.0f) - (A - 1.0f) * cos(omega) - beta * sin(omega);
	v.b0 = A * ((A + 1.0f) + (A - 1.0f) * cos(omega) + beta * sin(omega));
	v.b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cos(omega));
	v.b2 = A * ((A + 1.0f) + (A - 1.0f) * cos(omega) - beta * sin(omega));
	return v;
}

BQF_Param BQF_Builder::PK( float freq, float bw, float gain ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) * sinh(log(2.0f) / 2.0 * bw * omega / sin(omega));
	float A = pow(10.0f, (gain / 40.0f));

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha / A;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha / A;
	v.b0 = 1.0f + alpha * A;
	v.b1 = -2.0f * cos(omega);
	v.b2 = 1.0f - alpha * A;
	return v;
}

BQF_Param BQF_Builder::APF( float freq, float q ) const
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f *  freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	BQF_Param	v;
	// フィルタ係数を求める。
	v.a0 = 1.0f + alpha;
	v.a1 = -2.0f * cos(omega);
	v.a2 = 1.0f - alpha;
	v.b0 = 1.0f - alpha;
	v.b1 = -2.0f * cos(omega);
	v.b2 = 1.0f + alpha;
	return v;
}
