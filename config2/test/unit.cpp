#include "unit.h"

malloc_ptr g_mallocUT = malloc;
void* stub_malloc( size_t size ) {
	return NULL;
}

realloc_ptr g_reallocUT = realloc;
void* stub_realloc( void* ptr, size_t size ) {
	return NULL;
}

strdup_ptr g_strdupUT = strdup;
char* stub_strdup( const char* ptr ) {
	return NULL;
}

