#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/* global variables */
GLOBALPROP g_gprop = {0};


void trim( char* line )
{
	int i = 0, j = 0;
	if ( !line ) return;
	
	for ( i = strlen(line)-1; i >= 0 && isspace(line[i]); --i );
	line[i+1] = '\0';
	for( i = 0; isspace(line[i]); ++i );
	if ( i > 0 ) {
		while ( line[i] != '\0' ) { line[j++] = line[i++]; }
		line[j] = '\0';
	}
}

void strip_comment( char *line )
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

bool get_nibble( const char c, uint8_t &nibble )
{
	bool ret = true;
	switch ( c )
	{
	case '0': nibble = 0x0; break;
	case '1': nibble = 0x1; break;
	case '2': nibble = 0x2; break;
	case '3': nibble = 0x3; break;
	case '4': nibble = 0x4; break;
	case '5': nibble = 0x5; break;
	case '6': nibble = 0x6; break;
	case '7': nibble = 0x7; break;
	case '8': nibble = 0x8; break;
	case '9': nibble = 0x9; break;
	case 'a': 
	case 'A': nibble = 0xA; break;
	case 'b': 
	case 'B': nibble = 0xB; break;
	case 'c': 
	case 'C': nibble = 0xC; break;
	case 'd': 
	case 'D': nibble = 0xD; break;
	case 'e': 
	case 'E': nibble = 0xE; break;
	case 'f': 
	case 'F': nibble = 0xF; break;
	default: 
		ret = false; 
		break;
	}
	return ret;
}

void packbin( const char *fname_in, const char *fname_out )
{
	FILE *infp = NULL, *outfp = NULL;
	char line[1024] = "";
	
	infp = fopen(fname_in, "r");
	if ( !infp ) {
		fprintf(stderr, "could not open %s\n", fname_in);
		return;
	}
	
	outfp = fopen(fname_out, "w");
	if ( !infp ) {
		fprintf(stderr, "could not open %s\n", fname_out);
		return;
	}
	
	while ( fgets(line, sizeof(line), infp) )
	{
		trim(line);
		if ( strcasecmp(line, "struct dump {") == 0 ) {
			break;
		}
	}
	while ( fgets(line, sizeof(line), infp) )
	{
		strip_comment(line);
		
		char *line_ptr = line;
		uint8_t octet = 0, nibble = 0;
		bool first_flag = true;
		
		while ( *line_ptr != '\0' ) {
			if ( get_nibble(*line_ptr, nibble) ) {
				if ( first_flag ) {
					octet = nibble << 4;
				} else {
					octet |= nibble & 0xF;
					fwrite(&octet, sizeof(uint8_t), 1, outfp);
				}
				first_flag = !first_flag;
			}
			line_ptr++;
		}
	}
	
	fclose(infp);
	fclose(outfp);
}

//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void usage(void)
{
	printf("[ %s Version %s ]\n", MODULENAME_STRING, VERSION_STRING);
	printf("generate hex dump.\n");
	printf("\n");
	printf("Usage : %s [option...] <target>\n", MODULENAME_STRING);
	printf("        <file>    : target file.\n");
	printf("\n");
}


//--------------------------------------------------------------
// parse command line
//--------------------------------------------------------------
bool parse_cmdline( int argc, char **argv, GLOBALPROP &gprop )
{
	bool ret = true;
	char *s = NULL;

	/* initialize */
	gprop.argc = 0;
	
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
			gprop.argc = argc;
			gprop.argv = argv;
			break;
		}
	}

	return ret;
}


int main( int argc, char **argv )
{
	if ( !parse_cmdline(argc, argv, g_gprop) ) {
		return -1;
	}
	
	packbin( prop()->argv[0], prop()->argv[1] );
	
	return 0;
}

