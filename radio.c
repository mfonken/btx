#include "types.h"
#include "config.h"
#include "radio.h"
#include "comm.h"
#include "si4060.h"
#include "si4060_defs.h"
#include "radio_config.h"

U8 const Radio_Configuration_Data_Array[] = RADIO_CONFIGURATION_DATA_ARRAY;
tRadioConfiguration RadioConfiguration = RADIO_CONFIGURATION_DATA;
tRadioConfiguration *pRadioConfiguration = &RadioConfiguration;
tRadioPacket RadioPacket;

void Radio_PowerUp( void )
{
    U16 wDelay = 0u;
    
    //si4060_reset();
    for( ; wDelay < 50; wDelay++);//pRadioConfiguration->Radio_Delay_Cnt_After_Reset; wDelay++ );
    radio_comm_PollCTS();
}

void Radio_Init( void )
{
  /* Power Up the radio chip */
  Radio_PowerUp();

  /* Load radio configuration */
  while( SUCCESS != si4060_configuration_init( pRadioConfiguration->Radio_ConfigurationArray ) )
  {
    /* Power Up the radio chip */
    Radio_PowerUp();
  }

  // Read ITs, clear pending ones
  si4060_get_int_status(0u, 0u, 0u);
}

void Radio_StartTX( U8 channel, tx_complete_t complete, U8 retx, U8 length )
{
    U8 condition = ( complete << 4 ) + retx;
    
    // Read ITs, clear pending ones
    si4060_get_int_status( 0u, 0u, 0u );
    
    /* Start Receiving packet, channel 0, START immediately, Packet size by PH */
    si4060_start_tx( channel, condition, length );
}

U8 Radio_Check_Ezconfig( U16 crc )
{
    si4060_ezconfig_check( crc );
    
    return radioCmd[0u];
}
