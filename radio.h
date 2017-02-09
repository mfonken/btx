#ifndef RADIO_H
#define RADIO_H

#define EZCONFIG_ARRAY_SIZE     256u
#define EZCONFIG_RADIO_CFG_SIZE 202u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    U8 * Radio_ConfigurationArray;
    U8   Radio_ChannelNumber;
    U8   Radio_PacketLength;
    U8   Radio_State_After_Power_Up;
    U16  Radio_Delay_Cnt_After_Reset;
} tRadioConfiguration;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
extern tRadioConfiguration * pRadioConfiguration;

/*! Si446x configuration array */
extern const U8 Radio_Configuration_Data_Array[];
extern tRadioPacket RadioPacket;

/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
void Radio_Init( void );
void Radio_SetChip( void );
void Radio_StartTX( U8 channel, tx_complete_t complete, U8 retx, U8 length );
void Radio_PowerUp( void );
U8   Radio_Check_Ezconfig( U16 crc );

#endif /* radio_h */
