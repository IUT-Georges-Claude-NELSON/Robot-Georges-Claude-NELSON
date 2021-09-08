/* 
 * File:   UART_Protocol.h
 * Author: GEII Robot
 *
 * Created on 7 septembre 2021, 15:37
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartDecodeMessage(unsigned char c);

void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_PROTOCOL_H */

