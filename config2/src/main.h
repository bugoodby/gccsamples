#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

/*----------------------------------------*/
/* Module Name & Version                  */
/*----------------------------------------*/
#define MODULENAME_STRING	"config2"
#define VERSION_STRING		"0.01"


/*----------------------------------------*/
/* OPTION Structure                       */
/*----------------------------------------*/

typedef struct _GLOBALPROP {
	char* file;
} GLOBALPROP;


extern GLOBALPROP g_gprop;
inline GLOBALPROP* prop() { return &g_gprop; }



typedef struct {
	uint16_t id;
	uint8_t aaa;
	uint16_t bbb;
	uint32_t ccc;
	uint32_t size;
	void *value;
} SECTION_DATA;




#endif /* __MAIN_H__ */
