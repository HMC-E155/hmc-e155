/*
   Erik Meike and Kaveh Pezeshki and Christopher Ferrarin
   E155 Lab 6 Generic Webserver

   Modified by Josh Brake 9/23/19
   jbrake@hmc.edu

   Hosts a webserver displaying webpages sent over UART.

   In more detail:

   1) The webserver starts a 125000 baud serial connection over the hardware UART (for debug)
   2) The webserver connects to a given network or creates its own.
   3) The webserver waits for a request from the client. When it receives one, it transmits /REQ:<path>\n to the MCU. 

   Step 3 is repeated while the program runs

   Connecting to ESP:

   1) Connect to the ESP's WiFi network
   2) Go to http://192.168.4.1/ after your computer is assigned an IP address by the ESP. 
*/

// Importing required libraries
#include <ESP8266WiFi.h>
#define mcuSerial Serial
#define AP_MODE true

// defining start and end HTML tags
const String htmlStart = "<!DOCTYPE html><html>";
const String htmlEnd = "</html>";

// Defining network information
const char * networkName = "Lab6ESP";  // Set this to the selected network SSID
const char * password    = NULL;          // Set this to a non-null value if selected network requires authentication
String      ip;                           // Stores the current IP. Set in the setup function

// Defining the web server and HTTP request variables
WiFiServer server(80);           // The server is accessible over port 80
String     request;              // Stores the client HTTP request
String     parsedRequest = "<>"; // Stores a simplified version of the HTTP request to transmit to the MCU
String     currentLine;          // Stores a semi-parsed version of the HTTP request

extern "C" {
  #include "user_interface.h"
}

// Converts an IP address to a String
String ipToString(IPAddress address)
{
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

String parseRequest(String request) {
  // Parses an input http GET requested
  // Request is of the format 'GET <Requested resource> HTTP/1.1'
  // We want to just strip the <Requested resource> part out of the GET request and add a tag to send the request data to the MCU
  int getLocation = request.indexOf("GET /"); // Get index of GET request
  int httpLocation = request.indexOf(" HTTP"); // Index of HTTP request

  String parsedRequest = "/REQ:" + request.substring(getLocation + 5, httpLocation) + "\n"; // Prepend "/REQ:" as a tag for the MCU and add 5 to getLocation in order to find the start of the request info

  return parsedRequest;
}

// Algorithm
int substringInString(String haystack, String needle) {
  for (unsigned int i = 0; i < haystack.length() - needle.length(); i++) {
    bool foundsubString = true;
    for (unsigned int j = 0; j < needle.length(); j++) {
      if ( haystack[i + j] != needle[j]) {
        foundsubString = false;
      }
    }
    if (foundsubString) {
      return true;
    }
  }
  return false;
}

// Sends parsedRequest over UART to the MCU and waits for a complete webpage to be returned
String receiveWebPage(String parsedRequestIn, WiFiClient * webClient) {
  String webpage = ""; //clear webpage in preparation for new webpage to be transmitted


  // ignore favicon and other requests we don't want
  if (parsedRequestIn == "<>") return "";

  mcuSerial.flush();
  while (mcuSerial.available()) mcuSerial.read();
  mcuSerial.print(parsedRequestIn);

  // wait until the entire webpage has been received
  unsigned long lastByteTime = millis();
  while (true) {
    // checking for new serial data, adding to website
    while (mcuSerial.available()) {
      yield();
      char newData = mcuSerial.read();
      webpage.concat(newData);
      lastByteTime = millis();
    }

    // Print the output to the browser if a </html> was sent or the system times out
    if ((webpage.indexOf("</html>") != -1) || (millis() > lastByteTime + 200)) {
      if (webpage.length() < 10) {
        webpage = "Could not connect to the MCU. Please check your connections. (10 or fewer bytes were received from the processor.)";
      }

      webClient->printf("Content-Length: %u\r\n\r\n%s", webpage.length(), webpage.c_str());
      webClient->flush();
      break;

    }
  }
  while (mcuSerial.available()) mcuSerial.read();

  return webpage;
}


//Setup code. Runs once on program execution before loop code
void setup() {
  // initalize the MCU serial connection
  mcuSerial.begin(125000);

  // Initialize the wifi according to the requested mode
  if (AP_MODE) {
    WiFi.softAP(networkName, password);
  } else {
    WiFi.begin(networkName, password);
    digitalWrite(LED_BUILTIN, HIGH);
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
      // delay while we connect
      delay(100);
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
  ip = ipToString(WiFi.localIP());

  // Start ESP webserver
  server.begin();
}

//Main program. Runs repeatedly after setup code
void loop() {
  delay(1);
  yield();
  if (AP_MODE) {
    if (WiFi.softAPgetStationNum() > 0) {
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }

  // Wait for a new connection
  WiFiClient webClient = server.available();
  // If a client has connected, we wait for a request
  if (webClient) {
    currentLine = "";

    while (webClient.connected()) {
      // Reading available bytes from the client if available
      if (webClient.available()) {
        char byteIn = webClient.read();
        request += byteIn;

        // if the line is only a line feed, we have reached the end of the client request and will therefore send a response
        // This requires sending a request for a new webpage to the MCU
        if (request.indexOf("HTTP") != -1) {
          if (currentLine.length() == 0) {
            // transmitting the response
            // transmitting HTTP header and content type

            webClient.println("HTTP/1.1 200 OK");
            webClient.println("Content-type:text/html");
            
            // transmitting the full webpage

            if (parseRequest(request) != "<>") {
              parsedRequest = parseRequest(request);
            }
            
            receiveWebPage(parsedRequest, &webClient);

            break; //disconnect from the client by breaking from while loop
          }
          else {
            currentLine = "";
          }
        }
        else if (byteIn != '\r') {
          currentLine += byteIn;
        }
      }
    }
    //ending the transaction
    request = "";
    webClient.stop();

  }
}
