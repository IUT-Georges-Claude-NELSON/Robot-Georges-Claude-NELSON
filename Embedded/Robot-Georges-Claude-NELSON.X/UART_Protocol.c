#include <xc.h>
#include "UART_Protocol.h"
#include "IO.h"
#include "UART.h"
#include "CB_RX1.h"
#include "CB_TX1.h"
#include "QEI.h"
#include "Asservissement.h"
#include "robot.h"
#include "utilities.h"

int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;
int msgDecodedFunction = 0;

double Kp, Ki, Kd, KpMax, KiMax, KdMax, KpT, KiT, KdT, KpMaxT, KiMaxT, KdMaxT;

typedef enum {
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
} StateReception;

StateReception rcvState = Waiting;

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entrée la trame et sa longueur pour calculer le checksum
    unsigned char Checksum;
    Checksum = 0xFE;
    Checksum ^= (msgFunction >> 8);
    Checksum ^= (msgFunction >> 0);
    Checksum ^= (msgPayloadLength >> 8);
    Checksum ^= (msgPayloadLength >> 0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        Checksum ^= msgPayload[i];
    }
    return Checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction d?encodage et d?envoi d?un message
    unsigned char msg[6 + msgPayloadLength];
    int pos = 0;
    msg[pos++] = 0xFE;
    msg[pos++] = (msgFunction >> 8);
    msg[pos++] = (msgFunction >> 0);
    msg[pos++] = (msgPayloadLength >> 8);
    msg[pos++] = (msgPayloadLength >> 0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        msg[pos++] = msgPayload[i];
    }
    msg[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    SendMessage(msg, 6 + msgPayloadLength);


}

void UartDecodeMessage(unsigned char c) {

    unsigned char calculatedChecksum, receivedChecksum;
    switch (rcvState) {
        case Waiting:
            if (c == 0xFE) {
                rcvState = FunctionMSB;
                msgDecodedFunction = 0;
            }
            break;

        case FunctionMSB:
            msgDecodedFunction = c << 8;
            rcvState = FunctionLSB;
            break;

        case FunctionLSB:
            msgDecodedFunction += c << 0;
            rcvState = PayloadLengthMSB;
            break;

        case PayloadLengthMSB:
            msgDecodedPayloadLength = c << 8;
            rcvState = PayloadLengthLSB;
            break;

        case PayloadLengthLSB:
            msgDecodedPayloadLength += c << 0;
            if (msgDecodedPayloadLength == 0)
                rcvState = CheckSum;
            else if (msgDecodedPayloadLength >= 1024)
                rcvState = Waiting;
            else {
                rcvState = Payload;
                msgDecodedPayloadIndex = 0;
            }
            break;

        case Payload:
            msgDecodedPayload[msgDecodedPayloadIndex++] = c;
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                rcvState = CheckSum;
            break;

        case CheckSum:
            calculatedChecksum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            receivedChecksum = c;
            if (calculatedChecksum == receivedChecksum) {
                //Success, on a un message valide
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            } else {

            }
            rcvState = Waiting;

            break;
            
        default:
            rcvState = Waiting;
            break;
    }
}

void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload) {
    //Fonction appelée après le décodage pour exécuter l?action
    //correspondant au message reçu

    int numLed, etatLed = 0;

    switch (function) {

        case TEXTE:
            UartEncodeAndSendMessage(TEXTE, payloadLength, payload);
            break;

        case LED:
            numLed = payload[0];
            etatLed = payload[1];

            if (numLed == 0) {
                LED_ORANGE = etatLed;
            } else if (numLed == 1) {
                LED_BLEUE = etatLed;
            } else if (numLed == 2) {
                LED_BLANCHE = etatLed;
            }
            break;

        case SET_ROBOT_STATE:
            SetRobotState(payload[0]);
            break;

        case SET_ROBOT_MANUAL_CONTROL:
            SetRobotAutoControlState(payload[0]);
            break;
            
          
        case SET_PID:
            Kp = getFloat(payload, 0);
            Ki = getFloat(payload, 4);
            Kd = getFloat(payload, 8);
            KpMax = getFloat(payload, 12);
            KiMax = getFloat(payload, 16);
            KdMax = getFloat(payload, 20);
            
            setupPidAsservissement(&robotState.PidX, Kp, Ki, Kd, KpMax, KiMax, KdMax);
            
            KpT = getFloat(payload, 24);
            KiT = getFloat(payload, 28);
            KdT = getFloat(payload, 32);
            KpMaxT = getFloat(payload, 36);
            KiMaxT = getFloat(payload, 40);
            KdMaxT = getFloat(payload, 44);
            setupPidAsservissement(&robotState.PidTheta, KpT, KiT, KdT, KpMaxT, KiMaxT, KdMaxT);
            
        default:
            break;

    }
}
//
//SetRobotState(unsigned char c){
//    
//}


//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/

