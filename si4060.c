#include "comm.h"
#include "si4060_defs.h"
#include "si4060.h"
#include "types.h"
#include "config.h"
#include "spi.h"

#define SI4060_XTAL_FREQ 30000000L

union si4060_cmd_reply_union Si4060Cmd;
U8 radioCmd[16u];

U8 si4060_configuration_init( U8 * pSetPropCmd )
{
  U8 col = 0;
  U8 numOfBytes = 0;

  /* While cycle as far as the pointer points to a command */
  while ( *pSetPropCmd != 0x00 )
  {
    /* Commands structure in the array:
     * --------------------------------
     * LEN | <LEN length of data>
     */

    numOfBytes = *pSetPropCmd++;

    if ( numOfBytes > 16u )
    {
      /* Number of command bytes exceeds maximal allowable length */
      return COMMAND_ERROR;
    }

    for ( col = 0u; col < numOfBytes; col++ )
    {
      radioCmd[col] = *pSetPropCmd;
      pSetPropCmd++;
    }

    if ( !radio_comm_SendCmdGetResp( numOfBytes, radioCmd, 0, 0 ) )
    {
      /* Timeout occurred */
      return CTS_TIMEOUT;
    }

    if( !IRQ_PIN )
    {
      /* Get and clear all interrupts.  An error has occurred... */
      si4060_get_int_status( 0u, 0u, 0u );
//      if ( Si4060Cmd.GET_INT_STATUS.CHIP_PEND )// & SI4060_CMD_GET_CHIP_STATUS_REP_CMD_ERROR_PEND_MASK)
//      {
//        return COMMAND_ERROR;
//      }
    }
  }
  return SUCCESS;
}

void si4060_reset( void )
{
    /* No access to SDN pin */
    /* ? try software reset ? */
}


void si4060_nop( void )
{
    radioCmd[0] = SI4060_CMD_ID_NOP;

    radio_comm_SendCmd( SI4060_CMD_ARG_COUNT_NOP, radioCmd );
}


void si4060_part_info( void )
{
    radioCmd[0] = SI4060_CMD_ID_PART_INFO;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_PART_INFO,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_PART_INFO,
                               radioCmd );

    Si4060Cmd.PART_INFO.CHIPREV         = radioCmd[0];
    Si4060Cmd.PART_INFO.PART.U8[MSB]    = radioCmd[1];
    Si4060Cmd.PART_INFO.PART.U8[LSB]    = radioCmd[2];
    Si4060Cmd.PART_INFO.PBUILD          = radioCmd[3];
    Si4060Cmd.PART_INFO.ID.U8[MSB]      = radioCmd[4];
    Si4060Cmd.PART_INFO.ID.U8[LSB]      = radioCmd[5];
    Si4060Cmd.PART_INFO.CUSTOMER        = radioCmd[6];
    Si4060Cmd.PART_INFO.ROMID           = radioCmd[7];
    Si4060Cmd.PART_INFO.BOND            = radioCmd[8];
}


void si4060_power_up( U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ )
{
    radioCmd[0] = SI4060_CMD_ID_POWER_UP;
    radioCmd[1] = BOOT_OPTIONS;
    radioCmd[2] = XTAL_OPTIONS;
    radioCmd[3] = ( U8 )( XO_FREQ >> 24 );
    radioCmd[4] = ( U8 )( XO_FREQ >> 16 );
    radioCmd[5] = ( U8 )( XO_FREQ >> 8 );
    radioCmd[6] = ( U8 )( XO_FREQ );

    radio_comm_SendCmd( SI4060_CMD_ARG_COUNT_POWER_UP, radioCmd );
}


void si4060_func_info( void )
{
    radioCmd[0] = SI4060_CMD_ID_FUNC_INFO;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_FUNC_INFO,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_FUNC_INFO,
                               radioCmd );

    Si4060Cmd.FUNC_INFO.REVEXT          = radioCmd[0];
    Si4060Cmd.FUNC_INFO.REVBRANCH       = radioCmd[1];
    Si4060Cmd.FUNC_INFO.REVINT          = radioCmd[2];
    Si4060Cmd.FUNC_INFO.PATCH.U8[MSB]   = radioCmd[3];
    Si4060Cmd.FUNC_INFO.PATCH.U8[LSB]   = radioCmd[4];
    Si4060Cmd.FUNC_INFO.FUNC            = radioCmd[5];
    Si4060Cmd.FUNC_INFO.SVNFLAGS        = radioCmd[6];
    Si4060Cmd.FUNC_INFO.SVNREV.U8[3]    = radioCmd[7];
    Si4060Cmd.FUNC_INFO.SVNREV.U8[2]    = radioCmd[8];
    Si4060Cmd.FUNC_INFO.SVNREV.U8[1]    = radioCmd[9];
    Si4060Cmd.FUNC_INFO.SVNREV.U8[0]    = radioCmd[10];
}

