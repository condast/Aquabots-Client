#ifndef CompassBMM150_h
#define CompassBMM150_h

#define BMM150_COMPASS_ID F("org.rdm.aquabots.compass.seeed.BMM150")
#define BMM150_COMPASS F("Seeed Compass Module")
#define BMM150_COMPASS_DATA F("compass-data")

#define DECLINATION_ANGLE 0.02565634 //1.47

// Reference the BMM150 Compass Library
#include "bmm150.h"
#include "bmm150_defs.h"

class CompassBMM150 {
  private:
    BMM150 compass;
    bmm150_mag_data value;
    int error;
    double heading;
    void Output(float heading, float headingDegrees);
  public: CompassBMM150(void);
    void setup();
    double getHeading();
    void loop();
};

#endif
