// Note de version : Version 0.1 du 01/12/2015 pour premiere évaluation
// Utilisation du clavier Switch Managaer 16 touches:
//  1 2 3 4
//  x x x x
//  x x x x
//  5 6 7 8
// 1 : Programme Tutti Frutti
// 2 : Programme de Slow
// 3 : Programme Rock/Electro
// 4 : Beat sync en cours de dev hardware, le soft doit être OK
// 5/6 : -/+ vitesse des changements programme 1
// 7/8 : Allume/Eteint toutes les lumières et éteint tout les programmes.
//
// New: Tout
// BugFix:
// Rest: Fonction changement aléatoire de lumieres à améliorer : La boucle est bloquante temps qu'il ne trouve pas une prise à changer
// Rest: Définir d'autres séquences (disco, mode hysterique...)


// ###########################################
// ###########  VARIABLES
// ###########################################

//### Forcement le meme pour les bouttons et les leds:
#define NUMBER_OF_SHIFT_CHIPS   2              // How many shift register chips are daisy-chained.
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8 // Width of data (how many ext lines).

//### FONCT_Switch_Manager
#define PULSE_WIDTH_USEC   1      // Width of pulse to trigger the shift register to read and latch.
#define DEBOUNCE 5                // Debounce time in ms
#define BYTES_VAL_T unsigned int  // You will need to change the "int" to "long" if the NUMBER_OF_SHIFT_CHIPS is higher than 2

const byte Button_ClockPin = 4;  // Connects to the Clock pin
const byte Button_LatchPin = 5;  // Connects to Parallel Latch pin
const byte Button_DataPin = 6;   // Connects to the Data pin

const byte bitButton[] = {2,3,10,11,0,1,8,9,6,7,14,15,4,5,12,13};                // Matrice de correspondance physique bit/button - exemple BitButton[Mon Button 1] = Bit registre numero 10
boolean pressed[DATA_WIDTH], justpressed[DATA_WIDTH], justreleased[DATA_WIDTH];  // The buttons states

//### FONCT_Led_Manager
const byte Led_ClockPin = 7;  // Connects to the Clock pin
const byte Led_LatchPin = 8;  // Connects to Serial Latch pin
const byte Led_DataPin = 9;   // Connects to the Data pin

boolean registers[DATA_WIDTH];                                 // Stock les valeurs du registe
const byte bitLed[] = {10,11,2,3,8,9,0,1,14,15,6,7,12,13,4,5};  // Matrice de correspondance physique bit/led - exemple BitLed[Ma Led 1] = Bit registre numero 10

//### FONCT_Prise_Manager
const byte d = 165;              // delay adjusted to match original wireless remote.  Use a digital oscilloscope to check. -> conf pour les prises ibiza  
const byte pinData433 = 12;      // 433 MHz RF transmitter hooked to this pin (soldered parallel to the remote encoding chip output.)
#define REPEATE 3                // Nombre de fois ou il faut repeter le message
#define nb_prise 5               // Nombre de prises pilotees par multiprise
#define nb_multiprises 2         // Nombre de multiprises
boolean etat_prises[nb_multiprises][nb_prise];   // Etat des prises de A1->A5 et de B1->B5: false par défaut (éteint)

//### PROGRAMMES (tous)
boolean running_pgm0 = false;  // Gestion de l'état des pgm (running or not)
boolean running_pgm1 = false;
boolean running_pgm2 = false;

unsigned long startMillis;  // Gestion du temps
unsigned long currentMillis;
unsigned long random_time;

byte machine_etat = 0;        // Gestion de la machine d'état

//### PROGRAMME 0 : Spécificités Mode Automatique
byte first_Delais = 4; //Temps du premier changement d'état en secondes
unsigned int min_Delais = 4;   //Temps min du prochain changement d'état en secondes
unsigned int max_Delais = 16;  //Temps max du prochain changement d'état en secondes

//### BEAT_SYNC
boolean running_beatsync = false;  // Gestion de l'état des pgm (running or not)
const byte pinDataBeat = 2;        // Pin entrée avec interruption (2 ou 3 sur UNO)
boolean beat = false;              // Passe True à chaque coup de basse

long lastDebounceTime = 0;  // POUR TEST BEAT AVEC BOUTON

