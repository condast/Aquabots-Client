#ifndef Voltage_h
#define Voltage_h

#define VOLTAGE_SENSOR_ID F("org.rdm.aquabots.voltage.sensor")
#define VOLTAGE_SENSOR F("Voltage Sensor Module")
#define VOLTAGE_SENSOR_DATA F("voltage-data")

#define VOLTAGE_SCALER 13
#define MIN_VOLTAGE 4.0

class Voltage {

  private: 
  int analogPin = A0; //analog pin A0
  float val;

  public: Voltage(void);
    boolean setup();
    double getVoltage();
    boolean alarm();
    void loop();
};

#endif
