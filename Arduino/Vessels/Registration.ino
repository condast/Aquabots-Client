Registration::Registration() {};

//Repeatedly feed it characters from your
void Registration::setup( ) {
  enabled = true;
}

long Registration::registerVessel( String name, String passphrase ) {
  if ( !enabled )
    return;
  if ( ! webClient.connect() )
    return false;

  String url = F("&name=");
  url += String( name );
  url += F("&passphrase=");
  url += String( passphrase);

  boolean result = webClient.sendHttp( WebClient::REGISTER_VESSEL, false, url);
  if (!result ) {
    webClient.disconnect();
    return false;
  }
  String retval = "";
  while (webClient.client.available()) {
    char c = webClient.client.read();
    retval += c;
  }
  vesselId = atol( retval.c_str() );
  webClient.disconnect();
  return vesselId;
}

bool Registration::getConfig() {
}
