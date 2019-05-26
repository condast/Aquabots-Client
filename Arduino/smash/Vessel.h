#ifndef Vessel_h
#define Vessel_h

#include "Arduino.h"
#include <Servo.h>

#define AUTONOMY F("AUTONOMY")
#define WAYPOINTS_FILE F("waypoints.cfg")

#define TURN F("TURN")
#define TURN_ACTIVE F("TURN_ACTIVE")
#define TURN_SPEED F("TURN_SPEED")//25
#define TURN_ANGLE F("TURN_ANGLE")//3
#define TURN_OFFSET F("TURN_OFFSET") //-5

#define SPEED_OFFSET F("SPEED_OFFSET")//-4
#define SPEED_OFFSET F("SPEED_OFFSET")//-4

#define MIN_SERVO 32
#define MAX_SERVO 160

//The PWM outpus are the orange connectors on the Ethernet shield
#define PWM_OUT_SPEED   2 //Yellow grove connection
#define PWM_OUT_BEARING 3 //White grove connection

class Vessel {

  public: Vessel();
    struct WayPoint {
      String name;
      double latitude;
      double longitude;
      int index;
      bool end;
    };

    void setup();
    bool maintenance;
    bool initialise();
    int setCourse( double bearing, double thrust );//returns true if the course is within the turn angle
    int setThrust( double thrust );
    bool getWaypoint();
    void setAutonomy( bool autonomy );
    bool update( double latitude, double longitude, double bearing, double speed, bool updated );
    bool updateWaypoints();
    void interrupt();
    void loop();

  private:
    bool enable;
    bool initialised;
    int counter;
    int angle;//0-360
    int thrust;//-100 to 100
    int turn; //-100 to 100
    int turn_active; //The min turn speed for the controller to respond
    int turn_offset; //offset for turn motor
    int turn_speed;//speed of turning
    int turn_angle;//the minimum turn angle for a fix
    int speed_offset; //offset for turn motor
    double bearing;//0-360
    double brng;
    double speed;

    LatLng::VesselData data;
    double latitude, longitude;

    Servo bearingServo;  // create servo object to control the bearing
    Servo speedServo;  // create servo object to control the speed
    int getTurnSpeed( int diff );
    int setTurn( double turn );//The speed of turning the motor
    bool runMaintenance( int counter );
};

#endif
