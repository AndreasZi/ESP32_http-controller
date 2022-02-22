//
// A simple server implementation giving 
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>

#include <ESPAsyncWebServer.h>

#include "secrets.h" 


#define RED_PIN D2
#define GREEN_PIN D3
#define BLUE_PIN D4

int red = 0;
int green = 0;
int blue = 0;




AsyncWebServer server(80);



String htmlPage = "<form action='get' method='get'><label for='red'>Red   </label><input type='text' id='red' name='red'><br><label for='green'>Green </label><input type='text' id='green' name='green'><br><label for='blue'>Blue  </label><input type='text' id='y' name='blue'><br><input type='submit' value='Submit'></form>";


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);


    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", String(htmlPage));
    });

    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        if (request->hasParam("red")&&request->hasParam("green")&&request->hasParam("blue")) {
            red = request->getParam("red")->value().toInt();
            green = request->getParam("green")->value().toInt();
            blue = request->getParam("blue")->value().toInt();
        }
        request->send(200, "text/html", htmlPage + "<p> current x: " + red + ", current y: " + green + ", current y: " + blue);
    });
   

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
  digitalWrite(RED_PIN,red);
  digitalWrite(GREEN_PIN,green);
  digitalWrite(GREEN_PIN,blue);
  delay(1000);
}