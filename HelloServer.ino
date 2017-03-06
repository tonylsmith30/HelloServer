#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "PlusMobile";
const char* password = "Consult1370!!";

ESP8266WebServer server(80);

const int speaker = 16;
const int led = 5;
int ledState = LOW; 

void annoyingBuzz()
{
  for(int i = 0; i<= 2; i++)
  {
    digitalWrite(speaker, LOW);   // Turn the LED on (Note that LOW is the voltage level
    delay(250);                      // Wait for a second
    digitalWrite(speaker, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(1000);                      // Wait for two seconds (to demonstrate the active low LED)
  }
}

void handleRoot() {
  server.send(200, "text/plain", "Opportunity has been won!!!!!");
  digitalWrite(led, HIGH); //turn on
  annoyingBuzz();
  digitalWrite(led, LOW);//turn off
  digitalWrite(speaker, LOW);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  Serial.write("Setup has been called!");
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

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
