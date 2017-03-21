//#################################################
// PROGRAMME 1 : Le SLOW
// - Lumieres noires allumées
// - Boule allumée
// - Le reste éteint
//#################################################

void Stop_PGM1(){
    running_pgm1 = false;        // On indique que le PGM ne tourne pas
    Led_Manager_setLed(1, LOW);  // On indique l'état sur la LED associée
}
void Initialize_PGM1(){
    running_pgm1 = true;         // On indique que le PGM tourne
    Led_Manager_setLed(1, HIGH);
    
    // On éteint :
    Prise_Manager_Set(0, 1, LOW);
    Prise_Manager_Set(0, 2, LOW);
    Prise_Manager_Set(0, 3, LOW);
    Prise_Manager_Set(0, 4, LOW);
    Prise_Manager_Set(1, 2, LOW);
    Prise_Manager_Set(1, 3, LOW);
    Prise_Manager_Set(1, 4, LOW);
     
    // On allume les UV et la boule
    Prise_Manager_Set(0, 0, HIGH);
    Prise_Manager_Set(1, 0, HIGH);
    Prise_Manager_Set(1, 1, HIGH);    
}
void PGM1(){ 
  // Ben pas grand chose à faire .. rouler une pelle par exemple?
}
