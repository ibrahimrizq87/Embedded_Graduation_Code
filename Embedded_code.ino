
/* 
 * File:   Embedded Graduation project
 * Author: Ibrhime Rizq
 * description: the main purpose of this code is to connect the esp to the firebase project.
 *              start generating a QRcode on the screen only if the state on the firebase is true ,
 *              which means that the lecture or the section on the hall has been given the permission to generat the qrcode 
 *              which then used to record the attendance by scanning the code  
 * Created on MAR 3, 2023, 9:53 PM
 */
 
/* -------------------------------Section : Includes--------------------------------*/

/* including the esp8266WiFi library*/
#include <ESP8266WiFi.h>

/* including the firbase library*/
#include <FirebaseESP8266.h>

/* including the Adafruit library for the screen*/
#include <Adafruit_GFX.h>      

/* including the Adafruit_ST7735 library for screen module functions*/
#include <Adafruit_ST7735.h>   

/* including the SPI library for Serial Peripheral Interface communication protocol*/
#include <SPI.h>

/* including the QRcode library to generate QRcode for st7735 module*/
#include <qrcode_st7735.h>

/* ------------------------------- Section: Macro Declarations --------------------------------*/
#define TFT_RST   16  
#define TFT_CS    4    
#define TFT_DC    5 

#define TFTMODEL INITR_144GREENTAB
/* Declarations of network username and password */
#define WIFI_SSID "BEMO RIZQ"
#define WIFI_PASSWORD "123456789"

/* Declarations of firbase API key*/
#define API_KEY "T4u6rID3GJY1hL2ASYnC84QdT1jQv7Cz4kyqE431"
/* Declarations of firbase database URL*/
#define DATABASE_URL "https://graduation-ef946-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

const String code="ABC123";
const String hallID="M44";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7735 qrcode (&display);


String main2="";

int a, b, x, y;
bool state=false;
void setup()
{

  Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  /* Initialte the connection between the esp and the network source*/
  while (WiFi.status() != WL_CONNECTED)
  {
    /*Whilte until the connection happens*/
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;



/*configers the connection with firebase*/
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);
    display.initR(TFTMODEL);

    qrcode.init();
    qrcode.create("hellow world!");

}

void loop()
{
 
  /* generat a rondom code which will be used to generat the qr code and to be sent to the 
   * firbase for the application to check if the code scanned from the QRcode and the code in the firebase is the same
   */
    x=random(0,1000);

   /*if the firebase connection is good*/
  if (Firebase.ready()) 
  {
    /*read the state from the firebase database*/
    Serial.printf("Get boolean state data:  %s\n", Firebase.getBool(fbdo, "/hall/"+hallID+"/state") ? String(fbdo.to<bool>()).c_str() : fbdo.errorReason().c_str());
    state=fbdo.to<bool>();
    if (state){ 
      
       qrcode.init();
       /*generate a qrcode using the random code generated*/
       qrcode.create(hallID+"@"+code+"@"+x);
       /*send the code to firebase*/
       Firebase.setInt(fbdo, "/embedded/"+hallID+"/code", x);
       Firebase.setString(fbdo, "/embedded/"+hallID+"/hallID", hallID);
       Serial.println("value is true ");
       Serial.println("Code is : ");Serial.print(x);
      }else{
        
        qrcode.init();
        Serial.println("value is false ");
      }
  /*repeat the code every 7 seconds*/
  delay(7000);
  }
}
