LatLng::LatLng() {};

  /**
   * Get the heading in radians between the given points.
   * Change direction to get bearing 0 going north (radians)
   * @param latlng1
   * @param latlng2
   * @return
   * @Deprecated. THis doesn't seem to work well. Use getDirection
   */
  double LatLng::getHeading( Location latlng1, Location latlng2 ){
    double lat1 = toRadians( latlng1.latitude);
    double lat2 = toRadians( latlng2.latitude);
    double londiff = toRadians( latlng2.longitude - latlng1.longitude );
    double y = sin(londiff) * cos(lat2);
    double x = cos(lat1)*sin(lat2) -
            sin(lat1)*cos(lat2)*cos(londiff);
    double result = atan2(y, x);
    return abs(result);
  }

/**
   Get the distance between the given points
   @param latlng1
   @param latlng2
   @return
*/
double LatLng::getDistance( Location location1, Location location2 ) {
  double lat1 = toRadians( location1.latitude);
  double lat2 = toRadians( location2.latitude);
  double dlat = toRadians( location2.latitude - location1.latitude);
  double dlon = location2.longitude - location1.longitude;
  return haversine(lat1, lat2, dlat, toRadians( dlon ), 0, 0);
}

double LatLng::toRadians( int degree ) {
  return RADIAN_CONVERSION * degree;
}

int LatLng::toDegrees( double radians ) {
  return (int)( radians / RADIAN_CONVERSION );
}

double LatLng::haversin(double val) {
  return pow(sin(val / 2), 2);
}

double LatLng::haversine( double rlat1, double rlat2, double dLat, double dLon, double el1, double el2) {
  double a = haversin(dLat) + cos(rlat1) * cos(rlat2) * haversin(dLon);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EARTH_RADIUS * c;
  double height = el1 - el2;
  distance = pow(distance, 2) + pow(height, 2);
  return sqrt(distance);
}

double LatLng::lngDistance( double lat1, double lon1, double lat2, double lon2, double el1, double el2 ) {

  double rlat1 = toRadians( lat1 );
  double lonDistance = toRadians( lon2 - lon1);
  double lngdistance = lon1 - lon2;
  double sign = (lngdistance < 0.01d) ? -1 : 1;
  return sign * haversine( rlat1, rlat1, 0, lonDistance, el1, el2 );
}

/*
   Calculate distance between two points in latitude and longitude taking
   into account height difference. If you are not interested in height
   difference pass 0.0. Uses Haversine method as its base.

   lat1, lon1 Start point lat2, lon2 End point el1 Start altitude in meters
   el2 End altitude in meters
   @returns Distance in Meters
*/
double LatLng::latDistance( double lat1, double lon1, double lat2, double lon2, double el1, double el2 ) {

  double rlat1 = toRadians( lat1 );
  double rlat2 = toRadians( lat2 );
  double latdistance = lat2 - lat1;
  double sign = (latdistance < 0.01d ) ? -1 : 1;
  double rlatDistance = toRadians(lat2 - lat1);

  return sign * haversine( rlat1, rlat2, rlatDistance, 0, el1, el2 );
}
