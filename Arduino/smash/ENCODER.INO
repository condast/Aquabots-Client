
Encoder::Encoder() {};

void Encoder::setup() {
  pinMode(ENC_CS,    OUTPUT);
  pinMode(ENC_CLOCK, OUTPUT);
  pinMode(ENC_DI,    OUTPUT);
  pinMode(ENC_DO,    INPUT);
  pinMode(PIN_NORTH, INPUT);

  north_flank = readNorthPin();

  //Serial.println(F("ENCODER: checking... "));
  enable = sdcard.readBoolean( CONFIG, ENCODER );
  Serial.print(F("ENCODER: enabled = ")); Serial.println( enable);
  enc_sel = false;//select encoder activates cs
  enc_data = 0;
  enc_msb = 0;
  enc_status = 0;
  enc_current = 0;
  enc_total = 0;
  enc_counter = 0;
  enc_clock = false;
  result = 0;

  digitalWrite(ENC_CLOCK, HIGH);
  digitalWrite(ENC_CS, HIGH);

  trim = sdcard.read( CONFIG, ENCODER_TRIM ).toInt();//needs to be set
  gearTrim = sdcard.read( CONFIG, ENCODER_GEAR_TRIM ).toInt();//needs to be set
  average = sdcard.read( CONFIG, ENCODER_AVERAGE ).toInt();//needs to be set
  if ( average == 0 ) average = 20;
  delay = sdcard.read( CONFIG, ENCODER_DELAY ).toInt();//needs to be set
  if ( delay == 0 ) delay = 10;
  offset = sdcard.read( sdcard.getCacheLocation(), ENCODER_OFFSET ).toInt();//needs to be set
  Serial.print(F("OFFSET = ")); Serial.println( offset );
  Serial.print(F("GEAR TRIM = ")); Serial.println( gearTrim );
  Serial.print(F("TRIM = ")); Serial.println( trim );
  Serial.print(F("AVERAGE = ")); Serial.println( average );
  Serial.print(F("DELAY = ")); Serial.println( delay );
  Serial.println(F("ENCODER INITIALISED "));
}

bool Encoder::isEnabled() {
  return enable;
}

bool Encoder::isReady() {
  return (offset > 0);
}

bool Encoder::readNorthPin() {
  int counter = 0;
  int counts = 0;
  while ( counter < NORTH_PIN_DELAY ) {
    counts += digitalRead( PIN_NORTH );
    counter++;
  }
  return ( counts > 7 );
}

int Encoder::getEncoder() {
  return enc_total;
}

int Encoder::getMaxCount() {
  return (int)(ENC_COUNT * ENC_GEAR) + gearTrim;
}

/**
   Initialise the offset
*/
bool Encoder::initOffset() {
  loop();
  bool north = readNorthPin();
  bool ready = false;
  north_flank ^= north;
  //Serial.print( enc_total ); Serial.println(F(" INIT OFFSET"));
  if ( north_flank && !north ) {
    offset = (offset == 0 ) ? enc_total : (( AVERAGE_OFFSET - 1) * offset + enc_total) / AVERAGE_OFFSET;
    Serial.print(F("Offset: ")); Serial.print( enc_data ); Serial.print(F( ": ")); Serial.println( offset );
    ready = true;
  }
  north_flank = north;
  return ready;
}
bool Encoder::toggle() {
  enc_sel = !enc_sel;
  digitalWrite( ENC_CS, enc_sel );
  return enc_sel;
}

int Encoder::clip( int value ) {
  int max = getMaxCount();
  return ( max + value ) % max;
}

/**
   Get the difference (in counts), with the given bearing(radians)
*/
int Encoder::getDifference( double bearing ) {
  int max = getMaxCount();
  int direction = max * bearing / (2 * M_PI);
  int diff = clip( enc_total - getOffset() - direction);
  if ( diff > max / 2 )
    diff -= max;

  //if (( diff > -20) && ( diff < 20 )) {
  //  Serial.print(F(" Bearing: ")); Serial.print( bearing ); Serial.print(F(" Direction: ")); Serial.print( direction); Serial.print(F(" Offset: ")); Serial.print( offset );
  //  Serial.print(F(" Count: ")); Serial.print( enc_total ); Serial.print(F(" Difference: ")); Serial.println( diff );
 // }
  return diff;
}

