//#include "Compass.h"

#ifndef ServoController_h
#define ServoController_h

#define SERVO_ID "org.rdm.aquabots.motion.servo"
#define SERVO "Servo Controller"

#define SERVO_RANGE 45 //The clip range of the servo (45-135 in this case)
#define TURN F("TURN")
#define TURN_OFFSET F("TURN_OFFSET") //-5
#define CLIP_RANGE F("CLIP_RANGE")

//The PWM outpus are the orange connectors on the Ethernet shield
#define PWM_OUT_BEARING 2 //White grove connection  (green)

class ServoController {

  public: ServoController(void);
    virtual bool isReady();
    virtual void setup();
    virtual bool initialise();
    virtual bool maintenance();
    virtual void stop();
    virtual void toggle();
    virtual void loop( double bearing);

    /**
      get the angle for the servo, based on the desired bearing( 0-360 )
    */
    virtual void setBearing( double bearing );

  protected:
    //Compass compass;
    Servo bearingServo;  // create servo object to control the bearing
    bool initialised;
    bool ready;
    bool enable;
    //the offset between the bearing and the desired angle. Depends on the servo that is used
    //and/or the mounitng of the compass
    int offset;
    double bearing;//The new bearing
    double heading;//The actual position  at a given time

  private:
    int clipRange;
};

#endif
