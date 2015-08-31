#ifndef __VALUELOADER_H__
#define __VALUELOADER_H__

#include "handler.h"

//////////////////////////////////////////////////////////////////////
class ValueLoader
{
protected:
	uint16_t m_id;
	size_t m_size;

	virtual bool skipToEnd( StringList &strlist ) {
		char* line = NULL;
		while ( (line = strlist.getNext()) != NULL ) {
			if ( strcasecmp(line, "END") == 0 ) {
				fprintf(stderr, "  found [END]\n");
				break;
			}
		}
		return true;
	}
	
public:
	ValueLoader() : m_id(0), m_size(0) {};
	ValueLoader( uint16_t id, size_t size ) : m_id(id), m_size(size) {};
	virtual ~ValueLoader() {};
	
	virtual bool load( StringList &strlist, void* &pData )=0;
};



class HexValueLoader : public ValueLoader
{
protected:
	bool get_nibble( const char c, uint8_t &nibble )
	{
		bool ret = true;
		switch ( c )
		{
		case '0': nibble = 0x0; break;
		case '1': nibble = 0x1; break;
		case '2': nibble = 0x2; break;
		case '3': nibble = 0x3; break;
		case '4': nibble = 0x4; break;
		case '5': nibble = 0x5; break;
		case '6': nibble = 0x6; break;
		case '7': nibble = 0x7; break;
		case '8': nibble = 0x8; break;
		case '9': nibble = 0x9; break;
		case 'a': 
		case 'A': nibble = 0xA; break;
		case 'b': 
		case 'B': nibble = 0xB; break;
		case 'c': 
		case 'C': nibble = 0xC; break;
		case 'd': 
		case 'D': nibble = 0xD; break;
		case 'e': 
		case 'E': nibble = 0xE; break;
		case 'f': 
		case 'F': nibble = 0xF; break;
		default: 
			ret = false; 
			break;
		}
		return ret;
	}

public:
	HexValueLoader( uint16_t id, size_t size ) : ValueLoader(id, size) {};
	
	bool load( StringList &strlist, void* &pData )
	{
		uint8_t *pBuffer = (uint8_t*)malloc(m_size);
		if ( ! pBuffer ) {
			fprintf(stderr, "could not allocate enough memory (size=%lu)\n", m_size);
			return false;
		}
		pData = pBuffer;
		
		char* line = NULL;
		while ( (line = strlist.getNext()) != NULL )
		{
			if ( strcasecmp(line, "END") == 0 ) {
				fprintf(stderr, "found [END]\n");
				break;
			}
			
			char *line_ptr = line;
			uint8_t octet = 0, nibble = 0;
			size_t cnt = 0;
			bool first_flag = true;
			
			while ( *line_ptr != '\0' ) {
				if ( get_nibble(*line_ptr, nibble) ) {
					if ( first_flag ) {
						octet = nibble << 4;
					} else {
						octet |= nibble & 0xF;
						pBuffer[cnt++] = octet;
					}
					first_flag = !first_flag;
				}
				line_ptr++;
			}
		}
		
		skipToEnd(strlist);
		return true;
	}
};



class StructValueLoader : public ValueLoader
{
public:
	StructValueLoader( uint16_t id, size_t size ) : ValueLoader(id, size) {};
	
	bool load( StringList &strlist, void* &pData ) {
		bool ret = false;
		
		StringList valueLines;
		char* line = NULL;
		while ( (line = strlist.getNext()) != NULL ) {
			if ( strcasecmp(line, "END") == 0 ) {
				fprintf(stderr, "found [END]\n");
				break;
			}
			valueLines.push(line);
		}
		
		loadValuePtr ptr = getLoadFunc(m_id);
		if ( ptr ) {
			ret = ptr(valueLines, pData);
		} else {
			fprintf(stderr, "[ERROR] id=%u: not support struct value.\n", m_id);
			ret = false;
		}
		
		skipToEnd(strlist);
		return ret;
	}
};

#if 0
template <typename T>
class StructValueLoader2 : public ValueLoader
{
protected:
public:
	StructValueLoader2( uint16_t id, size_t size ) : ValueLoader(id, size) {};
	
	bool load( StringList &strlist, void* &pData ) {
		bool ret = false;
		
		T* p = (T*)malloc(sizeof(T));
		
		ret = load(strlist, p);
		
		skipToEnd(strlist);
		return ret;
	}
};
#endif


class ValueLoaderFactory
{
public:
	static ValueLoader* create( const char *type, uint16_t id, size_t size ) {
		ValueLoader* pLoader = NULL;
		
		if ( strcmp(type, "Hex") == 0 ) {
			pLoader = new HexValueLoader(id, size);
		}
		else if ( strcmp(type, "Struct") == 0 ) {
			pLoader = new StructValueLoader(id, size);
		}
		else {
			fprintf(stderr, "invalid value type: %s\n", type);
		}
		return pLoader;
	}
};


#endif /* __VALUELOADER_H__ */
