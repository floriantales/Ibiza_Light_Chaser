


// Set and display registers
// Only call AFTER all values are set how you would like (slow otherwise)
void Led_Manager_writeRegisters(){
  // ground LEDlatchPin and hold low for as long as you are transmitting
  digitalWrite(Led_LatchPin, LOW);
 
  // for each bit in registers
  for(int i = DATA_WIDTH - 1; i >=  0; i--){
    digitalWrite(Led_ClockPin, LOW);
    digitalWrite(Led_DataPin, registers[i]);
    digitalWrite(Led_ClockPin, HIGH);
  }
  
  // return the latch pin high to signal chip that it no longer needs to listen for information
  digitalWrite(Led_LatchPin, HIGH);
}
 
// set an individual pin HIGH or LOW
void Led_Manager_setLed(byte index, boolean value){  
  // BitLed converti le bon bit
  registers[bitLed[index]] = value;
  // On envoi
  Led_Manager_writeRegisters();
}

// set all register pins HIGH or LOW
void Led_Manager_setAllLed(boolean value){
  for(int i = DATA_WIDTH - 1; i >=  0; i--){
     registers[i] = value;
  }
  // On envoi
  Led_Manager_writeRegisters();
}

// demo leds : chenillard et clignotement
void Led_Manager_Demo(){
  for (int i=0; i <= 15; i++) {
    Led_Manager_setLed(i, HIGH);
    delay(80);
  }
  for (int i=0; i <= 3; i++) {
    Led_Manager_setAllLed(HIGH);
    delay(150);
    Led_Manager_setAllLed(LOW);
    delay(150);
  }
}
