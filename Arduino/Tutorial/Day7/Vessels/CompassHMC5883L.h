#ifndef CompassHMC5883L_h
#define CompassHMC5883L_h

#define HMC5883L_COMPASS_ID F("org.rdm.aquabots.compass.seeed.hmc5883l")
#define HMC5883L_COMPASS F("Seeed Compass Module")
#define HMC5883L_COMPASS_DATA F("compass-data")

#define DECLINATION_ANGLE 0.02565634 //1.47

// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

class CompassHMC5883L {
  private:
    HMC5883L compass;
    float mraw[3];
    float mscaled[3];
    float declinationAngle;
    int error;
    double heading;
    void Output(float heading, float headingDegrees);
  public: CompassHMC5883L(void);
    void setup();
    double getHeading();
    void loop();
};

#endif
