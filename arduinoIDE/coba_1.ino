#include <FS.h>
#include <FSImpl.h>
#include <vfs_api.h>


// Libraries for SD card
#include "FS.h"
#include "SD.h"
#include <SPI.h>

// Libraries for RTC
#include <Wire.h>
#include "RTClib.h"

// Define RTC
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define Ultrasonic
#define trigPin 17
#define echoPin 12

// Define ultrasonic variables:
long duration;
int distance;

//Define infrared variables
int sensorpin = 34;   // analog pin used to connect the sharp sensor
int val = 0;        // variable to store the values from sensor(initially zero)

//Define Magnetik variables
int led = 13 ; // LED on esp32
//int digitalPin = 13; // linear Hall magnetic sensor digital interface
int analogPin = 36; // linear Hall magnetic sensor analog interface
//int digitalVal ; // digital readings
int analogVal; // analog readings

// Define Cahaya
int sensorCahaya;

// Define CS pin for the SD card module
#define SD_CS 5

// Save reading number on RTC memory
RTC_DATA_ATTR int readingID = 0;

String dataMessage;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(9600);

  delay(3000); // wait for console opening
 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
       // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    //rtc.adjust(DateTime(2021, 1, 13, 16, 1, 1));
  }

  // Initialize SD card
  SD.begin(SD_CS);  
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/test9.txt");
  if(!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/test9.txt", "Reading ID, Day, YYYY/MM/DD, hh:mm:ss, Ultrasonic, Infrared, Magnetik, Cahaya \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();

}

void loop() {
  // Define ultrasonic inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance:
  distance = duration*0.034/2;
  
  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
  Serial.print("Ultrasonic: ");
  Serial.print(distance);
  Serial.println(" cm");
 

  //Define magnetik inputs and outputs
  pinMode (led, OUTPUT); 
  pinMode(analogPin, INPUT);

  //MAGNETIK
   analogVal = analogRead(analogPin);
  Serial.print("AO KY-024: ");
  Serial.println(analogVal); // print analog value
   
  
  //INFRARED
   val = analogRead(sensorpin); // reads the value of the sharp sensor
  Serial.print("Infrared: ");
  Serial.println(val);        // prints the value of the sensor to the serial monitor
        // wait for this much time before printing next value

 
  //CAHAYA
   sensorCahaya = analogRead(35);
  Serial.print("AO LDR: ");
  Serial.println(sensorCahaya);       // Kirim nilai sensor ke serial monitor
  
  logSDCard();
  
  // Increment readingID on every new reading
  readingID++;
  
  delay(5000);
}


// Write the sensor readings on the SD card
void logSDCard() {
  DateTime now = rtc.now();
  dataMessage = String(readingID) + "," + String(daysOfTheWeek[now.dayOfTheWeek()]) + "," + String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + "," + 
                String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "," + 
                String(distance) +"," + String(val) + "," + String(analogVal) + "," + String(sensorCahaya) +   "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/test9.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
