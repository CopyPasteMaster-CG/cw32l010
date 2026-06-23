
#include "APP_SWITCH.h"
#include "BSP_SWITCH.h"
#include "BSP_SYSTICK.h"
#include "string.h"
#include "APP_FLASH.h"
#include "APP_KEY.h"

static APP_SWITCH_DATA_Typedefstruct data;

static void fun_init(void)
{
    APP_FLASH_DATA_Typedefstruct flash;
    BSP_SWITCH_FUN.init();
    APP_FLASH_FUN.read(&flash);
    data.state =   flash.USB_SELECT.select_usb%4;
}
static void fun_loop(void)
{
    static uint32_t tick;
    static uint32_t const delay_tick = 50;
    static bool last=true;
    APP_KEY_DATA_Typedefstruct key;
    
    if((BSP_SYSTICK_FUN.get_tick()-tick)>delay_tick){
        tick = BSP_SYSTICK_FUN.get_tick();
        
        APP_KEY_FUN.read(&key);
        
        if((key.state[APP_KEY_NUM_USB]==true) && (last==false)){
            data.state = (data.state+1)%4;
        }
        
        if(data.state==0){
            BSP_SWITCH_FUN.out(BSP_SWITCH_MODE_1);
        }else if(data.state==1){
            BSP_SWITCH_FUN.out(BSP_SWITCH_MODE_2);
        }else if(data.state==2){
            BSP_SWITCH_FUN.out(BSP_SWITCH_MODE_3);
        }else if(data.state==3){
            BSP_SWITCH_FUN.out(BSP_SWITCH_MODE_4);
        }
        last=key.state[APP_KEY_NUM_USB];
    }
}
static void fun_read( APP_SWITCH_DATA_Typedefstruct * da )
{
    memcpy(da, &data, sizeof(APP_SWITCH_DATA_Typedefstruct));
}

const APP_SWITCH_FUN_Typestruct APP_SWITCH_FUN={
    .init   =   fun_init,
    .loop   =   fun_loop,
    .read   =   fun_read,
};

