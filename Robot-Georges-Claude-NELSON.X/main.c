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

    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

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
    } // fin main
}


