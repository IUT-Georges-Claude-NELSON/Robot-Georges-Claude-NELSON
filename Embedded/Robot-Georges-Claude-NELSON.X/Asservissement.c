#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "UART_Protocol.h"
#include "Robot.h"
#include "QEI.h"
#include "Asservissement.h"
#include "utilities.h"
#include "PWM.h"

unsigned char asservissementPayload[112];

////Simulation valeur asserve
//double consigneX = 0.01;
//double consigneTheta = 0.02;
//double mesureX = 0.03;
//double mesureTheta = 0.04;
//double errorX = 0.05;
//double errorTheta = 0.06;
//double commandX = 0.07;
//double commandTheta = 0.08;
////-------------------
//double corrPX = 0.09;
//double corrPTheta = 0.10;
//double corrIX = 0.11;
//double corrITheta = 0.12;
//double corrDX = 0.13;
//double corrDTheta = 0.14;
////-------------------
//double KpX = 0.15;
//double KpTheta = 0.16;
//double KiX = 0.17;
//double KiTheta = 0.18;
//double KdX = 0.19;
//double KdTheta = 0.20;
////-------------------
//double corrMaxPX = 0.21;
//double corrMaxPTheta = 0.22;
//double corrMaxIX = 0.23;
//double corrMaxITheta = 0.24;
//double corrMaxDX = 0.25;
//double corrMaxDTheta = 0.26;

void setupPidAsservissement(volatile PidCorrector* PidCorr, double Kp, double Ki, double Kd, double proportionnelleMax, double integraleMax, double deriveeMax)
{
    PidCorr->Kp=Kp;
    PidCorr->erreurProportionnelleMax = proportionnelleMax;
    PidCorr->Ki=Ki;
    PidCorr->erreurIntegraleMax = integraleMax;
    PidCorr->Kd=Kd;
    PidCorr->erreurDeriveeMax=deriveeMax;
}

double Correcteur(volatile PidCorrector* PidCorr, double erreur)
{
    PidCorr->erreur = erreur;
    double erreurProportionnelle = LimitToInterval(erreur,-PidCorr->erreurProportionnelleMax / PidCorr->Kp,PidCorr->erreurProportionnelleMax / PidCorr->Kp);
    PidCorr->corrP = erreurProportionnelle * PidCorr->Kp ;
    
    PidCorr->erreurIntegrale += erreur / FREQ_ECH_QEI;
    PidCorr->erreurIntegrale = LimitToInterval(PidCorr->erreurIntegrale,-PidCorr->erreurIntegraleMax / PidCorr->Ki, PidCorr->erreurIntegraleMax / PidCorr ->Ki);
    PidCorr->corrI = PidCorr->erreurIntegrale * PidCorr->Ki;
    
    double erreurDerivee = (erreur-PidCorr->epsilon_1)*FREQ_ECH_QEI;
    double deriveeBornee = LimitToInterval(erreurDerivee, -PidCorr->erreurDeriveeMax/PidCorr->Kd, PidCorr->erreurDeriveeMax/PidCorr->Kd);
    PidCorr -> epsilon_1 = erreur;
    PidCorr -> corrD =  PidCorr -> Kd * deriveeBornee ;
    
    return (PidCorr -> corrP + PidCorr->corrI + PidCorr -> corrD);
}

void asservissemtentValeur() {
    int nb_octet = 0;
    getBytesFromFloat(asservissementPayload, nb_octet, (float)robotState.PidX.consigne);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.consigne);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.vitesseLineaireFromOdometry);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.vitesseAngulaireFromOdometry);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.erreur);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.erreur);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.command);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.command);
    //-------------------   
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.corrP);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.corrP);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.corrI);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.corrI);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.corrD);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.corrD);
    //-------------------
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.Kp);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.Kp);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.Ki);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.Ki);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.Kd);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.Kd);
    //-------------------
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.erreurProportionnelleMax);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.erreurProportionnelleMax);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.erreurIntegraleMax);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.erreurIntegraleMax);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidX.erreurDeriveeMax);
    getBytesFromFloat(asservissementPayload, nb_octet += 4, (float)robotState.PidTheta.erreurDeriveeMax);
    //-------------------
    getBytesFromFloat(asservissementPayload, nb_octet += 4, robotState.vitesseDroitFromOdometry );
    getBytesFromFloat(asservissementPayload, nb_octet += 4, robotState.vitesseGaucheFromOdometry);

    UartEncodeAndSendMessage(DONNEES_ASSERV, nb_octet += 4, asservissementPayload);
}