#include "main.h"
#include "ConfigFileReader.h"




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




int main( int argc, char **argv )
{
	if ( argc < 2 ) {
		fprintf(stderr, "too few arguments.\n");
		return -1;
	}
	
	analyze_config_file(argv[1]);
	
	return 0;
}

