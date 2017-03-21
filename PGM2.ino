//#################################################
// PROGRAMME 2 : Rock Electro
// - Lumieres noires allumées
// - Scanners allumés
// - On abuse du strobe toutes les minutes
// - On alterne la boule et laser toutes les minutes
// - Le reste éteint
//#################################################

void Stop_PGM2(){
    running_pgm2 = false;        // On indique que le PGM ne tourne pas
    Led_Manager_setLed(2, LOW);  // On indique l'état sur la LED associée
}

void Initialize_PGM2(){
    running_pgm2 = true;         // On indique que le PGM tourne
    Led_Manager_setLed(2, HIGH);
    
    // On attend 1 minute avant de lancer le programme
    startMillis = 60000 + millis();  

    // On éteint :
    Prise_Manager_Set(0, 1, LOW);
    Prise_Manager_Set(0, 3, LOW);
    Prise_Manager_Set(0, 4, LOW);
    Prise_Manager_Set(1, 1, LOW);
    Prise_Manager_Set(1, 2, LOW);
     
    // On allume les UV, les scanners et le laser
    Prise_Manager_Set(0, 0, HIGH);
    Prise_Manager_Set(1, 0, HIGH);
    Prise_Manager_Set(0, 2, HIGH); 
    Prise_Manager_Set(1, 3, HIGH);
    Prise_Manager_Set(1, 4, HIGH); 
}

void PGM2(){
    // On recupere l'heure
    currentMillis = millis();
    
    // Si le temps est écoulé on lance le changement de scene
    if(currentMillis > startMillis){
      
      // LISTE DES JEUX DE LUMIERE
      // 1 - Un petit coup de flash
      Stroboscope_Set(3,HIGH); // Un petit coup de 4 secondes
      // 2 - Alternance boule/laser
      if (etat_prises[1][1]) {Prise_Manager_Set(1, 1, LOW); Prise_Manager_Set(0, 2, HIGH);}
      else {Prise_Manager_Set(1, 1, HIGH); Prise_Manager_Set(0, 2, LOW);}
      
      // On attend 1 minute
      startMillis = millis() + 60000;
    }  
}
