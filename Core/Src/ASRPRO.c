//
// Created by lak19 on 2025/8/8.
//

#include "../Inc/ASRPRO.h"

//报文规定
// 000 无干旱，不播报
// 111 识别成功
// 001 轻微干旱，播报“轻微干旱”
// 010 一般干旱，播报“一般干旱”
// 011 严重干旱，播报“严重干旱”


void Speaker_Reset(void)
{
    HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_RESET);
}

void Speaker_Active(uint8_t state)
{
    if (state == 0) //无干旱
    {
        HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_SET);
    }
    else if (state == 1) //轻微干旱
    {
        HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_RESET);
    }
    else if (state == 2) //一般干旱
    {
        HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_RESET);
    }
    else if (state == 3) //严重干旱
    {
        HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_SET);
    }
    else if (state == 4) //识别成功
    {
        HAL_GPIO_WritePin(Speak_0_GPIO_Port, Speak_0_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Speak_1_GPIO_Port, Speak_1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Speak_2_GPIO_Port, Speak_2_Pin, GPIO_PIN_SET);
    }
}


