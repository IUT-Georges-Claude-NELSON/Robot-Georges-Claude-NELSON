#ifndef ROBOT_H
#define ROBOT_H
typedef struct robotStateBITS {
union {

struct {
unsigned char taskEnCours;
float distanceTelemetreDroit;
float distanceTelemetreCentre;
float distanceTelemetreGauche;
float distanceTelemetreExGauche;
float distanceTelemetreExDroit;
float vitesseGaucheConsigne;
float vitesseGaucheCommandeCourante;
float vitesseDroiteConsigne;
float vitesseDroiteCommandeCourante;
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
}
;}
;} ROBOT_STATE_BITS;

extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */
