#include <xc.h> // library xc.h inclut tous les uC
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "utilities.h"
#include "main.h"
#include "Asservissement.h"
#include "QEI.h"

#define PWMPER 40.0
float acceleration = 15;

#define sens -1

void InitPWM(void)
{
PTCON2bits.PCLKDIV = 0b000; //Divide by 1
PTPER = 100*PWMPER; //P�riode en pourcentage

//R�glage PWM moteur 1 sur hacheur 1
IOCON1bits.POLH = 1; //High = 1 and active on low =0
IOCON1bits.POLL = 1; //High = 1
IOCON1bits.PMOD = 0b01; //Set PWM Mode to Redundant
FCLCON1 = 0x0003; //D�sactive la gestion des faults

//Reglage PWM moteur 2 sur hacheur 6
IOCON6bits.POLH = 1; //High = 1
IOCON6bits.POLL = 1; //High = 1
IOCON6bits.PMOD = 0b01; //Set PWM Mode to Redundant
FCLCON6 = 0x0003; //D�sactive la gestion des faults

/* Enable PWM Module */
PTCONbits.PTEN = 1;
}

//void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur)
//{
//robotState.vitesseGaucheCommandeCourante = vitesseEnPourcents;
//if(numeroMoteur==MOTEUR_DROIT)
//{
//    if(vitesseEnPourcents>0) //sens direct
//    {
//        MOTEUR_DROIT_L_PWM_ENABLE = 1; //Pilotage de la pin IN1 en mode PWM
//        MOTEUR_DROIT_L_IO_OUTPUT = 0; //Mise � 0 de la pin IN1
//        MOTEUR_DROIT_H_PWM_ENABLE = 0; //D�sactivation du PWM de la pin IN2 
//        MOTEUR_DROIT_H_IO_OUTPUT = 1; //Mise � 1 de la pin IN2
//
//    }
//    else // sens inverse
//    {
//        MOTEUR_DROIT_L_PWM_ENABLE = 0; //D�sactivation du PWM de la pin IN1
//        MOTEUR_DROIT_L_IO_OUTPUT = 1; //Mise � 1 de la pin IN1
//        MOTEUR_DROIT_H_PWM_ENABLE = 1; //Pilotage de la pin IN2 en mode PWM
//        MOTEUR_DROIT_H_IO_OUTPUT = 0; //Mise � 0 de la pin IN2
//    }  
//    MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);
//}
//else if(numeroMoteur==MOTEUR_GAUCHE)
//{
//    if(vitesseEnPourcents>0) //sens direct
//    {
//        MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //D�sactivation du PWM de la pin IN1
//        MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise � 1 de la pin IN1
//        MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin IN2 en mode PWM
//        MOTEUR_GAUCHE_H_IO_OUTPUT = 0; //Mise � 0 de la pin IN2
//
//    }
//    else // sens inverse
//    {
//        MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin IN1 en mode PWM
//        MOTEUR_GAUCHE_L_IO_OUTPUT = 0; //Mise � 0 de la pin IN1
//        MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //D�sactivation du PWM de la pin IN2 
//        MOTEUR_GAUCHE_H_IO_OUTPUT = 1; //Mise � 1 de la pin IN2
//    }  
//    MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante*PWMPER);
//}
//}

