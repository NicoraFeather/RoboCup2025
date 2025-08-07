//
// Created by lak19 on 2025/8/5.
//

#include "callback.h"

#include "can.h"
#include "tim.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
extern uint8_t NUC_Uart_Rx_Buff[128]; //接收串口2的数据缓存数组

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //定时器回调函数，用于计算速度
{
    if (htim->Instance == GAP_TIM.Instance) //1ms间隔定时器中断，计算速度、调整速度、发送参数
    {

    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART2)
    {
        HAL_UART_Transmit(&huart2, NUC_Uart_Rx_Buff, Size, HAL_MAX_DELAY); //回显接收到的命令
        USART_Parse_Command(NUC_Uart_Rx_Buff);

        HAL_UARTEx_ReceiveToIdle_DMA(huart, NUC_Uart_Rx_Buff, 128);
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    }
}

// 定义为全局变量方便调用
/*定义CAN接收头信息*/
CAN_RxHeaderTypeDef rxHeader;
/*
  uint32_t StdId;   // 标准标识符
  uint32_t ExtId;   // 扩展标识符
  uint32_t IDE;     // 指示使用标准标识符还是扩展标识符
  uint32_t RTR;     // 远程传输请求位
  uint32_t DLC;     // 数据长度码
  uint32_t FMI;     // 错误标志标识符
  uint32_t Timestamp; // 时间戳
*/
/*定义接收数据缓冲区*/
uint8_t rxData[8];
float actul_pos_l, actul_speed_l, actul_cur_l;
float actul_pos_r, actul_speed_r, actul_cur_r;
// 接收中断回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan->Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rxHeader, rxData);
        if (rxHeader.StdId == 0x201) // 1号电调反馈报文id
        {
            // 大疆C610通信协议举例
            actul_pos_l = (rxData[0] << 8) | rxData[1];
            actul_speed_l = (int16_t)(rxData[2] << 8) | rxData[3];
            actul_cur_l = (rxData[4] << 8) | rxData[5];
        }

        else if (rxHeader.StdId == 0x202)
        {
            actul_pos_r = (rxData[0] << 8) | rxData[1];
            actul_speed_r = (int16_t)(rxData[2] << 8) | rxData[3];
            actul_cur_r = (rxData[4] << 8) | rxData[5];
        }
    }
}