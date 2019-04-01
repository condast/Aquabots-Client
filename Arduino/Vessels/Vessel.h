#ifndef Vessel_h
#define Vessel_h

#include <Servo.h>

#define AUTONOMY F("AUTONOMY")
#define WAYPOINTS_FILE F("waypoints.cfg")

#define SPEED_OFFSET F("SPEED_OFFSET")//-4
#define BEARING_OFFSET F("BEARING_OFFSET")//-4

#define PWM_OUT_SPEED   3 //Yellow grove connection (orange)
#define PWM_OUT_REFERENCE 7 //Yellow grove connection (blue) on D6
#define PWM_OUT_SPARE 6 //White grove connection (brown) on D7

#define MIN_SERVO 10   // 32 USV_RDM
#define MAX_SERVO 170 // 160 USV_RDM

#define LOOP_TIMEOUT 6000 //6 sec to cut the engine
#define MAX_WAYPOINTS 6 //The maximum amount of waypoints that can be stored
#define DEFAULT_RANGE 12;//12 mtrs

class Vessel {

    struct Waypoint {
      String name;
      double latitude;
      double longitude;
      bool complete;
      bool last;
    };

  public: Vessel();

    void setup();
    bool maintenance;
    bool initialise();
    void setCourse( double bearing, double thrust );//returns true if the course is within the turn angle
    int setThrust( double thrust );
    void setAutonomy( bool autonomy );
    bool update( double latitude, double longitude, double bearing, double speed, bool updated );
    bool getWaypoint();
    void loop( double bearing);
    void interrupt( unsigned int loopCounter );
    void stop();

  private:
    bool enable;
    bool initialised;
    int thrust;//0-100%
    double bearing;//0-360
    double speed;
    int speed_offset; //offset for speed motor
    unsigned int range;//The range in between which a location has been reached

    LatLng::VesselData data;
    Waypoint waypoints[ MAX_WAYPOINTS ];
    unsigned int waypointIndex;

    Servo referenceServo; //The Anna uses two motors, of which this one controls the reference
    Servo speedServo;  // create servo object to control the speed
    bool runMaintenance( unsigned int counter );
    bool updateWaypoints();
};

#endif