void si4060_set_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP, ... )
{
    U8 cmdIndex = 0;

    radioCmd[0] = SI4060_CMD_ID_SET_PROPERTY;
    radioCmd[1] = GROUP;
    radioCmd[2] = NUM_PROPS;
    radioCmd[3] = START_PROP;

    radio_comm_SendCmd( cmdIndex, radioCmd );
}


void si4060_get_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP )
{
    radioCmd[0] = SI4060_CMD_ID_GET_PROPERTY;
    radioCmd[1] = GROUP;
    radioCmd[2] = NUM_PROPS;
    radioCmd[3] = START_PROP;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_GET_PROPERTY,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_GET_PROPERTY,
                               radioCmd );

    Si4060Cmd.GET_PROPERTY.DATA0    = radioCmd[0];
    Si4060Cmd.GET_PROPERTY.DATA1    = radioCmd[1];
    Si4060Cmd.GET_PROPERTY.DATA2    = radioCmd[2];
    Si4060Cmd.GET_PROPERTY.DATA3    = radioCmd[3];
    Si4060Cmd.GET_PROPERTY.DATA4    = radioCmd[4];
    Si4060Cmd.GET_PROPERTY.DATA5    = radioCmd[5];
    Si4060Cmd.GET_PROPERTY.DATA6    = radioCmd[6];
    Si4060Cmd.GET_PROPERTY.DATA7    = radioCmd[7];
    Si4060Cmd.GET_PROPERTY.DATA8    = radioCmd[8];
    Si4060Cmd.GET_PROPERTY.DATA9    = radioCmd[9];
    Si4060Cmd.GET_PROPERTY.DATA10   = radioCmd[10];
    Si4060Cmd.GET_PROPERTY.DATA11   = radioCmd[11];
    Si4060Cmd.GET_PROPERTY.DATA12   = radioCmd[12];
    Si4060Cmd.GET_PROPERTY.DATA13   = radioCmd[13];
    Si4060Cmd.GET_PROPERTY.DATA14   = radioCmd[14];
    Si4060Cmd.GET_PROPERTY.DATA15   = radioCmd[15];
}


void si4060_gpio_pin_cfg( U8 GPIO0, U8 GPIO1, U8 GPIO2, U8 GPIO3, U8 NIRQ, U8 SDO, U8 GEN_CONFIG )
{
    radioCmd[0] = SI4060_CMD_ID_GPIO_PIN_CFG;
    radioCmd[1] = GPIO0;
    radioCmd[2] = GPIO1;
    radioCmd[3] = GPIO2;
    radioCmd[4] = GPIO3;
    radioCmd[5] = NIRQ;
    radioCmd[6] = SDO;
    radioCmd[7] = GEN_CONFIG;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_GPIO_PIN_CFG,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_GPIO_PIN_CFG,
                               radioCmd );

    Si4060Cmd.GPIO_PIN_CFG.GPIO0        = radioCmd[0];
    Si4060Cmd.GPIO_PIN_CFG.GPIO1        = radioCmd[1];
    Si4060Cmd.GPIO_PIN_CFG.GPIO2        = radioCmd[2];
    Si4060Cmd.GPIO_PIN_CFG.GPIO3        = radioCmd[3];
    Si4060Cmd.GPIO_PIN_CFG.NIRQ         = radioCmd[4];
    Si4060Cmd.GPIO_PIN_CFG.SDO          = radioCmd[5];
    Si4060Cmd.GPIO_PIN_CFG.GEN_CONFIG   = radioCmd[6];
}

void si4060_fifo_info( U8 FIFO )
{
    radioCmd[0] = SI4060_CMD_ID_FIFO_INFO;
    radioCmd[1] = FIFO;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_FIFO_INFO,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_FIFO_INFO,
                               radioCmd );

    Si4060Cmd.FIFO_INFO.RX_FIFO_COUNT   = radioCmd[0];
    Si4060Cmd.FIFO_INFO.TX_FIFO_SPACE   = radioCmd[1];
}

void si4060_write_ezconfig_array( U8 numBytes, U8* pEzConfigArray )
{
  //radio_comm_WriteData(SI4060_CMD_ID_WRITE_TX_FIFO, 1, numBytes, pEzConfigArray);
}

void si4060_ezconfig_check( U16 CHECKSUM )
{
  /* Do not check CTS */

  SS_PIN = 0;

  /* Command byte */
  SPI_WriteByte( SI4060_CMD_ID_EZCONFIG_CHECK );

  /* CRC */
  SPI_WriteByte( ( U16 ) CHECKSUM >> 8u );
  SPI_WriteByte( ( U16 ) CHECKSUM & 0x00FF );

  SS_PIN = 1;

  /* Get the response from the radio chip */
  radio_comm_GetResp( 1u, radioCmd );
}


