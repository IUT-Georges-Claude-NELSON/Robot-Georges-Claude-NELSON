#ifndef PWM_H
#define PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#define COEFF_VITESSE_ANGULAIRE_PERCENT 75
#define COEFF_VITESSE_LINEAIRE_PERCENT 75

void InitPWM(void);

//void PWMSetSpeed(float vitesseEnPourcents, int numeroMoteur);
void PWMUpdateSpeed(void);
void PWMSetSpeedConsigne(float vitesseEnPourcents, unsigned char moteur);
void PWMSetSpeedConsignePolaire();
void UpdateAsservissemment();

#endif /* PWM_H */