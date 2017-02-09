#include "types.h"
#include "comm.h"
#include "spi.h"
#include "config.h"

U8 ctsWentHigh = 0;

U8 radio_comm_GetResp( U8 byteCount, U8* pData )
{
    U16 errCnt = RADIO_CTS_TIMEOUT;
    while( errCnt != 0 )      //wait until radio IC is ready with the data
    {
        SS_PIN = 0;
        SPI_WriteByte( 0x44 );    //read CMD buffer
        ctsWentHigh = SPI_ReadByte();
        if( ctsWentHigh )
        {
            if( byteCount )
            {
                SPI_ReadBytes( byteCount, pData );
            }
            SS_PIN = 1;
            break;
        }
        SS_PIN = 1;
        errCnt--;
    }

    if( errCnt == 0 )
    {
        return 0;
    }
    return ctsWentHigh;
}

U8 radio_comm_PollCTS(void)
{
    return radio_comm_GetResp( 0, 0 );
}

void radio_comm_SendCmd( U8 byteCount, U8* pData )
{
    /* There was a bug in A1 hardware that will not handle 1 byte commands. 
       It was supposedly fixed in B0 but the fix didn't make it at the last minute, so here we go again */
    if ( byteCount == 1 )
        byteCount++;

    while( !ctsWentHigh )
    {
        radio_comm_PollCTS();
    }
    SS_PIN = 0;
    SPI_WriteBytes( byteCount, pData );
    SS_PIN = 1;
    ctsWentHigh = 0;
}
                   
U8 radio_comm_SendCmdGetResp( U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData )
{
    radio_comm_SendCmd( cmdByteCount, pCmdData );
    return radio_comm_GetResp( respByteCount, pRespData );
}

void radio_comm_ReadData( U8 cmd, BIT pollCts, U8 byteCount, U8* pData )
{
    if( pollCts )
    {
        while( !ctsWentHigh )
        {
            radio_comm_PollCTS();
        }
    }
    SS_PIN = 0;
    SPI_WriteByte( cmd );
    SPI_ReadBytes( byteCount, pData );
    SS_PIN = 1;
    ctsWentHigh = 0;
}

void radio_comm_WriteData( U8 cmd, BIT pollCts, U8 byteCount, U8* pData )
{
    if( pollCts )
    {
        while( !ctsWentHigh )
        {
            radio_comm_PollCTS();
        }
    }
    SS_PIN = 0;
    SPI_WriteByte( cmd );
    SPI_WriteBytes( byteCount, pData );
    SS_PIN = 1;
    ctsWentHigh = 0;
}


