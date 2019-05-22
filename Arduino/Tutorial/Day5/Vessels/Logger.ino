//Constructor
Logger::Logger() {};

/**
   Set up the logger
*/
boolean Logger::setup( ) {
  logger = false;//webClient.requestLog();
  Serial.print(F( "SETUP Logger: " )); Serial.println( logger );
  return logger;
}

void Logger::setLogger( boolean choice ) {
  logger = choice;
  //Serial.print( "options request: "); Serial.println( String( choice ));
}

void Logger::print( String msg ) {
  if ( !logger )
    return;
  //Serial.print( "log request: "); Serial.println( msg );
  //options.getOptions( WebClient::LOG, msg);
}

void Logger::println( String msg ) {
  if ( !logger )
    return;
  //Serial.print( "log request: "); Serial.println( msg );
  //options.getOptions( WebClient::LOG, msg);
}

/**
   Send a request log message
*/
boolean Logger::requestLog() {
  return logMessage("");
}

/**
   send a log message
*/
boolean Logger::logMessage( String message ) {
  webClient.connect();
  //Serial.print(F( "log request: ")); Serial.println( message );
  boolean result = webClient.sendHttp( WebClient::LOG, false, message );
  webClient.disconnect();
  return result;
}
