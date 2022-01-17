/* 
 * File:   UART_Protocol.h
 * Author: GEII Robot
 *
 * Created on 7 septembre 2021, 15:37
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H

#define LED 0x0020
#define VITESSE 0x0040
#define ETAPE 0x0050
#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_MANUAL_CONTROL 0x0052
#define POSITION_DATA 0x0061
#define TEXTE 0x0080
#define DONNEES_ASSERV 0x0028
#define DONNEES_TELEMETRE 0x0030
#define SET_PID 0x0013

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);

void UartDecodeMessage(unsigned char c);

void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload);

void SetRobotState(unsigned char);

void SetRobotAutoControlState(unsigned char);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_PROTOCOL_H */

