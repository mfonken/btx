#ifndef TYPES_H
#define	TYPES_H

#include <xc.h>
#include <stdbool.h>

# define LSB 0
# define MSB 1

typedef unsigned char U8;
typedef unsigned char U8;
typedef unsigned int  U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

typedef union UU16
{
    U16 U16;
    S16 S16;
    U8 U8[2];
    S8 S8[2];
} UU16;

typedef union UU32
{
    U32     U32;
    S32     S32;
    UU16    UU16[2];
    U16     U16[2];
    S16     S16[2];
    U8      U8[4];
    S8      S8[4];
} UU32;

typedef struct
{
  U32       ChipID;
  U8        Flags;          /* 4 bit - Buttons */
  U16       RollingCounter;
} tRadioPacket;

typedef bool BIT;

typedef enum
{
    STARTUP,
    ARMED,
    RECEIVING,
} states;

typedef enum U8
{
    NOCHANGE = 0,
    SLEEP,
    SPI_ACTIVE,
    READY,
    READY2,
    TX_TUNE,
} tx_complete_t;

#endif	/* TYPES_H */

