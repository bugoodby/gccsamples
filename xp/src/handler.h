
typedef struct {
	uint32_t x;
	uint32_t y;
} USIZE32;

typedef struct {
	uint8_t count;
	char string[255];
} STRING8;


//-----------------------------------------------------------------------

typedef void (*dumpValuePtr)( FILE *fp, const void *pData, size_t size );
typedef void* (*loadValuePtr)( FILE *fp );

//#define DUMP_LINE( fmt, ... ) { fprintf( fp, (fmt), ## __VA_ARGS__ ); }
#define DUMP_LINE( fmt, ... ) { dump_line( fp, "#" fmt, ## __VA_ARGS__ ); }

void dump_line( FILE *fp, const char* format, ... )
{
	va_list argp;
	va_start( argp, format );
	vfprintf( fp, format, argp );
	vfprintf( stdout, format, argp );
	va_end( argp );
}

//-----------------------------------------------------------------------

static void trim( char* line )
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

static void get_string( char* line )
{
}

static bool get_token( FILE *fp, char *p_pType, char *p_pName, char *p_pValue )
{
	char line[1024] = "";
	char *pType = NULL, *pName = NULL, *pValue = NULL, *pTmp = NULL;
	
	while ( fgets(line, sizeof(line), fp) )
	{
		trim(line);
		if ( line[0] == '\0' ) continue;
		
		pType = line;
		pValue = strchr(line, '=');
		if ( pValue ) {
			*pValue++ = '\0';
			pTmp = strrchr(pValue, ';');
			if ( pTmp ) {
				*pTmp = '\0';
			} else {
				pValue = NULL;
			}
		}
		pName = strchr(pType, ' ');
		if ( pName ) {
			*pName++ = '\0';
		}
		
		if ( pType && pName && pValue ) {
			trim(pType); 
			strcpy(p_pType, pType);
			trim(pName); 
			strcpy(p_pName, pName);
			trim(pValue); 
			strcpy(p_pValue, pValue);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------

bool set_uint8_t( FILE *fp, uint8_t &buffer )
{
	char szType[1024] = "", szName[1024] = "", szValue[1024] = "";
	char *endptr = NULL;
		
	if ( ! get_token(fp, szType, szName, szValue) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	if ( strcmp(szType, "uint8_t") != 0 ) {
		WARN("syntax error! - invalid type: %s\n", szType);
		return false;
	}
	
	buffer = (uint8_t)strtoul(szValue, &endptr, 0);
	if ( endptr ) {
		WARN("syntax error! - invalid value: %s\n", szValue);
		return false;
	}
	
	return true;
}

bool set_uint16_t( FILE *fp, uint16_t &buffer )
{
	char szType[1024] = "", szName[1024] = "", szValue[1024] = "";
	char *endptr = NULL;
		
	if ( ! get_token(fp, szType, szName, szValue) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	if ( strcmp(szType, "uint16_t") != 0 ) {
		WARN("syntax error! - invalid type: %s\n", szType);
		return false;
	}
	
	buffer = (uint16_t)strtoul(szValue, &endptr, 0);
	if ( endptr ) {
		WARN("syntax error! - invalid value: %s\n", szValue);
		return false;
	}
	
	return true;
}

bool set_uint32_t( FILE *fp, uint32_t &buffer )
{
	char szType[1024] = "", szName[1024] = "", szValue[1024] = "";
	char *endptr = NULL;
		
	if ( ! get_token(fp, szType, szName, szValue) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	if ( strcmp(szType, "uint32_t") != 0 ) {
		WARN("syntax error! - invalid type: %s\n", szType);
		return false;
	}
	
	buffer = (uint32_t)strtoul(szValue, &endptr, 0);
	if ( endptr ) {
		WARN("syntax error! - invalid value: %s\n", szValue);
		return false;
	}
	
	return true;
}

bool set_STRING( FILE *fp, char *buffer, size_t size )
{
	char szType[1024] = "", szName[1024] = "", szValue[1024] = "";
	char *endptr = NULL;
		
	if ( ! get_token(fp, szType, szName, szValue) ) {
		WARN("could not get token!\n");
		return false;
	}
	
	if ( strcmp(szType, "char*") != 0 ) {
		WARN("syntax error! - invalid type: %s\n", szType);
		return false;
	}
	
	strncpy(buffer, szValue, size);
	
	return true;
}

//-----------------------------------------------------------------------

#define MALLOC_BUFFER( type )                       \
	type *pBuffer = (type*)malloc(sizeof(type));    \
	if ( !pBuffer ) {                               \
		return NULL;                                \
	}                                               \
	memset(pBuffer, 0, sizeof(type));


//! uint8_t
void dump_uint8_t( FILE *fp, const void *pData, size_t size )
{
	uint8_t* p = (uint8_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint8_t Value = %u;\n", *p );
	DUMP_LINE( "};\n" );
};
void* load_uint8_t( FILE *fp )
{
	MALLOC_BUFFER(uint8_t);
	
	set_uint8_t(fp, *pBuffer);
	return pBuffer;
};

//! uint16_t
void dump_uint16_t( FILE *fp, const void *pData, size_t size )
{
	uint16_t* p = (uint16_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint16_t Value = %u;\n", *p );
	DUMP_LINE( "};\n" );
};
void* load_uint16_t( FILE *fp )
{
	MALLOC_BUFFER(uint16_t);
	
	set_uint16_t(fp, *pBuffer);
	return pBuffer;
};

//! uint32_t
void dump_uint32_t( FILE *fp, const void *pData, size_t size )
{
	uint32_t* p = (uint32_t*)pData;
	
	DUMP_LINE( "struct body {\n" );
	DUMP_LINE( "  uint32_t Value = 0x%x;\n", *p );
	DUMP_LINE( "};\n" );
};
void* load_uint32_t( FILE *fp )
{
	MALLOC_BUFFER(uint32_t);
	
	set_uint32_t(fp, *pBuffer);
	return pBuffer;
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
void* load_USIZE32( FILE *fp )
{
	MALLOC_BUFFER(USIZE32);
	
	set_uint32_t(fp, pBuffer->x);
	set_uint32_t(fp, pBuffer->y);
	return pBuffer;
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
void* load_STRING8( FILE *fp )
{
	MALLOC_BUFFER(STRING8);
	
	set_uint8_t(fp, pBuffer->count);
	set_STRING(fp, pBuffer->string, sizeof(pBuffer->string));
	return pBuffer;
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
