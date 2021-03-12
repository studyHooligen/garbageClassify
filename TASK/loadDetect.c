/* 
 * loadDetect.h - The C source file of the detecting trash hight thread
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

#include "uart4Host.h"
#include "taskAPI.h"

/*---内存对象分配静态存储空间---*/
static struct rt_thread threadheightDetect_Source;
static char stack_heightDetect[512];

localMsg frameCache ={0};

/** @brief  单次垃圾桶高度检测线程
 *  @note   触发一次，检测一次，并自动发送结果
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval None
**/
static void threadEntry_heightDetect(void * param)
{
    while(1)
    {
        /* 挂起线程，等待恢复 */
        rt_thread_suspend(rt_thread_self());
        rt_schedule();
        
        /*---执行一次高度检测---*/
        steerEngine_SetPos(1,Pos1);
        rt_thread_mdelay(4000);
        frameCache.data[0] = HAL_GPIO_ReadPin(detector_GPI_GPIO_Port,detector_GPI_Pin);
       
        steerEngine_SetPos(1,Pos2);
        rt_thread_mdelay(4000);
        frameCache.data[1] = HAL_GPIO_ReadPin(detector_GPI_GPIO_Port,detector_GPI_Pin);
        
        steerEngine_SetPos(1,Pos3);
        rt_thread_mdelay(4000);
        frameCache.data[2] = HAL_GPIO_ReadPin(detector_GPI_GPIO_Port,detector_GPI_Pin);
        
        steerEngine_SetPos(1,Pos4);
        rt_thread_mdelay(4000);
        frameCache.data[3] = HAL_GPIO_ReadPin(detector_GPI_GPIO_Port,detector_GPI_Pin);
        
        /*---发送检测数据---*/
        frameCache.head = 0xa9;
        frameCache.tail = 0xa6;
        frameCache.status = 'f';
        
        uart4Host_send(&frameCache);
    }
}

/*---用户接口---*/

/** @brief  线程初始化
 *  @note   
 *  @author 江榕煜（2021.3.12）
 *  @param  None
 *  @retval None
**/
void threadInit_heightDetect(void)
{
    // 启动测量线程
    if( rt_thread_init(&threadheightDetect_Source,
                    "height detect",
                    threadEntry_heightDetect,
                    RT_NULL,
                    &stack_heightDetect[0],
                    sizeof(stack_heightDetect),
                    0,0) != RT_EOK)
    {
        rt_kprintf("init height detect thread fail!\n");
        return;
    }
    
    rt_thread_startup(&threadheightDetect_Source);
    rt_kprintf("init height detect thread success!\n");
    
    return;
}

/** @brief  触发一次测量反馈
 *  @note   使用前注意先初始化
 *  @author 江榕煜（2021.3.12）
 *  @param  None
 *  @retval None
**/
void heightDetect(void)
{
    rt_thread_resume(&threadheightDetect_Source);
}

