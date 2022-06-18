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

double Voltage::getVoltagePercent() {
  return (val * 100)/MAX_VOLTAGE;
}
/**
   Send a request log message
*/
boolean Voltage::alarm() {
  return ( val < MIN_VOLTAGE);
}

String Voltage::getVoltageMsg() {
  String msg = VOLTAGE;
  msg += F(": (");
  msg += String( voltage.getVoltage());
  msg += F(" V)");
  return msg;
}
String Voltage::getLowVoltageMsg() {
  String msg = V_LOW;
  msg += F(" ");
  msg += getVoltageMsg();
  return msg;
}

/**
   send a log message
*/
void Voltage::loop( ) {
  val = MAX_VOLTAGE * analogRead(analogPin) / 1023; // read the input pin
  Serial.print( VOLTAGE ); Serial.println(val);
  data.send(WebClient::DATA, VOLTAGE_SENSOR_ID, VOLTAGE_SENSOR, VOLTAGE_SENSOR_DATA, String( val ));
}