// ### FONCTIONS
// Fonction Strobo
#define strobo_multiprise 0 // Ou est branché le strobo : multiprise 0 ou 1 pour 2 multiprises..
#define strobo_prise 1 // Ou est branché le strobo : prise 0,1,2,3,4 ou 5
unsigned long strobo_stopMillis;  // Gestion du temsp strobo
boolean running_strobo = false;
// Fonction Changement aléatoire lumière
byte nombre_random = 0;
byte nombre_prise_a_modifier = 0; // Sera aléatoire
byte numero_prise_a_modifier = 0; // Sera aléatoire
byte numero_multiprise_a_modifier = 0; // Sera aléatoire
boolean prise_active[nb_multiprises][nb_prise] = { // On défini dans ce tableau les prises exclues du jeux aléatoire (autant de dimensions que de prise/multiprises dipos)
  {0,0,1,1,1},  // On positionne à 1 pour activer la prise // Ici on ne gerera pas automatiquement les UV et le strobo
  {0,1,1,1,1}   // Ici on ne gerera pas automatiquement les UV 
};
#define nb_max_prise_a_modifier 2 // Veiller à ce que l'on puisse modifier au plus autant de prises actives
byte mem_last_prise[nb_max_prise_a_modifier * 2]; // On enregistrera la liste des prises randomisées pour chaque changement


// ###########################################
// ###########  SETUP
// ###########################################
void setup()
{
  // Uncomment for debugging mode
  //Serial.begin(9600);
  
  //Interruptions
  pinMode(pinDataBeat, INPUT);                      // EN DEV BEAT AUDIO

  // Init Button pins
  pinMode(Button_ClockPin, OUTPUT);
  pinMode(Button_LatchPin, OUTPUT);
  pinMode(Button_DataPin, INPUT);
  digitalWrite(Button_ClockPin, LOW);
  digitalWrite(Button_LatchPin, HIGH);
  
  // Init Led pins
  pinMode(Led_ClockPin, OUTPUT);
  pinMode(Led_LatchPin, OUTPUT);
  pinMode(Led_DataPin, OUTPUT);
  digitalWrite(Led_ClockPin, LOW);
  digitalWrite(Led_LatchPin, HIGH);
  digitalWrite(Led_DataPin, LOW);
  
  // DATA 433MHz
  pinMode(pinData433, OUTPUT);
 
  // Init Led Registers : reset all register pins
  Led_Manager_setAllLed(LOW);
  
  // Bootup demo :
  Led_Manager_Demo();
}


// ###########################################
// ###########  MAIN
// ###########################################
void loop()
{ 
  // On lance l'analyse du clavier. Datas dans pressed justpressed justreleased
  Switch_Manager_checkButtons();
  
  // Tests des combinaisons de touches
  if (justpressed[0]){
    if (!running_pgm0){    // Si le programme ne tourne pas on active sinon on ne fait rien
      Stop_All_PGM();      // ON arrête tout
      Initialize_PGM0();   // On initialise le PGM
    }
  }
  if (justpressed[1]){
    if (!running_pgm1){
      Stop_All_PGM();
      Initialize_PGM1();
    }
  }
  if (justpressed[2]){
    if (!running_pgm2){
      Stop_All_PGM();
      Initialize_PGM2();
    }
  }
  if (justpressed[3]){
    if (!running_beatsync) {
      Initialize_BEAT_SYNC();
    }
    else {
      Stop_BEAT_SYNC();
    }
  }
  if (justpressed[12]){  
      if (min_Delais < 64) {
        min_Delais = min_Delais * 2;
        max_Delais = max_Delais * 2;
        Led_Manager_setLed(12, HIGH);
        startMillis = millis() + 2000;
        //Serial.print("Modif delais : min=");Serial.print(min_Delais);Serial.print(" - max=");Serial.println(max_Delais);
        delay(80);
      }
      else {
        for (byte i=0 ; i<5 ; i++) {Led_Manager_setLed(12, HIGH);delay(30);Led_Manager_setLed(12, LOW);delay(30);}
      }
      Led_Manager_setLed(12, LOW);
  }
  if (justpressed[13]){ 
      if (min_Delais > 1) {
        min_Delais = min_Delais / 2;
        max_Delais = max_Delais / 2;
        Led_Manager_setLed(13, HIGH);
        startMillis = millis() + 2000;
        //Serial.print("Modif delais : min=");Serial.print(min_Delais);Serial.print(" - max=");Serial.println(max_Delais);
        delay(80);
      }
      else {
        for (byte i=0 ; i<5 ; i++) {Led_Manager_setLed(13, HIGH);delay(30);Led_Manager_setLed(13, LOW);delay(30);}
      }
      Led_Manager_setLed(13, LOW);
  }
  if (justpressed[14]){
      Stop_All_PGM();
      Led_Manager_setLed(14, HIGH);
      Prise_Manager_SetAll(HIGH);
      Led_Manager_setLed(14, LOW);
  }
  if (justpressed[15]){
      Stop_All_PGM();
      Led_Manager_setLed(15, HIGH);
      Prise_Manager_SetAll(LOW);
      Led_Manager_setLed(15, LOW);
  }

  // ON LANCE LES PROGRAMMES ACTIFS
  if (running_pgm0) PGM0();
  if (running_pgm1) PGM1();
  if (running_pgm2) PGM2();
  if (running_strobo) Stroboscope();
}
