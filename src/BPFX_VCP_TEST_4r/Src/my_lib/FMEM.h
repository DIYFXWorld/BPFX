#ifndef	FMEM_h
#define	FMEM_h

#include	<utility>
#include	"hal.h"
#include	"hal_flash.h"
#include	"FMEM_Address.h"
#include	"FMEM_Base.h"

extern FLASH_ProcessTypeDef pFlash;

// 注)インデックス0xFFを空ページの識別に使っているので
// 使用できるインデックスは0x00～0xFEまでになります。

// 2バイト毎に書き込みをしているので
// データサイズは2バイト単位に揃えます。
// 2バイト単位であれば64ビット以上のデータでも書き込めます。

// インデックスが増えるとガベコレが遅くなります。

template <typename ElementT>
struct FMEM : public FMEM_Base<ElementT>
{
public:

	// デフォルトはページ60から4kバイト(4ページ分)、1セグメント=2k
	FMEM( uint32_t page = FMEM_PAGE_ADDRESS( 60 ), uint8_t pages_per_segment = 2 ) : 
		FMEM_Base<ElementT>( page, pages_per_segment )	{}

protected:

	virtual void WRITE( ElementT* write_address, const ElementT& v )
	{
		HAL_FLASH_Unlock();
		{
			uint16_t	*add( ( uint16_t* )write_address );
			uint16_t	*data_ptr( ( uint16_t* )&v );

			for( int i( 0 ); i < ( int )sizeof( ElementT ); i += 2 )
			{
				HAL_FLASH_Program( FLASH_TYPEPROGRAM_HALFWORD, ( uint32_t )add, *data_ptr );
				++add;
				++data_ptr;
			}
		}
		HAL_FLASH_Lock();
	}

	virtual void ERASE( ElementT* page )
	{
		HAL_FLASH_Unlock();
		{
			uint32_t	page_error;

		  FLASH_EraseInitTypeDef	s;
  		s.TypeErase   = FLASH_TYPEERASE_PAGES;
	  	s.PageAddress = ( uint32_t )page;
	  	s.NbPages     = FMEM_Base<ElementT>::Pages_Per_Segment;

  		HAL_FLASHEx_Erase( &s, &page_error );
		}
		HAL_FLASH_Lock();
	};
};

#endif
