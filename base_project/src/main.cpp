#include "main.h"
#include <stdio.h>

/* global variables */
GLOBALPROP g_gprop = {0};



//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void usage(void)
{
	fprintf(stdout, "[ %s Version %s ]\n", MODULENAME_STRING, VERSION_STRING);
	fprintf(stdout, "xxxxxxxxxxxxx\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage : %s [option...]\n", MODULENAME_STRING);
	fprintf(stdout, "\n");
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
	
	return 0;
}

