/* 
 * garbageThrow.h - The C source file of the throwing garbage thread
 * NOTE: This file is based on HAL library of stm32 platform
 *          这是一个线程（任务）文件
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
#include "taskAPI.h"

#define SCAN_NVIC_DELAY 500
#define SCAN_PART_TIME 1000

/*---内存对象分配静态存储空间---*/

rt_mailbox_t pos_mb;

/* 垃圾投入信息 服务邮箱控制块 */
static struct rt_mailbox gbgTr_mb;
/* 用于放邮件的内存池 */
static char gbgTr_mb_pool[128];

static struct rt_thread threadGarbageThrow_Source;
static char stack_GarbageThrow[512];

/*---线程内容定义---*/

/** @brief  垃圾投入任务线程
 *  @note   该线程作为服务挂载，外部触发垃圾投递
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval None
**/
static void threadEntry_GarbageThrow(void * param)
{
    uint32_t pos = 0;
    while(1)
    {
        /* 监听服务：从邮箱中收取邮件 */
        if (rt_mb_recv(&gbgTr_mb, (rt_uint32_t *)&pos, RT_WAITING_FOREVER) == RT_EOK)
        {
            switch(pos)
            {
                /*----NOTE:圆盘开机卡在位置1上----*/
                    
                case(1):
                    //  圆盘转至1号位置
                    steerEngine_SetPos(1,Pos1);
                
                    rt_thread_mdelay(SCAN_PART_TIME*1);
                    break;
                case(2):
                    //  圆盘转至2号位置
                    steerEngine_SetPos(1,Pos2);
                
                    rt_thread_mdelay(SCAN_PART_TIME*2);
                    break;
                case(3):
                    //  圆盘转至3号位置
                    steerEngine_SetPos(1,Pos3);
                
                    rt_thread_mdelay(SCAN_PART_TIME*3);
                    break;
                case(4):
                    //  圆盘转至4号位置
                    steerEngine_SetPos(1,Pos4);
                
                    rt_thread_mdelay(SCAN_PART_TIME*4);
                    break;
            }
            // 扫把扫描一圈
            brushScanOnce();
            rt_thread_mdelay(4000);
            steerEngine_SetPos(1,Pos2);
        }
    }
}

/*---用户接口---*/

/** @brief  线程初始化接口
 *  @note   必须调用该接口才会启动垃圾自动投入
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval 
 *      （rt_mailbox_t）  需要投入的垃圾箱号告知邮箱对象
**/
rt_mailbox_t threadInit_GarbageThrow(void)
{
    /* 初始化一个mailbox */
    if(rt_mb_init(&gbgTr_mb,
                "garbageThrow_mb",                   /* 名称是PosDtct_mb */
                &gbgTr_mb_pool[0],                /* 邮箱用到的内存池是mb_pool */
                sizeof(gbgTr_mb_pool) / 4,        /* 邮箱中的邮件数目，因为一封邮件占4字节 */
                RT_IPC_FLAG_FIFO)           /* 采用FIFO方式进行线程等待 */
        != RT_EOK)
    {
        rt_kprintf("init garbage throwing mailbox failed.\n");
        return RT_NULL;
    }
    
    /* 建立静态线程 */
    if(rt_thread_init(&threadGarbageThrow_Source,
                "garbage throw",
                threadEntry_GarbageThrow,
                RT_NULL,
                &stack_GarbageThrow[0],
                sizeof(stack_GarbageThrow),
                0,0)
        != RT_EOK)
    {
        rt_kprintf("init garbage throwing thread failed!\n");
        return RT_NULL;
    }
    /* 启动线程 */
    rt_thread_startup(&threadGarbageThrow_Source);  // 启动线程
    
    rt_kprintf("init garbage throwing thread success!\n");
    
    /* 初始化位置检测设备（BSP：EXTIdevice.c） */
    pos_mb = PosDtctDevice_Init();
    
    return &gbgTr_mb;
}

/** @brief  刷子扫一圈
 *  @note   
 *  @author 江榕煜（2021.3.11）
 *  @param  None
 *  @retval None
**/
void brushScanOnce(void)
{
    steerEngine_SetSpeed(0,SlowInver);
    rt_thread_mdelay(1000);
    
    if(__HAL_GPIO_EXTI_GET_IT(SCANO_EXTI_Pin) != 0x00U)
        __HAL_GPIO_EXTI_CLEAR_IT(SCANO_EXTI_Pin);
    
    HAL_NVIC_EnableIRQ(SCANO_EXTI_EXTI_IRQn);
}

