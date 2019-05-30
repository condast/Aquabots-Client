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

CompassHMC5883L::CompassHMC5883L() {};

// Out setup routine, here we will configure the microcontroller and compass.
void CompassHMC5883L::setup()
{
  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  Serial.println("Constructing new HMC5883L");

  Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.setScale(1.3); // Set the scale of the compass.
  if (error != 0) // If there is an error, print it out.
  Serial.println(compass.getErrorText(error));

    Serial.println("Setting measurement mode to continous.");
    error = compass.setMeasurementMode(MEASUREMENT_CONTINUOUS); // Set the measurement mode to Continuous
    if (error != 0) // If there is an error, print it out.
    Serial.println(compass.getErrorText(error));
    }

// Our main program loop.
void CompassHMC5883L::loop()
{
  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.readRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.readScaledAxis();

  // Values are accessed like so:
  mraw[0] = raw.XAxis;
  mraw[1] = raw.YAxis;
  mraw[2] = raw.ZAxis;

  mscaled[0] = scaled.XAxis;
  mscaled[1] = scaled.YAxis;
  mscaled[2] = scaled.ZAxis;

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  heading = atan2(scaled.YAxis, scaled.XAxis);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -2��37' which is -2.617 Degrees, or (which we need) -0.0456752665 radians, I will use -0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = DECLINATION_ANGLE;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if (heading < 0)
    heading += 2 * PI;

  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;

  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180 / M_PI;

  // Output the data via the serial port.
  Output(heading, headingDegrees);

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  delay(66);//of course it can be delayed longer.
}

// Convert radians to degrees for readability.
double CompassHMC5883L::getHeading() {
  return (heading * 180 / M_PI);
}

// Output the data down the serial port.
void CompassHMC5883L::Output(float heading, float headingDegrees)
{
  //Serial.print("Compass: "); Serial.println( getHeading() );
  String str = "c=" + String( getHeading()) + ";";
  str+= "r=" + String( mraw[0] ) + "," + String( mraw[1] )+ "," + String( mraw[2] ) + ";";
  str+= "s=" + String( mscaled[0] )+ "," + String( mscaled[1] ) + "," + String( mscaled[2] );
  data.send(WebClient::DATA, HMC5883L_COMPASS_ID, HMC5883L_COMPASS, HMC5883L_COMPASS_DATA, str );
}
