
#ifndef __APP_SWITCH_H__
#define __APP_SWITCH_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    uint8_t state;
} APP_SWITCH_DATA_Typedefstruct;


typedef struct {
    void ( * init ) ( void ) ;
    void ( * loop ) ( void ) ;
    void ( * read ) ( APP_SWITCH_DATA_Typedefstruct * da ) ;
}APP_SWITCH_FUN_Typestruct;
extern const APP_SWITCH_FUN_Typestruct APP_SWITCH_FUN;


#endif
