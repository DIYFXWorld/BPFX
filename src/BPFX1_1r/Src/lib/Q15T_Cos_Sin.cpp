#include	"Q15T_Cos_Sin.h"

static const int16_t Q15T_COS_HI[ 256 ] =
{
  32767, 32758, 32728, 32679, 32610, 32521, 32413, 32285, 32138, 31971, 31785, 31581, 31357, 31114, 30852, 30572,
  30273, 29956, 29621, 29269, 28898, 28511, 28106, 27684, 27245, 26790, 26319, 25832, 25330, 24812, 24279, 23732,
  23170, 22594, 22005, 21403, 20787, 20159, 19519, 18868, 18204, 17530, 16846, 16151, 15446, 14732, 14010, 13278,
  12539, 11793, 11039, 10278, 9512, 8739, 7961, 7179, 6392, 5602, 4808, 4011, 3211, 2410, 1607, 804,
  0, -804, -1607, -2410, -3211, -4011, -4808, -5602, -6392, -7179, -7961, -8739, -9512, -10278, -11039, -11793,
  -12539, -13278, -14010, -14732, -15446, -16151, -16846, -17530, -18204, -18868, -19519, -20159, -20787, -21403, -22005, -22594,
  -23170, -23732, -24279, -24812, -25330, -25832, -26319, -26790, -27245, -27684, -28106, -28511, -28898, -29269, -29621, -29956,
  -30273, -30572, -30852, -31114, -31357, -31581, -31785, -31971, -32138, -32285, -32413, -32521, -32610, -32679, -32728, -32758,
  -32768, -32758, -32728, -32679, -32610, -32521, -32413, -32285, -32138, -31971, -31785, -31581, -31357, -31114, -30852, -30572,
  -30273, -29956, -29621, -29269, -28898, -28511, -28106, -27684, -27245, -26790, -26319, -25832, -25330, -24812, -24279, -23732,
  -23170, -22594, -22005, -21403, -20787, -20159, -19519, -18868, -18204, -17530, -16846, -16151, -15446, -14732, -14010, -13278,
  -12539, -11793, -11039, -10278, -9512, -8739, -7961, -7179, -6392, -5602, -4808, -4011, -3211, -2410, -1607, -804,
  0, 804, 1607, 2410, 3211, 4011, 4808, 5602, 6392, 7179, 7961, 8739, 9512, 10278, 11039, 11793,
  12539, 13278, 14010, 14732, 15446, 16151, 16846, 17530, 18204, 18868, 19519, 20159, 20787, 21403, 22005, 22594,
  23170, 23732, 24279, 24812, 25330, 25832, 26319, 26790, 27245, 27684, 28106, 28511, 28898, 29269, 29621, 29956,
  30273, 30572, 30852, 31114, 31357, 31581, 31785, 31971, 32138, 32285, 32413, 32521, 32610, 32679, 32728, 32758,
};

static const int16_t Q15T_COS_LO[ 256 ] =
{
  32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,
  32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,
  32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,
  32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,
  32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767,
  32767, 32767, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766,
  32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766, 32766,
  32766, 32766, 32766, 32766, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765,
  32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32765, 32764, 32764,
  32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764, 32764,
  32764, 32764, 32764, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763, 32763,
  32763, 32763, 32763, 32763, 32763, 32763, 32763, 32762, 32762, 32762, 32762, 32762, 32762, 32762, 32762, 32762,
  32762, 32762, 32762, 32762, 32762, 32762, 32762, 32762, 32761, 32761, 32761, 32761, 32761, 32761, 32761, 32761,
  32761, 32761, 32761, 32761, 32761, 32761, 32761, 32761, 32760, 32760, 32760, 32760, 32760, 32760, 32760, 32760,
  32760, 32760, 32760, 32760, 32760, 32760, 32760, 32759, 32759, 32759, 32759, 32759, 32759, 32759, 32759, 32759,
  32759, 32759, 32759, 32759, 32759, 32758, 32758, 32758, 32758, 32758, 32758, 32758, 32758, 32758, 32758, 32758,
};

