/**
 *
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Various config parameters here
#include "config.h"

/*
 * Global variables
 */
double avg = INITAL_AVERAGE; // average of last N_AVERAGE samples
ESP8266WebServer server(80); // web server instance

/**
 *
 */
int getSampleUpdateAverage() {
  static int nSamples = 1;

  // get new sample and update counter
  int sample = analogRead(A0);
  nSamples = (nSamples +1) % N_AVERAGE;
  if (nSamples == 0)
    nSamples = 2;

  // update average
  avg = (avg*(nSamples-1) + sample)/nSamples;

  Serial.println("sam = " + String(sample));
  Serial.println("avg = " + String(avg));
  Serial.println("num = " + String(nSamples));
  return sample;
}
/**
 *
 */
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "Hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

/**
 * Return current voltage, read from the LM35 sensor
 */
int getNoise() {
  int ran=0;
  int s;
  int i = 0;

  while (i < NUM_BITS)
  {
    s = getSampleUpdateAverage();
    if (s > avg)
    {
      bitSet(ran, i);
      i++;
    }

    if (s < avg)
    {
      i++;
    }
    // if s == avg discard value
  }

  return ran;
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/temp", []() {
    server.send(200, "text/plain", String(getNoise()));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
