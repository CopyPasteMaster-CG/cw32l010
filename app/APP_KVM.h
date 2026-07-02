
#ifndef __APP_KVM_H__
#define __APP_KVM_H__

#include "stdint.h"
#include "stdbool.h"

typedef struct {
    uint8_t left_count;     //  左按钮计数
    uint8_t right_count;    //  右按钮计数
    uint8_t select_mode;    //  选择左按钮还是右按钮
} APP_KVM_DATA_Typedefstruct;


typedef struct {
    void ( * init ) ( void ) ;
    void ( * loop ) ( void ) ;
    bool ( * set_mode ) ( uint8_t mode ) ;
    void ( * read ) ( APP_KVM_DATA_Typedefstruct * da ) ;
}APP_KVM_FUN_Typestruct;
extern const APP_KVM_FUN_Typestruct APP_KVM_FUN;


#endif