void si4060_get_int_status( U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND )
{
    radioCmd[0] = SI4060_CMD_ID_GET_INT_STATUS;
    radioCmd[1] = PH_CLR_PEND;
    radioCmd[2] = MODEM_CLR_PEND;
    radioCmd[3] = CHIP_CLR_PEND;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_GET_INT_STATUS,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_GET_INT_STATUS,
                               radioCmd );

    Si4060Cmd.GET_INT_STATUS.INT_PEND       = radioCmd[0];
    Si4060Cmd.GET_INT_STATUS.INT_STATUS     = radioCmd[1];
    Si4060Cmd.GET_INT_STATUS.PH_PEND        = radioCmd[2];
    Si4060Cmd.GET_INT_STATUS.PH_STATUS      = radioCmd[3];
    Si4060Cmd.GET_INT_STATUS.MODEM_PEND     = radioCmd[4];
    Si4060Cmd.GET_INT_STATUS.MODEM_STATUS   = radioCmd[5];
    Si4060Cmd.GET_INT_STATUS.CHIP_PEND      = radioCmd[6];
    Si4060Cmd.GET_INT_STATUS.CHIP_STATUS    = radioCmd[7];
}

void si4060_start_tx( U8 CHANNEL, U8 CONDITION, U16 TX_LEN )
{
    radioCmd[0] = SI4060_CMD_ID_START_TX;
    radioCmd[1] = CHANNEL;
    radioCmd[2] = CONDITION;
    radioCmd[3] = ( U8 )( TX_LEN >> 8 );
    radioCmd[4] = ( U8 )( TX_LEN );
    
    radio_comm_SendCmd( SI4060_CMD_ARG_COUNT_START_TX, radioCmd );
}

void si4060_request_device_state(void)
{
    radioCmd[0] = SI4060_CMD_ID_REQUEST_DEVICE_STATE;

    radio_comm_SendCmdGetResp( SI4060_CMD_ARG_COUNT_REQUEST_DEVICE_STATE,
                               radioCmd,
                               SI4060_CMD_REPLY_COUNT_REQUEST_DEVICE_STATE,
                               radioCmd );

    Si4060Cmd.REQUEST_DEVICE_STATE.CURR_STATE       = radioCmd[0];
    Si4060Cmd.REQUEST_DEVICE_STATE.CURRENT_CHANNEL  = radioCmd[1];
}


void si4060_change_state( U8 NEXT_STATE1 )
{
    radioCmd[0] = SI4060_CMD_ID_CHANGE_STATE;
    radioCmd[1] = NEXT_STATE1;

    radio_comm_SendCmd( SI4060_CMD_ARG_COUNT_CHANGE_STATE, radioCmd );
}


void si4060_read_cmd_buff( void )
{
    // TODO:
}


void si4060_frr_a_read( U8 respByteCount )
{
    radio_comm_ReadData( SI4060_CMD_ID_FRR_A_READ,
                         0,
                         respByteCount,
                         radioCmd );

    Si4060Cmd.FRR_A_READ.FRR_A_VALUE = radioCmd[0];
    Si4060Cmd.FRR_A_READ.FRR_B_VALUE = radioCmd[1];
    Si4060Cmd.FRR_A_READ.FRR_C_VALUE = radioCmd[2];
    Si4060Cmd.FRR_A_READ.FRR_D_VALUE = radioCmd[3];
}


void si4060_frr_b_read( U8 respByteCount )
{
    radio_comm_ReadData( SI4060_CMD_ID_FRR_B_READ,
                         0,
                         respByteCount,
                         radioCmd );

    Si4060Cmd.FRR_B_READ.FRR_B_VALUE = radioCmd[0];
    Si4060Cmd.FRR_B_READ.FRR_C_VALUE = radioCmd[1];
    Si4060Cmd.FRR_B_READ.FRR_D_VALUE = radioCmd[2];
    Si4060Cmd.FRR_B_READ.FRR_A_VALUE = radioCmd[3];
}


void si4060_frr_c_read( U8 respByteCount )
{
    radio_comm_ReadData( SI4060_CMD_ID_FRR_C_READ,
                         0,
                         respByteCount,
                         radioCmd);

    Si4060Cmd.FRR_C_READ.FRR_C_VALUE = radioCmd[0];
    Si4060Cmd.FRR_C_READ.FRR_D_VALUE = radioCmd[1];
    Si4060Cmd.FRR_C_READ.FRR_A_VALUE = radioCmd[2];
    Si4060Cmd.FRR_C_READ.FRR_B_VALUE = radioCmd[3];
}


void si4060_frr_d_read( U8 respByteCount )
{
    radio_comm_ReadData( SI4060_CMD_ID_FRR_D_READ,
                         0,
                         respByteCount,
                         radioCmd );

    Si4060Cmd.FRR_D_READ.FRR_D_VALUE = radioCmd[0];
    Si4060Cmd.FRR_D_READ.FRR_A_VALUE = radioCmd[1];
    Si4060Cmd.FRR_D_READ.FRR_B_VALUE = radioCmd[2];
    Si4060Cmd.FRR_D_READ.FRR_C_VALUE = radioCmd[3];
}

void si4060_write_tx_fifo(U8 numBytes, U8* pTxData)
{
    radio_comm_WriteData( SI4060_CMD_ID_WRITE_TX_FIFO, 0, numBytes, pTxData );
}
