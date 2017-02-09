#include "config.h"
#include <xc.h>

void HW_Config( void )
{
    ANSELA  = 0x00;
    TRISA   = 0xCC;
    PORTA   = 0x22;
    SCK_PIN = 1;
    LED_PIN = 0;
    SS_PIN  = 1;
}