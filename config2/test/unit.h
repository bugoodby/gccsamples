#ifndef __UNIT_H__
#define __UNIT_H__
#include <stdlib.h>
#include <string.h>

#define TEST_ASSERT(condition)                             \
	if ( (condition) ) {                                   \
		printf("%s(%u): Test OK\n", __func__, __LINE__ );  \
	} else {                                               \
		printf("%s(%u): Test NG\n", __func__, __LINE__ );  \
		printf("      %s\n", #condition);                  \
	}                                                      \


typedef void* (*malloc_ptr)( size_t size );
extern malloc_ptr g_mallocUT;
void* stub_malloc( size_t size );

typedef void* (*realloc_ptr)( void* ptr, size_t size );
extern realloc_ptr g_reallocUT;
void* stub_realloc( void* ptr, size_t size );

typedef char* (*strdup_ptr)( const char* ptr );
extern strdup_ptr g_strdupUT;
char* stub_strdup( const char* ptr );

/*
#define malloc g_mallocUT
#define realloc g_reallocUT
#define strdup g_strdupUT
*/

#endif
