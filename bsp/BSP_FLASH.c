
#include "BSP_FLASH.h"
#include "BSP.h"
#include "cw32l052_flash.h"
#include "cw32l052_rcc.h"
static void fun_data_dafault_set(BSP_FLASH_DATA_Typedefstruct * data)
{
    memset(data, 0, sizeof(BSP_FLASH_DATA_Typedefstruct));
}
static BSP_FLASH_DATA_DEFAULT_Callback_typedef data_default_callback = fun_data_dafault_set;



typedef struct{
    BSP_FLASH_DATA_Typedefstruct data;  //  有效数据
    uint16_t save_size;                 //  存储在flash中，数据的大小，校验用，应该是个固定大小
    uint8_t sum_check;                  //  和校验，包含data和save_size
}BSP_FLASH_SAVE_DATA_Typedefstruct;


static const uint8_t FLASH_MARK_DATA   =   0xF5;
static const uint32_t FLASH_USER_SAVE_DATA_PAGE_MAX = 60;   //  使用多少页，用来存储自己的数据，页面越多，擦写的次数越多

static const uint32_t FLASH_REALY_ADDR_START    =   0x00000000; //  flash 物理起始地址
static const uint32_t FLASH_REALY_PAGE_SIZE     =   512;        //  flash 页面大小
static const uint32_t FLASH_REALY_PAGE_NUM      =   128;        //  flash 页面数量

static const uint32_t FLASH_ADDR_START = FLASH_REALY_ADDR_START+(FLASH_REALY_PAGE_NUM-FLASH_USER_SAVE_DATA_PAGE_MAX)*FLASH_REALY_PAGE_SIZE;

static const uint16_t save_in_flash_data_size = sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct)  + sizeof(FLASH_MARK_DATA);   //  实际存储在flash中数据的大小


static volatile uint32_t USER_ADDR;
static BSP_FLASH_SAVE_DATA_Typedefstruct buf;  //  用来缓存，当flash全部损坏，可以通过缓存保存本次上电后的配置（断电后的状态未知）

static inline uint8_t sum_check_uint8_t(void const *da, uint32_t len)
{
    uint8_t sum=0;
    for(uint32_t i=0; i<len; i++){
        sum+=((uint8_t const *)da)[i];
    }
    return sum;
}

static inline void flash_erase_psge(uint32_t start, uint32_t stop)
{
    FLASH_UnlockPages(start, stop);
    FLASH_ErasePages(start, stop);
    FLASH_LockPages(start, stop);
}

static inline void flash_write_mark(uint32_t address)
{
    uint8_t mark = FLASH_MARK_DATA;
    FLASH_UnlockPage(address/FLASH_REALY_PAGE_SIZE);
    FLASH_WriteBytes(address, &mark, sizeof(mark));
    FLASH_LockPage(address/FLASH_REALY_PAGE_SIZE);
}

