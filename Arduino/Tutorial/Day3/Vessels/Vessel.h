#ifndef Vessel_h
#define Vessel_h


#define AUTONOMY F("AUTONOMY")
#define WAYPOINTS_FILE F("waypoints.cfg")

#define LOOP_TIMEOUT 6000 //6 sec to cut the engine
#define MAX_WAYPOINTS 6 //The maximum amount of waypoints that can be stored
#define DEFAULT_RANGE 12;//12 mtrs

class Vessel {
    /**
       Vessel Data object
    */
    struct VesselData {
      int time;
      double latitude;//current position
      double longitude;
      double heading;
      double thrust;
      bool manual;
    };
    
    struct Waypoint {
      String name;
      double latitude;
      double longitude;
      bool complete;
      bool last;
    };

  public: Vessel();

    void setup();
    void setCourse( double heading, double thrust );//returns true if the course is within the turn angle
    bool update( double latitude, double longitude, double bearing, double speed, bool updated );
    void loop( double bearing);
    void stop();

  private:
    bool enable;
    bool initialised;
    double bearing;//0-360
    double speed;
    unsigned int range;//The range in between which a location has been reached
    VesselData data;
};

#endif
