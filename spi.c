#include "spi.h"
#include "config.h"

U8 dummy;

void SPI_Init( void ) 
{   
    /* Disable SSP */
    SSPEN   = 0;
    /* Set alternate pins locations */
    SDOSEL  = 1;
    SSSEL   = 0;
    /* SPI Data Input Sample Bit Reset */
	SMP     = 0;
    /* SPI Clock Edge - Idle to active */
	CKE     = 0;
    /* Idle clock on low */
	CKP     = 0;
    /* SS Mode - SPI Fosc/4 */
	SSPM0   = 0;
    SSPM1   = 0;
    SSPM2   = 0;
    SSPM3   = 0;

    SSP1IE  = 1;
    /* Enable SSP */
	SSPEN   = 1;
} 

void SPI_WriteBytes( U8 len, U8 * data )
{
    while( len-- )
    {
        SPI_WriteByte( *data++ );
    }
}

void SPI_WriteByte( U8 data )
{   
    SSPBUF = data; 
    while( !SSP1IF );
    data = SSPBUF;
}

void SPI_ReadBytes( U8 len, U8 * data )
{
    while( len-- )
    {
        *data++ = SPI_ReadByte();
    }
}

U8 SPI_ReadByte( void )
{   
    register U8 data = 0;
	SSPBUF = 0;				// write out to buffer
    while( BF == 0 );				// wait for flag
    data = SSPBUF;
    return data;
}


/*
U8 Comm_IF_Spi1ReadByteBitbang(void)
{
    SEGMENT_VARIABLE(read_byte, U8, SEG_DATA) = 0u;
    SEGMENT_VARIABLE(ii,        U8, SEG_DATA);
    
    MCU_SCK = TRUE;
    
    NOP();
    NOP();
    NOP();
    NOP();
    
    MCU_SCK = FALSE;
    
    NOP();
    
    for(ii = 0u; ii < 8u; ii++)
    {
        MCU_SCK = TRUE;
        read_byte <<= 1u;
        
        / * Sample the line * /
        if (TRUE == MCU_MISO)
        {
            read_byte |= 0x01;
        }
        MCU_SCK = FALSE;
    }
    
    return read_byte;
}

/ **
 *  Write one byte to the SPI1 using bit-bang method.
 *
 *  @param[in] data_in Data byte to be sent.
 *  @param[in] nmbr_bit Number of bits to be sent from the data byte.
 *
 *  @note
 *
 ****************************************************************************** /
void Comm_IF_Spi1WriteBitsBitbang(U8 data_in, U8 nmbr_bit)
{
    SEGMENT_VARIABLE(ii,     U8, SEG_DATA);
    SEGMENT_VARIABLE(lMask,  U8, SEG_DATA);
    
    lMask = (1u << (nmbr_bit - 1u));
    for (ii = 0u; ii < nmbr_bit; ii++)
    {
        MCU_SCK = FALSE;
        if ((data_in & lMask) != 0u)
        {
            MCU_MOSI = TRUE;
        }
        else
        {
            MCU_MOSI = FALSE;
        }
        / * clock pulse to sample * /
        MCU_SCK = TRUE;
        lMask >>= 1u;
    }
    
    MCU_SCK = FALSE;
}
*/
