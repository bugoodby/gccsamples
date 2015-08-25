#include <stdio.h>
#include <string.h>
#include "StringList.h"

#define BLOCK_SIZE 100

//! constructor
StringList::StringList() : m_buffer(NULL), m_count(0), m_size(0)
{
	x_reallocMemory(BLOCK_SIZE);
}

//! destructor
StringList::~StringList()
{
	if ( m_buffer ) {
		for ( size_t i = 0; i < m_count; i++ ) {
			free(m_buffer[i]);
		}
		free(m_buffer);
	}
}

bool StringList::x_reallocMemory( size_t size )
{
//	fprintf(stderr, "[StringList] realloc(size=%lu)\n", size);
	
	char** p = (char**)realloc(m_buffer, size * sizeof(char*));
	if ( p ) {
		m_buffer = p;
		m_size = size;
		memset(m_buffer+m_count, 0, m_size-m_count);
	} else {
		fprintf(stderr, "[StringList] realloc(size=%lu) failed\n", size);
	}
	
	return (p != NULL);
}

void StringList::clear()
{
	if ( m_buffer ) {
		for ( size_t i = 0; i < m_count; i++ ) {
			free(m_buffer[i]);
			m_buffer[i] = NULL;
		}
	}
	m_count = 0;
}

bool StringList::push( const char *str )
{
//	fprintf(stderr, "[StringList] push(%lu:%lu: %s )\n", m_size, m_count, str);
	
	if ( !str ) {
		return false;
	}
	if ( m_count >= m_size - 1 ) {
		if ( x_reallocMemory( m_size + BLOCK_SIZE ) ) {
			return false;
		}
	}
	char *p = strdup(str);
	if ( !p ) {
		fprintf(stderr, "[StringList] strdup(%s) failed\n", str);
		return false;
	}
	m_buffer[m_count++] = p;
	
//	debug();
	return true;
}

char* StringList::at( size_t index )
{
	if ( !m_buffer || index >= m_count ) {
		return NULL;
	}
	return m_buffer[index];
}

size_t StringList::count()
{
	return m_count;
}

void StringList::debug()
{
	fprintf(stderr, "----\n");
	for ( size_t i = 0; i < m_size; i++ ) {
		fprintf(stderr, " (%lu) %p : %s\n", i, m_buffer[i], m_buffer[i]);
	}
	fprintf(stderr, "----\n");
}

