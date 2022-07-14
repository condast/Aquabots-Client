#ifndef Vessel_h
#define Vessel_h

#include "Vessel.h"

#include "ServoController.h"
#include "doublePropellor.h"

#define AUTONOMY F("AUTONOMY")
#define WAYPOINTS_FILE F("waypoints.cfg")

#define LOOP_TIMEOUT 6000 //6 sec to cut the engine
#define MAX_WAYPOINTS 6 //The maximum amount of waypoints that can be stored
#define DEFAULT_RANGE 12//12 mtrs

#define THRUST_ALARM 30 //thrust when something goes wrong

#define DISTANCE_COMPLETE 3 //3 metres

class Vessel {

    enum ReturnStrategy {
      STOP,
      RETURN_TO_FIELD_COORDINATES
    };

    /**
       Vessel Data object
    */
    struct VesselData {
      int time;
      double latitude;//current position
      double longitude;
      double bearing;
      double thrust;
      bool active;
      bool manual;
      int returnStrategy;
    };

    struct Waypoint {
      int index;
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
    void setAutonomy( bool autonomy );
    bool update( double latitude, double longitude, double voltage, bool updated );
    bool getWaypoint();
    void loop( double heading);
    void interrupt( unsigned int loopCounter );
    void stop();

  private:
    bool enable;
    bool initialised;
    double heading;
    double speed;
    unsigned int range;//The range in between which a location has been reached
    VesselData data;

    Waypoint current;
    Waypoint waypoints[ MAX_WAYPOINTS ];
    unsigned int waypointIndex;
    DoublePropellor controller;
    bool runMaintenance( unsigned int counter );
    bool updateWaypoints( int index );
    void connectionLoss();
    void complete();
};

#endif
