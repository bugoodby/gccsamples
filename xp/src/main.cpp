#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <map>
#include <list>
#include <string>
#include "debuglog.h"

#include "handler.h"


//-------------------------------------------------------
// Singleton Class
//-------------------------------------------------------
/*
class Singleton
{
private:
	Singleton(){}
	Singleton( const Singleton &other ){}
	Singleton &operator=( const Singleton &other ){ return *this; }
	
public:
	static Singleton &getInstance() {
		static Singleton instance;
		return instance;
	}
};
*/


//-------------------------------------------------------
// gccAttributeManager
//-------------------------------------------------------
class gccAttributeManager
{
protected:
	std::map<uint16_t, dumpValuePtr> m_traitMap;
	std::map<uint16_t, std::string> m_idTbl;
	std::list<uint16_t> m_idList;
	
	bool loadIdStringTable();
	bool loadIdList();
	void dumpValueHex( FILE *fp, const void *pData, size_t size );
	
public:
	gccAttributeManager();
	virtual ~gccAttributeManager();
	
	const char* getIdString( uint16_t id );
	uint16_t getStringId( const char* str );
	void dumpValue( int id, FILE *fp, const void *pData, size_t size );
};



gccAttributeManager::gccAttributeManager()
{
	loadIdStringTable();
	loadIdList();
	
	m_traitMap[1] = dump_uint8_t;
	m_traitMap[3] = dump_uint32_t;
}

gccAttributeManager::~gccAttributeManager()
{
}

void gccAttributeManager::dumpValue( int id, FILE *fp, const void *pData, size_t size )
{
	fprintf( fp, "[ %s (%u) ]\n", getIdString(id), id );
	
#if 1
	dumpValuePtr ptr = getDumpFunc(id);
	if ( ptr ) {
		ptr( fp, pData, size );
	} else {
		dumpValueHex( fp, pData, size );
	}
#else
	if ( m_traitMap.find(id) == m_traitMap.end() ) {
		dumpValueHex( fp, pData, size );
	} else {
		m_traitMap[id]( fp, pData, size );
	}
#endif
}

//
// 数値に対応する文字列を返す。見つからなかったら???を返す。
//
const char* gccAttributeManager::getIdString( uint16_t id )
{
	const char *str = "???";
	
	if ( m_idTbl.find(id) == m_idTbl.end() ) {
		return str;
	}
	return m_idTbl[id].c_str();
}

//
// 文字列に対応する数値を返す。見つからなかったら0を返す。
//
uint16_t gccAttributeManager::getStringId( const char* str )
{
	for ( std::map<uint16_t, std::string>::iterator it = m_idTbl.begin(); it != m_idTbl.end(); it++ ) {
		if ( strcmp(it->second.c_str(), str) == 0 ) {
			return it->first;
		}
	}
	return 0;
}

//
// 「数値,文字列」形式の複数行ファイルをstd::mapに読み込む
//
bool gccAttributeManager::loadIdStringTable()
{
	char szResource[] = "./id_table.txt";
	char line[1024] = "";
	char *pFirst = NULL, *pSecond = NULL;

	FILE *fp = fopen(szResource, "r");
	if ( !fp ) {
		ERR("cannot open %s\n", szResource);
		return false;
	}
	
	while ( fgets(line, sizeof(line), fp) ) {
		pFirst = line;
		pSecond = strchr(line, ',');
		if ( pSecond ) {
			*pSecond++ = '\0';
			pSecond = strtok(pSecond, "\r\n\0");
		} else {
			WARN("invalid line - %s\n", line);
			continue;
		}
		uint16_t id = (uint16_t)strtoul(pFirst, NULL, 0);
		m_idTbl[id] = pSecond;
	}

	fclose(fp);
	
	//debug
	INFO("---\n");
	std::map<uint16_t, std::string>::iterator it = m_idTbl.begin();
	for ( ; it != m_idTbl.end(); ++it ) {
		INFO("%u = %s\n", it->first, it->second.c_str());
	}
	INFO("---\n");
	
	return true;
}

//
// 「文字列」形式の複数行ファイルをstd::listに読み込む
//  （mapを参照し、文字列を数値に変換）
//
bool gccAttributeManager::loadIdList()
{
	char szResource[] = "./id_list.txt";
	char line[1024] = "";

	FILE *fp = fopen(szResource, "r");
	if ( !fp ) {
		ERR("cannot open %s\n", szResource);
		return false;
	}
	
	while ( fgets(line, sizeof(line), fp) ) {
		uint16_t id = getStringId(line);
		if ( id != 0 ) {
			m_idList.push_back(id);
		}
	}
	
	fclose(fp);

	//debug
	INFO("---\n");
	std::list<uint16_t>::iterator it = m_idList.begin();
	for ( ; it != m_idList.end(); ++it ) {
		INFO("%u\n", *it);
	}
	INFO("---\n");
	
	return true;
}

//
// 16進ダンプ
//
void gccAttributeManager::dumpValueHex( FILE *fp, const void *pData, size_t size )
{
	const char szHexArray[] = "0123456789ABCDEF";
	const unsigned char *p = (const unsigned char*)pData;
	size_t l, line = 0;
	char lineBuf[52] = {0};
	char *pHexPtr = NULL;
	
	lineBuf[49] = '\n';
	
	fprintf( fp, "{\n" );
	
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

//-------------------------------------------------------


struct AttributeHandler_uint8_t
{
	void dump( FILE *fp, const void *pData )
	{
		uint8_t* p = (uint8_t*)pData;
		
		DUMP_LINE( "struct body {\n" );
		DUMP_LINE( "  uint8_t Value = %u;\n", *p );
		DUMP_LINE( "};\n" );
	};
	
	void* load( FILE *fp )
	{
		uint8_t *pBuffer = (uint8_t*)malloc(sizeof(uint8_t));
		if ( !pBuffer ) {
			return NULL;
		}
		
		set_uint8_t(fp, *pBuffer);
		return pBuffer;
	};
};
static AttributeHandler_uint8_t ah_uint8_t;


//-------------------------------------------------------

int main( int argc, char **argv )
{
	gccAttributeManager* pManager = new gccAttributeManager();
	
	FILE *fp = fopen("./dump.txt", "w");
	
	// dump
	uint8_t val1 = 55;
	pManager->dumpValue( 1, fp, &val1, sizeof(val1) );
	
	uint32_t val2 = 0x300001;
	pManager->dumpValue( 3, fp, &val2, sizeof(val2) );
	
	USIZE32 val3 = { 0x12345678, 0x23456789 };
	pManager->dumpValue( 4, fp, &val3, sizeof(val3));
	
	STRING8 val4 = { 3, "" };
	memset(val4.string, '.', sizeof(val4.string));
	val4.string[254] = '*';
	pManager->dumpValue( 5, fp, &val4, sizeof(val4));
	
	fclose(fp);
	
	
	
	// get id from string.
	const char *str1 = "str_piyopiyo";
	fprintf( stderr, "[STR=>ID] %s => %u\n", str1, pManager->getStringId(str1) );
	
	const char *str2 = "str_piyo";
	fprintf( stderr, "[STR=>ID] %s => %u\n", str2, pManager->getStringId(str2) );
	
	
	delete pManager;
	return 0;
}




