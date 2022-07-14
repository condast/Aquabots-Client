/*****************************************************************************/
//	Function:	 Get the Geographic direction of the X-axis.
//				If X-axis points to the North, it is 0 degree.
//				If X-axis points to the East, it is 90 degrees.
//				If X-axis points to the South, it is 180 degrees.
//				If X-axis points to the West, it is 270 degrees.
//  Hardware:   Grove - 3-Axis Digital Compass
//	Arduino IDE: Arduino-1.0
//	Author:	 Frankie.Chu
//	Date: 	 Jan 10,2013
//	Version: v1.0
//	by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

// Reference the I2C Library
#include <Wire.h>
#include "CompassBMM150.h"

CompassBMM150::CompassBMM150() {};

// Out setup routine, here we will configure the microcontroller and compass.
void CompassBMM150::setup()
{
  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new BMM150");

  if (compass.initialize() == BMM150_E_ID_NOT_CONFORM) {
    Serial.println("Chip ID can not read!");
    while (1);
  } else {
    Serial.println("Initialize done!");
  }
}

// Our main program loop.
void CompassBMM150::loop()
{
  compass.read_mag_data();

  value.x = compass.raw_mag_data.raw_datax;
  value.y = compass.raw_mag_data.raw_datay;
  value.z = compass.raw_mag_data.raw_dataz;

  float xyHeading = atan2(value.x, value.y);
  float zxHeading = atan2(value.z, value.x);
  heading = xyHeading;

  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
}

// Convert radians to degrees for readability.
double CompassBMM150::getHeading() {
  return (heading * 180 / M_PI);
}

// Output the data down the serial port.
void CompassBMM150::output()
{
  float xyHeading = atan2(value.x, value.y);
  float zxHeading = atan2(value.z, value.x);
  float headingDegrees = heading * 180/M_PI;
  float xyHeadingDegrees = xyHeading * 180 / M_PI;
  float zxHeadingDegrees = zxHeading * 180 / M_PI;
  //Serial.print("Compass: "); Serial.println( getHeading() );
  String str = "c=" + String( headingDegrees) + ";";
  str += "r=" + String( value.x ) + "," + String( value.y ) + "," + String( value.z );
  data.send(WebClient::DATA, BMM150_COMPASS_ID, BMM150_COMPASS, BMM150_COMPASS_DATA, str );
}
