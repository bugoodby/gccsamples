#ifndef __DEBUGLOG_H__
#define __DEBUGLOG_H__


#define USE_DEBUGLOG


#ifdef USE_DEBUGLOG

#define DEBUG_LOG( ... ) fprintf(stderr, __VA_ARGS__ )

#define INFO( fmt, ... )  DEBUG_LOG( "[INFO] " fmt, ## __VA_ARGS__)
#define WARN( fmt, ... )  DEBUG_LOG( "[WARN] %s: " fmt, __func__, ## __VA_ARGS__)
#define ERR( fmt, ... )   DEBUG_LOG( "[ERR ] %s(%d) %s: " fmt, __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#else

#define DEBUG_LOG( ... ) 
#define INFO( fmt, ... ) 
#define WARN( fmt, ... ) 
#define ERR( fmt, ... ) 

#endif




#endif /* __DEBUGLOG_H__ */
