#ifndef Data_h
#define Data_h

#include "Arduino.h"

class Data {

  public: Data();
    void send( String deviceId, String deviceName, int request, String type, String sentence );
    void sendNMEA( String deviceId, String deviceName, String sentence );
};

#endif
