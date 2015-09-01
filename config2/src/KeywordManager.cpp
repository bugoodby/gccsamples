#include "KeywordManager.h"
#include <string.h>
#include <stdlib.h>


KeywordManager::KeywordManager()
{
	loadIdStringTable();
}

KeywordManager::~KeywordManager()
{
}

//
// 数値に対応する文字列を返す。見つからなかったら???を返す。
//
const char* KeywordManager::getIdString( uint16_t id )
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
uint16_t KeywordManager::getStringId( const char* str )
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
bool KeywordManager::loadIdStringTable()
{
	const char szResource[] = "./id_table.txt";
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
			char *ptr = strchr(pSecond, '\n');
			if ( ptr ) { *ptr = '\0'; }
		} else {
			fprintf(stderr, "invalid line - %s\n", line);
			continue;
		}
		uint16_t id = (uint16_t)strtoul(pFirst, NULL, 0);
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

