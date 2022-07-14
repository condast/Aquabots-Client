//Constructor
Voltage::Voltage() {};

/**
   Set up the Voltage
*/
boolean Voltage::setup( ) {
   Serial.print(F( "SETUP Voltage: " )); Serial.println( val );
  return val;
}

double Voltage::getVoltage() {
  return val;
}

/**
   Send a request log message
*/
boolean Voltage::alarm() {
  return ( val < MIN_VOLTAGE);
}

/**
   send a log message
*/
void Voltage::loop( ) {
  val = 5.0 * analogRead(analogPin)/1023;  // read the input pin
  Serial.print( "VOLTAGE: ");Serial.println(val);
  data.send(WebClient::DATA, VOLTAGE_SENSOR_ID, VOLTAGE_SENSOR, VOLTAGE_SENSOR_DATA, String( val ));
}
