//Make sure to install the adafruit GPS library from https://github.com/adafruit/Adafruit-GPS-Library
#include <Adafruit_GPS.h> //Load the GPS Library. Make sure you have installed the library form the adafruit site above
#include <SoftwareSerial.h> //Load the Software Serial Library. This library in effect gives the arduino additional serial ports
#include <SD.h> // Load SD card library
#include <SPI.h> // Load SPI library
SoftwareSerial mySerial(3, 2); //Initialize SoftwareSerial, and tell it you will be connecting through pins 2 and 3
Adafruit_GPS GPS(&mySerial); //Create GPS object
 
String NMEA1;  //We will use this variable to hold our first NMEA sentence
String NMEA2;  //We will use this variable to hold our second NMEA sentence
char c;       //Used to read the characters spewing from the GPS module
int chipSelect = 4
File mySensorData; 


void setup()  
{
  Serial.begin(115200);  //Turn on the Serial Monitor
  GPS.begin(9600);       //Turn GPS on at baud rate of 9600
  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  delay(1000);  //Pause

  pinMode(10, OUTPUT);
  SD.begin(chipSelect);

  if (SD.exists("NMEA.txt")){
    SD.remove("NMEA.txt");
  }

  if (SD.exists("GPSData.txt")){
    SD.remove("GPSData.txt");
  }
}
 
 
void loop()                     // run over and over again
{
  readGPS();  //This is a function we define below which reads two NMEA sentences from GPS
 
  if(GPS.fix==1) {
    mySensorData = SD.open("NMEA.txt", FILE_WRITE);
    mySensorData.println(NMEA1);
    mySensorData.println(NMEA2);
    mySensorData.close();
    mySensorData = SD.open("GPSData.txt", FILE_WRITE);
    mySensorData.print(GPS.latitude, 4);
    mySensorData.print(GPS.lat);
    mySensorData.print(",");
    mySensorData.print(GPS.longitude, 4);
    mySensorData.print(GPS.lon);
    mySensorData.print(",");   
    mySensorData.println("GPS.altitude");
    mySensorData.close();
  }

}


void readGPS(){  //This function will read and remember two NMEA sentences from GPS
  
  clearGPS();    //Serial port probably has old or corrupt data, so begin by clearing it all out
  while(!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c=GPS.read(); //read a character from the GPS
  }
  GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1=GPS.lastNMEA();      //Once parsed, save NMEA sentence into NMEA1
  
  while(!GPS.newNMEAreceived()) {  //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
  NMEA2=GPS.lastNMEA();

  Serial.println(NMEA1);
  Serial.println(NMEA2);
  Serial.println("");
}


void clearGPS() {  //Since between GPS reads, we still have data streaming in, we need to clear the old data by reading a few sentences, and discarding these
  
  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

  while(!GPS.newNMEAreceived()) {
    c=GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
 
}