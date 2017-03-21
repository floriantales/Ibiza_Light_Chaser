/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
*/
BYTES_VAL_T Switch_Manager_readRegisters()
{
  long bitVal;
  BYTES_VAL_T bytesVal = 0;
  
  //Pulse the latch pin:
  digitalWrite(Button_LatchPin, HIGH);  //set it to 1 to collect parallel data
  delayMicroseconds(PULSE_WIDTH_USEC);  //set it to 1 to collect parallel data, wait
  digitalWrite(Button_LatchPin, LOW);  //set it to 0 to transmit data serially

  // while the shift register is in serial mode : Loop to read each bit value from the serial out line
  for(byte i = 0; i < DATA_WIDTH; i++)
  {
      bitVal = digitalRead(Button_DataPin);

      // Set the corresponding bit in bytesVal.
      bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

      // Pulse the Clock (rising edge shifts the next bit).
      digitalWrite(Button_ClockPin, HIGH);
      delayMicroseconds(PULSE_WIDTH_USEC);
      digitalWrite(Button_ClockPin, LOW);
  }
  return(bytesVal);
}


void Switch_Manager_checkButtons()
{
  static long lasttime;
  static byte previousstate[DATA_WIDTH];
  static byte currentstate[DATA_WIDTH];
  BYTES_VAL_T bytesVal = 0;
  
  // Initialise des etats temporaires
  for (byte i = 0; i < DATA_WIDTH; i++) {
    justpressed[i] = LOW;
    justreleased[i] = LOW;
  }
  
  // Debounce routine
  if (millis() < lasttime) {
    // we wrapped around, lets just try again
    lasttime = millis();
  }
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
 
  //Serial.println("START Analyse des boutons.");
  //Serial.println("--> Lecture des registres ...");
  // Switch in :
  bytesVal = Switch_Manager_readRegisters();
  //Serial.print("--> Donnees en sortie des registres : ");
  //Serial.println(bytesVal,BIN);
  
  // BitButton converti le bon bit
  for(byte i = 0; i < DATA_WIDTH; i++){
    pressed[i] = bitRead(bytesVal, bitButton[i]);
  }
  //Serial.println("--> Donnees traduites : ");
  //Serial.print("pressed[] = ");
  //for(byte i = 0; i < DATA_WIDTH; i++){
  // Serial.print(pressed[i],BIN);
  //  if (i < DATA_WIDTH - 1) Serial.print(";");
  //}
  //Serial.print("\n");
  
  // Enrichissement des données
  for (byte i = 0; i < DATA_WIDTH; i++) {
    if (pressed[i] != previousstate[i]) {
      if (pressed[i]) justpressed[i] = HIGH;
      else justreleased[i] = HIGH; 
    }
    previousstate[i] = pressed[i];
  }
  
  //Serial.println("--> Donnees enrichies : ");
  //Serial.print("justpressed[] = ");
  //for(byte i = 0; i < DATA_WIDTH; i++){
  //  Serial.print(justpressed[i],BIN);
  //  if (i < DATA_WIDTH - 1) Serial.print(";");
  //}
  //Serial.print("\n");
  //Serial.print("justreleased[] = ");
  //for(byte i = 0; i < DATA_WIDTH; i++){
  //  Serial.print(justreleased[i],BIN);
  //  if (i < DATA_WIDTH - 1) Serial.print(";");
  //}
  //Serial.print("\n");
  //Serial.println("STOP Analyse des boutons.");
  //Serial.println();
}

