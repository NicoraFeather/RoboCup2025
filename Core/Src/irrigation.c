//
// Created by lak19 on 2025/7/29.
//

#include "irrigation.h"

_Pump_Derection_e Pump_Derection_e = PUMP_LEFT; //水泵方向

void Irrigation_Init(void)
{
    HAL_GPIO_TogglePin(Relay_L_GPIO_Port,Relay_L_Pin);
    HAL_GPIO_TogglePin(Relay_R_GPIO_Port,Relay_R_Pin);
    HAL_Delay(TIME_PUMP_INIT); // 等待水泵初始化,保证水泵充满水
    HAL_GPIO_TogglePin(Relay_L_GPIO_Port,Relay_L_Pin);
    HAL_GPIO_TogglePin(Relay_R_GPIO_Port,Relay_R_Pin);
}

void Pump_Task(_Pump_Derection_e Pump_Derection_e)
{
    if (Pump_Derection_e==PUMP_LEFT)
    {
        HAL_GPIO_TogglePin(Relay_L_GPIO_Port,Relay_L_Pin);
        HAL_Delay(TIME_PUMP_TASK);
        HAL_GPIO_TogglePin(Relay_L_GPIO_Port,Relay_L_Pin);
    }
    else
    {
        HAL_GPIO_TogglePin(Relay_R_GPIO_Port,Relay_R_Pin);
        HAL_Delay(TIME_PUMP_TASK);
        HAL_GPIO_TogglePin(Relay_R_GPIO_Port,Relay_R_Pin);
    }
}

void TelescopingPole_Task(void)
{

}