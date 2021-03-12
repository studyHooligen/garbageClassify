/* 
 * hostService.h - The C source file of the host cmd analysis thread
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
static struct rt_thread threadCmdAnalys_Source;
static char stack_CmdAnalys[512];

hostCommand recvCache = {0};    // 接收缓存
rt_sem_t recvCmdSem = RT_NULL;  // 系统信号量：接收到数据

rt_mailbox_t garbageThrowControlServer = RT_NULL;

/*---线程内容定义---*/

/** @brief  上位机命令解析线程
 *  @note   该线程作为服务挂载
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval None
**/
static void threadEntry_CmdAnalysis(void* param)
{
    while(1)
    {
        if(rt_sem_take(recvCmdSem,RT_WAITING_FOREVER) == RT_EOK)
        {
            /*--- 获取到命令数据了 ---*/
            if(recvCache.head == 0xa9 && recvCache.tail == 0xa6)
            {
                // 帧格式检查完毕
                switch(recvCache.command)
                {
                    
                    case('g'):
                        rt_mb_send(garbageThrowControlServer,
                                    recvCache.arg_1);
                        
                        #ifdef __USER_DEBUG__
                        rt_kprintf("recv a command: g\n");
                        rt_kprintf("    throw garbage into type %d\n",recvCache.arg_1);
                        #endif
                    
                        break;
                    
                    case('f'):
                        
                        // 触发高度检测
                        heightDetect();
                    
                        #ifdef __USER_DEBUG__
                        rt_kprintf("recv a command: f\n");
                        #endif
                        break;
                    
                    default:
                        rt_kprintf("get a useless cmd\n");
                        break;
                }
            }
        }
        
    }
}    

/*---用户接口---*/

/** @brief  线程初始化接口
 *  @note   必须调用该接口才能启动命令伺服服务
 *  @author 江榕煜（2021.3.9）
 *  @param  
 *      GbgTrMb(struct rt_mailbox  *)   垃圾投入控制器邮箱对象
 *  @retval None
**/
void threadInit_CmdAnalysis(rt_mailbox_t GbgTrMb)
{
    // 初始化硬件通信接口（BSP：uart4Host.c）
    recvCmdSem = uart4HostInit(&recvCache);
    
    // 启动解析服务
    if( rt_thread_init(&threadCmdAnalys_Source,
                    "cmd analys",
                    threadEntry_CmdAnalysis,
                    RT_NULL,
                    &stack_CmdAnalys[0],
                    sizeof(stack_CmdAnalys),
                    0,0) != RT_EOK)
    {
        rt_kprintf("init cmd analys thread fail!\n");
        return;
    }
    
    garbageThrowControlServer = GbgTrMb;
                    
    // 启动线程
    rt_thread_startup(&threadCmdAnalys_Source);
    rt_kprintf("init cmd analys thread success!\n");
                    
    return;
}
