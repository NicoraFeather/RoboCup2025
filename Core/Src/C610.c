//
// Created by lak19 on 2025/8/7.
//

#include "C610.h"

/**
 * 控制两个电机的电流
 * @param Current_l 左电机电流
 * @param Current_r 右电机电流
 * @note Proc类型函数，建议1ms内调用一次
 */
void C610_Control(int Current_l, int Current_r)
{
    uint8_t txData[8] ={0};
    //进行数据处理
    txData[0] = Current_l >> 8;   // 赋值高八位
    txData[1] = Current_l & 0xff; // 赋值低八位
    txData[2] = Current_r >> 8;   // 赋值高八位
    txData[3] = Current_r & 0xff; // 赋值低八位
    CAN_SendMessage(&hcan1, 0x200, txData, 8);
}

