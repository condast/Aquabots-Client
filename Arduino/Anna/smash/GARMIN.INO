  Garmin::Garmin() {
};

//Repeatedly feed it characters from your
void Garmin::setup( ) {
  enabled = sdcard.readBoolean( CONFIG, GARMIN );
  Serial.print(F("GARMIN: enabled = ")); Serial.println( enabled);
  if (!enabled )
    return;
  Serial2.begin(4800);
  Serial2.println(F("Polling Garmin"));
}

void Garmin::loop() {
  if (!enabled )
    return;
  String sentence;
  boolean send = false;
  char chr;
  do {
    if (Serial2.available()) {
      chr =  Serial2.read();
      if ( chr == 0x24 )
        send = true;
      if ( send )
        sentence += chr;
    }
  }
  while (( sentence.length() > 0 ) && ( chr != 0x0A ));
  if ( sentence.length() == 0 )
    return;
  logger.print("GARMIN Connected"); logger.println( sentence);
  data.sendNMEA( GARMIN_ID, GARMIN, sentence );
}

