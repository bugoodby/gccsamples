#ifndef __LINEUTILITY_H__
#define __LINEUTILITY_H__

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class LineUtility
{
public:
	static void trim( char* line ) {
		size_t i = 0, j = 0;
		if ( !line ) return;
		if ( line[0] == '\0' ) return;
		
		for ( i = strlen(line); i > 0 && isspace(line[i-1]); --i );
		line[i] = '\0';
		for( i = 0; isspace(line[i]); ++i );
		if ( i > 0 ) {
			while ( line[i] != '\0' ) { line[j++] = line[i++]; }
			line[j] = '\0';
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
		if ( line[0] == '\0' ) return;
		
		size_t len = strlen(line);
		if ( line[0] == '\"' && line[len-1] == '\"' ) {
			line[len-1] = '\0';
			memmove(line, &line[1], len-1);
		}
	}
	
	static bool get_key_value( char *line, char* &key, char* &value ) {
		char *pos = NULL;
		
		if ( !line ) return false;
		
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
