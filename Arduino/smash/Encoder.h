#ifndef Encoder_h
#define Encoder_h
#include "math.h"
#include "Arduino.h"

/**
   See: http://forum.arduino.cc/index.php?topic=110250.0
*/
#define ENC_OFFSET -5 //correction to align the encoder zero position with the motor position 
#define ENC_COUNT 1024// Encoder max count
#define ENC_DATA 10// Encoder max count

#define ENCODER_ID         F("encoder.bourne.1024")
#define ENCODER            F("ENCODER")
#define ENCODER_OFFSET     F("ENCODER_OFFSET")
#define ENCODER_TRIM       F("ENCODER_TRIM")
#define ENCODER_GEAR_TRIM  F("ENCODER_GEAR_TRIM")
#define ENCODER_AVERAGE    F("ENCODER_AVERAGE")
#define ENCODER_DELAY      F("ENCODER_DELAY")

#define ENC_STATUS_PARITY 0x100000
#define ENC_STATUS_DOWN   0x010000
#define ENC_STATUS_UP     0x001000
#define ENC_STATUS_LINEAR 0x000100
#define ENC_STATUS_CORDIC 0x000010
#define ENC_STATUS_EOOFF  0x000001

//The I/O for the encoder
#define PIN_NORTH 8// yellow
#define ENC_CLOCK 4//4 yellow( pin 2)
#define ENC_CS    5//4 white(pin 6)
#define ENC_DO    6//5 yellow ( pin 4)
#define ENC_DI    7//4 white ( pin 1)
//pin 3 gnd, pint 5 rd, vcc

//The max diff between two reads. Used to determine transitions
#define DIFF_ENCODER 0xFF

//Averaging function for the encoder
#define AVERAGE_OFFSET 10
#define NORTH_PIN_DELAY 20

const static double ENC_GEAR = 4; // 40/10the amount of full turns of the encoder for one turn of the motor

class Encoder {

  public: Encoder( void );
    void setup();
    bool readNorthPin();
    int getOffset();
    int getMaxCount();
    int getEncoder();
    double getDifferenceInPercent( double bearing );
    bool isEnabled();
    bool isReady();
    bool initOffset();
    void syncOffset(); //sync with sd card
    bool toggle();//used by interrupt routine
    void loop();
    void debug();
    void log();

  private:
    bool enable;
    bool enc_sel;
    int enc_data;
    int enc_status;
    int enc_msb;
    int enc_counter;
    bool enc_clock;
    int enc_current;
    int enc_total;//total count after counting
    int result;//average
    bool north_flank;
    int decoder;
    int offset; //correction for north bearing
    int gearTrim;//additional correction to be set manually
    int average;
    int delay;
    int trim;//additional correction to be set manually
    int getDifference( double bearing );
    int clip( int value );
};

#endif


