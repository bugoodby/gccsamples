#include "main.h"
#include "ConfigFileReader.h"

/* global variables */
GLOBALPROP g_gprop = {0};



//////////////////////////////////////////////////////////////////////
bool analyze_config_file( const char* fname )
{
	if ( !fname ) {
		fprintf(stderr, "%s: Invalid argument\n", __func__);
		return false;
	}
	
	std::list<SECTION_DATA> list;
	
	ConfigFileReader reader;
	
	if ( !reader.open(fname) ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return false;
	}
	
	reader.read(list);
	
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

