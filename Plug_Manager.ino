// #################################################################################################################################
// ###   Emittion des trames du HS2260A-R4 de la commande des prises multiples Ibiza
// ###   Programme : https://github.com/Cribstone/Arduino-Sketchbook/blob/master/HomeTempControl/HomeTempControl.ino
// ###   
// ###   Attention : Code synchrone et donc bloquant -> enchainer les commandes rique le blocage du programme
// ###     => pour d=175us une commande = 22,4ms et fois 2 si REPEATE à 2!!!
// #################################################################################################################################


void I() {  // sends one
    digitalWrite(pinData433, HIGH); delayMicroseconds(3*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(1*d); 
    digitalWrite(pinData433, HIGH); delayMicroseconds(3*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(1*d); }
void O() {  // sends zero
    digitalWrite(pinData433, HIGH); delayMicroseconds(1*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(3*d); 
    digitalWrite(pinData433, HIGH); delayMicroseconds(1*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(3*d); }
void FL() {   // sends float
    digitalWrite(pinData433, HIGH); delayMicroseconds(1*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(3*d); 
    digitalWrite(pinData433, HIGH); delayMicroseconds(3*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(1*d); }
void S() {  // synchronization
    digitalWrite(pinData433, HIGH); delayMicroseconds(1*d);  digitalWrite(pinData433, LOW);  delayMicroseconds(31*d); }
         

// #########################################################
// # Modification des prises de la multiprise IBIZA
// # d = 165
// #########################################################
void code_prise(byte numero_multiprise) {
  switch (numero_multiprise) {
    case 0: O();O();FL();FL();FL(); break;  // On reference le code de la prise.
    case 1: FL();O();FL();FL();O(); break;
  } 
}

// Modification de l'état d'une prise : 
void Prise_Manager_Set(byte numero_multiprise, byte numero_prise, boolean value) {
  etat_prises[numero_multiprise][numero_prise] = value; // On renseigne ce que sera l'état (si tout va bien ^^) de la lumiere.
  
  for (byte i=0; i <= REPEATE; i++) {
    if (value) { // Si on veux allumer
      switch (numero_prise) {
        case  0: code_prise(numero_multiprise);FL();FL();FL();O();I();O();I();S(); break;
        case  1: code_prise(numero_multiprise);FL();FL();FL();I();O();O();I();S(); break;
        case  2: code_prise(numero_multiprise);FL();FL();I();O();O();O();I();S(); break;
        case  3: code_prise(numero_multiprise);FL();I();FL();O();O();O();I();S(); break;
        case  4: code_prise(numero_multiprise);I();FL();FL();O();O();O();I();S(); break;
      }
    }
    else {      // Sinon on veux eteindre
      switch (numero_prise) {
        case  0: code_prise(numero_multiprise);FL();FL();FL();O();I();I();O();S(); break;
        case  1: code_prise(numero_multiprise);FL();FL();FL();I();O();I();O();S(); break;
        case  2: code_prise(numero_multiprise);FL();FL();I();O();O();I();O();S(); break;
        case  3: code_prise(numero_multiprise);FL();I();FL();O();O();I();O();S(); break;
        case  4: code_prise(numero_multiprise);I();FL();FL();O();O();I();O();S(); break;
      }
    }   
  }
}

// Modification de l'état de toutes les prises :
void Prise_Manager_SetAll(boolean value){
    for (byte i=0 ; i <= nb_multiprises - 1 ; i++){
      for (byte j=0 ; j <= nb_prise - 1 ; j++){
        Prise_Manager_Set(i, j, value);
      }
    }
}


// #########################################################
// # Telecommande des transmetteurs chinois 433
// # A -> FFFFFFFF0010S
// # B -> FFFFFFFF1000S
// # C -> FFFFFFFF0001S
// # D -> FFFFFFFF0100S
// # d = 540
// #########################################################
