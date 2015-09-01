#include "handler.h"
#include "StringList.h"
#include <stdarg.h>
#include <ctype.h>

#define DEBUG_LOG( ... ) fprintf(stderr, __VA_ARGS__ )

#define INFO( fmt, ... )  DEBUG_LOG( "[INFO] " fmt, ## __VA_ARGS__)
#define WARN( fmt, ... )  DEBUG_LOG( "[WARN] %s: " fmt, __func__, ## __VA_ARGS__)
#define ERR( fmt, ... )   DEBUG_LOG( "[ERR ] %s(%d) %s: " fmt, __FILE__, __LINE__, __func__, ## __VA_ARGS__)

typedef struct {
	uint32_t x;
	uint32_t y;
} USIZE32;

typedef struct {
	uint8_t count;
	char string[255];
} STRING8;


//-----------------------------------------------------------------------

//#define DUMP_LINE( fmt, ... ) { fprintf( fp, (fmt), ## __VA_ARGS__ ); }
//#define DUMP_LINE( fmt, ... ) { dump_line( fp, "#" fmt, ## __VA_ARGS__ ); }
#define DUMP_LINE( fmt, ... ) { dump_line( fp, fmt, ## __VA_ARGS__ ); }

void dump_line( FILE *fp, const char* format, ... )
{
	va_list argp;
	va_start( argp, format );
	vfprintf( fp, format, argp );
	vfprintf( stdout, format, argp );
	va_end( argp );
}

//-----------------------------------------------------------------------

#if 0
bool get_token( StringList &strlist, simple_string &type, simple_string &name, simple_string &value ) 
{
	char* line = NULL;
	while ( (line = strlist.getNext()) != NULL ) {
		fprintf(stderr, "###%s\n", line);
		
		lexer lex(line);
		
		type = lex.get();
		name = lex.get();
		if ( name == "*" ) name = lex.get();
		simple_string equal = lex.get();
		value = lex.get();
		simple_string term = lex.get();
		
		if ( type.len() > 0 && name.len() > 0 && value.len() > 0 && equal == "=" && term == ";" ) {
			return true;
		}
		fprintf(stderr, "  not value decration.\n");
	}
	return false;
}
#endif

bool is_identifier( char c )
{
	switch ( c ) {
	case '*':
	case '=':
	case ';':
		return true;
	default:
		return false;
	}
};

bool lex( char* line, StringList &tokens ) 
{
	while ( *line != '\0' ) {
		while ( isspace(*line) ) line++;
		
		char *pBegin = line;
		
		if ( *pBegin == '\"' ) {
			do {
				line++;
			} while ( *line != '\0' && *line != '\"' );
			line++;
		}
		else if ( is_identifier(*pBegin) ) {
			while ( is_identifier(*line) ) line++;
		} else {
			while ( *line != '\0' && !is_identifier(*line) && !isspace(*line) ) line++;
		}
		size_t size = line - pBegin;
		if ( size != 0 ) {
			char tmp = *line;
			*line = '\0';
			fprintf(stderr, "  lex...[%s]\n", pBegin);
			tokens.push(pBegin);
			*line = tmp;
		}
	}
	return true;
}

bool getValueDeclaration( StringList &strlist, char* valueStr ) 
{
	char* line = NULL;
	while ( (line = strlist.getNext()) != NULL ) {
		fprintf(stderr, "###%s\n", line);
		
		StringList tokens;
		lex(line, tokens);
		
		int i = 0;
		char *pType = tokens.at(i++);
		char *pName = tokens.at(i++);
		if ( pName && strcmp(pName, "*") == 0 ) pName = tokens.at(i++);
		char *pEqual = tokens.at(i++);
		char *pValue = tokens.at(i++);
		char *pTerm = tokens.at(i++);
		
		if ( pType && pName && strcmp(pEqual, "=") == 0 && pValue && strcmp(pTerm, ";") == 0 ) {
			strcpy(valueStr, pValue);
			fprintf(stderr, "  [%s] [%s] = [%s]\n", pType, pName, pValue);
			return true;
		}
		fprintf(stderr, "  not value decration.\n");
	}
	return false;
}

//-----------------------------------------------------------------------