void PWMUpdateSpeed()
{
// Cette fonction est appel�e sur timer et permet de suivre des rampes d?acc�l�ration
if (robotState.vitesseDroiteCommandeCourante < robotState.vitesseDroiteConsigne)
    {
        robotState.vitesseDroiteCommandeCourante = Min(robotState.vitesseDroiteCommandeCourante + acceleration, robotState.vitesseDroiteConsigne);
    }

if (robotState.vitesseDroiteCommandeCourante > robotState.vitesseDroiteConsigne)
    {
        robotState.vitesseDroiteCommandeCourante = Max(robotState.vitesseDroiteCommandeCourante - acceleration, robotState.vitesseDroiteConsigne);
    }

if (robotState.vitesseDroiteCommandeCourante > 0)
    {
    MOTEUR_DROIT_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
    MOTEUR_DROIT_L_IO_OUTPUT = 1; //Mise � 1 de la pin
    MOTEUR_DROIT_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
else
    {
    MOTEUR_DROIT_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
    MOTEUR_DROIT_H_IO_OUTPUT = 1; //Mise � 1 de la pin
    MOTEUR_DROIT_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
MOTEUR_DROIT_DUTY_CYCLE = Abs(robotState.vitesseDroiteCommandeCourante)*PWMPER;

if (robotState.vitesseGaucheCommandeCourante < robotState.vitesseGaucheConsigne)
    {
        robotState.vitesseGaucheCommandeCourante = Min( robotState.vitesseGaucheCommandeCourante + acceleration, robotState.vitesseGaucheConsigne);
    }

if (robotState.vitesseGaucheCommandeCourante > robotState.vitesseGaucheConsigne)
    {
        robotState.vitesseGaucheCommandeCourante = Max(robotState.vitesseGaucheCommandeCourante - acceleration, robotState.vitesseGaucheConsigne);
    }

if (robotState.vitesseGaucheCommandeCourante > 0){
    MOTEUR_GAUCHE_L_PWM_ENABLE = 0; //pilotage de la pin en mode IO
    MOTEUR_GAUCHE_L_IO_OUTPUT = 1; //Mise � 1 de la pin
    MOTEUR_GAUCHE_H_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
else
    {
    MOTEUR_GAUCHE_H_PWM_ENABLE = 0; //pilotage de la pin en mode IO
    MOTEUR_GAUCHE_H_IO_OUTPUT = 1; //Mise � 1 de la pin
    MOTEUR_GAUCHE_L_PWM_ENABLE = 1; //Pilotage de la pin en mode PWM
    }
MOTEUR_GAUCHE_DUTY_CYCLE = Abs(robotState.vitesseGaucheCommandeCourante) * PWMPER;
}

void PWMSetSpeedConsigne(float vitesseEnPourcents, unsigned char moteur)
{
    if(moteur==MOTEUR_GAUCHE)
    {
        robotState.vitesseGaucheConsigne =  1 * vitesseEnPourcents;
    }
    if(moteur==MOTEUR_DROIT)
    {
        robotState.vitesseDroiteConsigne = -1 * vitesseEnPourcents;
    }
}

void PWMSetSpeedConsignePolaire() {
    /********************** Correction Angulaire **********************/
    robotState.vitesseAngulairePourcent = robotState.PidTheta.command * COEFF_VITESSE_ANGULAIRE_PERCENT;
    //robotState.vitesseAngulairePourcent = robotState.vitesseAngulaireConsigne * COEFF_VITESSE_ANGULAIRE_PERCENT;

    /********************** Correction Lineaire *****************************/
    robotState.vitesseLineairePourcent = robotState.PidX.command * COEFF_VITESSE_LINEAIRE_PERCENT;
    //robotState.vitesseLineairePourcent = robotState.vitesseLineaireConsigne * COEFF_VITESSE_LINEAIRE_PERCENT;

    /************* G�n�ration des consignes droites et gauches ******************/
    robotState.vitesseDroiteConsigne = -1*(robotState.vitesseLineairePourcent + robotState.vitesseAngulairePourcent * DISTROUES / 2);
    robotState.vitesseDroiteConsigne = LimitToInterval(robotState.vitesseDroiteConsigne, -100, 100);
    robotState.vitesseGaucheConsigne = 1*(robotState.vitesseLineairePourcent - robotState.vitesseAngulairePourcent * DISTROUES / 2);
    robotState.vitesseGaucheConsigne = LimitToInterval(robotState.vitesseGaucheConsigne, -100, 100);
}

void UpdateAsservissemment(){
    robotState.PidX.erreur = robotState.PidX.consigne - robotState.vitesseLineaireFromOdometry;
    //robotState.PidX.erreur = 0;
    robotState.PidTheta.erreur = robotState.PidTheta.consigne - robotState.vitesseAngulaireFromOdometry;
 
    robotState.PidX.command = Correcteur(&robotState.PidX, robotState.PidX.erreur);
    robotState.PidTheta.command = Correcteur(&robotState.PidTheta, robotState.PidTheta.erreur);
}