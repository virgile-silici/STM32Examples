/*
 * uart.c
 *
 *  Created on: Mar 21, 2015
 *      Author: Dustin
 *
 * Copyright (c) 2015, Dustin Reynolds
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of [project] nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "stm32l1xx.h"
#include "uart.h"

void USART2_IRQHandler(void)
{
	if( USART_GetITStatus(USART2, USART_IT_RXNE)){
		char t = USART_ReceiveData(USART2);

		//do something.
		USART_SendData(USART2, t);
	}
}

void USART1_IRQHandler(void)
{
	if( USART_GetITStatus(USART1, USART_IT_RXNE)){
		char t = USART_ReceiveData(USART1);

		//do something.
		USART_SendData(USART1, (t+10));
	}
}

void UART5_IRQHandler(void)
{
	if( USART_GetITStatus(UART5, USART_IT_RXNE)){
		char t = USART_ReceiveData(UART5);

		//do something.
		USART_SendData(USART2, t);
	}
}

void uart_NVIC_init(USART_TypeDef * USARTx)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (USARTx == USART1){
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}else if (USARTx == USART2){
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}else if (USARTx == UART5){
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void uart_NVIC_deinit(USART_TypeDef * USARTx)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	if (USARTx == USART1){
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	}else if (USARTx == USART2){
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}else if (USARTx == UART5){
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void uart_switch_mode(USART_TypeDef * USARTx,uart_init_mode_t mode){
	if (mode == UART_INTERRUPT_RX){
		uart_NVIC_init(USARTx);
	}else{
		uart_NVIC_deinit(USARTx);
	}
}

void uart_Configuration(USART_TypeDef * USARTx, uart_init_mode_t mode)
{
	USART_InitTypeDef USART_InitStructure;
	if (mode == UART_INTERRUPT_RX){
		uart_NVIC_init(USARTx);
	}else{
		uart_NVIC_deinit(USARTx);
	}

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USARTx, &USART_InitStructure);

	USART_Cmd(USARTx, ENABLE);

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
}

void uart_OutString(USART_TypeDef * USARTx, char *s)
{
	while(*s)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); // Wait for Empty

		USART_SendData(USARTx, *s++); // Send Char
	}
}


