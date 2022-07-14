#define TRUE F("true");

SDCard::SDCard() {};

void SDCard::setup() {
  Serial.print(F("Initializing SD card..."));
  pinMode ( PIN_SD_CARD_CS , OUTPUT);    // pin 4 on Uno, pin 53 on Mega
  pinMode ( PIN_SD_CARD_W5100_SEL, OUTPUT);
  digitalWrite(PIN_SD_CARD_W5100_SEL, HIGH);
  if (!SD.begin(PIN_SD_CARD_CS))
    Serial.println(F("failed!"));
  else
    Serial.println(F("done."));
}

String SDCard::readConfig( String key ) {
  return read( CONFIG_FILE, key );
}

void SDCard::writeConfig( String key, String value ) {
  write( CONFIG_FILE, key, value );
}

String SDCard::getCacheLocation() {
  String cache = readConfig( NAME );
  cache += F("/");
  cache += CACHE;
  //Serial.println( cache );
  return cache;
}

String SDCard::readCache( String key ) {
  return read( getCacheLocation(), key );
}

void SDCard::writeCache( String key, String value ) {
  write( getCacheLocation(), key, value );
}

String SDCard::read( String fileName, String key ) {
  file = SD.open( fileName, FILE_READ);
  //Serial.print(F("Read File: " ));  Serial.println(fileName);

  // if the file opened okay, write to it:
  String line;
  if ( file) {
    while ( file.available() ) {
      line = scan( file );
      //Serial.print( line );
      if ( line.length() == 0 )
        continue;
      line.trim();
      //comments
      if ( line.startsWith("#" ))
        continue;
      if ( !line.startsWith( key ))
        continue;

      int index = line.indexOf("=");
      line = line.substring( index + 1, line.length());
      line.trim();
      break;
    }
    file.close();
  }
  return line;
}

bool SDCard::readBoolean( String fileName, String key ) {
  String true_str = TRUE;
  String result = read( fileName, key );
  //Serial.println( fileName );
  //Serial.print(F("RESULT: checking lowercase ")); Serial.print( key ); Serial.println( "=" );Serial.println( result );
  if ( true_str.equals( result ))
    return true;
  true_str.toUpperCase();
  // Serial.print(F("RESULT: checking uppercase ")); Serial.println( result );
  return true_str.equals( result );
}

String SDCard::scan( File file ) {
  String result;
  char chr;
  while ( file.available() && ( chr != 0x0A )) {
    chr = file.read();
    result += chr;
  }
  return result;
}

void SDCard::toByteArray( byte results[], String line, int base ) {
  String num;
  //Serial.print(F("CHAR line :")); Serial.println(line);
  int index = 0;
  for ( int i = 0; i < line.length(); i++ ) {
    if ( line.charAt(i) == '{' )
      continue;
    if (( line.charAt(i) != ',' ) && ( line.charAt(i) != '}' )) {
      num += line.charAt(i);
      continue;
    }
    num.trim();
    char temp[6];
    num.toCharArray( temp, 6 );
    results[index] = strtol( temp, NULL, base );
    index++;
    num = "";
  }
  Serial.print(line); Serial.print("-");
  for ( int j = 0; j < sizeof(results); j++ ) {
    Serial.print( results[j]);
  }
  Serial.println( );
}

void SDCard::write( String fileName, String key, String val ) {
  //SD.remove( fileName );
  Serial.print(F("ENTERING key: ")); Serial.print( key ); Serial.print(F("-")); Serial.println( val );
  file = SD.open( fileName, FILE_READ);

  // if the file opened okay, write to it:
  String buffer;
  bool found = false;
  if ( file) {
    while ( file.available()) {
      String line = scan( file );
      if ( line.length() == 0 )
        continue;
      line.trim();
      if ( line.length() == 0 )
        continue;
      int index = line.indexOf("=");
      String keyv = line.substring(0, index);
      if ( keyv.equals( key )) {
        line = line.substring( 0, index + 1 );
        line += val;
        found = true;
        Serial.print( "Replace line: " ); Serial.println( line );
      }
      line += F("\r\n");
      buffer += line;
    }
    if ( !found ) {
      buffer += key;
      buffer += F("=");
      buffer += val;
      buffer += F("\r\n");
      Serial.print("Adding line: "); Serial.println( key );
    }
    file.close();
  }
  SD.remove( fileName );
  file = SD.open( fileName, FILE_WRITE);
  file.print( buffer );
  file.close();
}

void SDCard::writeToIndex( String fileName, int index, String val ) {
  //SD.remove( fileName );
  Serial.print(F("ENTERING key: ")); Serial.print( index); Serial.print(F("-")); Serial.println( val );
  file = SD.open( fileName, FILE_READ);

  // if the file opened okay, write to it:
  String buffer;
  bool found = false;
  int ind;

  while ( file && file.available()) {
    String line = scan( file );
    if ( line.length() == 0 )
      continue;
    line.trim();
    if ( line.length() == 0 )
      continue;

    int strindex = line.indexOf("=");
    String keyv = line.substring(0, strindex);
    ind = keyv.toInt();
    if ( ind == index ) {
      buffer += index;
      buffer += "=";
      buffer += val;
      buffer += F("\r\n");
      found = true;
      Serial.print(F("Adding line: ")); Serial.println( index );
    } else {
      buffer += line;
    }
    buffer += F("\r\n");
  }
  if ( !found ) {
    buffer += String(index);
    buffer += F("=");
    buffer += val;
    buffer += F("\r\n");
    Serial.print(F("Adding line: ")); Serial.println( index  );
  }
  file.close();
  SD.remove( fileName);
  file = SD.open( fileName, FILE_WRITE);
  file.print( buffer );
  file.close();
}

void SDCard::print( String fileName ) {
  file = SD.open( fileName, FILE_READ);

  // if the file opened okay, write to it:
  if ( file) {
    char chr;
    while ( file.available()) {
      chr = file.read();
      Serial.print( chr );
    }
    file.close();
  }
}