static const int16_t Q15T_SIN_HI[ 256 ] =
{
  0, 804, 1607, 2410, 3211, 4011, 4808, 5602, 6392, 7179, 7961, 8739, 9512, 10278, 11039, 11793,
  12539, 13278, 14010, 14732, 15446, 16151, 16846, 17530, 18204, 18868, 19519, 20159, 20787, 21403, 22005, 22594,
  23170, 23732, 24279, 24812, 25330, 25832, 26319, 26790, 27245, 27684, 28106, 28511, 28898, 29269, 29621, 29956,
  30273, 30572, 30852, 31114, 31357, 31581, 31785, 31971, 32138, 32285, 32413, 32521, 32610, 32679, 32728, 32758,
  32767, 32758, 32728, 32679, 32610, 32521, 32413, 32285, 32138, 31971, 31785, 31581, 31357, 31114, 30852, 30572,
  30273, 29956, 29621, 29269, 28898, 28511, 28106, 27684, 27245, 26790, 26319, 25832, 25330, 24812, 24279, 23732,
  23170, 22594, 22005, 21403, 20787, 20159, 19519, 18868, 18204, 17530, 16846, 16151, 15446, 14732, 14010, 13278,
  12539, 11793, 11039, 10278, 9512, 8739, 7961, 7179, 6392, 5602, 4808, 4011, 3211, 2410, 1607, 804,
  0, -804, -1607, -2410, -3211, -4011, -4808, -5602, -6392, -7179, -7961, -8739, -9512, -10278, -11039, -11793,
  -12539, -13278, -14010, -14732, -15446, -16151, -16846, -17530, -18204, -18868, -19519, -20159, -20787, -21403, -22005, -22594,
  -23170, -23732, -24279, -24812, -25330, -25832, -26319, -26790, -27245, -27684, -28106, -28511, -28898, -29269, -29621, -29956,
  -30273, -30572, -30852, -31114, -31357, -31581, -31785, -31971, -32138, -32285, -32413, -32521, -32610, -32679, -32728, -32758,
  -32768, -32758, -32728, -32679, -32610, -32521, -32413, -32285, -32138, -31971, -31785, -31581, -31357, -31114, -30852, -30572,
  -30273, -29956, -29621, -29269, -28898, -28511, -28106, -27684, -27245, -26790, -26319, -25832, -25330, -24812, -24279, -23732,
  -23170, -22594, -22005, -21403, -20787, -20159, -19519, -18868, -18204, -17530, -16846, -16151, -15446, -14732, -14010, -13278,
  -12539, -11793, -11039, -10278, -9512, -8739, -7961, -7179, -6392, -5602, -4808, -4011, -3211, -2410, -1607, -804,
};

static const int16_t Q15T_SIN_LO[ 256 ] =
{
  0, 3, 6, 9, 12, 15, 18, 21, 25, 28, 31, 34, 37, 40, 43, 47,
  50, 53, 56, 59, 62, 65, 69, 72, 75, 78, 81, 84, 87, 91, 94, 97,
  100, 103, 106, 109, 113, 116, 119, 122, 125, 128, 131, 135, 138, 141, 144, 147,
  150, 153, 157, 160, 163, 166, 169, 172, 175, 179, 182, 185, 188, 191, 194, 197,
  201, 204, 207, 210, 213, 216, 219, 223, 226, 229, 232, 235, 238, 241, 245, 248,
  251, 254, 257, 260, 263, 267, 270, 273, 276, 279, 282, 285, 289, 292, 295, 298,
  301, 304, 307, 311, 314, 317, 320, 323, 326, 329, 333, 336, 339, 342, 345, 348,
  351, 354, 358, 361, 364, 367, 370, 373, 376, 380, 383, 386, 389, 392, 395, 398,
  402, 405, 408, 411, 414, 417, 420, 424, 427, 430, 433, 436, 439, 442, 446, 449,
  452, 455, 458, 461, 464, 468, 471, 474, 477, 480, 483, 486, 490, 493, 496, 499,
  502, 505, 508, 512, 515, 518, 521, 524, 527, 530, 534, 537, 540, 543, 546, 549,
  552, 556, 559, 562, 565, 568, 571, 574, 578, 581, 584, 587, 590, 593, 596, 600,
  603, 606, 609, 612, 615, 618, 621, 625, 628, 631, 634, 637, 640, 643, 647, 650,
  653, 656, 659, 662, 665, 669, 672, 675, 678, 681, 684, 687, 691, 694, 697, 700,
  703, 706, 709, 713, 716, 719, 722, 725, 728, 731, 735, 738, 741, 744, 747, 750,
  753, 757, 760, 763, 766, 769, 772, 775, 779, 782, 785, 788, 791, 794, 797, 801,
};

Q15T Q15T_Cos( uint16_t i )
{
	uint16_t	hi = i >> 8;
	uint16_t	lo = i & 0xFF;

	Q15T	Result;

	Result.Value = ( ( ( int32_t( Q15T_COS_HI[ hi ] ) * int32_t( Q15T_COS_LO[ lo ] ) ) )
							 -   ( ( int32_t( Q15T_SIN_HI[ hi ] ) * int32_t( Q15T_SIN_LO[ lo ] ) ) ) ) >> 15;

	return Result;
}

Q15T Q15T_Sin( uint16_t i )
{
	uint16_t	hi = i >> 8;
	uint16_t	lo = i & 0xFF;

	Q15T	Result;

	Result.Value = ( ( ( int32_t( Q15T_SIN_HI[ hi ] ) * int32_t( Q15T_COS_LO[ lo ] ) ) )
							 +   ( ( int32_t( Q15T_COS_HI[ hi ] ) * int32_t( Q15T_SIN_LO[ lo ] ) ) ) ) >> 15;
	return Result;
}

