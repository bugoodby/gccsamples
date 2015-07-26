#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/* global variables */
GLOBALPROP g_gprop = {0};


void dumpValueHex( FILE *fp, const void *pData, size_t size )
{
	const char szHexArray[] = "0123456789ABCDEF";
	const unsigned char *p = (const unsigned char*)pData;
	size_t l, line = 0;
	char lineBuf[52] = {0};
	char *pHexPtr = NULL;
	
	lineBuf[49] = '\n';
	
	fprintf( fp, "struct dump {\n" );
	
	do {
		line = ( size > 16 ) ? 16 : size;
		size -= line;
		
		for ( int i = 0; i < 49; i++ ) lineBuf[i] = ' ';
		pHexPtr = lineBuf + 2;
		
		for ( l = 0; l < line; l++, p++ ) {
			*pHexPtr++ = szHexArray[ *p >> 4 ];
			*pHexPtr++ = szHexArray[ *p & 0xF ];
			pHexPtr++;
		}
		fprintf( fp, lineBuf );
		
	} while ( size > 0 );
	
	fprintf( fp, "};\n" );
}

void dumpbin( const char *fname )
{
	FILE *fp = NULL;
	void *buffer = NULL;
	
	fp = fopen(fname, "r");
	if ( !fp ) {
		fprintf(stderr, "could not open %s\n", fname);
		return;
	}
	
	// get file size.
	fpos_t fpos = 0;
	if ( fseek(fp, 0L, SEEK_END) != 0 ) {
		fprintf(stderr, "fseek() failed.\n");
		goto EXIT;
	}
	if ( fgetpos(fp, &fpos) != 0 ) {
		fprintf(stderr, "fgetpos() failed.\n");
		goto EXIT;
	}
	if ( fseek(fp, 0L, SEEK_SET) != 0 ) {
		fprintf(stderr, "fseek() failed.\n");
		goto EXIT;
	}
	
	// allocate memory.
	buffer = malloc(fpos);
	if ( !buffer ) {
		fprintf(stderr, "could not allocate memory (%llubyte)\n", fpos);
		goto EXIT;
	}
	
	// read all data.
	if ( fread(buffer, 1, fpos, fp) != (size_t)fpos ) {
		fprintf(stderr, "fread() failed.\n");
		goto EXIT;
	}
	
	dumpValueHex( stdout, buffer, fpos );
	
EXIT:
	free(buffer);
	fclose(fp);
	
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
	
	dumpbin( prop()->argv[0] );
	
	return 0;
}

