#include <String.h>
#include <ESP8266WiFi.h>
#include "ESP8266SMTP.h"
#include "Appliance.h"

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define FROM_EMAIL "something@gmail.com"
#define GMAIL_PASSWORD "PASSWORD"
#define EMAIL_SUBJECT "Laundry Notification"
#define TO_EMAIL "EMAIL1,EMAIL2" // you can add commas here

#define ANNOY_INTERVAL 300000 // 5 mins in millis
#define WASHER_PIN D1
#define DRYER_PIN D2

#define EMAIL_RETRIES 3

#pragma region Globals
uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000;             // If not connected wait time to try again
#pragma endregion Globals

Appliance *washer;
Appliance *dryer;

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    WiFi.persistent(false);
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    } else {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.println(WIFI_SSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0) {
            Serial.println("Check if access point available or SSID and Password\r\n");
        }
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

void sendDoneNotification(String message) {
  Serial.println("Sending message");
  Serial.println(message);

  while (connection_state != 1) {
    Awaits();
  }

  delay(1000);

  Serial.println("Connected to WiFi");

  SMTP.setEmail(FROM_EMAIL)
		.setPassword(GMAIL_PASSWORD)
		.Subject(EMAIL_SUBJECT)
		.setFrom("Laundry Bot")
		.setForGmail();						// simply sets port to 465 and setServer("smtp.gmail.com");						
																   // message text from http://www.blindtextgenerator.com/lorem-ipsum
	if(SMTP.Send(TO_EMAIL, message)) {
		Serial.println(F("Message sent"));
	} else {
		Serial.print(F("Error sending message: "));
		Serial.println(SMTP.getError());
	}
}

void setup () {
  washer = new Appliance(WASHER_PIN);
  dryer = new Appliance(DRYER_PIN);

  Serial.begin(115200);
  Serial.println("Started up");

  sendDoneNotification("Laundry System has started up.");
}

void loop () {
  if (washer->hasStopped()) {
    delay(3000); // Sleep for 3 seconds
    while (dryer->hasStarted() == false) {
      delay(ANNOY_INTERVAL);
      if (dryer->hasStarted() == false) {
        sendDoneNotification("Flip the laundry");
      }
    }
  }
  delay(5);
}
