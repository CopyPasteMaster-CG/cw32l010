#include "APP_KVM.h"
#include "BSP_SYSTICK.h"
#include "string.h"
#include "APP_FLASH.h"
#include "APP_KEY.h"
#include "APP_UART.h"

static APP_KVM_DATA_Typedefstruct data;

static const uint8_t kvm_uart_mode[2][8] =
{
    {
        KVM_MODE_PORT1_ONLY,
        KVM_MODE_PORT2_ONLY,
        KVM_MODE_PORT3_ONLY,
        KVM_MODE_PORT4_ONLY,
        KVM_MODE_TWO_SMALL_MAIN_PORT1,
        KVM_MODE_TWO_SMALL_MAIN_PORT2,
        KVM_MODE_TWO_SMALL_MAIN_PORT3,
        0,
    },
    {
        KVM_MODE_PORT12_ONLY,
        KVM_MODE_PORT34_ONLY,
        KVM_MODE_PORT_ALL,
        KVM_MODE_PORT_ALL_SYNC,
        KVM_MODE_THREE_SMALL_MAIN_PORT1,
        KVM_MODE_THREE_SMALL_MAIN_PORT2,
        KVM_MODE_THREE_SMALL_MAIN_PORT3,
        KVM_MODE_THREE_SMALL_MAIN_PORT4,
    },
};

static uint8_t get_uart_mode(void)
{
    if (data.select_mode)
    {
        return kvm_uart_mode[1][data.right_count % 8];
    }

    return kvm_uart_mode[0][data.left_count % 7];
}




static void fun_init(void)
{
    APP_FLASH_DATA_Typedefstruct flash;
    
    APP_FLASH_FUN.read(&flash);
    data.left_count = flash.USB_KVM.left_count % 7;
    data.right_count = flash.USB_KVM.right_count % 8;
    data.select_mode = flash.USB_KVM.select_mode != 0;
}

static void fun_loop(void)
{
    static uint32_t tick;
    static uint32_t const delay_tick = 50;
    static bool key_err = true;
    static APP_KEY_DATA_Typedefstruct last_key;
    APP_KEY_DATA_Typedefstruct key;
    bool mode_changed = false;

    if ((BSP_SYSTICK_FUN.get_tick() - tick) > delay_tick)
    {
        tick = BSP_SYSTICK_FUN.get_tick();

        APP_KEY_FUN.read(&key);

        if (key.state[APP_KEY_NUM_LEFT] && key.state[APP_KEY_NUM_RIGHT])
        {
            key_err = true;
        }
        else if ((key.state[APP_KEY_NUM_LEFT] || key.state[APP_KEY_NUM_RIGHT]) == false)
        {
            key_err = false;
        }

        //  防止两颗按键都按下的状态
        if (key_err == false)
        {
            if ((key.state[APP_KEY_NUM_LEFT] == true) && (last_key.state[APP_KEY_NUM_LEFT] == false))
            {
                data.left_count = (data.left_count + 1) % 7;
                data.select_mode = 0;
                mode_changed = true;
            }

            if ((key.state[APP_KEY_NUM_RIGHT] == true) && (last_key.state[APP_KEY_NUM_RIGHT] == false))
            {
                data.right_count = (data.right_count + 1) % 8;
                data.select_mode = 1;
                mode_changed = true;
            }
        }

        if (mode_changed)
        {
             KVM_UART_SendFrame(BSP_UART_AX6800,KVM_CMD_SET_MODE, get_uart_mode());
             KVM_UART_SendFrame(BSP_UART_N32,KVM_CMD_SET_MODE, get_uart_mode());
        }

        memcpy(&last_key, &key, sizeof(key));
    }
}

static void fun_read(APP_KVM_DATA_Typedefstruct * da)
{
    memcpy(da, &data, sizeof(APP_KVM_DATA_Typedefstruct));
}

const APP_KVM_FUN_Typestruct APP_KVM_FUN =
{
    .init = fun_init,
    .loop = fun_loop,
    .read = fun_read,
};
