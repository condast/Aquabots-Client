#include <ArduinoJson.h>

#include "WebClient.h"
#include "Registration.h"
#include "TinyGPS.h"
#include "Vessel.h"
#include "ServoController.h"
#include "Interrupts.h"
#include "Options.h"
#include "Logger.h"
#include "Data.h"
#include "IMU_10DoF.h";

#define VESSEL_ID F("org.rdm.coe.shuang.ma")
#define VESSEL F("Shuang Ma")
#define PASSPHRASE F("ShuangMeGood")
#define LATITUDE 51.2
#define LONGITUDE 4.2
#define TIME_OUT 3000 //msec
#define REFRESH 10

static WebClient webClient;
static Registration registration;
static TinyGPS gps;
static Imu10DoF imu10dof;
static Vessel vessel;
static Interrupts interrupt;
static Options options;
static Logger logger;
static Data data;

long vesselId;
int load;

void setup() {
  Serial.begin(9600);
  Serial.print(F("Setup Vessel: ")); Serial.println( VESSEL );
  vesselId = -1;
  webClient.setup();
  interrupt.setup();
  registration.setup();
  gps.setup();
  imu10dof.setup();
  options.setup();
  logger.setup();
  vessel.setup();
  load = 0;
}

void loop() {
  gps.loop( vesselId >= 0);
  imu10dof.loop();
  if ( interrupt.getSecondsFlank()) {
    interrupt.clearSecondsFlank();
    load = ( load + 1 ) % 120;
    int balance = ( vesselId <0)? 0: load%REFRESH;
    //Serial.println( balance );
    switch ( balance ) {
      case 0:
        if ( vesselId < 0 ) {
          vesselId =  registration.registerVessel( VESSEL, PASSPHRASE, gps.getLatitude(), gps.getLongitude() );
          if ( vesselId > 0 ) {
            Serial.print(F("REGISTERED VESSEL: ")); Serial.println( vesselId );
            webClient.setAuthentication( vesselId, PASSPHRASE );
          }else
            Serial.print(F("REGISTRATION FAILED: ")); Serial.println( vesselId );
        } else {
          //Serial.print(F("VESSEL: ")); Serial.println( vesselId );
          //vessel.loop( gps.getBearing());
        }
        delay(1000);
        break;
      case 1:
        logger.setup();
        //Serial.println( "LOGGER SETUP COMPLETE" );
        break;
      case 2:
        //Serial.println( "Aquabots message" );
        logger.println("HELLO AQUABOTS");
        break;
      case 9:
        options.getOptions();
        //Serial.println( "OPTIONS RECEIVED" );
        break;
      default:
        break;
    }
  }
 }
