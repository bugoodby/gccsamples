#ifndef __KEYWORDMANAGER_H__
#define __KEYWORDMANAGER_H__

#include <map>
#include <string>
#include <stdio.h>
#include <stdint.h>

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
// KeywordManager
//-------------------------------------------------------
class KeywordManager
{
protected:
	std::map<uint16_t, std::string> m_idTbl;
	
	bool loadIdStringTable();
	
public:
	KeywordManager();
	virtual ~KeywordManager();
	
	const char* getIdString( uint16_t id );
	uint16_t getStringId( const char* str );
};




#endif
