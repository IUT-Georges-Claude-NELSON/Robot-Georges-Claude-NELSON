#include <xc.h>
#include "IO.h"
#include "main.h"

void InitIO()
{
    // IMPORTANT : d�sactiver les entr�es analogiques, sinon on perd les entr�es num�riques
    ANSELA = 0; // 0 desactive
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    //********** Configuration des sorties : _TRISxx = 0 ********************************
    //********** Configuration des entr�es : _TRISxx = 1 ********************************
    
    // LED
    _TRISC10 = 0;  // LED Orange
    _TRISG6 = 0; //LED Blanche
    _TRISG7 = 0; // LED Bleue
    
    // Moteur 1
    _TRISB14=0; 
    _TRISB15=0;
    
    //Moteur 6
    _TRISC6=0;
    _TRISC7=0;
    

    _U1RXR = 24; // Remape la RP 24 sur l'entr�e Rx1
    _RP36R = 0b000001; //Remappe la sortie Tx1 vers RP
    
    //********** QEI **********
    
    _QEA2R = 97; // assign QEI A to pin RP97
    _QEB2R = 96; // assign QEI B to pin RP96
    
    _QEA1R = 70; // assign QEI A to pin RP70
    _QEB1R = 69; // assign QEI B to pin RP69
    
}

