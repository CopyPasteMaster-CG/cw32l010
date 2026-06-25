#include "APP_KEY.h"
#include "bsp_key.h"
#include "BSP_SYSTICK.h"
#include "string.h"

static APP_KEY_DATA_Typedefstruct data;
static BSP_KEY_DATA_Typedefstruct bsp_value[10];
static const uint8_t bsp_num = (sizeof(bsp_value))/(sizeof(bsp_value[0]));
static uint8_t bsp_count;

static void fun_init(void)
{
    BSP_KEY_FUN.init();
}
static void fun_loop(void)
{
    static uint32_t tick;
    static uint32_t const delay_tick = 10;
    bool check_ok;
    static BSP_KEY_DATA_Typedefstruct last;
    if((BSP_SYSTICK_FUN.get_tick()-tick)>delay_tick){
        tick = BSP_SYSTICK_FUN.get_tick();
        
        //  多次采样滤波
        bsp_count = (bsp_count+1)%bsp_num;
        BSP_KEY_FUN.read(&(bsp_value[bsp_count]));
        
        for(uint8_t i=0; i<(sizeof(bsp_value[0].state)/sizeof(bsp_value[0].state[0])); i++){
            check_ok=true;
            for(uint8_t j=1; j<bsp_num; j++){
                if(bsp_value[j-1].state[i]!=bsp_value[j].state[i]){
                    check_ok=false;
                }
            }
            if(check_ok){
                data.state[i] = bsp_value[0].state[i];
            }
        }
    }
}
static void fun_read( APP_KEY_DATA_Typedefstruct * da )
{
    memcpy(da, &data, sizeof(APP_KEY_DATA_Typedefstruct));
}

const APP_KEY_FUN_Typestruct APP_KEY_FUN={
    .init   =   fun_init,
    .loop   =   fun_loop,
    .read   =   fun_read,
};

