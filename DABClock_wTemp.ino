
//  DABClock is a CroNixie knockoff using heat-bent acrylic rods
//  each digit is a high-output LED lighting an acrylic rod

 unsigned long time_now = 0; //for mills() comparison
 String rcv;
 String latestTemperature;

 const int radioRCV = 53;
 
 const int XSecondInterval = 992;  //adjusted for processing 992
 const int XHourDigit1_1 = 2;
 const int tempSymbol = 52;

 const int XHourDigit2_0 = 3;
 const int XHourDigit2_1 = 4;
 const int XHourDigit2_2 = 5;
 const int XHourDigit2_3 = 6;
 const int XHourDigit2_4 = 7;
 const int XHourDigit2_5 = 8;
 const int XHourDigit2_6 = 9;
 const int XHourDigit2_7 = 10;
 const int XHourDigit2_8 = 11;
 const int XHourDigit2_9 = 12;

 const int negativeSymbol = 13;  // for temperature display

 const int XMinuteDigit1_0 = 22;
 const int XMinuteDigit1_1 = 23;
 const int XMinuteDigit1_2 = 24;
 const int XMinuteDigit1_3 = 25;
 const int XMinuteDigit1_4 = 26;
 const int XMinuteDigit1_5 = 27;

 // these add'l 4 digits are for possible Temperature function
 const int XMinuteDigit1_6 = 40;
 const int XMinuteDigit1_7 = 41;
 const int XMinuteDigit1_8 = 42;
 const int XMinuteDigit1_9 = 43;

 const int XMinuteDigit2_0 = 29;
 const int XMinuteDigit2_1 = 30;
 const int XMinuteDigit2_2 = 31;
 const int XMinuteDigit2_3 = 32;
 const int XMinuteDigit2_4 = 33;
 const int XMinuteDigit2_5 = 34;
 const int XMinuteDigit2_6 = 35;
 const int XMinuteDigit2_7 = 36;
 const int XMinuteDigit2_8 = 37;
 const int XMinuteDigit2_9 = 38;

 int XHour = 12;
 int XMinute = 0;
 int XSecond = 0;

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

//RH_ASK driver;
 RH_ASK driver(2000, radioRCV, 50, 51); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 


void setup() {
  // initialize serial:

  #ifdef RH_HAVE_SERIAL
    Serial.begin(9600);    // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
  ;
#endif

  // make the pins outputs:
  pinMode(XHourDigit1_1, OUTPUT);

 pinMode(XHourDigit2_0, OUTPUT);
 pinMode(XHourDigit2_1, OUTPUT);
 pinMode(XHourDigit2_2, OUTPUT);
 pinMode(XHourDigit2_3, OUTPUT);
 pinMode(XHourDigit2_4, OUTPUT);
 pinMode(XHourDigit2_5, OUTPUT);
 pinMode(XHourDigit2_6, OUTPUT);
 pinMode(XHourDigit2_7, OUTPUT);
 pinMode(XHourDigit2_8, OUTPUT);
 pinMode(XHourDigit2_9, OUTPUT);

 pinMode(XMinuteDigit1_0, OUTPUT);
 pinMode(XMinuteDigit1_1, OUTPUT);
 pinMode(XMinuteDigit1_2, OUTPUT);
 pinMode(XMinuteDigit1_3, OUTPUT);
 pinMode(XMinuteDigit1_4, OUTPUT);
 pinMode(XMinuteDigit1_5, OUTPUT);
 pinMode(XMinuteDigit1_6, OUTPUT);
 pinMode(XMinuteDigit1_7, OUTPUT);
 pinMode(XMinuteDigit1_8, OUTPUT);
 pinMode(XMinuteDigit1_9, OUTPUT);

 pinMode(XMinuteDigit2_0, OUTPUT);
 pinMode(XMinuteDigit2_1, OUTPUT);
 pinMode(XMinuteDigit2_2, OUTPUT);
 pinMode(XMinuteDigit2_3, OUTPUT);
 pinMode(XMinuteDigit2_4, OUTPUT);
 pinMode(XMinuteDigit2_5, OUTPUT);
 pinMode(XMinuteDigit2_6, OUTPUT);
 pinMode(XMinuteDigit2_7, OUTPUT);
 pinMode(XMinuteDigit2_8, OUTPUT);
 pinMode(XMinuteDigit2_9, OUTPUT);

 pinMode(tempSymbol, OUTPUT);
 pinMode(negativeSymbol, OUTPUT);

      lightNumbersXMinute(XMinute);
      lightNumbersXHour(XHour);
      
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0
//  resetFunc();  //call reset


void loop() {
 int XMinutePASS = 0;
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

      CLEARAll();
    // look for the next valid integer in the incoming serial stream:
     XHour = Serial.parseInt();
    // do it again:
     XMinute = Serial.parseInt();
    // do it again:
     XSecond = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if (Serial.read() == '\n') {
      XHour = constrain(XHour, 1, 99);
      XMinute = constrain(XMinute, 0, 59);
      XSecond = constrain(XSecond, 0, 59);
      Serial.print(XHour);
      Serial.println(" Got it");
      if (XHour == 13){
        XHour = 1;
        LightShow(100);  // run the lightshow
        CLEARAll();
        } 
      if (XHour == 14){
        XHour = 1;
        LightShow(80);  // run the lightshow
        LightShow(80);  // run the lightshow
        LightShow(80);  // run the lightshow
        LightShow(80);  // run the lightshow
        CLEARAll();
        } 
      if (XHour == 15){
        XHour = 1;
        LightShow(1000);  // run the lightshow SLOW for troubleshooting
        CLEARAll();
        } 
      lightNumbersXMinute(XMinute);
      lightNumbersXHour(XHour);

    }
  }

  // 5 seconds out of every minute, check for temp transmissions  
      if (XSecond > 54) {
        OneSecond_GetTemp(); // wait one second (adjusted) while listening for temperature
      } else {
        delay(XSecondInterval);
      }
        
  // do math for time on clock
      XSecond++;
      
      if (XSecond > 59) {
          XSecond = 0;
          XMinute++;
          XMinutePASS = XMinute;
          if (XMinutePASS > 59) {
            XMinutePASS = 0;
          }
          lightNumbersXMinute(XMinutePASS);
        }
        
      if (XMinute > 59) {
          XMinute = 0;
          XHour++;
          if (XHour > 12) {
            XHour = 1;
          } 
        lightNumbersXHour(XHour);
        }

        
      // periodic light show

      if (XMinute == 10 || XMinute == 40) {
    //    LightShow();  // run the lightshow
      }
      
      // show temperature
      
      if (XSecond == 10 || XSecond == 40) {
        lightNumberTemperature();
        XSecond = XSecond + 5;
        
        lightNumbersXMinute(XMinute);
        lightNumbersXHour(XHour);
      }


  
}

