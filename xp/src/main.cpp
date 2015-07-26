#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>


typedef void (*dumpValuePtr)( FILE *fp, const void *pData, size_t size );


//! uint8_t
void dump_uint8_t( FILE *fp, const void *pData, size_t size )
{
	uint8_t* p = (uint8_t*)pData;
	
	fprintf( fp, "struct body {\n" );
	fprintf( fp, "  uint8_t Value = %u;\n", *p );
	fprintf( fp, "};\n" );
};

//! uint16_t
void dump_uint16_t( FILE *fp, const void *pData, size_t size )
{
	uint16_t* p = (uint16_t*)pData;
	
	fprintf( fp, "struct body {\n" );
	fprintf( fp, "  uint16_t Value = %u;\n", *p );
	fprintf( fp, "};\n" );
};

//! uint32_t
void dump_uint32_t( FILE *fp, const void *pData, size_t size )
{
	uint32_t* p = (uint32_t*)pData;
	
	fprintf( fp, "struct body {\n" );
	fprintf( fp, "  uint32_t Value = 0x%x;\n", *p );
	fprintf( fp, "};\n" );
};


class gccAttributeManager
{
protected:
	std::map<uint16_t, dumpValuePtr> m_traitMap;
	std::map<uint16_t, std::string> m_idTbl;
	
	void dumpValueHex( FILE *fp, const void *pData, size_t size );
	
public:
	gccAttributeManager();
	virtual ~gccAttributeManager();
	
	bool loadIdStringTable();
	const char* getIdString( uint16_t id );
	uint16_t getStringId( const char* str );
	void dumpValue( int id, FILE *fp, const void *pData, size_t size );
};



gccAttributeManager::gccAttributeManager()
{
	loadIdStringTable();
	
	m_traitMap[1] = dump_uint8_t;
	m_traitMap[3] = dump_uint32_t;
}

gccAttributeManager::~gccAttributeManager()
{
}

void gccAttributeManager::dumpValue( int id, FILE *fp, const void *pData, size_t size )
{
	fprintf( fp, "[ %s (%u) ]\n", getIdString(id), id );
	
	if ( m_traitMap.find(id) == m_traitMap.end() ) {
		dumpValueHex( fp, pData, size );
	} else {
		m_traitMap[id]( fp, pData, size );
	}
}

const char* gccAttributeManager::getIdString( uint16_t id )
{
	const char *str = "???";
	
	if ( m_idTbl.find(id) == m_idTbl.end() ) {
		return str;
	}
	return m_idTbl[id].c_str();
}

uint16_t gccAttributeManager::getStringId( const char* str )
{
	for ( std::map<uint16_t, std::string>::iterator it = m_idTbl.begin(); it != m_idTbl.end(); it++ ) {
		if ( strcmp(it->second.c_str(), str) == 0 ) {
			return it->first;
		}
	}
	return 0;
}

bool gccAttributeManager::loadIdStringTable()
{
	char szResource[] = "./id_table.txt";
	char line[1024] = "";
	char *pFirst = NULL, *pSecond = NULL;

	FILE *fp = fopen(szResource, "r");
	if ( !fp ) {
		fprintf(stderr, "cannot open %s\n", szResource);
		return false;
	}
	
	while ( fgets(line, sizeof(line), fp) ) {
		pFirst = line;
		pSecond = strchr(line, ',');
		if ( pSecond ) {
			*pSecond++ = '\0';
			pSecond = strtok(pSecond, "\r\n\0");
		} else {
			fprintf(stderr, "invalid line - %s\n", line);
			continue;
		}
		uint16_t id = (uint16_t)atoi(pFirst);
		m_idTbl[id] = pSecond;
	}

	fclose(fp);
	
	//debug
	fprintf(stderr, "---\n");
	std::map<uint16_t, std::string>::iterator it = m_idTbl.begin();
	for ( ; it != m_idTbl.end(); ++it ) {
		fprintf(stderr, "%u = %s\n", it->first, it->second.c_str());
	}
	fprintf(stderr, "---\n");
	
	return true;
}

void gccAttributeManager::dumpValueHex( FILE *fp, const void *pData, size_t size )
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


int main( int argc, char **argv )
{
	gccAttributeManager* pManager = new gccAttributeManager();
	
	// dump
	uint8_t val1 = 55;
	pManager->dumpValue( 1, stdout, &val1, sizeof(val1) );
	
	uint32_t val2 = 0x300001;
	pManager->dumpValue( 3, stdout, &val2, sizeof(val2) );
	
	const char *str = "0123456789ABCDEFGHIJKLMNOPQ";
	pManager->dumpValue( 5, stdout, str, strlen(str));
	
	
	// get id from string.
	const char *str1 = "str_piyopiyo";
	fprintf( stderr, "[STR=>ID] %s => %u\n", str1, pManager->getStringId(str1) );
	
	const char *str2 = "str_piyo";
	fprintf( stderr, "[STR=>ID] %s => %u\n", str2, pManager->getStringId(str2) );
	
	
	delete pManager;
	return 0;
}




