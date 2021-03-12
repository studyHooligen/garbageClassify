/* 
 * BSP.h - The C head file of the whole system BoardSupportPackage
 * NOTE: This file is based on HAL library of stm32 platform
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-04     江榕煜       first version   2373180028@qq.com
**/

#ifndef _BSP_H_
#define _BSP_H_

#include "main.h"
//#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
//#include "spi.h"
#include "dma.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

/***********BSP用户类型声明************/
typedef enum steerEngine_Mode{
    FullAhead = 0,
    SlowAhead = 1,
    Stop      = 2,
    SlowInver = 3,
    FullInver = 4
} steerEngine_Mode;

typedef enum steerEngine_Pos{
    PosC = 3,
    Pos1 = 1,
    Pos2 = 2,
    Pos3 = 4,
    Pos4 = 5
} steerEngine_Pos;

/***********BSP用户接口声明************/

/** @brief  舵机停转
 *  @note   None
 *  @author 江榕煜（2021.3.4）
 *  @param 
 *      channel(uint32_t)   要停止的舵机通道号
 *  @retval
 *      None
**/
void steerEngine_Stop(uint32_t channel);

/** @brief  舵机转动到指定位置
 *  @note   位置参数使用360°制
 *  @author 江榕煜（2021.3.4）
 *  @param
 *      channel(uint32_t)   控制的舵机通道号
 *      position(uint32_t)  舵机转到的位置，单位：°
 *  @retval
 *      None
**/
void steerEngine_SetPos(uint32_t channel,steerEngine_Pos position);

/** @brief  设置舵机速度
 *  @note   适用于360度旋转的速度舵机
 *          目前该接口支持5枚举值模式
 *  @author 江榕煜（2021.3.4）
 *  @param
 *      channel(uint32_t)       通道号
 *      speed(steerEngine_Mode) 速度
 *  @retval
 *      None
**/
void steerEngine_SetSpeed(uint32_t channel,steerEngine_Mode speed);

/***********BSP系统初始化接口声明************/

/** @brief  舵机初始化
 *  @note   实际上初始化PWM外设
 *  @author 江榕煜（2021.3.4）
 *  @param  None
 *  @retval None
**/
void steerEngine_Init(void);

/** @brief  位置检测初始化接口
 *  @note   该接口中开启了中断
 *  @author 江榕煜（2021.3.9）
 *  @param  None
 *  @retval 位置检测邮箱对象
**/
struct rt_mailbox  * PosDtctDevice_Init(void);
    
#endif
