
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <gprs.h>
#include <SoftwareSerial.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);  
File sdcard_file;
GPRS gprs;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tuesd", "Wed", "Thurs", "Fri", "Sat"};
int CS_pin = 10;
int solenoidPin = 3;
int analogInPin1 = A0; // Analog input pin that the Sensor is attached to 
int analogInPin2 = A1;
int sensorValue1,sensorValue2 = 0; // value read from the Soil Moisture 
char* number = "081392584007";

uint8_t       SmsCountKering = 0; // SMS Count Variable
uint8_t       SmsCountBasah = 0;
const int     SmsMaxKering = 3;  //Maksimal SMS yang akan Dikirim
const int     SmsMaxBasah = 3; 

const long    intervalSMS = 10;  //Interval pengiriman setiap sms dalam detik
unsigned long previousMillis = 0; 



void setup() {
pinMode(analogInPin1,INPUT);
pinMode(analogInPin2, INPUT);
pinMode(solenoidPin, OUTPUT);
pinMode(CS_pin, OUTPUT);

  Serial.begin(9600);
  gprs.preInit();
  lcd.begin();
  SPI.begin();
  delay(3000); // wait for console opening
  
//menyiapkan rtc
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }

//menyiapkan sdcard
 if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  } 
   
} 

void loop() {
//sensor 1
int sensorValue1 = analogRead(analogInPin1);// read the analog in value:
int sensor1 = ( 100 - ( (sensorValue1/1023.00) * 100 ) );

//sensor 2
int sensorValue2 = analogRead(analogInPin2);// read the analog in value:
int sensor2 = ( 100 - ( (sensorValue2/1023.00) * 100 ) );

DateTime now = rtc.now();
    
  Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(",");
    Serial.print(sensor1);
    Serial.print("%");
    Serial.print(",");
    Serial.print(sensor2);
    Serial.print("%");
    Serial.println();

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print(now.day());
 lcd.print("/");
 lcd.print(now.month());
 lcd.print("/");
 lcd.print(now.year()); 
 lcd.print(" ");
 lcd.print(now.hour());
 lcd.print(":");
 lcd.print(now.minute());
  
 lcd.setCursor(0, 1);
 lcd.print("S1=");
 lcd.print(sensor1);
 lcd.print("%, S2=");
 lcd.print(sensor2);
 lcd.print("%");
 lcd.print(" ");
 delay(5000);

int lembab = ((sensor1 + sensor2)/2);

if (lembab <60) {
  digitalWrite(solenoidPin, HIGH);
  Serial.println("Irigasi berjalan");
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Tanah Kering!"); 
    lcd.setCursor(1,1); 
    delay(1000);


    if(SmsCountKering<SmsMaxKering){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= (intervalSMS*1000)) {
      previousMillis = currentMillis; 
      sendSMSkering();
      SmsCountKering=SmsCountKering+1;
         }
      }
   
   //membuat datalog
    sdcard_file = SD.open("DataLog.txt", FILE_WRITE);
    if (sdcard_file) { 
    sdcard_file.print(now.year(), DEC); 
    sdcard_file.print('/');
    sdcard_file.print(now.month(), DEC);
    sdcard_file.print('/');
    sdcard_file.print(now.day(), DEC);
    sdcard_file.print(" (");
    sdcard_file.print(daysOfTheWeek[now.dayOfTheWeek()]);
    sdcard_file.print(") ");
    sdcard_file.print(now.hour(), DEC);
    sdcard_file.print(':');
    sdcard_file.print(now.minute(), DEC);
    sdcard_file.print(':');
    sdcard_file.print(now.second(), DEC);
    sdcard_file.print(", ");
    sdcard_file.print(sensor1);
    sdcard_file.print("%");
    sdcard_file.print(", ");
    sdcard_file.print(sensor2);
    sdcard_file.print("%");
    sdcard_file.println();  
    sdcard_file.close(); // close the file
    }
     else {
    Serial.println("error opening DataLog.txt");
    }

}

else if ((lembab > 60) && (lembab < 80 )){
    digitalWrite(solenoidPin, LOW);
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Tanah Lembab");
    delay(1000);
}

else {

    digitalWrite(solenoidPin, LOW);
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Tanah Basah!"); 

   if(SmsCountBasah<SmsMaxBasah){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= (intervalSMS*1000)) {
      previousMillis = currentMillis; 
      sendSMSbasah();
      SmsCountBasah=SmsCountBasah+1;
         }
      }
      
    SmsCountKering=0; 
    SmsCountBasah=0;

      }
}

void sendSMSkering(){
  gprs.sendSMS(number,"Tanah kering, irigasi berjalan.");
  delay(3000);
}
void sendSMSbasah(){
   gprs.sendSMS(number,"Tanah terlalu basah!");
  delay(3000);
}
