#include "main.h"
#include "StringList.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/* global variables */
GLOBALPROP g_gprop = {0};


//-----------------------------------------------------------------
#define INI_LINE_MAX 1024


void trim( char* line )
{
	size_t i = 0, j = 0;
	if ( !line ) return;
	if ( line[0] == '\0' ) return;
	
	for ( i = strlen(line)-1; i >= 0 && isspace(line[i]); --i );
	line[i+1] = '\0';
	for( i = 0; isspace(line[i]); ++i );
	if ( i > 0 ) {
		while ( line[i] != '\0' ) { line[j++] = line[i++]; }
		line[j] = '\0';
	}
}

void delete_comment( char *line )
{
	if ( !line ) return;
	
	while ( *line != '\0' ) {
		if ( *line == '#' ) {
			*line = '\0';
			break;
		} else if ( *line == '\"' ) {
			while ( *line != '\"' && *line != '\0' ) line++;
		}
		line++;
	}
}

void strip( char *line )
{
	delete_comment(line);
	trim(line);
}

void strip_double_quote( char* line )
{
	if ( !line ) return;
	if ( line[0] == '\0' ) return;
	
	size_t len = strlen(line);
	if ( line[0] == '\"' && line[len-1] == '\"' ) {
		line[len-1] = '\0';
		memmove(line, &line[1], len-1);
	}
}

static bool is_section( const char* line, char* sectname, size_t bufsize )
{
	size_t cnt = 0;
	bool found_start_end = false;
	
	const char* pWork = line;
	if ( *pWork++ == '[' ) {
		while ( *pWork != '\0' ) {
			if ( *pWork == ']' ) {
				found_start_end = true;
				break;
			}
			pWork++;
			cnt++;
		}
	}
	if ( found_start_end && sectname ) {
		sectname[0] = '\0';
		if ( cnt < bufsize ) {
			memcpy(sectname, &line[1], cnt);
			sectname[cnt] = '\0';
			trim(sectname);
		} else {
			fprintf(stderr, "%s: Too small buffer size. (size=%lu, need=%lu)\n", __func__, bufsize, cnt+1);
		}
	}
	
	return found_start_end;
}


char* lex( char* &p )
{
	while ( isspace(*p) ) p++;
	char *pBegin = p;
	while ( *p != '\0' && *p != '*' && *p != '=' && *p != ';' && !isspace(*p) ) p++;
	*p++ = '\0';
	fprintf(stderr, "  lex...%s\n", pBegin);
	return pBegin;
}

static bool get_token( StringList &strlist, size_t &idx, char* &p_pType, char* &p_pName, char* &p_pValue )
{
	char *pType = NULL, *pName = NULL, *pValue = NULL, *pTmp = NULL;
	
	for ( ; idx < strlist.count(); idx++ ) {
		char* line = strlist.at(idx);
		fprintf(stderr, "###%s\n", line);
		pType = lex(line);
		pName = lex(line);
		if ( strcmp(pName, "*") == 0 ) {
			pName = lex(line);
		}
		pTmp = lex(line);
		if ( strcmp(pTmp, "=") != 0 ) {
			continue;
		}
		pValue = lex(line);
		pTmp = lex(line);
		if ( strcmp(pTmp, ";") != 0 ) {
			continue;
		}
		
		p_pType = pType;
		p_pName = pName;
		p_pValue = pValue;
		return true;
	}
	return false;
}

static bool parse_value( StringList &strlist, size_t &idx )
{
	char *pType = NULL, *pName = NULL, *pValue = NULL;
	if ( get_token(strlist, idx, pType, pName, pValue) ) {
		fprintf(stderr, "[%s] [%s] = [%s]\n", pType, pName, pValue);
	}
	return true;
}

static bool parse_section( StringList &strlist )
{
	for ( size_t idx = 0; idx < strlist.count(); idx++ ) {
		char* line = strlist.at(idx);
		if ( strstr(line, "Value") != NULL ) {
			fprintf(stderr, "##Value## line:%s\n", line);
			parse_value( strlist, ++idx );
		}
	}
	return true;
}

bool analyze_config_file( const char* fname )
{
	char line[INI_LINE_MAX] = "";
	StringList strlist;
	
	if ( !fname ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return false;
	}
	
	FILE *fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return false;
	}
	
	while ( fgets(line, sizeof(line), fp) )
	{
		strip(line);
		if ( line[0] == '\0' ) continue;
		
		if ( is_section(line, NULL, 0) ) {
			parse_section(strlist);
			strlist.clear();
		} else {
			strlist.push(line);
		}
	}
	parse_section(strlist);
	
	return true;
}



//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void usage(void)
{
	printf("[ %s Version %s ]\n", MODULENAME_STRING, VERSION_STRING);
	printf("...\n");
	printf("\n");
	printf("Usage : %s [option...] <file1>\n", MODULENAME_STRING);
	printf("        <file1>    : ini file.\n");
	printf("\n");
}


//--------------------------------------------------------------
// parse command line
//--------------------------------------------------------------
bool parse_cmdline( int argc, char **argv )
{
	bool ret = true;
	char *s = NULL;

	/* initialize */
	g_gprop.file = NULL;
	
	/* parse */
	while ( --argc > 0 ) {
		s = *++argv;
		if ( *s == '-' ) {
			switch ( *++s )
			{
			case 'h':
			case '?':
				usage();
				ret = false;
				break;
			default:
				fprintf(stderr, "Unknown parameter : -%s\n", s);
				ret = false;
				break;
			}
		}
		else {
			if ( !g_gprop.file ) g_gprop.file = s;
		}
	}

	return ret;
}


int main( int argc, char **argv )
{
	if ( !parse_cmdline(argc, argv) ) {
		return -1;
	}
	
	if ( !prop()->file ) {
		fprintf(stderr, "too few arguments.\n");
		return -1;
	}
	
	analyze_config_file(prop()->file);
	
	return 0;
}

