#ifndef WebClient_h
#define WebClient_h

#include "Arduino.h"
#include <SPI.h>
#include <Ethernet2.h>

const unsigned long HTTP_TIMEOUT = 10000;// max respone time from server

const char server[] = "www.condast.com";
IPAddress ip(79, 170, 90, 5);
const int PORT = 8080;
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
const byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

/*
  Web client

  This sketch connects to a website
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe, based on work by Adrian McEwen

*/

class WebClient {

  public: WebClient();
    enum request {
      UNKNOWN = 0,
      DEBUG = 1,
      FIELD = 2,
      UPDATE = 3,
      WAYPOINTS = 4,
      DATA = 5,
      NMEA = 6,
      OPTIONS = 7,
      LOG = 8
    };

    EthernetClient client;
    void setup( String file);
    bool connect();
    void disconnect();
    bool requestLog();
    bool logMessage( String message );
    bool getWaypoint();
    bool sendUpdate( String url );
    bool sendHttp( int request, String msg );
    bool sendHttp( int request, boolean post, String attrs );
    String urlencode(String str);
    String printResponse( int request );
    void logRequest( int request, boolean post, String attrs );//for debugging
    void logRequestStr( int request ); //dito
    void loop();

  private:

    int port;
    String host;
    bool connected;
    String context;
    String id;
    int token;

   // Initialize the Ethernet client library
    // with the IP address and port of the server
    // that you want to connect to (port 80 is default for HTTP):
    void requestService( int request );
    bool processResponse( int request );
    boolean update( JsonObject& root );
    String urldecode(String str);
    unsigned char h2int(char c);
};

#endif
