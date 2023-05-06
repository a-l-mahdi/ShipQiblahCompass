/*--------------------------------------------------------------
  Program:      Ship Qiblah Compass

  Description:  Arduino web server that serves up a web page
                that displays the status of ship Qiblah direction.
                The web page is stored on the SD card.
                The web page contains JavaScript code that uses
                Ajax to request the state of the switch every
                second.
  
  Hardware:     Arduino Uno and official Arduino Ethernet
                shield.
                micro SD card formatted FAT16
                RS422 To TTL interface.
                For simulation USB to RS422 interface needed.
                
  Software:     Developed using Arduino 1.8.13 software
                Should be compatible with Arduino 1.0 +
                SD card contains web page called index.htm
  
  References:   - WebServer example by David A. Mellis and 
                  modified by Tom Igoe
                - SD card examples by David A. Mellis and
                  Tom Igoe
                - Ethernet library documentation:
                  http://arduino.cc/en/Reference/Ethernet
                - SD Card library documentation:
                  http://arduino.cc/en/Reference/SD
                -Arduino SD Card Ajax Web Server by W.A. Smith
                https://startingelectronics.org/tutorials/arduino/ethernet-shield-web-server-tutorial/SD-card-AJAX-web-server/
                - NMEAParser Library from https://reference.arduino.cc/reference/en/libraries/nmeaparser/
  Date:         27 October 2022
  Last Modified: 22 April 2023
       
  Author:       H.S. Moghaddam
                h.seyedzadegan@gmail.com
--------------------------------------------------------------*/

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <NMEAParser.h>
#include <math.h>

// size of buffer used to capture HTTP requests
#define REQ_BUF_SZ   40

// MAC address from Ethernet shield sticker under board
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 120); // IP address, may need to change depending on network
EthernetServer server(80);  // create a server at port 80
File webFile;
char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer


SoftwareSerial mySerial(2 , 3); // RX, TX

NMEAParser<2> parser;

String positionSplitter = "";
String positionDecode = "";

  float arg0;   //True Heading
  char arg1;    //Checksum
  float arg2=0;   //Latitude= is a Float number
  char arg3;    //N or S
  float arg4=0;   //Longitude= is a Float number
  char arg5;    //E or W
  double lat=1.1111;
  double lon=1.1111;
  double latMecca=21.45;
  double lonMecca=-39.75;
  double qiblah = 0;
  double qiblahDown = 0;
  String qiblahL = "";

void firstHandler()
{
  if (parser.getArg(0,arg0)) Serial.println(arg0);
  if (parser.getArg(1,arg1)) Serial.println(arg1);
}

void secondHandler()
{
if (parser.getArg(0,arg2)) Serial.println(arg2);
if (parser.getArg(1,arg3)) Serial.println(arg3);
if (parser.getArg(2,arg4)) Serial.println(arg4);
if (parser.getArg(3,arg5)) Serial.println(arg5);
}

void errorHandler()
{
  Serial.print("*** Error : ");
  Serial.println(parser.error()); 
}

int iq = 0;

void setup()
{
    // disable Ethernet chip
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);       // for debugging
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
    pinMode(7, INPUT);        // switch is attached to Arduino pin 3

    mySerial.begin(9600);

  parser.setErrorHandler(errorHandler);
  parser.addHandler("HCHDT", firstHandler);
  parser.addHandler("GPGLL", secondHandler);

    
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin();           // start to listen for clients
}

void loop()
{
     iq=0;
     delay(10);
    while(mySerial.available()>0 && iq<65)
    {
     char cq = mySerial.read();
     parser << cq;
     iq++;
    }
     ethernetProvider();
}

void ethernetProvider(){

    EthernetClient client = server.available();  // try to get client

    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                // buffer first part of HTTP request in HTTP_req array (string)
                // leave last element in array as 0 to null terminate string (REQ_BUF_SZ - 1)
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // save HTTP request character
                    req_index++;
                }
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println();
                    // Ajax request
                    if (StrContains(HTTP_req, "ajax_switch")) {
                      delay(20);
                        // read switch state and send appropriate paragraph text
                        GetSwitchState(client);
                        delay(10);
                    }
                    else {  // web page request
                        // send web page
                        webFile = SD.open("index.htm");        // open web page file
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // send web page to client
                            }
                            webFile.close();
                        }
                    }
                    // display received HTTP request on serial port
                    Serial.println(HTTP_req);
                    // reset buffer index and all buffer elements to 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // every line of text received from the client ends with \r\n
                if (c == '\n') {
                    // last character on line of received text
                    // starting new line with next character read
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }
            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)

}

// send the state of the switch to the web browser
void GetSwitchState(EthernetClient cl)
{
   double qqq = qiblahCompasser();
   double tHeading = double(arg0);
   qqq = qqq>0 ? qqq : qqq+180;
   cl.println(String(qqq>tHeading ? qqq - tHeading : 360 - tHeading + qqq));
}

// sets every element of str to 0 (clears array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// searches for the string sfind in the string str
// returns 1 if string found
// returns 0 if string not found
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
    
}


//Qiblah Calculator 
double qiblahCompasser(){

    Serial.print("lat: ");
    Serial.println(arg2);
    Serial.print("lon: ");
    Serial.println(arg4);
    
    lat = arg2;
    lat = (double)((((lat / 100)- ((int)(lat / 100)))*100)/60)+((int)(lat/100)); 
    lon = arg4;
    lon = (double)((((lon / 100)- ((int)(lon / 100)))*100)/60)+((int)(lon/100));

    switch (arg3) {
    case 'N':    
      lat = lat;
      break;
    case 'S':   
      lat = -lat;
      break;
    }

    switch (arg5) {
    case 'E':    
      lon = -lon;
      break;
    case 'W':   
      lon = lon;
      break;
    }

    qiblah = sin(((lon - lonMecca) * PI)/180);
    Serial.println(qiblah,4);
    qiblahDown = (    (  cos((lat * PI)/180) * tan((latMecca * PI)/180)  )-(sin((lat * PI)/180) * cos(((lon - lonMecca) * PI)/180))    );
    Serial.println(qiblahDown);
    qiblah = qiblah / qiblahDown;
    Serial.println(qiblah);
    qiblah = atan(qiblah);
    qiblah = (qiblah * 180) / PI;
    
   return qiblah;

}
