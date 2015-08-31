#ifndef __LEXER_H__
#define __LEXER_H__

#include "simple_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class lexer
{
protected:
	char* m_pBuffer;
	char* m_ptr;
	
	bool is_identifier( char c ) {
		switch ( c ) {
		case '*':
		case '=':
		case ';':
			return true;
		default:
			return false;
		}
	};
	
public:
	lexer( const char* pBuffer ) : m_pBuffer(NULL), m_ptr(NULL) {
		if ( pBuffer ) {
			m_pBuffer = strdup(pBuffer);
			m_ptr = m_pBuffer;
		}
	}
	
	virtual ~lexer() {
		free(m_pBuffer);
	}
	
	simple_string get() {
		simple_string s;
		
		if ( !m_ptr ) return s;
		
		while ( isspace(*m_ptr) ) m_ptr++;
		
		char *pBegin = m_ptr;
		
		if ( *pBegin == '\"' ) {
			do {
				m_ptr++;
			} while ( *m_ptr != '\0' && *m_ptr != '\"' );
		}
		if ( is_identifier(*pBegin) ) {
			while ( is_identifier(*m_ptr) ) m_ptr++;
		} else {
			while ( *m_ptr != '\0' && !is_identifier(*m_ptr) && !isspace(*m_ptr) ) m_ptr++;
		}
		size_t size = m_ptr - pBegin;
		if ( size != 0 ) {
			char tmp = *m_ptr;
			*m_ptr = '\0';
			s = pBegin;
			*m_ptr = tmp;
		}
		fprintf(stderr, "  lex...[%s]\n", s.ptr());
		return s;
	}
};



#endif
