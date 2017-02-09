#ifndef _SI4060_H_
#define _SI4060_H_

extern union si4060_cmd_reply_union Si4060Cmd;
extern U8 radioCmd[16];

#define SI4060_FIFO_SIZE 64

enum
{
    SUCCESS,
    NO_PATCH,
    CTS_TIMEOUT,
    PATCH_FAIL,
    COMMAND_ERROR
};

U8   si4060_configuration_init( U8 * pSetPropCmd );
void si4060_reset( void );
void si4060_nop( void );
void si4060_part_info( void );
void si4060_power_up( U8 BOOT_OPTIONS, U8 XTAL_OPTIONS, U32 XO_FREQ );
void si4060_func_info( void);
void si4060_set_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP, ... );
void si4060_get_property( U8 GROUP, U8 NUM_PROPS, U8 START_PROP );
void si4060_gpio_pin_cfg( U8 GPIO0, U8 GPIO1, U8 GPIO2, U8 GPIO3, U8 NIRQ, U8 SDO, U8 GEN_CONFIG );
void si4060_fifo_info( U8 FIFO);
void si4060_write_ezconfig_array( U8 numBytes, U8* pEzConfigArray );
void si4060_ezconfig_check( U16 CHECKSUM);
void si4060_get_int_status( U8 PH_CLR_PEND, U8 MODEM_CLR_PEND, U8 CHIP_CLR_PEND );
void si4060_start_tx( U8 CHANNEL, U8 CONDITION, U16 TX_LEN );
void si4060_request_device_state( void );
void si4060_change_state( U8 NEXT_STATE1 );
void si4060_read_cmd_buff( void );
void si4060_frr_a_read( U8 respByteCount );
void si4060_frr_b_read( U8 respByteCount );
void si4060_frr_c_read( U8 respByteCount );
void si4060_frr_d_read( U8 respByteCount );
void si4060_write_tx_fifo( U8 numBytes, U8* pData );

#endif //_SI4060_API_LIB_H_
