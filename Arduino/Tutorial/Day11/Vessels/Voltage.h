#ifndef Voltage_h
#define Voltage_h

#define VOLTAGE_SENSOR_ID F("org.rdm.aquabots.voltage.sensor")
#define VOLTAGE_SENSOR F("Voltage Sensor Module")
#define VOLTAGE_SENSOR_DATA F("voltage-data")

#define VOLTAGE F("VOLTAGE: ")
#define LOW_VOLTAGE F("LOW_VOLTAGE");
#define V_LOW F("LOW");

#define MAX_VOLTAGE 5.0
#define MIN_VOLTAGE 2.4

class Voltage {

  private:
    int analogPin = A1; //analog pin 3
    float val;

  public: Voltage(void);
    boolean setup();
    double getVoltage();
    double getVoltagePercent();
    boolean alarm();
    String getVoltageMsg();
    String getLowVoltageMsg();
    void loop();
};

#endif
