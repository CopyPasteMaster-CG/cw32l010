
#ifndef __APP_KEY_H__
#define __APP_KEY_H__

#include "stdint.h"
#include "stdbool.h"

#define APP_KEY_NUM_LEFT    (0x0)  //  ×ó°´¼ü
#define APP_KEY_NUM_RIGHT   (0x1)  //  ÓÒ°´¼ü
#define APP_KEY_NUM_USB     (0x2)  //  USBÇÐ»»°´¼ü

typedef struct {
    bool state[3];
} APP_KEY_DATA_Typedefstruct;


typedef struct {
    void ( * init ) ( void ) ;
    void ( * loop ) ( void ) ;
    void ( * read ) ( APP_KEY_DATA_Typedefstruct * da ) ;
}APP_KEY_FUN_Typestruct;
extern const APP_KEY_FUN_Typestruct APP_KEY_FUN;


#endif
