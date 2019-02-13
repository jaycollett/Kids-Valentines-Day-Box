// *******************************************************************************
//
// Valentine's Day Box Firmware v1.0 (2/12/2019)
// A simple firmware to allow the Feather M0 to create an amazing expirence
// for a kid's Valentine's day box.
//
// Written by Jay Collett (jay AT jaycollett.com)
// http://www.jaycollett.com
//
// Additional unmodified libraries used:
// VS1053 from Adafruit (www.adafruit.com)
//
// This code is licensed under the MIT license.
//
// *******************************************************************************

#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define WINC_EN         2     // enable pin for WINC1500
#define CARDCS          5     // Card chip select pin
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define SENSOR_IN_PIN  12     // Beam break sensor input
#define BOX_LED_LEFT   13     // Controls LEDS around left side of box
#define BOX_LED_RIGHT  14     // Controls LEDS around right side of box (A0 pin = 14)
#define HEART_BLUE_LED 16     // Controls the bullseye in the top heart
#define HEART_RED_LED1 18     // Controls one half of the top heart lobe
#define HEART_RED_LED2 19     // Controls one half of the top heart lobe

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

char* music[] = {
  "/track01.mp3", // Put your valentine in the box (Charlie Brown)...
  "/track02.mp3", // Im afraid to look (Charlie Brown)...
  "/track03.mp3", // Chance to have a Valentime (Charlie Brown)...
  "/track04.mp3", // Happy Valentine's Day (Female Voice)...
  "/track05.mp3", // Bon Jovi Give love a bad name...
  "/track06.mp3", // Lionel Richie Hello...
  "/track07.mp3", // Roxette It must have been love...
  "/track08.mp3" // Elton John Can you feel the love...
  
};
#define NUM_TRACKS (sizeof(music) / sizeof(char*))


//**************************************************************************************************************************
// SETUP ROUTINE
//**************************************************************************************************************************
void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);

  if (! musicPlayer.begin()) { // initialise the music player
    Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
    while (1);
  }

  Serial.println(F("VS1053 found"));
  delay(100);

  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  delay(250);

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(14, 14);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Configure LED control pins as output
  pinMode(BOX_LED_LEFT, OUTPUT);
  pinMode(BOX_LED_RIGHT, OUTPUT);
  pinMode(HEART_RED_LED1, OUTPUT);
  pinMode(HEART_RED_LED2, OUTPUT);
  pinMode(HEART_BLUE_LED, OUTPUT);
  pinMode(WINC_EN, OUTPUT);

  // default all LEDs as off...
  digitalWrite(BOX_LED_LEFT, LOW);
  digitalWrite(BOX_LED_RIGHT, LOW);
  digitalWrite(HEART_RED_LED1, LOW);
  digitalWrite(HEART_RED_LED2, LOW);
  digitalWrite(HEART_BLUE_LED, LOW);

  // disable Wifi (to save some juice)
  digitalWrite(WINC_EN, LOW);

  // Beam break sensor setup (int pullup)
  pinMode(SENSOR_IN_PIN, INPUT_PULLUP);
}


//**************************************************************************************************************************
// MAIN LOOP
//**************************************************************************************************************************
void loop() {

  if (digitalRead(SENSOR_IN_PIN) == LOW) {
    // very simple debounce-type feature, not too long here tho, cards can drop through this quickly...
    delay(75);
    if (digitalRead(SENSOR_IN_PIN) == LOW) {
      Serial.println("Bean was broken, running LED animation.");
      runAnimation(getRandomAnimationValue());
    }
  }
}


