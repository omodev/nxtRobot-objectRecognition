#include "implementation.oil"

CPU ATMEL_AT91SAM7S256
{
  OS LEJOS_OSEK
  {
    STATUS = EXTENDED;
    STARTUPHOOK = FALSE;
    ERRORHOOK = FALSE;
    SHUTDOWNHOOK = FALSE;
    PRETASKHOOK = FALSE;
    POSTTASKHOOK = FALSE;
    USEGETSERVICEID = FALSE;
    USEPARAMETERACCESS = FALSE;
    USERESSCHEDULER = FALSE;
  };

  // Définition du mode de l'application
  APPMODE appmode1{};

  // Définition de la tâche acquerir_ultrason
  TASK acquerir_ultrason
  {
    AUTOSTART = FALSE;
    PRIORITY = 2;
    ACTIVATION = 1;
    SCHEDULE = FULL;
    STACKSIZE = 512;
    RESOURCE = obstacle;
  };

  // Définition de la tâche rouler
  TASK rouler
  {
    AUTOSTART = FALSE;
    PRIORITY = 1;
    ACTIVATION = 1;
    SCHEDULE = FULL;
    STACKSIZE = 512;
    RESOURCE = obstacle;
  };

  // Définition de la ressource pour l'exclusion mutuelle
  RESOURCE obstacle
  {
    RESOURCEPROPERTY = STANDARD;
  };

  // Définition du compteur utilisé par l'alarme
  COUNTER compteur
  {
    MINCYCLE = 1;
    MAXALLOWEDVALUE = 100000;
    TICKSPERBASE = 1;
  };

  // Définition de l'alarme périodique utilisée pour réveiller la tâche acquerir_ultrason
  ALARM alarme_reveil_acquerirUltrason
  {
    COUNTER = compteur;
    ACTION = ACTIVATETASK
    {
     TASK = acquerir_ultrason;
    };
    AUTOSTART = TRUE
    {
     ALARMTIME = 10;
     CYCLETIME = 10;
     APPMODE = appmode1;
    };

  };

  // Définition de l'alarme périodique utilisée pour réveiller la tâche rouler
  ALARM alarme_reveil_rouler
  {
    COUNTER = compteur;
    ACTION = ACTIVATETASK
    {
     TASK = rouler;
    };
    AUTOSTART = TRUE
    {
     ALARMTIME = 10;
     CYCLETIME = 10;
     APPMODE = appmode1;
    };

  };

};
