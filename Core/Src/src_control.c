//
// Created by lak19 on 2025/8/4.
//

#include "src_control.h"

#include <stdio.h>
#include <string.h>

#include "usart.h"

static const uint8_t end_command[3] = {0xFF, 0xFF, 0xFF};
void Src_Send_End()
{
    HAL_UART_Transmit(&huart1,end_command,strlen(end_command),HAL_MAX_DELAY);
}

/**
 *
 * @param region 改变的区域 'A'or'B'or'C'
 * @param num 改变的序号
 * @param state 干旱状态 1-轻微干旱 2-一般干旱 3-严重干旱 0-无干旱
 */
void Src_Change_Num(uint8_t region, uint8_t num, uint8_t state)
{
    uint8_t Change_Command[50] = {0};
    if (region == 'A')
    {
        uint8_t temp_num = num - 1;
        sprintf(Change_Command, "n%d.pco=BLACK\xFF\xFF\xFF", temp_num);
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);

        sprintf(Change_Command, "n%d.val=%d\xFF\xFF\xFF", temp_num, state);
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
    }
    else if (region == 'B')
    {
        uint8_t temp_num = num + 5;
        if (state == 0)
        {
            sprintf(Change_Command, "n%d.pco=WHITE\xFF\xFF\xFF", temp_num);
            HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
        }
        else
        {
            sprintf(Change_Command, "n%d.pco=BLACK\xFF\xFF\xFF", temp_num);
            HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);

            sprintf(Change_Command, "n%d.val=%d\xFF\xFF\xFF", temp_num, state);
            HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
        }

    }
    else if (region == 'C')
    {
        uint8_t temp_num = num + 11;
        sprintf(Change_Command, "n%d.pco=BLACK\xFF\xFF\xFF", temp_num);
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);

        sprintf(Change_Command, "n%d.val=%d\xFF\xFF\xFF", temp_num, state);
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
    }
}

/**
 * 改变颜色
 * @param region 改变的区域 'A'or'B'or'C'
 * @param num 改变的序号
 * @param state 干旱状态 1-绿色 2-蓝色 3-红色 0-白色
 */
void Src_Change_Color(uint8_t region, uint8_t num, uint8_t state)
{
    uint8_t Change_Command[50] = {0};
    if (region == 'A')
    {
        uint8_t temp_num = num - 1;
        if (state == 1)
        {
            sprintf(Change_Command, "n%d.bco=GREEN\xFF\xFF\xFF", temp_num);
        }
        else if (state == 2)
        {
            sprintf(Change_Command, "n%d.bco=BLUE\xFF\xFF\xFF", temp_num);
        }
        else if (state == 3)
        {
            sprintf(Change_Command, "n%d.bco=RED\xFF\xFF\xFF", temp_num);
        }
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
    }
    else if (region == 'B')
    {
        uint8_t temp_num = num + 5;
        if (state == 1)
        {
            sprintf(Change_Command, "n%d.bco=GREEN\xFF\xFF\xFF", temp_num);
        }
        else if (state == 2)
        {
            sprintf(Change_Command, "n%d.bco=BLUE\xFF\xFF\xFF", temp_num);
        }
        else if (state == 3)
        {
            sprintf(Change_Command, "n%d.bco=RED\xFF\xFF\xFF", temp_num);
        }
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
    }
    else if (region == 'C')
    {
        uint8_t temp_num = num + 11;
        if (state == 1)
        {
            sprintf(Change_Command, "n%d.bco=GREEN\xFF\xFF\xFF", temp_num);
        }
        else if (state == 2)
        {
            sprintf(Change_Command, "n%d.bco=BLUE\xFF\xFF\xFF", temp_num);
        }
        else if (state == 3)
        {
            sprintf(Change_Command, "n%d.bco=RED\xFF\xFF\xFF", temp_num);
        }
        HAL_UART_Transmit(&huart1, Change_Command, strlen(Change_Command), HAL_MAX_DELAY);
    }
}

