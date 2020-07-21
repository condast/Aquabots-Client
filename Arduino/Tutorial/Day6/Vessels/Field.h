#ifndef Field_h
#define Field_h

#include "Arduino.h"

class Field {

  public: Field(void);
    struct FieldData {
      String name;
      double latitude;
      double longitude;
    };
    boolean initialised;
    bool requestField(double latitude, double longitude);
    void setup();
    void loop(double latitude, double longitude);

  private: FieldData field;
};

#endif
