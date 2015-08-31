#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "stdio.h"
#include "stdint.h"
class StringList;

typedef void (*dumpValuePtr)( FILE *fp, const void *pData, size_t size );
typedef bool (*loadValuePtr)( StringList &strlist, void* &pData );

dumpValuePtr getDumpFunc( uint16_t id );
loadValuePtr getLoadFunc( uint16_t id );

#endif
