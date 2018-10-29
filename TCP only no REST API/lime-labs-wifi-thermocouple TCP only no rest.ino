/* THIS TCP ONLY SERVER VERSION IS WIP!!!
 * Lime Labs GmbH
 * https://limelabs.io
 * 
 * lime labs wifi thermocouple
 * 
 * ESP32 / ESP8266 based WiFi thermocouple. Primarily used in our "lime labs remote reflow controller" project (https://github.com/lime-labs/lime-labs-remote-reflow-controller)
 * 
 * @author: Peter Winter <code@limelabs.io>
 * @version: 0.0.1
 * @initialrelease: 10/24/2018
 */

// change to <WiFi.h> for ESP32
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Adafruit_MAX31855.h>

// #################### SETUP ########################

// Default connection for the thermocouple amplifier, using software SPI. Wemos D1 Mini (ESP8266) pin nomenclature used below.
#define MAXDO   D5
#define MAXCS   D6
#define MAXCLK  D7

// WiFi parameters
#define WIFI_SSID     "yourwifi"
#define WIFI_PASSWORD "yourpassword"

// The port to listen for incoming TCP connections
#define LISTEN_PORT 4000

// #################### END SETUP ####################

// initialize the thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float celsius;
float fahrenheit;

void updateTemps() {
  celsius = thermocouple.readCelsius();
  if (isnan(celsius)) {
    Serial.println("Something is wrong with the thermocouple! Setting temps to 99999.99 C/F to indicate error state.");
    celsius = 99999.99;
    fahrenheit = 99999.99;
  } else {
    Serial.print("C = "); 
    Serial.println(celsius);
    fahrenheit = celsius * 1.8 + 32;
    Serial.print("F = "); 
    Serial.println(fahrenheit);
  }
}

void setup() {
  Serial.begin(115200);

  // wait for Serial
  while (!Serial);
  
  Serial.println("Waiting for MAX31855K to stabilize, then reading its internal temperature.");
  delay(500);
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  // init variables and update them right away to a real value
  updateTemps();

  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");

  // Start the server
  server.begin();
  Serial.print("TCP server started, listening on ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(LISTEN_PORT);
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  Serial.println("Client connected");
  // update the temperature variables
  updateTemps();
  
  client.write(celsius);
  client.stop();
  Serial.println("Client disconnected");
}
