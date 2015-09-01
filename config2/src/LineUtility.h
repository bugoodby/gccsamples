#ifndef __LINEUTILITY_H__
#define __LINEUTILITY_H__

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class LineUtility
{
public:
	static void trim( char *line ) {
		if ( !line ) return;
		if ( *line == '\0' ) return;
		
		char *ptr = line;
		while ( *ptr != '\0' ) { ptr++; }
		for ( ptr--; ptr >= line && isspace(*ptr); ptr-- ) {}
		*(ptr + 1) = '\0';
		
		ptr = line;
		while ( isspace(*ptr) ) { ptr++; }
		if ( ptr > line ) {
			memmove(line, ptr, strlen(ptr)+1);
		}
	}

	static void delete_comment( char *line ) {
		if ( !line ) return;
		
		while ( *line != '\0' ) {
			if ( *line == '#' ) {
				*line = '\0';
				break;
			} else if ( *line == '\"' ) {
				do {
					line++;
				} while ( *line != '\"' && *line != '\0' );
			}
			line++;
		}
	}

	static void strip_double_quote( char* line ) {
		if ( !line ) return;
		if ( *line == '\0' ) return;
		
		size_t len = strlen(line);
		if ( len >= 2 && line[0] == '\"' && line[len-1] == '\"' ) {
			line[len-1] = '\0';
			memmove(line, &line[1], len-1);
		}
	}
	
	static bool get_key_value( char *line, char* &key, char* &value ) {
		if ( !line ) return false;
		
		char *pos = NULL;
		if ( (pos = strstr(line, "=")) != NULL ) {
			*pos = '\0';
			key = line;
			value = pos + 1;
			trim(key);
			trim(value);
			return true;
		}
		return false;
	}
};



#endif
