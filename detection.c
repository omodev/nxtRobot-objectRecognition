#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

DeclareResource(obstacle);
DeclareTask(acquerir_ultrason);
DeclareTask(rouler);
DeclareCounter(compteur);
DeclareAlarm(alarme_reveil_acquerirUltrason);
DeclareAlarm(alarme_reveil_rouler);

//structure de donnée informant de l'existence ou non d'un obstacle
typedef struct{
  int obstacleEnVue; // vaut 1 si obstacle et 0 sinon
}type_Obstacle;

//routine de traitement d'interruption nxtOSEK de type 2,
//elle est fournie par l'API du robot et doit figurer dans
//la liste des fonctions du programme
void user_1ms_isr_type2(void){
    StatusType ercd;
  // Increment System Timer Count
  ercd = SignalCounter(compteur);
  if(ercd != E_OK){
    ShutdownOS(ercd);
  }
}

//fonction d'initialisation des pilotes de matériels utilisés par le robot
//dans l'application. Elle est fournie par l'API du robot et doit obligatoirement
//figurer dans la liste des fonctions du programme
void ecrobot_device_initialize(void){
  // Initialize ECRobot used devices
  ecrobot_init_sonar_sensor(NXT_PORT_S1);
}

//déclaration de la variable globale partagée par les deux tâches
type_Obstacle donneeObstacle;

//corps de la tâche acquerir_ultrason
TASK(acquerir_ultrason){
  int distance = ecrobot_get_sonar_sensor(NXT_PORT_S1);
  GetResource(obstacle);
  if(distance > 20){
    donneeObstacle.obstacleEnVue = 0;
  }else{
    donneeObstacle.obstacleEnVue = 1;
  }
  ReleaseResource(obstacle);
  TerminateTask();
}

//corps de la tâche rouler
TASK(rouler){
  int vitesseDeplacementRobot = 70;
  int angleRotationRobot = 45;
  int obs = 0;
  GetResource(obstacle);
  obs = donneeObstacle.obstacleEnVue;
  ReleaseResource(obstacle);
  if(obs == 0){
    //commander les deux roues du robot à rouler :
    nxt_motor_set_speed(NXT_PORT_A, vitesseDeplacementRobot, 0);
    nxt_motor_set_speed(NXT_PORT_B, vitesseDeplacementRobot, 0);
  }else{
    //commander les roues du robot à tourner :
    nxt_motor_set_speed(NXT_PORT_A, vitesseDeplacementRobot - 30, 1); //freiner en réduisant de 30 la vitesse de la 1ère roue du robot
    nxt_motor_set_speed(NXT_PORT_B, -40, 1); //freiner en essayant d'imobiliser (vitesse négative) la 2ème roue, pour permettre au robot de pivoter
    nxt_motor_set_count(NXT_PORT_A, angleRotationRobot);
  }
  TerminateTask();
}
