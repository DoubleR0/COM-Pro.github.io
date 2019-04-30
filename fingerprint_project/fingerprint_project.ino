/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, D1, D2);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(D3, D4);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int stax = 0;
void setup()  
{
  digitalWrite(D0, 1);
  lcd.begin();
  lcd.backlight();
  pinMode(D0, OUTPUT);
  lcd.setCursor(3, 0);
  lcd.print("WELCOME!!!");
  delay(2000);
  lcd.setCursor(1, 0);
  lcd.print("SCAN TO START");
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
}

void loop()           // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return 0;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return 0;
  
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return 0;
  
  // found a match!
    if(finger.fingerID >= 1){
      if(stax==0){
          stax = 1;
          digitalWrite(D0, 0);
          lcd.setCursor(1, 0);
          lcd.print(" STARTING !!!  ");
          delay(2000);
          lcd.setCursor(1, 0);
          lcd.print("PUT YOUR FINGER ");
      }
      else{
          stax = 0;
          digitalWrite(D0, 1);
          lcd.setCursor(1, 0);
          lcd.print("SHUTDOWNING !!!");
          delay(2000);
          lcd.setCursor(1, 0);
          lcd.print("PUT YOUR FINGER ");

      }
    }
    
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
