/**
 * Day 10
 */
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "SDCard.h"
#include "Config.h"
#include "Field.h"
#include "TinyGPSSensor.h"
#include "ServoController.h"
#include "Interrupts.h"
#include "Options.h"
#include "Logger.h"
#include "Data.h"
#include "CompassBMM150.h"
#include "IMU_10DoF.h"
#include "Voltage.h"
#include "Vessel.h"

#define VESSEL_ID F("org.rdm.coe.shuang.ma")
#define VESSEL F("Shuang Ma")
#define PASSPHRASE F("ShuangMeGood")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 3000 //msec
#define REFRESH 10

static WebClient webClient;
static SDCard sdcard;
static Registration registration;
static Field field;
static Config config;
static TinyGPSSensor gps;
static CompassBMM150 compassModule;
static Imu10DoF imu10dofModule;
static Interrupts interrupt;
static Options options;
static Logger logger;
static Data data;
static Voltage voltage;
static Vessel vessel;

long vesselId;
int load;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  vesselId = -1;
  webClient.setup();
  sdcard.setup();
  interrupt.setup();
  registration.setup();
  gps.setup();
  field.setup();
  compassModule.setup();
  imu10dofModule.setup();
  options.setup();
  logger.setup();
  vessel.setup();
  load = 0;
  logger.println(F("HELLO AQUABOTS"));
  config.send();
}

void loop() {
  bool enabled = ( vesselId >= 0);
  gps.loop( enabled );
  compassModule.loop();
  imu10dofModule.loop();
  if ( interrupt.getSecondsFlank()) {
    interrupt.clearSecondsFlank();
    load = ( load + 1 ) % 120;
    int balance = ( vesselId < 0) ? 0 : load % REFRESH;
    //Serial.println( balance );
    switch ( balance ) {
      case 0:
        if ( vesselId < 0 ) {
          vesselId =  registration.registerVessel( VESSEL, PASSPHRASE, gps.getLatitude(), gps.getLongitude() );
          if ( vesselId > 0 ) {
            Serial.print(F("REGISTERED VESSEL: ")); Serial.println( vesselId );
            webClient.setAuthentication( vesselId, PASSPHRASE );
          } else
            Serial.print(F("REGISTRATION FAILED: ")); Serial.println( vesselId );
        } else {
          //Serial.print(F("VESSEL: ")); Serial.println( vesselId );
          vessel.loop( gps.getHeading());
        }
        delay(1000);
        break;
      case 1:
        if ( enabled )
          field.loop( gps.getLatitude(), gps.getLongitude());
        logger.setup();
        vessel.update( gps.getLatitude(), gps.getLongitude(), voltage.getVoltage(), true );
        //Serial.println( "LOGGER SETUP COMPLETE" );
        break;
      case 2:
        //Serial.println( "Aquabots message" );
        voltage.loop();
        config.send();
        break;
      case 3:
        //Serial.println( "Aquabots message" );
        compassModule.output();
        String str = F("COMPASS:");
        str += compassModule.getHeading();
        logger.println( str );
        break;
      case 8:
        //if ( enabled )
         // vessel.getWaypoint();
        //Serial.println( "OPTIONS RECEIVED" );
        break;
      case 9:
        options.getOptions();
        //Serial.println( F("OPTIONS RECEIVED") );
        break;
      default:
        break;
    }
  }
}
