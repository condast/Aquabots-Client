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
    void setup();
    void loop();

  private: FieldData field;
    bool requestField();
};

#endif


