/* 
 * taskAPI.h - The C head file of the user private API
 * NOTE: This file is based on rt-thread(RTOS)
 *          这个文件仅作为用户操作系统API链接文件
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-09     江榕煜       first version   2373180028@qq.com
**/

#include <rtthread.h>

#define __USER_DEBUG__

/** @brief  线程初始化接口
 *  @note   必须调用该接口才会启动垃圾自动投入
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval 
 *      （rt_mailbox_t）  需要投入的垃圾箱号告知邮箱对象
**/
rt_mailbox_t threadInit_GarbageThrow(void);

/** @brief  线程初始化接口
 *  @note   必须调用该接口才能启动命令伺服服务
 *  @author 江榕煜（2021.3.9）
 *  @param  
 *      GbgTrMb(rt_mailbox_t)   垃圾投入控制器邮箱对象
 *  @retval None
**/
void threadInit_CmdAnalysis(rt_mailbox_t GbgTrMb);

/** @brief  刷子扫一圈
 *  @note   
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval None
**/
void brushScanOnce(void);

/** @brief  满载检测线程初始化
 *  @note   
 *  @author 江榕煜（2021.3.12）
 *  @param  None
 *  @retval None
**/
void threadInit_heightDetect(void);

/** @brief  触发一次测量反馈
 *  @note   使用前注意先初始化
 *  @author 江榕煜（2021.3.12）
 *  @param  None
 *  @retval None
**/
void heightDetect(void);
