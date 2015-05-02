#include "main.h"
#include "config.h"
#include <stdio.h>


/* global variables */
GLOBALPROP g_gprop = {0};

//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void analyze_config_file( char *file )
{
	char szValue[2048] = "";
	
	//-------------------------------
	// セクション一覧
	std::vector<std::string> sectlist;
	GetSectionList(file, sectlist);
	printf("*** sections ***\n");
	for ( std::vector<std::string>::iterator it = sectlist.begin(); it != sectlist.end(); it++ ) {
		printf("#%s#\n", it->c_str());
	}
	
	// セクション中のアイテム一覧
	std::vector<std::string> itemlist;
	printf("*** items ***\n");
	for ( std::vector<std::string>::iterator it = sectlist.begin(); it != sectlist.end(); it++ ) {
		printf("[%s]\n", it->c_str());
		GetItemList(file, it->c_str(), itemlist);
		for ( std::vector<std::string>::iterator itk = itemlist.begin(); itk != itemlist.end(); itk++ ) {
			printf("#%s#\n", itk->c_str());
		}
	}
	
	// セクション中のキー一覧
	std::vector<std::string> keylist;
	printf("*** keys ***\n");
	for ( std::vector<std::string>::iterator it = sectlist.begin(); it != sectlist.end(); it++ ) {
		printf("[%s]\n", it->c_str());
		GetKeyList(file, it->c_str(), keylist);
		for ( std::vector<std::string>::iterator itk = keylist.begin(); itk != keylist.end(); itk++ ) {
			printf("#%s#\n", itk->c_str());
		}
	}
	
	// 値の取得
	printf("*** get value ***\n");
	for ( std::vector<std::string>::iterator it = sectlist.begin(); it != sectlist.end(); it++ ) {
		printf("[%s]\n", it->c_str());
		GetKeyList(file, it->c_str(), keylist);
		for ( std::vector<std::string>::iterator itk = keylist.begin(); itk != keylist.end(); itk++ ) {
			GetValueString(file, it->c_str(), itk->c_str(), szValue, sizeof(szValue));
			printf("#%s# = #%s#\n", itk->c_str(), szValue);
		}
	}
	
}

//--------------------------------------------------------------
// show usage
//--------------------------------------------------------------
void usage(void)
{
	fprintf(stdout, "[ %s Version %s ]\n", MODULENAME_STRING, VERSION_STRING);
	fprintf(stdout, "read config file.\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Usage : %s [option...] <target>\n", MODULENAME_STRING);
	fprintf(stdout, "        <file>    : target config file.\n");
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
	
	analyze_config_file(prop()->argv[0]);
	
	return 0;
}

