#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "stdio.h"
#include "stdint.h"
class StringList;

typedef void (*dumpValuePtr)( FILE *fp, const void *pData, size_t size );
typedef bool (*loadValuePtr)( StringList &strlist, void* &pData );

dumpValuePtr getDumpFunc( uint16_t id );
loadValuePtr getLoadFunc( uint16_t id );


void lex( char* line, StringList &tokens );
bool getValueDeclaration( StringList &strlist, char* valueStr );

typedef struct {
	uint32_t x;
	uint32_t y;
} USIZE32;

typedef struct {
	uint8_t count;
	char string[255];
} STRING8;



#endif