template <typename T>
bool set_uint( StringList &strlist, T &buffer )
{
	char valueStr[1024] = "";
	
	if ( !getValueDeclaration(strlist, valueStr) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	buffer = static_cast<T>(strtoul(valueStr, NULL, 0));
	return true;
}

#define set_uint8_t(strlist, buffer)       set_uint<uint8_t>(strlist, buffer)
#define set_uint16_t(strlist, buffer)       set_uint<uint16_t>(strlist, buffer)
#define set_uint32_t(strlist, buffer)       set_uint<uint32_t>(strlist, buffer)


bool set_string( StringList &strlist, char *buffer, size_t size )
{
	char valueStr[1024] = "";
	
	if ( !getValueDeclaration(strlist, valueStr) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	strncpy(buffer, valueStr, size);
	return true;
}

#define set_STRING(strlist, buffer, size)       set_string(strlist, buffer, size)


//-----------------------------------------------------------------------

#define MALLOC_BUFFER( type )                       \
	type *p = (type*)malloc(sizeof(type));          \
	if ( !p ) {                                     \
		pData = NULL;                               \
		return false;                               \
	}                                               \
	memset(p, 0, sizeof(type));                     \
	pData = (void*)p;


//! uint8_t
void dump_uint8_t( FILE *fp, const void *pData, size_t size )
{
	uint8_t* p = (uint8_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint8_t Value = %u;\n", *p );
	DUMP_LINE( "};\n" );
};
bool load_uint8_t( StringList &strlist, void* &pData )
{
	bool ret = true;
	MALLOC_BUFFER(uint8_t);
	
	ret &= set_uint8_t(strlist, *p);
	
	return ret;
};

//! uint16_t
void dump_uint16_t( FILE *fp, const void *pData, size_t size )
{
	uint16_t* p = (uint16_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint16_t Value = %u;\n", *p );
	DUMP_LINE( "};\n" );
};
bool load_uint16_t( StringList &strlist, void* &pData )
{
	bool ret = true;
	MALLOC_BUFFER(uint16_t);
	
	ret &= set_uint16_t(strlist, *p);
	
	return ret;
};

//! uint32_t
void dump_uint32_t( FILE *fp, const void *pData, size_t size )
{
	uint32_t* p = (uint32_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint32_t Value = 0x%x;\n", *p );
	DUMP_LINE( "};\n" );
};
bool load_uint32_t( StringList &strlist, void* &pData )
{
	bool ret = true;
	MALLOC_BUFFER(uint32_t);
	
	ret &= set_uint32_t(strlist, *p);
	
	return ret;
};

//! USIZE32
void dump_USIZE32( FILE *fp, const void *pData, size_t size )
{
	USIZE32* p = (USIZE32*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint32_t x = 0x%08x;\n", p->x );
	DUMP_LINE( "  uint32_t y = 0x%08x;\n", p->y );
	DUMP_LINE( "};\n" );
};
bool load_USIZE32( StringList &strlist, void* &pData )
{
	bool ret = true;
	MALLOC_BUFFER(USIZE32);
	
	ret &= set_uint32_t(strlist, p->x);
	ret &= set_uint32_t(strlist, p->y);
	
	return ret;
};

//! STRING8
void dump_STRING8( FILE *fp, const void *pData, size_t size )
{
	STRING8* p = (STRING8*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint8_t count = 0x%u;\n", p->count );
	DUMP_LINE( "  char* string = \"%.*s\";\n", p->count, p->string );
	DUMP_LINE( "};\n" );
};
bool load_STRING8( StringList &strlist, void* &pData )
{
	bool ret = true;
	MALLOC_BUFFER(STRING8);
	
	ret &= set_uint8_t(strlist, p->count);
	ret &= set_STRING(strlist, p->string, p->count);
	
	return ret;
};

//-----------------------------------------------------------------------

typedef struct {
	uint16_t id;
	dumpValuePtr dumper;
	loadValuePtr loader;
} FUNC_INFO;

#define FUNC_INFO_RECORD( id, type ) { (id), dump_##type, load_##type },

static FUNC_INFO g_funcInfoTbl[] = {
	FUNC_INFO_RECORD( 1, uint8_t )
	FUNC_INFO_RECORD( 3, uint32_t )
	FUNC_INFO_RECORD( 4, USIZE32 )
	FUNC_INFO_RECORD( 5, STRING8 )
	{ 0, NULL, NULL }
};

dumpValuePtr getDumpFunc( uint16_t id )
{
	size_t cnt = sizeof(g_funcInfoTbl) / sizeof(g_funcInfoTbl[0]);
	for ( size_t i = 0; i < cnt; i++ ) {
		if ( g_funcInfoTbl[i].id == id ) {
			return g_funcInfoTbl[i].dumper;
		}
	}
	return NULL;
}

loadValuePtr getLoadFunc( uint16_t id )
{
	size_t cnt = sizeof(g_funcInfoTbl) / sizeof(g_funcInfoTbl[0]);
	for ( size_t i = 0; i < cnt; i++ ) {
		if ( g_funcInfoTbl[i].id == id ) {
			return g_funcInfoTbl[i].loader;
		}
	}
	return NULL;
}


