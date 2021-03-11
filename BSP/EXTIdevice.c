/* 
 * EXTIdevice.c - The C source file of external interrupt devices
 * NOTE: This file is based on HAL library of stm32 platform
 *          
 *          接口声明功能注释见BSP.h
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-09     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"


/* 邮箱控制块 */
static struct rt_mailbox POS_mb;
/* 用于放邮件的内存池 */
static char POS_mb_pool[128];

/** @brief  GPIO外部中断回调函数
 *  @note   这是HAL库标准函数
 *  @author 江榕煜（2021.3.9）
 *  @param  
 *  @retval
**/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        // 圆盘鼠标开关
        case(POS1_EXTI2_Pin):   // 位置1
            rt_mb_send(&POS_mb,1);
            break;
        case(POS2_EXTI3_Pin):   // 位置2
            rt_mb_send(&POS_mb,2);
            break;
        case(POS3_EXTI4_Pin):   // 位置3
            rt_mb_send(&POS_mb,3);
            break;
        case(POS4_EXTI13_Pin):   // 位置4
            rt_mb_send(&POS_mb,4);
            break;
        
        // 扫把原点开关
        case(SCANO_EXTI5_Pin):
            HAL_NVIC_DisableIRQ(SCANO_EXTI5_EXTI_IRQn); // 屏蔽中断
            steerEngine_SetSpeed(0,Stop);  // 卡停扫把
            break;
    }
}

/** @brief  位置检测初始化接口
 *  @note   该接口中开启了中断
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval 位置检测邮箱对象
**/
struct rt_mailbox  * PosDtctDevice_Init(void)
{
    rt_err_t result;
    
    /* 初始化一个mailbox */
    result = rt_mb_init(&POS_mb,
                        "PosDtct_mb",                   /* 名称是PosDtct_mb */
                        &POS_mb_pool[0],                /* 邮箱用到的内存池是mb_pool */
                        sizeof(POS_mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占4字节 */
                        RT_IPC_FLAG_FIFO);              /* 采用FIFO方式进行线程等待 */
    if (result != RT_EOK)
    {
        rt_kprintf("init position detect mailbox failed.\n");
        return NULL;
    }
    
    /* 启动外部中断 */
//    HAL_NVIC_SetPriority(EXTI2_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
//    
//    HAL_NVIC_SetPriority(EXTI3_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
//    
//    HAL_NVIC_SetPriority(EXTI4_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
//    
//    HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,0);
//    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    
    HAL_NVIC_SetPriority(SCANO_EXTI5_EXTI_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    
    /* 成功初始化，返回通信邮箱对象 */
    return &POS_mb;
}
