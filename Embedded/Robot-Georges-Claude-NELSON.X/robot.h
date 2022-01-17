#ifndef ROBOT_H
#define ROBOT_H

#include "Asservissement.h"

typedef struct robotStateBITS {

    union {

        struct {
            unsigned char taskEnCours;
            float distanceTelemetreDroit;
            float distanceTelemetreCentre;
            float distanceTelemetreGauche;
            float distanceTelemetreExGauche;
            float distanceTelemetreExDroit;
            
            //----Odometrie----
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            double vitesseLineaireFromOdometry_1;
            double vitesseAngulaireFromOdometry_1;
            double vitesseDroitFromOdometry;
            double vitesseGaucheFromOdometry;
            double vitesseLineaireFromOdometry;
            double vitesseAngulaireFromOdometry;
            double xPosFromOdometry;
            double yPosFromOdometry;
            double angleRadianFromOdometry;
            double xPosFromOdometry_1;
            double yPosFromOdometry_1;
            double angleRadianFromOdometry_1;
            
            //----Asservissement----
            PidCorrector PidX;
            PidCorrector PidTheta;
            
            //Commande Moteur
            double thetaCorrectionVitesseCommande;
            double xCorrectionVitesseCommande;
            double vitesseLineairePourcent;
            double vitesseLineaireConsigne;
            double vitesseAngulaireConsigne;
            double xCorrectionVitessePourcent;
            double vitesseAngulairePourcent;
            double thetaCorrectionVitessePourcent;
        }
        ;
    }
    ;
} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */
