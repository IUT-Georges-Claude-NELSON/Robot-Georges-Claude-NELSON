/* 
 * File:   main.c
 * Author: GEII Robot
 *
 * Created on 26 janvier 2021, 15:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "main.h"
#include "Toolbox.h"
#include "UART.h"

/*
 * 
 */
int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();
    InitTimer23();
    InitTimer1();
    InitPWM();
    InitADC1();
    InitTimer4();
    InitUART();

    /****************************************************************************************************/
    //Allumage LED
    /****************************************************************************************************/
    //    LED_BLANCHE = 1;
    //    LED_BLEUE = 1;
    //    LED_ORANGE = 1;

    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/
    while (1) {

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [4]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [3]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExGauche = 34 / volts - 5;
            volts = ((float) result [0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExDroit = 34 / volts - 5;
        }
        if (robotState.distanceTelemetreDroit < 30 || robotState.distanceTelemetreGauche < 30) {
            LED_ORANGE = 1;
        }
        else {
            LED_ORANGE = 0;
        }
        if (robotState.distanceTelemetreCentre < 15) {
            LED_BLEUE = 1;
        }
        else {
            LED_BLEUE = 0;
        }
        if (robotState.distanceTelemetreExGauche < 15 || robotState.distanceTelemetreExDroit < 15) {
            LED_BLANCHE = 1;
        }
        else {
            LED_BLANCHE = 0;
        }
        
        SendMessageDirect((unsigned char *) "Bonjour",7);
    }
}

unsigned char stateRobot;
int testBlocage = 0;

void OperatingSystemLoop(void) {
//    int gaucheOuDroite = 0;

    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(-25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            testBlocage++;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_GAUCHE_PROCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_PROCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(-5, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_TOURNE_DROITE_PROCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_PROCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
            
        case STATE_TOURNE_EX_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_EX_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
            case STATE_TOURNE_EX_GAUCHE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_EX_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_FREINAGE1:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_FREINAGE1_EN_COURS;
            testBlocage++;
            break;
        case STATE_FREINAGE1_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_FREINAGE2:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_FREINAGE2_EN_COURS;
            testBlocage++;
            break;
        case STATE_FREINAGE2_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_COULOIR_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(-4, MOTEUR_DROIT);
            PWMSetSpeedConsigne(3, MOTEUR_GAUCHE);
            stateRobot = STATE_COULOIR_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_COULOIR_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
            
        case STATE_COULOIR_TOURNE_DROITE:
            PWMSetSpeedConsigne(-3, MOTEUR_DROIT);
            PWMSetSpeedConsigne(4, MOTEUR_GAUCHE);
            stateRobot = STATE_COULOIR_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_COULOIR_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_AVANCE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    
   
    if ( robotState.distanceTelemetreDroit < 10  && robotState.distanceTelemetreGauche < 10 ) //Obstacle en face proche
        positionObstacle = OBSTACLE_EN_FACE_PROCHE;
    else if ( robotState.distanceTelemetreCentre < 15) //Obstacle en face proche
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit < 15) //Obstacle à droite proche
        positionObstacle = OBSTACLE_A_DROITE_PROCHE;
    else if (robotState.distanceTelemetreDroit < 25) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreGauche < 15) //Obstacle à gauche proche
        positionObstacle = OBSTACLE_A_GAUCHE_PROCHE;
    else if (robotState.distanceTelemetreGauche < 25) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if ( robotState.distanceTelemetreCentre < 25) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE_BIENTOT;
    else if ( robotState.distanceTelemetreCentre < 50) //Obstacle en face loin
        positionObstacle = OBSTACLE_EN_FACE_LOIN;
    else if (robotState.distanceTelemetreDroit < 30) //Obstacle à droite
        positionObstacle = OBSTACLE_A_DROITE_LOIN;
    else if (robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
        positionObstacle = OBSTACLE_A_GAUCHE_LOIN;
    else if ( robotState.distanceTelemetreExGauche < 15)
        positionObstacle = OBSTACLE_EX_GAUCHE;
    else if (robotState.distanceTelemetreExDroit < 15)
        positionObstacle = OBSTACLE_EX_DROITE;
    else if (robotState.distanceTelemetreExDroit < 15 && robotState.distanceTelemetreExGauche < 15 && robotState.distanceTelemetreExDroit < robotState.distanceTelemetreExGauche) //Couloir obstacle droite
        positionObstacle = COULOIR_OBS_DROITE;
    else if (robotState.distanceTelemetreExDroit < 15 && robotState.distanceTelemetreExGauche < 15 && robotState.distanceTelemetreExDroit > robotState.distanceTelemetreExGauche) //Couloir obstacle gauche
        positionObstacle = COULOIR_OBS_GAUCHE;
    else if (robotState.distanceTelemetreExDroit > 15 && robotState.distanceTelemetreDroit > 30 && robotState.distanceTelemetreCentre > 50 && robotState.distanceTelemetreGauche > 30 && robotState.distanceTelemetreExGauche > 15) //pas d'obstacle
        positionObstacle = PAS_D_OBSTACLE;
    
    //Détermination de l'état à venir du robot
    if (positionObstacle == OBSTACLE_EN_FACE_PROCHE)
        if(testBlocage % 2 == 0)
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        }
        else
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
        }
    else if (positionObstacle == OBSTACLE_EN_FACE)
        if(testBlocage % 2 == 0)
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        }
        else
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
        }
    else if (positionObstacle == OBSTACLE_EN_FACE_LOIN || positionObstacle == OBSTACLE_A_DROITE_LOIN || positionObstacle == OBSTACLE_A_GAUCHE_LOIN )
        nextStateRobot = STATE_FREINAGE2;
     else if (positionObstacle == OBSTACLE_EN_FACE_BIENTOT)
        nextStateRobot = STATE_FREINAGE1;
    else if (positionObstacle == OBSTACLE_A_DROITE_PROCHE)
        nextStateRobot = STATE_TOURNE_GAUCHE_PROCHE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE_PROCHE)
        nextStateRobot = STATE_TOURNE_DROITE_PROCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == COULOIR_OBS_DROITE)
        nextStateRobot = STATE_COULOIR_TOURNE_GAUCHE;
    else if (positionObstacle == COULOIR_OBS_DROITE)
        nextStateRobot = STATE_COULOIR_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EX_DROITE)
        nextStateRobot = STATE_TOURNE_EX_GAUCHE;
    else if (positionObstacle == OBSTACLE_EX_GAUCHE)
        nextStateRobot = STATE_TOURNE_EX_DROITE;
    else if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;

    //Si l'on n'est pas dans la transition de l'étape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
    }

}

// fin main

