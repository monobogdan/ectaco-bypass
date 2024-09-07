#ifndef _NES_DATATYPE_H_
#define _NES_DATATYPE_H_

#include <stdlib.h>
#include <stdio.h>
 
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef  unsigned char BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef  int  DU16;
typedef  unsigned short U16;
typedef  int BOOL;
typedef  unsigned char U8;
typedef  unsigned long int  U32;

#define uint8 unsigned char
#define int8 char
#define int16 short
#define uint16 unsigned short
#define int32 int
#define uint32 unsigned int

typedef struct 
{
	uint8 r;
	uint8 g;
	uint8 b;
}Color;

#define NULL 0

#endif