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

/*
 * 
 */
int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entr�es sorties
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
        unsigned int * resultat;
        if(ADCIsConversionFinished() == 1)
        {
            ADCClearConversionFinishedFlag();
            resultat = ADCGetResult();
        }
        if(resultat [0] > 310)
        {
            LED_ORANGE = 1;
        }
        else
        {
            LED_ORANGE = 0;
        }
        if(resultat [1] > 310)
        {
            LED_BLEUE = 1;
        }
        else
        {
            LED_BLEUE = 0;
        }
        if(resultat [2] > 310)
        {
            LED_BLANCHE = 1;
        }
        else
        {
            LED_BLANCHE = 0;
        }
    } // fin main
}