void runAnimation(int animationNumber) {

  if (musicPlayer.stopped())
  {
    int trackNumber = random(0, NUM_TRACKS);
    Serial.print("Playing random track number: ");
    Serial.println(trackNumber);
    musicPlayer.startPlayingFile(music[trackNumber]);
  }

  Serial.print("Running animation number: ");
  Serial.println(animationNumber);

  // run the first of four animation sequences when a valentine is dropped in the box...
  if (animationNumber == 1) {
    for (int i = 0; i <= 6; i++) {
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(HEART_RED_LED2, HIGH);
      digitalWrite(BOX_LED_LEFT, HIGH);
      delay(50);
      digitalWrite(BOX_LED_LEFT, LOW);
      delay(30);
      digitalWrite(HEART_RED_LED1, LOW);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(50);
      digitalWrite(HEART_RED_LED2, LOW);
      digitalWrite(BOX_LED_RIGHT, HIGH);
      delay(50);
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(HEART_RED_LED2, HIGH);
      delay(50);
      digitalWrite(BOX_LED_RIGHT, LOW);
      digitalWrite(HEART_BLUE_LED, LOW);
      delay(50);
    }
    allLEDsOFF();
  } else if (animationNumber == 2) {
    for (int i = 0; i <= 4; i++) {
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(HEART_RED_LED2, HIGH);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(60);
      digitalWrite(BOX_LED_LEFT, HIGH);
      delay(30);
      digitalWrite(HEART_BLUE_LED, LOW);
      digitalWrite(HEART_RED_LED1, LOW);
      delay(50);
      digitalWrite(BOX_LED_LEFT, LOW);
      digitalWrite(BOX_LED_RIGHT, HIGH);
      delay(50);
      digitalWrite(HEART_RED_LED1, LOW);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(50);
      digitalWrite(BOX_LED_RIGHT, LOW);
      digitalWrite(HEART_RED_LED2, LOW);
      delay(50);
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(HEART_RED_LED2, HIGH);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(75);
      digitalWrite(HEART_RED_LED1, LOW);
      digitalWrite(HEART_RED_LED2, LOW);
      digitalWrite(HEART_BLUE_LED, LOW);
      delay(75);
    }
    allLEDsOFF();
  } else if (animationNumber == 3) {
    for (int i = 0; i <= 8; i++) {
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(70);
      digitalWrite(HEART_RED_LED1, LOW);
      digitalWrite(HEART_RED_LED2, HIGH);
      digitalWrite(BOX_LED_LEFT, HIGH);
      delay(60);
      digitalWrite(HEART_BLUE_LED, LOW);
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(BOX_LED_LEFT, LOW);
      digitalWrite(BOX_LED_RIGHT, HIGH);
      delay(70);
      digitalWrite(HEART_RED_LED2, LOW);
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(BOX_LED_RIGHT, LOW);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(70);
    }
    allLEDsOFF();
  } else if (animationNumber == 4) {
    for (int i = 0; i <= 5; i++) {
      digitalWrite(BOX_LED_LEFT, HIGH);
      digitalWrite(BOX_LED_RIGHT, HIGH);
      delay(50);
      digitalWrite(BOX_LED_RIGHT, LOW);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(70);
      digitalWrite(HEART_RED_LED2, HIGH);
      digitalWrite(BOX_LED_LEFT, LOW);
      delay(50);
      digitalWrite(HEART_RED_LED1, HIGH);
      digitalWrite(BOX_LED_RIGHT, HIGH);
      digitalWrite(HEART_BLUE_LED, LOW);
      delay(80);
      digitalWrite(HEART_RED_LED2, LOW);
      digitalWrite(BOX_LED_RIGHT, LOW);
      digitalWrite(HEART_BLUE_LED, HIGH);
      delay(60);
    } else if (animationNumber == 5) {
      for (int i = 0; i <= 4; i++) {
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(25);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(25);
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(25);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(25);
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(25);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(25);
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(25);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(25);
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(25);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(25);
        digitalWrite(BOX_LED_RIGHT, HIGH);
        delay(80);
        digitalWrite(HEART_RED_LED2, LOW);
        digitalWrite(BOX_LED_RIGHT, LOW);
        digitalWrite(HEART_BLUE_LED, HIGH);
        delay(60);
        digitalWrite(BOX_LED_RIGHT, HIGH);
        digitalWrite(HEART_BLUE_LED, LOW);
        delay(80);
        digitalWrite(BOX_LED_RIGHT, LOW);
        digitalWrite(HEART_RED_LED1, HIGH);
        digitalWrite(HEART_RED_LED2, HIGH);
        delay(50);
        digitalWrite(HEART_RED_LED1, LOW);
        digitalWrite(HEART_RED_LED2, LOW);
      }
      allLEDsOFF();
    }

  }

  //**************************************************************************************
  //
  // Method to ensure all LEDs are turned off, called by any animation routine
  //
  //**************************************************************************************
  void allLEDsOFF() {
    digitalWrite(BOX_LED_LEFT, LOW);
    digitalWrite(BOX_LED_RIGHT, LOW);
    digitalWrite(HEART_RED_LED1, LOW);
    digitalWrite(HEART_RED_LED2, LOW);
    digitalWrite(HEART_BLUE_LED, LOW);
  }

  //**************************************************************************************
  //
  // Method used to get a random number from the number of available animations. This
  // provides a more dynamic LED light show...
  //
  // RETURNS: INT value which is a random number between 1 and 4
  //**************************************************************************************
  int getRandomAnimationValue() {
    // setup our randomizer to play a random song when the beam break is triggered
    randomSeed(analogRead(A1));
    int randomSeed1 = random(1, 100);
    int randomSeed2 = random(1, 100);
    randomSeed(randomSeed1 + randomSeed2);
    return random(1, 6); // random number between 1 and 5 (6 excluded)
  }
