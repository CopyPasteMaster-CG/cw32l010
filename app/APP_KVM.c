
#include "APP_KVM.h"
#include "BSP_KVM.h"
#include "BSP_SYSTICK.h"
#include "string.h"
#include "APP_FLASH.h"
#include "APP_KEY.h"

static APP_KVM_DATA_Typedefstruct data;

static void fun_init(void)
{
    APP_FLASH_DATA_Typedefstruct flash;
    BSP_KVM_FUN.init();
    APP_FLASH_FUN.read(&flash);
    data.left_count =   flash.USB_KVM.left_count;
    data.right_count=   flash.USB_KVM.right_count;
    data.select_mode=   flash.USB_KVM.select_mode!=0;
}
static void fun_loop(void)
{
    static const bool kvm_out_state[2][4][4]={
        {
            {   false,  false,  false,  false,  },  //  单port1USB接口功能激活
            {   true,   false,  false,  false,  },  //  单port2USB接口功能激活
            {   false,  true,   false,  false,  },  //  单port3USB接口功能激活
            {   true,   true,   false,  false,  },  //  单port4USB接口功能激活
        },
        {
            {   false,  false,  true,   false,  },  //  KM只在pot1/2两个口组成左右分布的范围内进行穿越
            {   true,   false,  true,   false,  },  //  KM只在pot3/4两个口组成左右分布的范围内进行穿越
            {   false,  false,  false,  true,   },  //  KM在port1234四个口组成一个田字格范围内穿越
            {   true,   true,   true,   true,   },  //  同步控制port1234
        },
    };
    
    static uint32_t tick;
    static uint32_t const delay_tick = 50;
    static bool key_err=true;
    static APP_KEY_DATA_Typedefstruct last_key;
    static bool start_kvm = false;
    APP_KEY_DATA_Typedefstruct key;
    BSP_KVM_DATA_Typedefstruct bsp;
    uint8_t temp;
    
    if((BSP_SYSTICK_FUN.get_tick()-tick)>delay_tick){
        tick = BSP_SYSTICK_FUN.get_tick();
        
        APP_KEY_FUN.read(&key);
        
        if(key.state[APP_KEY_NUM_LEFT]&&key.state[APP_KEY_NUM_RIGHT]){
            key_err=true;
        }else if((key.state[APP_KEY_NUM_LEFT]||key.state[APP_KEY_NUM_RIGHT])==false){
            key_err=false;
        }
        
        //  防止两颗按键都按下的状态
        if(key_err==false){
            if((key.state[APP_KEY_NUM_LEFT]==true)&&(last_key.state[APP_KEY_NUM_LEFT]==false)){
                data.left_count=(data.left_count+1)%4;
                data.select_mode=0;
            }
            if((key.state[APP_KEY_NUM_RIGHT]==true)&&(last_key.state[APP_KEY_NUM_RIGHT]==false)){
                data.right_count=(data.right_count+1)%4;
                data.select_mode=1;
            }
        }
        if(data.select_mode){
            temp = data.right_count;
        }else{
            temp = data.left_count;
        }
        bsp.state[0]    =   kvm_out_state[data.select_mode][temp][0];
        bsp.state[1]    =   kvm_out_state[data.select_mode][temp][1];
        bsp.state[2]    =   kvm_out_state[data.select_mode][temp][2];
        bsp.state[3]    =   kvm_out_state[data.select_mode][temp][3];
        
        if(start_kvm || (BSP_SYSTICK_FUN.get_tick()>1000)){
            BSP_KVM_FUN.out(&bsp);
            start_kvm=true;
        }
        
        memcpy(&last_key, &key, sizeof(key));
    }
}
static void fun_read( APP_KVM_DATA_Typedefstruct * da )
{
    memcpy(da, &data, sizeof(APP_KVM_DATA_Typedefstruct));
}

const APP_KVM_FUN_Typestruct APP_KVM_FUN={
    .init   =   fun_init,
    .loop   =   fun_loop,
    .read   =   fun_read,
};

