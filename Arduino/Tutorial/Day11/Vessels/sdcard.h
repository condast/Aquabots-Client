#ifndef SDCard_h
#define SDCard_h

#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

#define PIN_SD_CARD_CS 4 //Chip Select for Ethernet Shield 2
#define PIN_SD_CARD_W5100_SEL 53 //disable W5100, pin 10 on Uno, pin 53 on Mega

#define CONFIG_FILE F("config.ini")
#define CACHE F("cache.chc")

#define NAME F("NAME")

class SDCard {

  public: SDCard(void);
    void setup();

    String readConfig( String key );
    void writeConfig( String key, String value );

    String getCacheLocation();
    String readCache( String key );
    void writeCache( String key, String value );

    String read(  String fileName, String key );
    bool readBoolean( String fileName, String key );
    void write( String fileName, String key, String val );
    void writeToIndex( String fileName, int index, String val );
    void print( String fileName );
    void toByteArray( byte results[], String line, int base );

  private: File file;
    String scan( File file );
};

#endif
