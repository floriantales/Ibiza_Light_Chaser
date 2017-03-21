void Stop_BEAT_SYNC(){
    running_beatsync = false;            // On indique que le PGM n'est pas actif
    Led_Manager_setLed(3, LOW);          // On indique l'état sur la LED associée
    detachInterrupt(pinDataBeat-2);      // On arrête la capture du Beat par interruption
    beat = false;                        // On passe le beat a false  
}

void Initialize_BEAT_SYNC(){
    running_beatsync = true;                                 // On indique que le PGM doit être actif
    Led_Manager_setLed(3, HIGH);                              // On indique l'état sur la LED associée  
    attachInterrupt(pinDataBeat-2, love_the_beat, FALLING);  // On capture le beat par interruption
}

void love_the_beat() // S'execute à chaque coup de basse grace à l'interruption
{
  if ((millis() - lastDebounceTime) > 200) {// POUR TEST BEAT AVEC BOUTON
    beat = true;// On passe le beat à True
    lastDebounceTime = millis();// POUR TEST BEAT AVEC BOUTON
    //Serial.print("I love the Beat!! \n");
  } // POUR TEST BEAT AVEC BOUTON
}
