//
// FONCTION Stop
// Permet de stoper les programmes en cours
// A enrichir du nombre de programmes possibles
void Stop_All_PGM() {
    // On stop les programmes
    Stop_PGM0();
    Stop_PGM1();
    Stop_PGM2();
    Stop_BEAT_SYNC();
}

//
// FONCTION Stroboscope
// Pour assurer un petit coup de strob (non bloquant)
//
// Utilisation :
// Activer en invoquant  Stroboscope_Set(Nombre_de_secondes,HIGH/LOW) , on peut couper prématurément en repassant à LOW ou en invoquant directement Stroboscope_Stop()..
// Stroboscope() est invoqué dans le main a chaque boucle pour assurer l'extinction.
void Stroboscope_Set(byte time_to_active, boolean value) {
  if(value) {
    strobo_stopMillis = millis() + (time_to_active * 1000);  // Heure à laquelle il faudra couper le strobo
    Prise_Manager_Set(strobo_multiprise, strobo_prise, HIGH);
    running_strobo = true; // C'est partit
  }
  else Stroboscope_Stop();
}

void Stroboscope() {
  if (millis() > strobo_stopMillis) Stroboscope_Stop();
}

void Stroboscope_Stop() {
    Prise_Manager_Set(strobo_multiprise, strobo_prise, LOW);
    Prise_Manager_Set(strobo_multiprise, strobo_prise, LOW);
    Prise_Manager_Set(strobo_multiprise, strobo_prise, LOW); // Jamais trop prudent avec le strobo
    Prise_Manager_Set(strobo_multiprise, strobo_prise, LOW); // Non, jamais trop ...
    running_strobo = false;
}

//
// FONCTION Changement aléatoire de lumieres.
// Effectue un changemenbt aléatoire d'un nombre aléatoire de prises définies dans la variable 'boolean prise_active'
//
// Utilisation :
// Invoquer Changement_Prise_Aleatoire() et paramétrer la variable 'boolean prise_active' ou 'nb_max_prise_a_modifier' au besoin.
// 
// Pistes d'améliorations :
// - Peut être bloquant si il n'arrive pas a randomiser une prise disponible.
//
//
void Changement_Prise_Aleatoire(){
  // On définie au hasard le nombre de prise à modifier
  unsigned long seed1=seedOut(31);
  randomSeed(seed1);
  nombre_prise_a_modifier = random(nb_max_prise_a_modifier);
  
  for (byte i=0; i <= nombre_prise_a_modifier; i++) {
    // On cherche une prise à modifier
    boolean lock = true;
    
    // On va définir les prises qui vont changer switcher
    while(lock){
      // On definie au hasard quelle prise modifier :
      unsigned long seed1=seedOut(31);
      randomSeed(seed1);
      numero_prise_a_modifier = random(0, nb_prise); // Entre 0 et le nb_prise - 1
             
      // On definie au hasard quelle multiprise modifier :
      unsigned long seed2=seedOut(31);
      randomSeed(seed2);
      numero_multiprise_a_modifier = random(nb_multiprises);
    
      // Si la prise choisie n'est pas exclue on sort de la boucle:
      if (prise_active[numero_multiprise_a_modifier][numero_prise_a_modifier]) lock = false;
      
      // Si la prise choisie à déjà été selectionnée on en cherche une autre:
      for (byte j=0; j <= i; j++) if ((numero_multiprise_a_modifier==mem_last_prise[2*j])&&(numero_prise_a_modifier==mem_last_prise[(2*j)+1])) lock = true;
    }
    
    // On mémorise la prise modifiée:
    mem_last_prise[2*i]=numero_multiprise_a_modifier;
    mem_last_prise[(2*i)+1]=numero_prise_a_modifier;
    
    // DEBUT On change l'état de la prise
    // Ici je jerrai le cas ou il ne fallait pas eteindre tout, mais avce les scanners toujours allumés ça change la donne
    /*
    if (etat_prises[numero_multiprise_a_modifier][numero_prise_a_modifier]){
      // Avant d'éteindre on s'assure quand même de ne pas tout couper -> ambiance assurée ;)
      // On compte donc le nombre de prises allumées:
      byte count_status_true = 0;
      for (byte i=0; i < nb_multiprises; i++){
        for (byte j=0; j < nb_prise; j++){
          if (etat_prises[i][j] && prise_active[i][j]) count_status_true++;
        }
      }      
      // Si il y a plusieurs prises allumées ou que l'on ne va pas éteindre la seule allumée, on peux couper:
      if (count_status_true >= 2) Prise_Manager_Set(numero_multiprise_a_modifier, numero_prise_a_modifier, LOW);
      else if ((count_status_true == 1) && !etat_prises[numero_multiprise_a_modifier][numero_prise_a_modifier]) Prise_Manager_Set(numero_multiprise_a_modifier, numero_prise_a_modifier, LOW);
      }   
    else {
      Prise_Manager_Set(numero_multiprise_a_modifier, numero_prise_a_modifier, HIGH);
    }
    */
    if (etat_prises[numero_multiprise_a_modifier][numero_prise_a_modifier]){
      Prise_Manager_Set(numero_multiprise_a_modifier, numero_prise_a_modifier, LOW);
    }   
    else {
      Prise_Manager_Set(numero_multiprise_a_modifier, numero_prise_a_modifier, HIGH);
    }
    // FIN On change l'état de la prise
    
  }
}
