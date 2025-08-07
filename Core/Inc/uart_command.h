//
// Created by lak19 on 2025/8/5.
//

#ifndef UART_COMMAND_H
#define UART_COMMAND_H

#include "main.h"
void USART_Parse_Command(uint8_t * command);
int crc32_check(uint8_t *frame, size_t data_len);
void Task_Parse(uint32_t data);
#endif //UART_COMMAND_H
