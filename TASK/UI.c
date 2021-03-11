/* 
 * UI.c - The C source file of the User Interface
 * NOTE: This file is based on HAL library of stm32 platform
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-07     江榕煜       first version   2373180028@qq.com
**/

#include "taskAPI.h"
#include "BSP.h"

/** @brief  启动PWM输出接口（控制台命令）
 *  @note   在不输入参数的情况下，默认启动所有
 *  @author 江榕煜（2021.3.7）
 *  @param
 *      channel(unsigned int)   可选参数~指定输出PWM通道号
 *  @retval 无
**/
void beginPWM(int argn,char** argv)
{
    if(argn == 1)
    {
        // 直接启动所有PWM输出
        steerEngine_Init();
        rt_kprintf("begin all pwm!\n");
    }
    else{
        int channel;
        sscanf(argv[1],"%d",&channel);
        // TODO : 启动指定PWM
    }
}

MSH_CMD_EXPORT(beginPWM, begin PWM);


/** @brief  调整舵机（控制台命令）
 *  @note   五个模式
 *  @author 江榕煜（2021.3.7）
 *  @param
 *      channel(unsigned int)   PWM通道号
 *      mode(int)               运行模式
 *  @retval 无
**/
void setEngineSpeed(int argn,char** argv)
{
    int channel,mode;
    sscanf(argv[1],"%d",&channel);
    sscanf(argv[2],"%d",&mode);
    
    switch(mode)
    {
        case(2):
            steerEngine_SetSpeed(channel,FullAhead);
            break;
        case(1):
            steerEngine_SetSpeed(channel,SlowAhead);
            break;
        case(0):
            steerEngine_SetSpeed(channel,Stop);
            break;
        case(-1):
            steerEngine_SetSpeed(channel,SlowInver);
            break;
        case(-2):
            steerEngine_SetSpeed(channel,FullInver);
            break;
    }
    rt_kprintf("set SteerEngine Success!");
}
MSH_CMD_EXPORT(setEngineSpeed, set speed of SteerEngine);

/** @brief  调整舵机（控制台命令）
 *  @note   4个位置
 *  @author 江榕煜（2021.3.7）
 *  @param
 *      channel(unsigned int)   PWM通道号
 *      position(int)           位置
 *  @retval 无
**/
void setEnginePos(int argn,char** argv)
{
    int channel,mode;
    sscanf(argv[1],"%d",&channel);
    sscanf(argv[2],"%d",&mode);
    
    switch(mode)
    {
        case(1):
            steerEngine_SetPos(channel,Pos1);
            break;
        case(2):
            steerEngine_SetPos(channel,Pos2);
            break;
        case(3):
            steerEngine_SetPos(channel,Pos3);
            break;
        case(4):
            steerEngine_SetPos(channel,Pos4);
            break;
    }
    rt_kprintf("set SteerEngine Success!");
}
MSH_CMD_EXPORT(setEnginePos, set position of SteerEngine);

/** @brief  扫把 扫一圈
 *  @note   
 *  @author 江榕煜（2021.3.10）
 *  @param  无
 *  @retval 无
**/
void brushScan(int argn,char** argv)
{
    brushScanOnce();
}
MSH_CMD_EXPORT(brushScan,let brush scan once);
