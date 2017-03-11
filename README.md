# Laundry-Monitor

## Video
This is the code and models that accompany a [youtube video](https://youtu.be/UcFt0l35fvk).

## Before you begin
Some simple steps you need working before you can use this code is to have the correct drivers and install the support for your board in the Arduino IDE for your ESP board. If you can upload a sketch to your board and it runs then you are ready to proceed.

## Modify your gmail account
Modify the gmail account your going to be using for the notification system to allow devices with lower security standards to login in. More robust instructions can be found in [this instructable](http://www.instructables.com/id/ESP8266-GMail-Sender).

## Configure the code
Download a copy of the code and modify the following lines.
In the ‘Gsender.h’ file.

```
const char* EMAILBASE64_LOGIN = "YOUR_EMAIL_ADDRESS_IN_BASE64";
const char* EMAILBASE64_PASSWORD = "YOUR_EMAIL_PASSWORD_IN_BASE64";
const char* FROM = "FROM_EMAIL_ADDRESS";
```
These will need to be your email address and password encode in base64.
And your email address for the from Field.

In the ‘Monior.ino’ file
```
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define EMAIL_SUBJECT "Laundry Notification"
#define EMAIL "EMAIL_1"
#define EMAIL_2 "EMAIL_2"
```
These fields allow the ESP chip to connect to your WiFi so you will need to supply the SSID and the PASSWORD for your WiFi network. As well as some email addresses. I wanted myself and my wife to receive an email when the laundry was finished so there are two email define fields.

If you wish to only send the notification to one email address then you will need to delete the line containing the last define field named  EMAIL_2. You will also need to modify line #90 from

```
if(gsender->Subject(EMAIL_SUBJECT)->Send(EMAIL, EMAIL_2, message)) {
```

to

```
if(gsender->Subject(EMAIL_SUBJECT)->Send(EMAIL, message)) {
```

## Enjoy


