/* 
 * File:   spi.h
 */

#ifndef SPI_H
#define	SPI_H

#include "types.h"

void SPI_Init( void );
void SPI_WriteBytes( U8 len, U8 * data );
void SPI_WriteByte( U8 data );
void SPI_ReadBytes( U8 len, U8 * data );
U8   SPI_ReadByte( void );

#endif	/* SPI_H */
