#ifndef	FX_Config_h
#define	FX_Config_h

const int _FS_ = 40000;

#define	MEMORY_ALLOCATOR			Array_Pool
#define	MEMORY_ALLOCATOR_4BIT	Array_Pool_4bit

#define	TTT	7800

const int FX_NONE_BUFFER_LENGTH									= TTT;
const int FX_DELAY_0_BUFFER_LENGTH							= TTT;
const int FX_DELAY_BUFFER_LENGTH								= TTT;
const int FX_REVERSE_DELAY_BUFFER_LENGTH				= TTT;
const int FX_REVERSE_DELAY_PCMU_BUFFER_LENGTH		= TTT;
const int FX_ANALOG_DELAY_BUFFER_LENGTH					= TTT;

const	int	FX_DELAY_PCMU_BUFFER_LENGTH						= TTT*2-50;
const	int FX_DELAY_ADPCM_BUFFER_LENGTH					= TTT*4-50;
const	int FX_TWO_TAP_DELAY_BUFFER_LENGTH				= TTT*4-50;
const int FX_MODULATION_DELAY_BUFFER_LENGTH			= TTT-500;

#endif
