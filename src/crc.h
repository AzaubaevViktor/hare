/**********************************************************************
*
* Filename: crc.h
*
* Description: A header file describing the various CRC standards.
*
* Notes:
*
*
* Copyright (c) 2000 by Michael Barr. This software is placed into
* the public domain and may be used for any purpose. However, this
* notice must not be changed or removed and no warranty is either
* expressed or implied by its publication or distribution.
**********************************************************************/

#ifndef _crc_h
#define _crc_h

#define FALSE 0
#define TRUE !FALSE

#include <inttypes.h>

typedef int32_t crc;

#define CRC_NAME "CRC-32"
#define POLYNOMIAL 0x04C11DB7
#define INITIAL_REMAINDER 0xFFFFFFFF
#define FINAL_XOR_VALUE 0xFFFFFFFF
#define REFLECT_DATA TRUE
#define REFLECT_REMAINDER TRUE
#define CHECK_VALUE 0xCBF43926

#include <inttypes.h>

void crcInit(crc *crcTable);
crc crcFast(unsigned char const *message, int64_t nBytes, crc *crcTable, crc *remainder);


#endif /* _crc_h */
