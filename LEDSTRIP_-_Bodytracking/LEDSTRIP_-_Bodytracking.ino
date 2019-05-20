#include <Adafruit_NeoPixel.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#define NUMPIXELS 10 // Number of LEDs in strip
#define DATAPIN    4
#define CLOCKPIN   5

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, DATAPIN, NEO_GRB + NEO_KHZ800);

WiFiClient net;
MQTTClient client;

//NEOPIXEL VAR
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = (255, 255, 255);    // 'On' color (starts red)

//MQTT VAR

const char ssid[] = "ddlabwifi";
const char pass[] = "balddbaldd";

const char key[] = "99e72d4e";
const char secret[] = "60aae166ad92ba1d";

int payloadInt = 0;
unsigned long lastMillis = 0;

int lysOutput = 14;

void connect();  // <- predefine connect() for setup()

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  
// MQTT
  pinMode(lysOutput, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();

  
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("sorenthemaster", key, secret)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

 client.subscribe("/hello/output/lys");
  // client.unsubscribe("/hello");
}



void loop(){




client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  // Skriver på adressen(topic)  og sender beskeden(message)
  //publishMessage("/hello/p5/diameter", "500");
  //delay(500);
}
//Deklarere funktionen for at sende beskeden til (topic,message)
void publishMessage(String topic, String message){
  client.publish(topic, message);
}
// læser beskeder fra de subscribede topics (adresser)
void messageReceived(String &topic, String &payload) {
  //Serial.println("incoming: " + topic + " - " + payload);
delay(1000);

    payloadInt = payload.toInt();

    if (payloadInt == 1) {
      Serial.println("tænd");
      pixelStart();
      //Serial.println("modtaget");
    } else {
      Serial.println("sluk");
         pixelStop();
      }
    }
   


  

void pixelStart() {
  head = 0;
   while (head < NUMPIXELS){
    

  strip.setPixelColor(head, pixels.Color(255, 255, 255)); // 'On' pixel at head
  strip.setPixelColor(tail, pixels.Color(255, 255, 255));   // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);
  head++;
 
  }

    
  
}


void pixelStop() {
  head = 0;
  while (head < NUMPIXELS){
  
  
  strip.setPixelColor(head, pixels.Color(0,0,0)); // 'On' pixel at head
  strip.setPixelColor(tail, pixels.Color(0,0,0) );   // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);

  head++;
  

  }
  
}
