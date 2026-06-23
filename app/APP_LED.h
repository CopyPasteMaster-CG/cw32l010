
#ifndef __APP_LED_H__
#define __APP_LED_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    bool    state[8];
} APP_LED_DATA_Typedefstruct;


typedef struct {
    void ( * init ) ( void ) ;
    void ( * loop ) ( void ) ;
    void ( * read ) ( APP_LED_DATA_Typedefstruct * da ) ;
}APP_LED_FUN_Typestruct;
extern const APP_LED_FUN_Typestruct APP_LED_FUN;


#endif