void lightNumbersXHour(int displayXHour) {

int digit1 = 0;
int digit2 = 0;
String digitsBOTH = "";
String digitSingleWorkVar = "";

    digitsBOTH = (displayXHour);
      
  if (displayXHour > 9) {
   if (digitsBOTH.length() < 2){
      digitsBOTH = "1" + digitsBOTH;
      } 
    digitalWrite(XHourDigit1_1, HIGH);   // turn the 1 LED on 
    digitSingleWorkVar = (digitsBOTH.substring(1));
    digit2 = abs(digitSingleWorkVar.toInt());
  
  } else {
       if (digitsBOTH.length() < 2){
      digitsBOTH = "0" + digitsBOTH;
      digit2 = digitsBOTH.toInt();
      } 
}
    
    CLEARXHours();
  
  digitalWrite(XHourDigit2_0 + digit2, HIGH);   // turn the proper LED on     

  if (displayXHour > 9) {
     digitalWrite(XHourDigit1_1, HIGH);   // turn the 1 LED on 

  }
  
  }

void lightNumbersXMinute(int displayXMinute) {
int digit1 = 0;
int digit2 = 0;
String digitsBOTH = "";
String digitSingleWorkVar = "";

    CLEAR3rdDigit();

    //2 digits (do the math)

    digitsBOTH = (displayXMinute);
    if (digitsBOTH.length() < 2){
      digitsBOTH = "0" + digitsBOTH;
      } 

      digitSingleWorkVar = (digitsBOTH.substring(0,1));
      digit1 = abs(digitSingleWorkVar.toInt());
      digit1 = constrain(digit1, 0, 9);
      
      digitSingleWorkVar = (digitsBOTH.substring(1));
      digit2 = abs(digitSingleWorkVar.toInt());
      digit2 = constrain(digit2, 0, 9);
      

    CLEAR3rdDigit();
    digitalWrite(XMinuteDigit1_0 + digit1, HIGH);   // turn the proper LED on     
    
    //XSecond digit
    CLEAR4thDigit();
    digitalWrite((XMinuteDigit2_0 + digit2), HIGH);   // turn the proper LED on     
      
  }


void CLEARXHours() {

     digitalWrite(XHourDigit1_1, LOW);
     digitalWrite(XHourDigit2_0, LOW);
     digitalWrite(XHourDigit2_1, LOW);
     digitalWrite(XHourDigit2_2, LOW);
     digitalWrite(XHourDigit2_3, LOW);
     digitalWrite(XHourDigit2_4, LOW);
     digitalWrite(XHourDigit2_5, LOW);
     digitalWrite(XHourDigit2_6, LOW);
     digitalWrite(XHourDigit2_7, LOW);
     digitalWrite(XHourDigit2_8, LOW);
     digitalWrite(XHourDigit2_9, LOW);
     
}

void CLEARAll() {
  CLEARXHours();  //XHours first
    CLEAR4thDigit();
    CLEAR3rdDigit();
  }

