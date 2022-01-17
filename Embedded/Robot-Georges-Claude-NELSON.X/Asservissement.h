#ifndef ASSERVISSEMENT_H
#define	ASSERVISSEMENT_H

typedef struct _PidCorrector
{
    double Kp;
    double Ki;
    double Kd;
    double erreurProportionnelleMax;
    double erreurIntegraleMax;
    double erreurDeriveeMax;
    double erreurIntegrale;
    double epsilon_1;
    
    //For debug only
    double corrP;
    double corrI;
    double corrD; 
    
    double consigne;
    double command;
    double erreur;
    double value;
    
}PidCorrector;

extern PidCorrector PidX;
extern PidCorrector PidTheta;

void asservissemtentValeur();
void setupPidAsservissement(volatile PidCorrector* , double Kp, double Ki, double Kd, double proportionnelleMax, double integraleMax, double deriveeMax);
double Correcteur(volatile PidCorrector* PidCorr, double erreur);


#endif	/* ASSERVISSEMENT_H */

