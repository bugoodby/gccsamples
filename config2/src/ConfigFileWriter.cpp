#include "ConfigFileWriter.h"
#include "handler.h"
#include <string.h>

//
// 16iƒ_ƒ“ƒv
//
void ConfigFileWriter::dumpValueHex( FILE *fp, const void *pData, size_t size )
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

bool ConfigFileWriter::writeValueData( uint16_t id, void *pData, size_t size )
{
	bool ret = false;
	
	dumpValuePtr ptr = getDumpFunc(id);
	if ( ptr ) {
		ptr( m_fp, pData, size );
	} else {
		dumpValueHex( m_fp, pData, size );
	}
	return ret;
}

ConfigFileWriter::ConfigFileWriter() : m_fp(NULL)
{
}

ConfigFileWriter::~ConfigFileWriter() 
{
	close();
}

bool ConfigFileWriter::open( const char *fname, bool isCreate )
{
	m_fp = fopen(fname, (isCreate)?"w":"a");
	if ( !m_fp ) {
		fprintf(stderr, "%s: Could not open %s\n.", __func__, fname);
		return false;
	}
	return true;
}

void ConfigFileWriter::close() 
{
	if ( m_fp ) {
		fclose(m_fp);
		m_fp = NULL;
	}
}

bool ConfigFileWriter::write( std::list<SECTION_DATA> &list ) 
{
	if ( !m_fp ) {
		return false;
	}
	
	std::list<SECTION_DATA>::iterator it = list.begin();
	for ( ; it != list.end(); it++ ) {
		fprintf(m_fp, "id : %u\n", it->id);
		fprintf(m_fp, "aaa : %u\n", it->aaa);
		fprintf(m_fp, "bbb : %u\n", it->bbb);
		fprintf(m_fp, "ccc : %u\n", it->ccc);
		fprintf(m_fp, "size : %u\n", it->size);
		
		writeValueData(it->id, it->value, it->size);
	}
	
	return true;
}

