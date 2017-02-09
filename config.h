#include "types.h"

#ifndef CONFIG_H
#define	CONFIG_H

// PIN CONFIGURATION
#define LED_PIN     RA0
#define SCK_PIN     RA1
#define SDI_PIN     RA2
#define IRQ_PIN     RA3
#define SDO_PIN     RA4
#define SS_PIN      RA5
    
extern states STATE;

void HW_Config( void );

#endif	/* SPI_H */
