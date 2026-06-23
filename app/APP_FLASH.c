
#include "APP_FLASH.h"
#include "BSP_FLASH.h"
#include "BSP_SYSTICK.h"
#include "string.h"

#include "APP_KVM.h"
#include "APP_SWITCH.h"

static APP_FLASH_DATA_Typedefstruct data;




static inline void data_bsp_to_app(BSP_FLASH_DATA_Typedefstruct *bsp_data, APP_FLASH_DATA_Typedefstruct * app_data)
{
    app_data->EXT_MODE.ext_enable   =   bsp_data->UART_ENABLE;
    app_data->USB_KVM.left_count    =   bsp_data->left_count;
    app_data->USB_KVM.right_count   =   bsp_data->right_count;
    app_data->USB_KVM.select_mode   =   bsp_data->select_mode;
    app_data->USB_SELECT.select_usb =   bsp_data->select_usb;
}
static inline void data_app_to_bsp(BSP_FLASH_DATA_Typedefstruct *bsp_data, APP_FLASH_DATA_Typedefstruct * app_data)
{
    bsp_data->UART_ENABLE   =   app_data->EXT_MODE.ext_enable;
    bsp_data->left_count    =   app_data->USB_KVM.left_count;
    bsp_data->right_count   =   app_data->USB_KVM.right_count;
    bsp_data->select_mode   =   app_data->USB_KVM.select_mode;
    bsp_data->select_usb    =   app_data->USB_SELECT.select_usb;
}

static void fun_init(void)
{
    BSP_FLASH_DATA_Typedefstruct bsp_data;
    BSP_FLASH_FUN.init(NULL);
    BSP_FLASH_FUN.read(&bsp_data);
    data_bsp_to_app(&bsp_data, &data);
}
static void fun_loop(void)
{
    static uint32_t tick;
    static uint32_t const delay_tick = 100;
    bool check_ok;
    
    APP_KVM_DATA_Typedefstruct kvm;
    APP_SWITCH_DATA_Typedefstruct sw;
    BSP_FLASH_DATA_Typedefstruct bsp;
    
    if((BSP_SYSTICK_FUN.get_tick()-tick)>delay_tick){
        tick = BSP_SYSTICK_FUN.get_tick();
        
        APP_KVM_FUN.read(&kvm);
        APP_SWITCH_FUN.read(&sw);
        
        data.USB_KVM.left_count =   kvm.left_count;
        data.USB_KVM.right_count=   kvm.right_count;
        data.USB_KVM.select_mode=   kvm.select_mode;
        data.USB_SELECT.select_usb= sw.state;
        
        data_app_to_bsp(&bsp, &data);
        BSP_FLASH_FUN.write(&bsp);
        BSP_FLASH_FUN.read(&bsp);
        data_bsp_to_app(&bsp, &data);
    }
}
static void fun_read( APP_FLASH_DATA_Typedefstruct * da )
{
    memcpy(da, &data, sizeof(APP_FLASH_DATA_Typedefstruct));
}

const APP_FLASH_FUN_Typestruct APP_FLASH_FUN={
    .init   =   fun_init,
    .loop   =   fun_loop,
    .read   =   fun_read,
};

