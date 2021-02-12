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
        
        if(ADCIsConversionFinished() == 1)
        {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts -5;
            volts = ((float) result [1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts -5;
            volts = ((float) result [0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts -5;
        }
        if(robotState.distanceTelemetreDroit < 20)
        {
            LED_ORANGE = 1;
        }
        else
        {
            LED_ORANGE = 0;
        }
        if(robotState.distanceTelemetreCentre < 20)
        {
            LED_BLEUE = 1;
        }
        else
        {
            LED_BLEUE = 0;
        }
        if(robotState.distanceTelemetreGauche < 20)
        {
            LED_BLANCHE = 1;
        }
        else
        {
            LED_BLANCHE = 0;
        }
    } // fin main
}


