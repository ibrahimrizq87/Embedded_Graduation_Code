

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


#define TFT_RST   16  
#define TFT_CS    4    
#define TFT_DC    5 

#include <Adafruit_GFX.h>      
#include <Adafruit_ST7735.h>   
#include <SPI.h>
#include <qrcode_st7735.h>

#define TFTMODEL INITR_144GREENTAB

//#include <addons/TokenHelper.h>

//#include <addons/RTDBHelper.h>

#define WIFI_SSID "BEMO RIZQ"
#define WIFI_PASSWORD "123456789"


#define API_KEY "T4u6rID3GJY1hL2ASYnC84QdT1jQv7Cz4kyqE431"

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
  while (WiFi.status() != WL_CONNECTED)
  {
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




  Firebase.begin(DATABASE_URL, API_KEY);



  Firebase.setDoubleDigits(5);

    display.initR(TFTMODEL);
    // Initialize QRcode display using library
    qrcode.init();
    qrcode.create("hellow world!");

}

void loop()
{
 
  
    x=random(0,1000);
    //y=random(10,19);
    
   
  if (Firebase.ready()) 
  {
    Serial.printf("Get boolean state data:  %s\n", Firebase.getBool(fbdo, "/hall/"+hallID+"/state") ? String(fbdo.to<bool>()).c_str() : fbdo.errorReason().c_str());
    state=fbdo.to<bool>();
    if (state){ 
       qrcode.init();
    qrcode.create(hallID+"@"+code+"@"+x);
      Firebase.setInt(fbdo, "/embedded/"+hallID+"/code", x);
    Firebase.setString(fbdo, "/embedded/"+hallID+"/hallID", hallID);
  
      Serial.println("value is true ");
       Serial.println("Code is : ");Serial.print(x);
      }else{
        qrcode.init();
        Serial.println("value is false ");
      }
    //Firebase.setInt(fbdo, main, 5);
  

    /*Serial.printf("Get int a--  %s\n", Firebase.getInt(fbdo, "/test/a") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     a=fbdo.to<int>();
    Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/test/b") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     b=fbdo.to<int>();

  Serial.println();  
  Serial.print("a:");
  Serial.print(a);
  Serial.print("  b: ");
  Serial.print(b);
  
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  
*/
  delay(7000);
  }
}
