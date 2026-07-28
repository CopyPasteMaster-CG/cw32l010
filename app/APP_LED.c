
#include "APP_LED.h"
#include "BSP_LED.h"
#include "BSP_SYSTICK.h"
#include "string.h"

#include "APP_KVM.h"
#include "APP_SWITCH.h"

static APP_LED_DATA_Typedefstruct data;

static void fun_init(void)
{
    BSP_LED_FUN.init(true);
}
static void fun_loop(void)
{
    static uint32_t usb_all_control_led_tick = 100;
    static uint32_t tick;
    static uint32_t const delay_tick = 10;
    //bool check_ok;
    APP_KVM_DATA_Typedefstruct kvm;
    APP_SWITCH_DATA_Typedefstruct sw;
    
    if((BSP_SYSTICK_FUN.get_tick()-tick)>delay_tick){
        tick = BSP_SYSTICK_FUN.get_tick();
        
        APP_KVM_FUN.read(&kvm);
        APP_SWITCH_FUN.read(&sw);
        
        BSP_LED_FUN.off(BSP_LED_NUM_1);
        BSP_LED_FUN.off(BSP_LED_NUM_2);
        BSP_LED_FUN.off(BSP_LED_NUM_3);
        BSP_LED_FUN.off(BSP_LED_NUM_4);
        BSP_LED_FUN.off(BSP_LED_USB_1);
        BSP_LED_FUN.off(BSP_LED_USB_2);
        BSP_LED_FUN.off(BSP_LED_USB_3);
        BSP_LED_FUN.off(BSP_LED_USB_4);
        if(kvm.select_mode==0){
            if(kvm.left_count==0){
                BSP_LED_FUN.on(BSP_LED_NUM_4);
            }else if(kvm.left_count==1){
                BSP_LED_FUN.on(BSP_LED_NUM_3);
            }else if(kvm.left_count==2){
                BSP_LED_FUN.on(BSP_LED_NUM_2);
            }else if(kvm.left_count==3){
                BSP_LED_FUN.on(BSP_LED_NUM_1);
            }else if(kvm.left_count==4){
                BSP_LED_FUN.on(BSP_LED_NUM_4);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%2)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%2)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }
            }else if(kvm.left_count==5){
                BSP_LED_FUN.on(BSP_LED_NUM_3);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%2)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%4)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }
            }else if(kvm.left_count==6){
                BSP_LED_FUN.on(BSP_LED_NUM_2);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%2)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%2)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }
            }
        }else{
            if(kvm.right_count==0){
                BSP_LED_FUN.on(BSP_LED_NUM_3);
                BSP_LED_FUN.on(BSP_LED_NUM_4);
            }else if(kvm.right_count==1){
                BSP_LED_FUN.on(BSP_LED_NUM_1);
                BSP_LED_FUN.on(BSP_LED_NUM_2);
            }else if(kvm.right_count==2){
                BSP_LED_FUN.on(BSP_LED_NUM_1);
                BSP_LED_FUN.on(BSP_LED_NUM_2);
                BSP_LED_FUN.on(BSP_LED_NUM_3);
                BSP_LED_FUN.on(BSP_LED_NUM_4);
            }else if(kvm.right_count==3){
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%4)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_1);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%4)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%4)==2){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%4)==3){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }
            }else if(kvm.right_count==4){
                BSP_LED_FUN.on(BSP_LED_NUM_4);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==2){
                    BSP_LED_FUN.on(BSP_LED_NUM_1);
                }
            }else if(kvm.right_count==5){
                BSP_LED_FUN.on(BSP_LED_NUM_3);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_1);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==2){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }
            }else if(kvm.right_count==6){
                BSP_LED_FUN.on(BSP_LED_NUM_2);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_1);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==2){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }
            }else if(kvm.right_count==7){
                BSP_LED_FUN.on(BSP_LED_NUM_1);
                if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==0){
                    BSP_LED_FUN.on(BSP_LED_NUM_4);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==1){
                    BSP_LED_FUN.on(BSP_LED_NUM_2);
                }else if(((BSP_SYSTICK_FUN.get_tick()/usb_all_control_led_tick)%3)==2){
                    BSP_LED_FUN.on(BSP_LED_NUM_3);
                }
            }
            
        }
        
        if(sw.state==0){
            BSP_LED_FUN.on(BSP_LED_USB_1);
        }else if(sw.state==1){
            BSP_LED_FUN.on(BSP_LED_USB_2);
        }else if(sw.state==2){
            BSP_LED_FUN.on(BSP_LED_USB_3);
        }else if(sw.state==3){
            BSP_LED_FUN.on(BSP_LED_USB_4);
        }
        
        
        BSP_LED_FUN.refresh();
    }
}
static void fun_read( APP_LED_DATA_Typedefstruct * da )
{
    memcpy(da, &data, sizeof(APP_LED_DATA_Typedefstruct));
}

const APP_LED_FUN_Typestruct APP_LED_FUN={
    .init   =   fun_init,
    .loop   =   fun_loop,
    .read   =   fun_read,
};

