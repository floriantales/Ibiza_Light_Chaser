//#################################################
// PROGRAMME 0 : Aléatoire toutes lumieres
// - Lumieres noires allumées
// - Scanners allumés
// - Un coup de stroboscope rarement (1 chance sur 5)
// - Fonctionnement aléatoire du reste
//#################################################

void Stop_PGM0(){
    running_pgm0 = false;        // On indique que le PGM ne tourne pas
    Led_Manager_setLed(0, LOW);  // On indique l'état sur la LED associée
}

void Initialize_PGM0(){
    running_pgm0 = true;         // On indique que le PGM tourne
    Led_Manager_setLed(0, HIGH);

    startMillis = first_Delais * 1000 + millis();  // On recupere le temps de début du pgm  
    
    // On éteint tout :
    // Prise_Manager_SetAll(LOW);  // Plus trop convaincu non plus
    Prise_Manager_Set(0, 1, LOW);
    Prise_Manager_Set(0, 2, LOW);
    Prise_Manager_Set(0, 3, LOW);
    Prise_Manager_Set(0, 4, LOW);
    Prise_Manager_Set(1, 1, LOW);
    Prise_Manager_Set(1, 2, LOW);
     
    // On allume les UV et les scanners
    //if (!etat_prises[0][0]) {Prise_Manager_Set(0, 0, HIGH);} Test si necessaire .. mais bon pas trop convaincu
    Prise_Manager_Set(0, 0, HIGH);
    Prise_Manager_Set(1, 0, HIGH);
    Prise_Manager_Set(1, 3, HIGH);
    Prise_Manager_Set(1, 4, HIGH);

    // Configuration du sous programme Changement_Prise_Aleatoire();
    //prise_active[][] = { // On défini dans ce tableau les prises exclues du jeux aléatoire (autant de dimensions que de prise/multiprises dipos)
    //{0,0,1,1,1},  // On positionne à 1 pour activer la prise // Ici on ne gerera pas automatiquement les UV et le strobo
    //{0,1,1,0,0}   // Ici on ne gerera pas automatiquement les UV 
    //};
    prise_active[0][0] = 0 ; prise_active[0][1] = 0 ; prise_active[0][2] = 1 ; prise_active[0][3] = 1 ; prise_active[0][4] = 1 ;
    prise_active[1][0] = 0 ; prise_active[1][1] = 1 ; prise_active[1][2] = 1 ; prise_active[1][3] = 0 ; prise_active[1][4] = 0 ;
   
}

void PGM0(){
    // Si le beat n'a pas la main, on recupere l'heure
    if (!running_beatsync) currentMillis = millis();
    
    // Si le temps est écoulé ou si le Beat le permet on lance le changement de scene
    if((currentMillis > startMillis) || beat){
      
      // LISTE DES JEUX DE LUMIERE
      // 1- Lancement d'un changement aléatoire
      Changement_Prise_Aleatoire();
      // 2 - Pourquoi pas un petit coup de flash (pas trop souvent quand même)
      unsigned long seed=seedOut(31);
      randomSeed(seed);
      if (random(7)== 6) Stroboscope_Set(3,HIGH); // Un petit coup de 3 secondes
  
      // Ce n'est pas le beat qui à pris la main? On modifie donc l'heure du prochain évènement.
      if (!running_beatsync){ 
        // On recréé un random (voir dans le code RANDOM) pour déterminer le temps du prochain changement
        unsigned long seed=seedOut(31);
        randomSeed(seed);
        random_time = random(min_Delais, max_Delais)*1000 ;// On prend une valeur aléatoire entre 1 le délais max
        //Serial.print("Prochain changement d etat dans (en ms): ");  Serial.println(random_time);
        startMillis = millis() + random_time;
      }
      
      beat = false; // On a consommé le beat
    }  
}