void CLEAR4thDigit() {
 digitalWrite(XMinuteDigit2_0, LOW);
 digitalWrite(XMinuteDigit2_1, LOW);
 digitalWrite(XMinuteDigit2_2, LOW);
 digitalWrite(XMinuteDigit2_3, LOW);
 digitalWrite(XMinuteDigit2_4, LOW);
 digitalWrite(XMinuteDigit2_5, LOW);
 digitalWrite(XMinuteDigit2_6, LOW);
 digitalWrite(XMinuteDigit2_7, LOW);
 digitalWrite(XMinuteDigit2_8, LOW);
 digitalWrite(XMinuteDigit2_9, LOW);
  
  }

void CLEAR3rdDigit() {
 digitalWrite(XMinuteDigit1_0, LOW);
 digitalWrite(XMinuteDigit1_1, LOW);
 digitalWrite(XMinuteDigit1_2, LOW);
 digitalWrite(XMinuteDigit1_3, LOW);
 digitalWrite(XMinuteDigit1_4, LOW);
 digitalWrite(XMinuteDigit1_5, LOW);
 digitalWrite(XMinuteDigit1_6, LOW);
 digitalWrite(XMinuteDigit1_7, LOW);
 digitalWrite(XMinuteDigit1_8, LOW);
 digitalWrite(XMinuteDigit1_9, LOW);
    
  }

void LightShow(int delayLShow) {
  
int i;
int iPrevious;

  // 1 digit at a time
    time_now = millis();
   

  for(i=2; i<28; i++) {
    digitalWrite(i, HIGH);   // turn the proper LED on     
    iPrevious = (i - 1);
    digitalWrite(iPrevious, LOW);   // turn the proper LED off     
    delay(delayLShow);    
  }

  //clear all
  CLEARAll();

  for(i=40; i<44; i++) {
    digitalWrite(i, HIGH);   // turn the proper LED on     
    iPrevious = (i - 1);
    digitalWrite(iPrevious, LOW);   // turn the proper LED off     
    delay(delayLShow);    
  }

  //clear all
  CLEARAll();


  for(i=29; i<40; i++) {
    digitalWrite(i, HIGH);   // turn the proper LED on     
    iPrevious = (i - 1);
    digitalWrite(iPrevious, LOW);   // turn the proper LED off     
    delay(delayLShow);    
  }

  //clear all
  CLEARAll();
  
  Serial.print("elapsed = ");
  Serial.println( millis()- time_now);

  
  }

  void OneSecond_GetTemp() {
       //listen for temp broadcast
    time_now = millis();

    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
       
    while(millis() < time_now + XSecondInterval){
        //wait approx. [period] ms

       if (driver.recv(buf, &buflen)) // Non-blocking
        {

          rcv = "";

          // Message with a good checksum received, dump it.
          //driver.printBuffer("Got:", buf, buflen);

          for (int i = 0; i < buflen; i++) {
            rcv +=  (char)buf[i];
          }

          latestTemperature = (rcv);
        }

        
      }
    }

void lightNumberTemperature() {
int digit1 = 0;
int digit2 = 0;
//int displayXTemp;
String digitsBOTH = "";
String digitSingleWorkVar = "";
long X5seconds;

  time_now = millis();   // get for comparision- we show temperature for a total of 5 seconds

    //2 digits (do the math)
  CLEARAll();

    digitsBOTH = latestTemperature;
    if (digitsBOTH.length() > 2){
       if (digitsBOTH.substring(0) == "-") {
         digitalWrite(negativeSymbol, HIGH);   // turn the '-' LED on 
       } else {
         digitalWrite(XHourDigit1_1, HIGH);   // turn the 1 LED on 
        }
       digitsBOTH = digitsBOTH.substring(1);
    }
      
    if (digitsBOTH.length() < 2){
      digitsBOTH = "0" + digitsBOTH;
      } 

      digitSingleWorkVar = (digitsBOTH.substring(0,1));
      digit1 = abs(digitSingleWorkVar.toInt());
      digit1 = constrain(digit1, 0, 9);
      
      digitSingleWorkVar = (digitsBOTH.substring(1));
      digit2 = abs(digitSingleWorkVar.toInt());
      digit2 = constrain(digit2, 0, 9);
      
    digitalWrite(XHourDigit2_0 + digit1, HIGH);   // turn the proper LED on     
    //Second digit
    if (digit2 < 6) {
      digitalWrite((XMinuteDigit1_0 + digit2), HIGH);   // turn the proper LED on     
    } else {
      digitalWrite((XMinuteDigit1_6 + (digit2 - 6)), HIGH);   // turn the proper LED on          
      }

    digitalWrite(tempSymbol, HIGH);  // turn on temperature symbol

    X5seconds = (XSecondInterval * 5);
    
    // remainder of 5 seconds
        while(millis() < (time_now + X5seconds)){
        } 
      
    digitalWrite(negativeSymbol, LOW);   // turn the '-' LED off
    digitalWrite(tempSymbol, LOW);  // turn OFF temperature symbol
    
  CLEARAll();
    
  }