static inline void flash_write_data(uint32_t address, BSP_FLASH_SAVE_DATA_Typedefstruct *save)
{
    FLASH_UnlockPages(address+sizeof(FLASH_MARK_DATA),address+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
    FLASH_WriteBytes(address+sizeof(FLASH_MARK_DATA), (void*)(save), sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
    FLASH_LockPages(address+sizeof(FLASH_MARK_DATA),address+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
}



//  初始化
static void fun_init(BSP_FLASH_DATA_DEFAULT_Callback_typedef callback)
{
    uint8_t mark;
    if(callback){
        data_default_callback = callback;
    }
    
    __RCC_FLASH_CLK_ENABLE();
    FLASH_LockAllPages();
    
    if(0){
        //  读取到错误数据，需要将数据恢复成默认值
        data_error:
        data_default_callback(&buf.data);
        buf.save_size = save_in_flash_data_size;
        buf.sum_check = ((uint8_t)(sum_check_uint8_t(&buf.data, sizeof(BSP_FLASH_DATA_Typedefstruct))+save_in_flash_data_size));
        flash_erase_psge(FLASH_ADDR_START, FLASH_ADDR_START+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
        flash_write_data(FLASH_ADDR_START, &buf);
    }
    USER_ADDR = FLASH_ADDR_START;
    
    while(1){
        mark = *((uint8_t*)(USER_ADDR));
        if(mark==0xFF){
            //  数据块有效
            memcpy(&buf, (void const *)(USER_ADDR+sizeof(FLASH_MARK_DATA)), sizeof(buf));    //  flash保存的数据
            if( (((uint8_t)(sum_check_uint8_t(&buf.data, sizeof(BSP_FLASH_DATA_Typedefstruct))+save_in_flash_data_size)) == buf.sum_check)
            &&  (buf.save_size == save_in_flash_data_size)){
                //  校验通过，退出
                return;
            }else{
                //  校验失败，数据丢失，数据恢复默认值
                goto data_error;
            }
        }else if(mark!=FLASH_MARK_DATA){
            //  无效的mark数据，退出
            goto data_error;
        }else{
            //  数据块无效，查找下一处
            USER_ADDR+=sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct);
            if( ((USER_ADDR+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct))>=(FLASH_ADDR_START+FLASH_USER_SAVE_DATA_PAGE_MAX*FLASH_REALY_PAGE_SIZE)) 
            ||  (USER_ADDR>=(FLASH_ADDR_START+FLASH_USER_SAVE_DATA_PAGE_MAX*FLASH_REALY_PAGE_SIZE))
            ||  (USER_ADDR<FLASH_ADDR_START)){
                //  剩余的存储空间大小，无法满足存放的需求，数据丢失
                goto data_error;
            }
        }
    }
    
}

static void fun_read(BSP_FLASH_DATA_Typedefstruct * data)
{
    memcpy(data, &buf.data, sizeof(BSP_FLASH_DATA_Typedefstruct));
}

static void fun_write(BSP_FLASH_DATA_Typedefstruct * data)
{
    if(memcmp(&buf.data, data, sizeof(BSP_FLASH_DATA_Typedefstruct))==0){
        //  待保存的数据，和存储的数据相同，不需要执行保存操作，直接退出
        return;
    }else{
        //  注销上一个存储空间
        flash_write_mark(USER_ADDR);
        
        //  对数据存放地址进行判定
        USER_ADDR+=sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct);
        if( ((USER_ADDR+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct))>=(FLASH_ADDR_START+FLASH_USER_SAVE_DATA_PAGE_MAX*FLASH_REALY_PAGE_SIZE)) 
        ||  (USER_ADDR>=(FLASH_ADDR_START+FLASH_USER_SAVE_DATA_PAGE_MAX*FLASH_REALY_PAGE_SIZE))
        ||  (USER_ADDR<FLASH_ADDR_START)){
            //  地址超出限制，需要从头开始存储
            USER_ADDR = FLASH_ADDR_START;
        }
        
        //  擦除使用的空间，准备存放新的数据
        if(USER_ADDR%FLASH_REALY_PAGE_SIZE){
            //  新存储空间的开头，和之前存储空间的结尾，在同一页上
            if((USER_ADDR/FLASH_REALY_PAGE_SIZE) != ((USER_ADDR+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct))/FLASH_REALY_PAGE_SIZE)){
                //  新存储空间的结尾，和之前存储空间的结尾，在不同页上，需要进行擦除操作
                flash_erase_psge(((USER_ADDR/FLASH_REALY_PAGE_SIZE)+1)*FLASH_REALY_PAGE_SIZE, USER_ADDR+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
            }
        }else{
            //  新的一页
            flash_erase_psge(USER_ADDR, USER_ADDR+sizeof(FLASH_MARK_DATA)+sizeof(BSP_FLASH_SAVE_DATA_Typedefstruct));
        }
        
        //  写入数据
        memcpy(&buf.data, data, sizeof(BSP_FLASH_DATA_Typedefstruct));
        buf.save_size = save_in_flash_data_size;
        buf.sum_check = ((uint8_t)(sum_check_uint8_t(&buf.data, sizeof(BSP_FLASH_DATA_Typedefstruct))+save_in_flash_data_size));
        flash_write_data(USER_ADDR, &buf);
    }
    
}

const BSP_FLASH_FUN_Typedefstruct BSP_FLASH_FUN={
    .init   =   fun_init,
    .read   =   fun_read,
    .write  =   fun_write,
};


