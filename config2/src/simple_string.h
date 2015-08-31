#ifndef __SIMPLE_STRING_H__
#define __SIMPLE_STRING_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class simple_string
{
protected:
	char* m_pBuffer;
	char m_nulstr[1];
	
	void free_buffer() {
		free(m_pBuffer);
		m_pBuffer = NULL;
	}

public:
	simple_string() {
//		fprintf(stderr, "%s()\n", __func__);
		m_nulstr[0] = '\0';
		m_pBuffer = NULL;
	}
	simple_string( const char* str ) {
//		fprintf(stderr, "%s(%s)\n", __func__, str);
		m_nulstr[0] = '\0';
		m_pBuffer = strdup(str);
	}
	simple_string( const simple_string& other ) {
//		fprintf(stderr, "%s(o:%s)\n", __func__, other.m_pBuffer);
		m_nulstr[0] = '\0';
		m_pBuffer = strdup(other.m_pBuffer);
	}
	virtual ~simple_string() {
//		fprintf(stderr, "%s\n", __func__);
		free_buffer();
	}
	simple_string& operator = ( const char* str ) {
//		fprintf(stderr, "operator=(%s)\n", str);
		free_buffer();
		m_pBuffer = strdup(str);
		return *this;
	}
	simple_string& operator = ( const simple_string& str ) {
//		fprintf(stderr, "operator=(o:%s)\n", str.m_pBuffer);
		free_buffer();
		if ( str.m_pBuffer ) m_pBuffer = strdup(str.m_pBuffer);
		return *this;
	}
	
	bool operator == ( const char* str ) {
		if ( !m_pBuffer || !str ) return false;
		return (strcmp(m_pBuffer, str) == 0);
	}
	bool operator == ( const simple_string& str ) {
		if ( !m_pBuffer || !str.m_pBuffer ) return false;
		return (strcmp(m_pBuffer, str.m_pBuffer) == 0);
	}
	
	operator bool() { return (m_pBuffer); }
	char *ptr() { return (m_pBuffer) ? m_pBuffer : m_nulstr; }
	size_t len() { return (m_pBuffer) ? strlen(m_pBuffer) : 0; }
};


#endif
