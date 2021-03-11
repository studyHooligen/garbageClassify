/* 
 * PWMdevice.c - The C source file of PWM devices
 * NOTE: This file is based on HAL library of stm32 platform
 *          PWM设备BSP接口实现定义
 *          接口声明功能注释见BSP.h
 *
 * Copyright (c) 2020-, garbage classify Project
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes           mail
 * 2021-03-04     江榕煜       first version   2373180028@qq.com
**/

#include "BSP.h"

#define _STEER_ENGINE_CHANNEL_ 3

const TIM_HandleTypeDef* steerEngine_PWMHandler[_STEER_ENGINE_CHANNEL_] = {
    &htim17, &htim16, &htim2
};

// 舵机PWM计数器周期
const uint32_t steerEngine_PWMCounter[_STEER_ENGINE_CHANNEL_] = {
    60000,  60000,  100000
};

// 不同模式舵机占空比，适用通道：0
//  360度，模式控制速度
const float steerEngine_ModeDuty[5] = {
    0.0375, 0.0625, 0.075,  0.0875, 0.1125
};

// 不同位置舵机占空比，适用通道：1
//  SPT5535LV-320转角：300%对应PWM占空比500~2500us
const float steerEngine_PosDuty[7] = {
    0.025,  //  最小占空比：-150°
    0.03,   //  -135°
    0.06,   //  -45°
    0.075,  // 舵机中位：0°
    0.09,   //  +45°
    0.12,   //  +135°
    0.125   //  最大占空比:  +150°
};

void steerEngine_Stop(uint32_t channel)
{
    switch(channel)
    {
        case(0):
            htim17.Instance->CCR1 = 
                steerEngine_ModeDuty[Stop]*steerEngine_PWMCounter[channel];
            break;
        case(1):
            htim16.Instance->CCR1 = 
                steerEngine_ModeDuty[Stop]*steerEngine_PWMCounter[channel];
            break;
        case(2):
            htim2.Instance->CCR2 = 
                steerEngine_ModeDuty[Stop]*steerEngine_PWMCounter[channel];
            break;
        default:
            break;
    }
}

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
void steerEngine_SetSpeed(uint32_t channel,steerEngine_Mode speed)
{
    uint32_t duty = 
        steerEngine_ModeDuty[speed]*steerEngine_PWMCounter[channel];;
    switch(channel)
    {
        case(0):
            htim17.Instance->CCR1 = duty;
            break;
//        case(1):
//            htim16.Instance->CCR1 = duty;
//            break;
        case(2):
            htim2.Instance->CCR2 = duty;
            break;
    }
}

void steerEngine_SetPos(uint32_t channel,steerEngine_Pos position)
{
    uint32_t duty =
        steerEngine_PosDuty[position]*steerEngine_PWMCounter[channel];
    switch(channel)
    {
        case(1):
            htim16.Instance->CCR1 = duty;
            break;
    }
}

void steerEngine_Init(void)
{
    HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
}
