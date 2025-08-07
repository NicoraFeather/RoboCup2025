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
            //HAL_UART_Transmit(&huart2, "regionB", 9, HAL_MAX_DELAY);
        }
        else if (data_array[1] == 1 && data_array[2] == 1)
            region = 'C';
        //遍历之后的12位，判断各个点位的干旱情况并且发送消息
        for (int i = 3; i < 15; i += 2)
        {
            uint8_t num = i / 2;
            uint8_t state = 0;

            if (data_array[i] == 0 && data_array[i + 1] == 0)
            {
                state = 1; // 轻微干旱
                Src_Change_Num(region, num, state);
            }
            else if (data_array[i] == 0 && data_array[i + 1] == 1)
            {
                state = 2; // 一般干旱
                Src_Change_Num(region, num, state);
            }
            else if (data_array[i] == 1 && data_array[i + 1] == 1)
            {
                state = 3; // 严重干旱
                Src_Change_Num(region, num, state);
            }
        }
    }

    else
    {

    }
}

//data 00001010 10101010 00000000 00000000
//data 00000000 00000000 01010101 01010000
// 0x00005550
//message 2B 00 00 55 50 8D E3 27 F9 2A

//data 00101010 10101010 00000000 00000000
//data 00000000 00000000 01010101 01010100
// 00005554
//  2B 00 00 55 54 8A 8E E3 E0 2A

//设计B为1100
//DATA 00101010 00000000 00000000 00000000
//data 00000000 00000000 00000000 01010100
//0x 00 00 00 54
//发送B的测试指令 2B 00 00 00 54 4D 42 4A F1 2A
//发送A的测试指令 2B 00 00 55 50 8D E3 27 F9 2A

/***********首先测试区域性质***********/
//我们规定00是A，01是B，11是C
//初始化的时候是00，说明默认为A


//0x2B, 0x00, 0x00, 0x55, 0x50, 0x8D, 0xE3, 0x27, 0xF9, 0x2A,// 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00
// A 区 123123
//data 00000011 10001110 00000000 00000000
//00000000 00000000 01110001 11000000
//2B 00 00 71 C0
//2B C0 71 00 00 53 4D 05 69 2A

//实际发送的应当是 2B C0 71 00 00 69 05 4D 53 2A
