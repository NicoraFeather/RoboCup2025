//
// Created by lak19 on 2025/8/5.
//

#include "../Inc/uart_command.h"

#include <stdio.h>
#include <string.h>

#include "usart.h"

uint8_t src_complete_flag = 0;
uint8_t irrigation_complete_flag = 0;

int crc32_check(uint8_t *frame, size_t data_len)
{
    // 取出帧尾前的 4 字节 CRC（小端序）
    uint32_t recv_crc = ((uint32_t)frame[data_len + 1] <<  0) |
                        ((uint32_t)frame[data_len + 2] <<  8) |
                        ((uint32_t)frame[data_len + 3] << 16) |
                        ((uint32_t)frame[data_len + 4] << 24);

    uint32_t calc_crc = crc32(frame, data_len + 1); // 计算 CRC，包含帧头和数据部分
    uint8_t message[32] = {0};
    sprintf(message, "%d", calc_crc);
    HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);
    if (calc_crc == recv_crc)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void USART_Parse_Command(uint8_t * command)
{
    if (command[0] == '+')
    {
        if (command[9] == '*')
        {
            //HAL_UART_Transmit(&huart2, "Test_OK", 9, HAL_MAX_DELAY);
            uint8_t Bool = crc32_check(command, 4);
            if (Bool == 1)
            {
                HAL_UART_Transmit(&huart2, "Test_OK", 9, HAL_MAX_DELAY);
                //对取出的cmd进行小端序重排，得到一般的uint32_t 类型数据
                uint32_t data = ((uint32_t)command[1 + 0] <<  0) |
                        ((uint32_t)command[1 + 1] <<  8) |
                        ((uint32_t)command[1 + 2] << 16) |
                        ((uint32_t)command[1 + 3] << 24);
                Task_Parse(data);
            }
        }
    }
}

void Task_Parse(uint32_t data)
{
    uint8_t data_array[32] = {0}; // 用于存储解析后的数据
    for (int i = 0; i < 32; ++i) {
        data_array[i] = (data >> i) & 1u;   // 从最低位开始存储
    }

    // for (int i = 0; i < 32; ++i) {
    //     /* 从最高位 (bit31) 开始向右移，然后取最低位 */
    //     data_array[i] = (data >> (31 - i)) & 1u;
    // }

    if (data_array[0] == 0) //执行屏幕显示操作
    {
        //HAL_UART_Transmit(&huart2, data_array, sizeof(data_array), HAL_MAX_DELAY);
        //第1和2位来判断区域类别
        uint8_t region = 'B';
        if (data_array[1] == 0 && data_array[2] == 0)
        {
            region = 'A';
            HAL_UART_Transmit(&huart2, "regionA", 9, HAL_MAX_DELAY);
        }
        else if (data_array[1] == 0 && data_array[2] == 1)
        {
            region = 'B';
            HAL_UART_Transmit(&huart2, "regionB", 9, HAL_MAX_DELAY);
        }
        else if (data_array[1] == 1 && data_array[2] == 1)
        {
            region = 'C';
            HAL_UART_Transmit(&huart2, "regionC", 9, HAL_MAX_DELAY);
        }
        //遍历之后的12位，判断各个点位的干旱情况并且发送消息
        for (int i = 3; i < 15; i += 2)
        {
            uint8_t num = i / 2;
            uint8_t state = 0;

            if (data_array[i] == 0 && data_array[i + 1] == 0)
            {
                state = 1; // 轻微干旱
                Src_Change_Num(region, num, state);
                Src_Change_Color(region, num, state);
            }
            else if (data_array[i] == 0 && data_array[i + 1] == 1)
            {
                state = 2; // 一般干旱
                Src_Change_Num(region, num, state);
                Src_Change_Color(region, num, state);
            }
            else if (data_array[i] == 1 && data_array[i + 1] == 1)
            {
                state = 3; // 严重干旱
                Src_Change_Num(region, num, state);
                Src_Change_Color(region, num, state);
            }
            else if (data_array[i] == 1 && data_array[i + 1] == 0)
            {
                state = 0; // 无干旱
                Src_Change_Num(region, num, state);
            }
        }
    }

    else
    {
        uint8_t state = 0;
        uint8_t direction = 0; // 方向0为左，1为右
        //首先判断左右
        if (data_array[1] == 0)
        {
            direction = 0;
        }
        else
        {
            direction = 1;
        }

        //然后判断旱情程度
        if (data_array[2] == 0 && data_array[3] == 0)
        {
            state = 1;
        }
        else if (data_array[2] == 0 && data_array[3] == 1)
        {
            state = 2;
        }
        else if (data_array[2] == 1 && data_array[3] == 1)
        {
            state = 3;
        }
        else if (data_array[2] == 1 && data_array[3] == 0)
        {
            state = 0;
        }
        Speaker_Active(state); //语音播报
        Speaker_Reset();
        /****************
        这里放置C610代码
        ****************/
    }
}

/*

B区data
230120
从低到高
00101111 00001100 00000000 00000000
真正的uint32_t数据
00000000 00000000 00110000 11110100
即0x000030F4
2B 00 00 30 F4 15 A5 E9 FE

小端重排
2B F4 30 00 00
CRC 5D5C8D68
再次小端重排
2B F4 30 00 00 68 8D 5C 5D 2A


A区data
00000011 10001110 00000000 00000000
真正的uint32_t数据
00000000 00000000 01110001 11000000
即0x000071C0
小端重排
2B C0 71 00 00
CRC 534D0569

再次小端重排
2B C0 71 00 00 69 05 4D 53 2A

C区data
333111
01111111 10000000 00000000 00000000
真正的uint32_t数据
00000000 00000000 00000001 11111110
即0x000001FE
小端重排
2B FE 01 00 00
CRC 1748E2AB
再次小端重排
2B FE 01 00 00 AB E2 48 17 2A
*/


//最终测试数据
//2B C0 71 00 00 69 05 4D 53 2A
//2B F4 30 00 00 68 8D 5C 5D 2A
//2B FE 01 00 00 AB E2 48 17 2A


/*下面是语音播报和C610的测试推导

11010000 00000000 00000000 00000000
真正的uint32_t数据
00000000 00000000 00000000 00001011
即0x0000000B
小端重排
2B 0B 00 00 00
CRC A7 32 6E 09
再次小端重排
2B 0B 00 00 00 09 6E 32 A7 2A
*/