#ifndef __MAIN_H__
#define __MAIN_H__

/*----------------------------------------*/
/* Module Name & Version                  */
/*----------------------------------------*/
#define MODULENAME_STRING	"dumpbin"
#define VERSION_STRING		"0.01"


/*----------------------------------------*/
/* OPTION Structure                       */
/*----------------------------------------*/

typedef struct _GLOBALPROP {
	char* file;
} GLOBALPROP;


extern GLOBALPROP g_gprop;
inline GLOBALPROP* prop() { return &g_gprop; }


#endif /* __MAIN_H__ */