/**
   Get the difference (in percent), with the given bearing( degress )
   Because the diff works between <-MAX_COUNT/2, MAX_COUNT/2> the result is multiplied with 200
*/
double Encoder::getDifferenceInPercent( double bearing ) {
  double diff = getDifference( bearing * DEG_TO_RAD );
  return diff * 200 / getMaxCount();
}

int Encoder::getOffset() {
  return offset + trim;
}

void Encoder::syncOffset() {
  sdcard.write( CONFIG, ENCODER_OFFSET, String( offset ));
}

void Encoder::loop() {
  digitalWrite(ENC_CLOCK, HIGH);
  digitalWrite(ENC_CS, HIGH);
  digitalWrite(ENC_CS, LOW);
  for ( int i = 0; i < ENCODER_DELAY; i++) {
    /*wait*/
  }

  //first read the encoder. Store the current value
  enc_current = enc_data;
  enc_data = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(ENC_CLOCK, LOW);
    digitalWrite(ENC_CLOCK, HIGH);
    enc_data <<= 1;
    enc_data += digitalRead( ENC_DO );
  }

  //update the status
  enc_status = 0;
  for (int i = 0; i < 6; i++) {
    digitalWrite(ENC_CLOCK, LOW);
    digitalWrite(ENC_CLOCK, HIGH);
    enc_status <<= 1;
    enc_status += digitalRead(ENC_DO );
  }
  digitalWrite(ENC_CLOCK, HIGH);
  digitalWrite(ENC_CS, HIGH);

  //return if the status is bad
  byte mask = enc_status & ENC_STATUS_PARITY;
  if (mask > 0  ) {
    Serial.println(F( "PARITY CHECK FAILURE, PROBABLY EM PULSE " ));
    return;
  }

  mask = enc_status & ENC_STATUS_CORDIC;
  if (mask > 0  ) {
    Serial.println(F( "CORDIC FAILURE, PROBABLY EM PULSE " ));
    return;
  }

  mask = ( enc_status & ENC_STATUS_LINEAR);
  if ( mask  ) {
    Serial.println(F( "LINEARITY ALARM " ));
    return;
  }

  //Serial.print(F(" Data ")); Serial.print( enc_data); Serial.print(F(" Status" )); Serial.println( enc_status );
  result = clip(( enc_total + (average - 1) * result) / average);
  //Serial.print(F("Count: ")); Serial.print( enc_data ); Serial.print(" "); Serial.print( enc_total ); Serial.print(" Status"); Serial.println( msk );

  //Check for an 'UP' or 'DOWN' transition
  bool transition = (( enc_current - enc_data ) > DIFF_ENCODER ) || (( enc_data - enc_current ) > DIFF_ENCODER );
  if ( !transition ) {
    enc_total = enc_msb + enc_data;
    enc_total = clip( enc_total );
    return;
  }
  //Serial.println(); Serial.print(F("Transition "));

  int up = ( enc_status & ENC_STATUS_UP);
  int down = ( enc_status );

  if ( enc_data < enc_current ) {
    enc_msb += ENC_COUNT;
    //Serial.print(F("UP ")); Serial.print( up ); Serial.print(F(" MASK ")); Serial.println( enc_msb);
  } else {
    enc_msb -= ENC_COUNT;
    //Serial.print(F("DOWN ")); Serial.print( down ); Serial.print(F(" MASK ")); Serial.println( enc_msb );
  }
  //Serial.print(F(" TOTAL ")); Serial.print( enc_total );
  enc_msb = clip(enc_msb);
  enc_total = enc_msb + enc_data;
  enc_total = clip( enc_total );
  //Serial.print(F(" RESULT ")); Serial.print( enc_total ); Serial.print(F(" DATA ")); Serial.println( enc_data );
}

void Encoder::debug() {
  if (!enable)
    return;
  String sentence = F("[{count:");
  sentence += enc_current;
  sentence += F(", offset:");
  sentence += offset;
  sentence += F(",total:");
  sentence += enc_total;
  sentence += F("}]");
  logger.print(F("ENCODER Connected")); logger.println( sentence);
  data.send( ENCODER_ID, ENCODER, WebClient::DATA, F("String"), sentence );
}

void Encoder::log() {
  if (( decoder ^ encoder.getEncoder() ) > 0) {
    decoder = encoder.getEncoder();
    logger.print(F("DECODER: " )); logger.println( String( decoder ));
  }
}
