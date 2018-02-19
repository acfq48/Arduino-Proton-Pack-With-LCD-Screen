

//******************LCD Setup***************************
        #include <Wire.h>
        #include <LCD.h>
        #include <LiquidCrystal_I2C.h> // F Malpartida's NewLiquidCrystal library
        #define I2C_ADDR 0x27 // Define I2C Address for controller
        #define En_pin 2
        #define Rw_pin 1
        #define Rs_pin 0
        #define D4_pin 4
        #define D5_pin 5
        #define D6_pin 6
        #define D7_pin 7
        #define BACKLIGHT 3
        
        LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);



//LCD - Ghost Character 1
byte ghostCharacter[8] = {
  0b00000,
  0b01110,
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b10101,
  0b00000
};

//LCD - Lightning Bolt Character
byte lightningBolt[8] = {
  B00010,
  B00100,
  B01000,
  B11111,
  B00010,
  B00100,
  B01000,
};

//LCD - Lock Character
byte lockCharacter[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
  0b00000
};

//LCD - Unlock Character
byte unlockCharacter[8] = {
  0b01110,
  0b10001,
  0b00001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
  0b00000
};

//LCD - Inverse Cross
byte inverseCross[8] = {
  0b10010,
  0b10010,
  0b10111,
  0b10010,
  0b10010,
  0b10010,
  0b10010,
  0b00000
};


//LCD No Sign Left Half
byte noSignLeft[8] = {
  0b00011,
  0b00100,
  0b01000,
  0b01000,
  0b01001,
  0b01010,
  0b00100,
  0b00011
};


//LCD No Sign Right Half
byte noSignRight[8] = {
  0b11000,
  0b00100,
  0b01010,
  0b10010,
  0b00010,
  0b00010,
  0b00100,
  0b11000
};

//LCD Note Character
byte noteCharacter[8] = {
  0b00100,
  0b00110,
  0b00101,
  0b00101,
  0b00100,
  0b11100,
  0b11100,
  0b00000
};







//******************LCD Setup***************************


#include <QueueArray.h>

// for the sound board
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

#include <Adafruit_NeoPixel.h>

// for led triggers
#define HIGH 0x1
#define LOW  0x0

// neopixel pins / setup
#define NEO_POWER 2 // for cyclotron and powercell
Adafruit_NeoPixel powerStick = Adafruit_NeoPixel(48, NEO_POWER, NEO_GRB + NEO_KHZ800);

#define NEO_NOSE 3 // for nose of wand
Adafruit_NeoPixel noseJewel = Adafruit_NeoPixel(7, NEO_NOSE, NEO_GRB + NEO_KHZ800);

#define NEO_WAND 4 // for nose of wand
Adafruit_NeoPixel wandLights = Adafruit_NeoPixel(4, NEO_WAND, NEO_GRB + NEO_KHZ800);

// LED indexes into the neopixel powerstick chain for the cyclotron
const int powercellLedCount = 14;                                         // total number of led's in the animation
const int powercellIndexOffset = 1;                                       // first led offset into the led chain for the animation

int c1Start = 16;
int c1End = 22;
int c2Start = 23;
int c2End = 29;
int c3Start = 30;
int c3End = 36;
int c4Start = 37;
int c4End = 43;
int ventStart = 44;
int ventEnd = 47;

// inputs for switches and buttons
const int THEME_SWITCH = 5;
const int STARTUP_SWITCH = 6;
const int SAFETY_SWITCH = 7;
const int FIRE_BUTTON = 8;

// soundboard pins and setup
#define SFX_RST 9
#define SFX_RX 10
#define SFX_TX 11
const int ACT = 12;    // this allows us to know if the audio is playing

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard( &ss, NULL, SFX_RST);

// ##############################
// available options
// ##############################
const bool useGameCyclotronEffect = true;   // set this to true to get the fading previous cyclotron light in the idle sequence
const bool useCyclotronFadeInEffect = false; // Instead of the yellow alternate flashing on boot/vent this fades the cyclotron in from off to red
const bool useDialogTracks = true;          // set to true if you want the dialog tracks to play after firing for 5 seconds
const bool useThemeTracks = true;           // set to true if you want the theme tracks to play after hitting the theme button


// Possible Pack states
bool powerBooted = false;   // has the pack booted up
bool isFiring = false;      // keeps track of the firing state
bool shouldWarn = false;    // track the warning state for alert audio
bool shuttingDown = false;  // is the pack in the process of shutting down
bool poweredDown = true;    // is the pack powered down
bool venting = false;       // is the pack venting

// physical switch states
bool startup = false;
bool theme = true;  //Alex - Change was made here from false to true in order to stop the theme music from playing directly on startup of battery pack (3 themes seem to be cycling ok)
bool safety = false;
bool fire = false;
bool warning = false;

// audio track names on soundboard
const String startupTrack =   "T00     WAV";
const String blastTrack =     "T01     WAV";
const String endTrack =       "T02     WAV";
const String idleTrack =      "T03     WAV";
const String shutdownTrack =  "T04     WAV";
const String clickTrack =     "T05     WAV";
const String chargeTrack =    "T06     WAV";
const String warnTrack =      "T07     WAV";
const String ventTrack =      "T08     WAV";
const String texTrack =       "T09     WAV";
const String choreTrack =     "T10     WAV";
const String toolsTrack =     "T11     WAV";
const String listenTrack =    "T12     WAV";
const String thatTrack =      "T13     WAV";
const String ididthatTrack=   "T14     WAV";  //Formally neutronizedTrack
const String positronTrack =  "T15     WAV";  //Formally boxTrack
const String wasntTrack =     "T16     WAV";  //Recently Added
const String particleTrack =  "T17     WAV";  //Recently Added
const String boxTrack =       "T18     WAV";  //Recently Added
const String themeTrack1 =     "T01     OGG";
const String themeTrack2 =     "T02     OGG";
const String themeTrack3 =     "T03     OGG";
const String themeTrack4 =     "T04     OGG";
const String themeTrack5 =     "T05     OGG";

// this queue holds a shuffled list of dialog tracks we can pull from so we don't
// play the same ones twice
QueueArray <int> dialogQueue;
int numDialog = 10;

QueueArray <int> themeQueue;
int numTheme = 5;

// timer trigger times/states
unsigned long firingStateMillis;
const int firingWarmWaitTime = 1000;  // how long to hold down fire for lights to speed up
const int firingWarnWaitTime = 7000;  // how long to hold down fire before warning sounds

byte count = 0;
// Arduino setup function
  














//VOID SETUP #############################################################################################################################################################################

void setup() {    
  // softwareserial at 9600 baud for the audio board
  ss.begin(9600);


//LCD***********************************************************  
        lcd.begin (16,1); // initialize the lcd
      
      // Switch on the backlight
      lcd.setBacklightPin(BACKLIGHT,POSITIVE);
      lcd.setBacklight(LOW);
      
      // set up the LCD's number of columns and rows:
        lcd.begin(16, 2);
        // Print a message to the LCD.
        lcd.setCursor(0,0);
        lcd.print("Warming Up");
        delay(500);
        lcd.clear();
        delay(300);
        lcd.print("Warming Up");
        delay(500);
        lcd.clear();
        delay(300);
        lcd.print("Warming Up");
        delay(500);
        lcd.clear();
        lcd.noBacklight();


        //LCD Custom Character Loadouts   



        lcd.createChar(1, ghostCharacter);
        lcd.createChar(2, lightningBolt);
        lcd.createChar(3, lockCharacter);
        lcd.createChar(4, unlockCharacter);
        lcd.createChar(5, inverseCross);
        lcd.createChar(6, noSignLeft);
        lcd.createChar(7, noSignRight);
        lcd.createChar(8, noteCharacter);

        
//LCD***********************************************************












  // see if we have the soundboard
  // If we fail to communicate, loop forever for now but it would be nice to warn the user somehow
  if (!sfx.reset()) {
    while (1);
  }

  // set act modes for the fx board
  pinMode(ACT, INPUT);

  // configure nose jewel
  noseJewel.begin();
  noseJewel.setBrightness(100);
  noseJewel.show(); // Initialize all pixels to 'off'

  // configure powercell/cyclotron
  powerStick.begin();
  powerStick.setBrightness(75);
  powerStick.show(); // Initialize all pixels to 'off'

  // configure wand lights
  wandLights.begin();
  wandLights.setBrightness(75);
  wandLights.show();

  // set the modes for the switches/buttons
  pinMode(THEME_SWITCH, INPUT);
  digitalWrite(THEME_SWITCH, HIGH);
  pinMode(STARTUP_SWITCH, INPUT);
  digitalWrite(STARTUP_SWITCH, HIGH);
  pinMode(SAFETY_SWITCH, INPUT);
  digitalWrite(SAFETY_SWITCH, HIGH);
  pinMode(FIRE_BUTTON, INPUT);
  digitalWrite(FIRE_BUTTON, HIGH);
}

/* ************* Audio Board Helper Functions ************* */
// helper function to play a track by name on the audio board
void playAudio( String trackname, int playing ) {
  // stop track if one is going
  if (playing == 0) {
    sfx.stop();
  }
  
  char charName[20];
  trackname.toCharArray(charName, 20);
  
  // now go play
  if (sfx.playTrack(charName)) {
    sfx.unpause();
  }
}

void playDialogTrack( int playing ){
  // if the queue is empty reseed it
  if ( dialogQueue.isEmpty() ){
    for (int i=1; i<=numDialog; i++){
      dialogQueue.enqueue(i);
    }
  }
  
  switch (dialogQueue.dequeue()){
    case (5):
      playAudio(texTrack, playing);
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      
      lcd.setCursor(0,0);
      lcd.print("Whoa, Whoa, Whoa...  Nice Shootin'");
      lcd.setCursor(0,1);
      lcd.print("                         Tex!"); 
      delay(1900);      
      for (int positionCounter = 0; positionCounter < 19; positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(70);
      }
      delay(2200);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (4):
      playAudio(listenTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Listen......    Do you smell");
      lcd.setCursor(0,1);
      lcd.print("                   something?"); 
      delay(3100);
      for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(50);
      }
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("Do you smell");
      lcd.setCursor(3,1);
      lcd.print("something?");
      delay(2000);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (6):
      playAudio(choreTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("That wasnt such a chore now was it?");
      delay(1500);
      for (int positionCounter = 0; positionCounter < 19; positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(150);
      }
      delay(1000);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (8):
      playAudio(positronTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Slime a guy with a positron collider!");
      lcd.setCursor(16,1);
      lcd.write(byte(1));
      delay(1100);
      for (int positionCounter = 0; positionCounter < 21; positionCounter++) {
      lcd.scrollDisplayLeft();
      delay(170);
      }
      delay(1000);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (3):
      playAudio(thatTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("So be good, for");
      lcd.setCursor(0,1);
      lcd.print("goodness' sake.");
      delay(4600);
      lcd.clear();
      lcd.setCursor(6,1);
      lcd.print("Whoa");
      delay(1500);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (7):
      playAudio(ididthatTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("I did that."); 
      lcd.setCursor(2,1);
      lcd.print("I did that.");
      delay(3500);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("That's my fault.");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Its ok the table");
      lcd.setCursor(0,1);
      lcd.print("broke the fall.");
      delay(3000);
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (1):
      playAudio(toolsTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("We had");
      lcd.setCursor(3,1);
      lcd.print("the tools.");
      delay(3500);
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("We had");
      lcd.setCursor(3,1);
      lcd.print("the talent!");
      delay(1500);
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("It's Miller Time");
      delay(3000);
      lcd.clear();
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (10):
      playAudio(wasntTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Wasn't So Hard");
      delay (4000);
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (9):
      playAudio(particleTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("We neutronized");
      lcd.setCursor(7,1);
      lcd.print("it.");
      delay(3500);
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("100% Particle");
      lcd.setCursor(4,1);
      lcd.print("Reversal");
      delay (3000);
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
    case (2):
      playAudio(boxTrack, playing);
            //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Two In The Box");
      lcd.setCursor(3,1);
      lcd.print("Ready To Go");
      delay(4000);
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("We Be Fast");
      lcd.setCursor(0,1);
      lcd.print("And They Be Slow");
      delay(3000);
      //LCD WRITE------------------------------------------------------------------------------------------------------------------------------
      break;
            
      
    default: 
      playAudio(endTrack, playing);
      break;
  }
}

void playThemeTrack( int playing ){
  // if the queue is empty reseed it
  if ( themeQueue.isEmpty() ){
    for (int a=1; a<=numTheme; a++){
      themeQueue.enqueue(a);
    }
  }
  
  switch (themeQueue.dequeue()){
    case (1):
      playAudio(themeTrack1, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(2,0);
      lcd.print("Ghostbusters");
      lcd.setCursor(15,0);
      lcd.write (byte(8));
      lcd.setCursor(1,1);
      lcd.print("Ray Parker Jr.");
      break;
    case (2):
      playAudio(themeTrack2, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(3,0);
      lcd.print("On Our Own");
      lcd.setCursor(15,0);
      lcd.write (byte(8));      
      lcd.setCursor(2,1);
      lcd.print("Bobby  Brown");
      break;
    case (3):
      playAudio(themeTrack3, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(1,0);
      lcd.print("Saving the Day");
      lcd.setCursor(15,0);
      lcd.write (byte(8));
      lcd.setCursor(0,1);
      lcd.print("Alessi  Brothers");
      break;
    case (4):
      playAudio(themeTrack4, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(5,0);
      lcd.print("Higher");
      lcd.setCursor(15,0);
      lcd.write (byte(8));      
      lcd.setCursor(1,1);
      lcd.print("Jackie  Wilson");
      break;
    case (5):
      playAudio(themeTrack5, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(3,0);
      lcd.print("Main Theme");
      lcd.setCursor(15,0);
      lcd.write (byte(8));      
      lcd.setCursor(0,1);
      lcd.print("Elmer  Bernstein");
      break;
    default: 
      playAudio(themeTrack1, playing);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write (byte(8));      
      lcd.setCursor(1,0);
      lcd.print("Saving the Day");
      lcd.setCursor(15,0);
      lcd.write (byte(8));
      lcd.setCursor(0,1);
      lcd.print("Alessi  Brothers");
      break;
  }
}

//LCD TIME SET UPS------------------------------------------------------------------------------------------------------------------------------------------









//LCD TIME SET UPS------------------------------------------------------------------------------------------------------------------------------------------






/* ************* Main Loop ************* */
int cyclotronRunningFadeOut = 255;  // we reset this variable every time we change the cyclotron index so the fade effect works
int cyclotronRunningFadeIn = 0;     // we reset this to 0 to fade the cyclotron in from nothing

// intervals that can be adjusted in real time to speed up animations 
int pwr_interval = 60;        // interval at which to cycle lights for the powercell. We update this in the loop to speed up the animation so must be declared here (milliseconds)
int cyc_interval = 1000;      // interval at which to cycle lights for the cyclotron.
int cyc_fade_interval = 15;   // fade the inactive cyclotron to light to nothing
int firing_interval = 40;     // interval at which to cycle firing lights on the bargraph. We update this in the loop to speed up the animation so must be declared here (milliseconds).

void loop() {
  // get the current time
  int currentMillis = millis();

  // find out of the audio board is playing audio
  int playing = digitalRead(ACT);

  // get the current switch states
  int theme_switch = digitalRead(THEME_SWITCH);

  // if the theme switch has recently changed from off to on we 
  // should play the full ghostbusters theme song
  if (theme_switch == 1) {
    if (theme == false) {
      playThemeTrack(playing);   
      theme = true;
    }
  } else {
    theme = false;
  }

  int startup_switch = digitalRead(STARTUP_SWITCH);
  int safety_switch = digitalRead(SAFETY_SWITCH);
  int fire_button = digitalRead(FIRE_BUTTON);
  
  // while the startup switch is set on
  if (startup_switch == 1) {   
    // in general we always try to play the idle sound if started
    if (playing == 1 && startup == true) {
      playAudio(idleTrack, playing);
      
        //LCD------------------------------------------------------------------------------------------------------------------------------ 
        unsigned long prevMillisLCD = 0;    // the last time we changed a powercell light in the boot sequence
        int intervalLCD = 100;       // interval at which to cycle lights (milliseconds). Adjust this if 


        if(currentMillis - prevMillisLCD > intervalLCD){
          prevMillisLCD = currentMillis;
          count++;
            if (count == 1){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Proton Grid - OK");
              lcd.setCursor (0,1);
              lcd.print("Cycle Check - OK");
            }      
            if (count == 2){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor (0,0);
              lcd.write(byte(1));
              lcd.setCursor(2,0);
              lcd.print("Ghostbusters");
              lcd.setCursor (15,0);
              lcd.write(byte(1));
              lcd.setCursor (0,1);
              lcd.print("Call: 2125552368");
            }      
            if (count == 3){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Synchrotron-100%");
              lcd.setCursor (0,1);
              lcd.print("Nuclear POW-100%");
            }      
            if (count == 4){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(1,0);
              lcd.print("We're Ready To");
              lcd.setCursor (2,1);
              lcd.print("Believe You!");
            }   
            if (count == 5){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Power Cells - OK");
              lcd.setCursor (0,1);
              lcd.print("Ecto Filter - OK");
            }   
            if (count == 6){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("No Job Is 2 Big");
              lcd.setCursor (0,1);
              lcd.print("No Fee Is 2 Big");
            }   
            if (count == 7){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(5,0);
              lcd.print("DANGER");
              lcd.setCursor (0,1);
              lcd.write(byte(2));
              lcd.setCursor (2,1);
              lcd.print("High Voltage");
              lcd.setCursor (15,1);
              lcd.write(byte(2));
            }   
            if (count == 8){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.write(byte(6));
              lcd.setCursor (1,0);
              lcd.write(byte(7));
              lcd.setCursor(3,0);
              lcd.print("Paranormal");
              lcd.setCursor(14,0);
              lcd.write(byte(6));
              lcd.setCursor (15,0);
              lcd.write(byte(7));
              lcd.setCursor (2,1);
              lcd.print("Eliminations");
            }   
              if (count == 9){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Rays Occu");
              lcd.setCursor (9,0);
              lcd.write(byte(5));
              lcd.setCursor(11,0);
              lcd.print("Books");
              lcd.setCursor (0,1);
              lcd.print("7pm M-F 12am Sat");
            }  
              if (count == 10){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("$  Now Hiring  $");
              lcd.setCursor (0,1);
              lcd.print("Full / Part Time");
            }  
              if (count == 11){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Must Buy: Tobins");
              lcd.setCursor (0,1);
              lcd.print("Spirit Guide #2");
            }  
              if (count == 12){
              lcd.clear();
              lcd.backlight();
              lcd.setCursor(0,0);
              lcd.print("Hi-C Ecto Cooler");
              lcd.setCursor(0,1);
              lcd.write(byte(1));
              lcd.setCursor (2,1);
              lcd.print("RIP  1989-97");
              lcd.setCursor(15,1);
              lcd.write(byte(1));
              count = 0;
            }  
        }
        //LCD------------------------------------------------------------------------------------------------------------------------------ 
     
    }
    
    // choose the right powercell animation sequence for booted/on
    if ( powerBooted == true ) {
      // standard idle power sequence for the pack
      poweredDown = false;
      shuttingDown = false;
      venting = false;
      setWandLightState(3, 0, 0); //set sloblow red
      setVentLightState(ventStart, ventEnd, 2);
      powerSequenceOne(currentMillis, pwr_interval, cyc_interval, cyc_fade_interval);
    } else {
      // boot up the pack. powerSequenceBoot will set powerBooted when complete
      powerSequenceBoot(currentMillis);
      setWandLightState(3, 7, currentMillis);   //set sloblow red blinking
    }

    // if we are not started up we should play the startup sound and begin the boot sequence
    if (startup == false) {
      startup = true;
        playAudio(startupTrack, playing);
              
        //LCD------------------------------------------------------------------------------------------------------------------------------    
        lcd.clear();
        delay(100);
        lcd.backlight(); 
        lcd.clear();
        lcd.print("Proton Pack");
        lcd.setCursor(0,1);
        lcd.print("Initializing...");
        //LCD------------------------------------------------------------------------------------------------------------------------------   
        
      // get the current safety switch state
      if (safety_switch == 1 && safety == false) {
        safety = true;
      }
    }
    
    if( startup == true && safety_switch == 1 ){
      if( venting == false && powerBooted == true ){
        setWandLightState(1, 2, 0);    //  set back light orange
        setWandLightState(2, 1, 0);    //  set body led white
      }else{
        setWandLightState(1, 4, 0);    //  set back light off
        setWandLightState(2, 4, 0);    //  set body led off
      }

      // if the safety switch is set off then we can fire when the button is pressed
      if ( fire_button == 0) {
        // if the button is just pressed we clear all led's to start the firing animations
        if ( isFiring == false ) {
          shutdown_leds();
          isFiring = true;
        }
  
        // show the firing bargraph sequence
        barGraphSequenceTwo(currentMillis);

        // strobe the nose pixels
        fireStrobe(currentMillis); 

        // now powercell/cyclotron/wand lights
        // if this is the first time reset some variables and play the blast track
        if (fire == false) {
          shouldWarn = false;
          fire = true;
          firingStateMillis = millis();
          playAudio(blastTrack, playing);

      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Caution: Do Not");
      lcd.setCursor(0,1);
      lcd.print("Cross Streams");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
     
        } else {
          // find out what our timing is
          long diff = millis() - firingStateMillis;
          
          if ( diff > firingWarnWaitTime) { // if we are in the fire warn interval
            pwr_interval = 10;      // speed up the powercell animation
            firing_interval = 20;   // speed up the bar graph animation
            cyc_interval = 50;      // really speed up cyclotron
            cyc_fade_interval = 5;  // speed up the fade of the cyclotron
            if (playing == 1 || shouldWarn == false ) {
              shouldWarn = true;
              playAudio(warnTrack, playing); // play the firing track with the warning
      
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Warning:");
      lcd.setCursor(0,1);
      lcd.print("System Overheat!");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
             }
            setWandLightState(0, 8, currentMillis);    // set top light red flashing fast
          } else if ( diff > firingWarmWaitTime) { // if we are in the dialog playing interval
            pwr_interval = 30;      // speed up the powercell animation
            firing_interval = 30;   // speed up the bar graph animation
            cyc_interval = 200;     // speed up cyclotron
            cyc_fade_interval = 10; // speed up the fade of the cyclotron
            if (playing == 1) {
              playAudio(blastTrack, playing); // play the normal blast track
                    
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Caution: Do Not");
      lcd.setCursor(0,1);
      lcd.print("Cross Streams");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
            }
            setWandLightState(0, 6, currentMillis);    // set top light orange flashing
          }
        }
      } else { // if we were firing and are no longer reset the leds
        if ( isFiring == true ) {
          shutdown_leds();
          isFiring = false;
        }
  
        // and do the standard bargraph sequence
        barGraphSequenceOne(currentMillis);

        if (fire == true) { // if we were firing let's reset the animations and play the correct final firing track
          clearFireStrobe();
          setWandLightState(0, 4, currentMillis);    // set top light off
          
          pwr_interval = 60;
          firing_interval = 40;
          cyc_interval = 1000;
          cyc_fade_interval = 15;
          fire = false;

          // see if we've been firing long enough to get the dialog or vent sounds
          long diff = millis() - firingStateMillis;

          if ( diff > firingWarnWaitTime) { // if we are past the warning let's vent the pack
            playAudio(ventTrack, playing);
                  
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("< Pack Venting >");
      lcd.setCursor(3,1);
      lcd.print("Stand Back");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
            venting = true;
            clearPowerStrip(); // play the boot animation on the powercell
          } else if ( diff > firingWarmWaitTime) { // if in the dialog time play the dialog in sequence
            if( useDialogTracks == true ){
              playDialogTrack(playing);
            }else{
              playAudio(endTrack, playing);
                                
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("System Resetting");
      lcd.setCursor(1,1);
      lcd.print("Please Wait...");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
            }
          } else {
            playAudio(endTrack, playing);
                                            
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("System Resetting");
      lcd.setCursor(1,1);
      lcd.print("Please Wait...");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
          }
        }
      }

      // if the safety was just changed play the click track
      if (safety == false) {
        safety = true;
        playAudio(chargeTrack, playing);
                                        
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.write(byte(4));
      lcd.setCursor(2,0);
      lcd.print("[Safety Off]");
      lcd.setCursor (15,0);
      lcd.write(byte(4));
      lcd.setCursor(0,1);
      lcd.print("System Unlocked!");
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
      }
    } else {
      // if the safety is switched off play the click track
      if (safety == true) {
        setWandLightState(1, 4, 0);    // set back light off
        setWandLightState(2, 4, 0);    // set body off
        safety = false;
        playAudio(clickTrack, playing);
                                        
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.write(byte(3));
      lcd.setCursor(2,0);
      lcd.print("[Safety On]");
      lcd.setCursor (15,0);
      lcd.write(byte(3));
      lcd.setCursor(0,1);
      lcd.print("System Lockdown");  
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      
      }
    }
  } else { // if we are powering down
    if( poweredDown == false ){
      if( shuttingDown == false ){
        playAudio(shutdownTrack, playing); // play the pack shutdown track
                                                
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("System Shutdown");
      lcd.setCursor(0,1);
      lcd.print("Cycle Time");
      lcd.setCursor(13, 1);
      // print the number of seconds since reset:
      lcd.print(millis() / 1000);      
      //LCD--------------------------------------------------------------------------------------------------------------------------------------------------

      
        shuttingDown = true;
      }
      cyclotronRunningFadeOut = 255;
      powerSequenceShutdown(currentMillis);

    }else{
      if (startup == true) { // if started reset the variables
        clearPowerStrip(); // clear all led's
        shutdown_leds();
        startup = false;
        safety = false;
        fire = false;
      }
    }
  }
  delay(1);
  
}

/*************** Wand Light Helpers *********************/
unsigned long prevFlashMillis = 0; // last time we changed a powercell light in the idle sequence
bool flashState = false;
int wandFastFlashInterval = 100; // interval at which we flash the top led on the wand
int wandMediumFlashInterval = 500; // interval at which we flash the top led on the wand

void setWandLightState(int lednum, int state, int currentMillis){
  switch ( state ) {
    case 0: // set led red
      wandLights.setPixelColor(lednum, wandLights.Color(255, 0, 0));
      break;
    case 1: // set led white
      wandLights.setPixelColor(lednum, wandLights.Color(255, 255, 255));
      break;
    case 2: // set led orange
      wandLights.setPixelColor(lednum, wandLights.Color(255, 127, 0));
      break;
    case 3: // set led blue
      wandLights.setPixelColor(lednum, wandLights.Color(0, 0, 255));
      break;
    case 4: // set led off
      wandLights.setPixelColor(lednum, 0);
      break;
    case 5: // fast white flashing    
      if (currentMillis - prevFlashMillis > wandFastFlashInterval) {    
        prevFlashMillis = currentMillis;    
        if( flashState == false ){    
          wandLights.setPixelColor(lednum, wandLights.Color(255, 255, 255));    
          flashState = true;    
        }else{    
          wandLights.setPixelColor(lednum, 0);    
          flashState = false;   
        }   
      }   
      break;
    case 6: // slower orange flashing    
      if (currentMillis - prevFlashMillis > wandMediumFlashInterval) {    
        prevFlashMillis = currentMillis;    
        if( flashState == false ){    
          wandLights.setPixelColor(lednum, wandLights.Color(255, 127, 0));    
          flashState = true;    
        }else{    
          wandLights.setPixelColor(lednum, 0);    
          flashState = false;   
        }   
      }   
      break;
    case 7: // medium red flashing    
      if (currentMillis - prevFlashMillis > wandMediumFlashInterval) {    
        prevFlashMillis = currentMillis;    
        if( flashState == false ){    
          wandLights.setPixelColor(lednum, wandLights.Color(255, 0, 0));    
          flashState = true;    
        }else{    
          wandLights.setPixelColor(lednum, 0);    
          flashState = false;   
        }   
      }   
      break;
    case 8: // fast red flashing    
      if (currentMillis - prevFlashMillis > wandFastFlashInterval) {    
        prevFlashMillis = currentMillis;    
        if( flashState == false ){    
          wandLights.setPixelColor(lednum, wandLights.Color(255, 0, 0));    
          flashState = true;    
        }else{    
          wandLights.setPixelColor(lednum, 0);    
          flashState = false;   
        }   
      }   
      break;
  }

  wandLights.show();
}

/*************** Vent Light *************************/
void setVentLightState(int startLed, int endLed, int state ){
  switch ( state ) {
    case 0: // set all leds to white
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, powerStick.Color(255, 255, 255));
      }
      break;
    case 1: // set all leds to blue
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, powerStick.Color(0, 0, 255));
      }
      break;
    case 2: // set all leds off
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, 0);
      }
      break;
  }
}

/*************** Powercell/Cyclotron Animations *********************/
// timer helpers and intervals for the animations
unsigned long prevPwrBootMillis = 0;    // the last time we changed a powercell light in the boot sequence
const int pwr_boot_interval = 60;       // interval at which to cycle lights (milliseconds). Adjust this if 

unsigned long prevCycBootMillis = 0;    // the last time we changed a cyclotron light in the boot sequence
const int cyc_boot_interval = 500;      // interval at which to cycle lights (milliseconds).
const int cyc_boot_alt_interval = 600;      // interval at which to cycle lights (milliseconds).

unsigned long prevShtdMillis = 0;       // last time we changed a light in the idle sequence
const int pwr_shutdown_interval = 200;  // interval at which to cycle lights (milliseconds).

unsigned long prevPwrMillis = 0;        // last time we changed a powercell light in the idle sequence
unsigned long prevCycMillis = 0;        // last time we changed a cyclotron light in the idle sequence
unsigned long prevFadeCycMillis = 0;    // last time we changed a cyclotron light in the idle sequence

// LED tracking variables
const int powerSeqTotal = powercellLedCount;                              // total number of led's for powercell 0 based
int powerSeqNum = powercellIndexOffset;                                   // current running powercell sequence led
int powerShutdownSeqNum = powercellLedCount - powercellIndexOffset;       // shutdown sequence counts down

// animation level trackers for the boot and shutdown
int currentBootLevel = powercellIndexOffset;                              // current powercell boot level sequence led
int currentLightLevel = powercellLedCount - powercellIndexOffset;         // current powercell boot light sequence led

void setCyclotronLightState(int startLed, int endLed, int state ){
  switch ( state ) {
    case 0: // set all leds to red
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, powerStick.Color(255, 0, 0));
      }
      break;
    case 1: // set all leds to orange
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, powerStick.Color(255, 106, 0));
      }
      break;
    case 2: // set all leds off
      for(int i=startLed; i <= endLed; i++) {
        powerStick.setPixelColor(i, 0);
      }
      break;
    case 3: // fade all leds from red
      for(int i=startLed; i <= endLed; i++) {
        if( cyclotronRunningFadeOut >= 0 ){
          powerStick.setPixelColor(i, 255 * cyclotronRunningFadeOut/255, 0, 0);
          cyclotronRunningFadeOut--;
        }else{
          powerStick.setPixelColor(i, 0);
        }
      }
      break;
    case 4: // fade all leds to red
      for(int i=startLed; i <= endLed; i++) {
        if( cyclotronRunningFadeIn < 255 ){
          powerStick.setPixelColor(i, 255 * cyclotronRunningFadeIn/255, 0, 0);
          cyclotronRunningFadeIn++;
        }else{
          powerStick.setPixelColor(i, powerStick.Color(255, 0, 0));
        }
      }
      break;
  }
}

// shuts off and resets the powercell/cyclotron leds
void clearPowerStrip() {
  // reset vars
  powerBooted = false;
  poweredDown = true;
  powerSeqNum = powercellIndexOffset;
  powerShutdownSeqNum = powercellLedCount - powercellIndexOffset;
  currentLightLevel = powercellLedCount;
  currentBootLevel = powercellIndexOffset;
  cyclotronRunningFadeIn = 0;
      
      //LCD----------------------------------------------------------------------------------------------------------------------------------   
if( poweredDown == true ){
      if( shuttingDown == true ){   
      lcd.noBacklight();
      }
}
      //LCD----------------------------------------------------------------------------------------------------------------------------------  
  
  // shutoff the leds
  for ( int i = 0; i <= c4End; i++) {
    powerStick.setPixelColor(i, 0);
  }
  powerStick.show();

  for ( int j=0; j<=3; j++ ){
    wandLights.setPixelColor(j, 0);
  }
  wandLights.show();

  if( venting == true ){
    setVentLightState(ventStart, ventEnd, 0);
  }
}

// boot animation on the powercell/cyclotron
bool reverseBootCyclotron = false;
void powerSequenceBoot(int currentMillis) {
  bool doUpdate = false;

  // START CYCLOTRON
  if( useCyclotronFadeInEffect == false ){
    if (currentMillis - prevCycBootMillis > cyc_boot_interval) {
      prevCycBootMillis = currentMillis;

      if( reverseBootCyclotron == false ){
        setCyclotronLightState(c1Start, c1End, 1);
        setCyclotronLightState(c2Start, c2End, 2);
        setCyclotronLightState(c3Start, c3End, 1);
        setCyclotronLightState(c4Start, c4End, 2);
        
        doUpdate = true;
        reverseBootCyclotron = true;
      }else{
        setCyclotronLightState(c1Start, c1End, 2);
        setCyclotronLightState(c2Start, c2End, 1);
        setCyclotronLightState(c3Start, c3End, 2);
        setCyclotronLightState(c4Start, c4End, 1);
        
        doUpdate = true;
        reverseBootCyclotron = false;
      }
    }
  }else{
    if (currentMillis - prevCycBootMillis > cyc_boot_alt_interval) {
      prevCycBootMillis = currentMillis;
      setCyclotronLightState(c1Start, c4End, 4);
      doUpdate = true;
    }
  }
  // END CYCLOTRON
  
  if (currentMillis - prevPwrBootMillis > pwr_boot_interval) {
    // save the last time you blinked the LED
    prevPwrBootMillis = currentMillis;

    // START POWERCELL
    if( currentBootLevel != powerSeqTotal ){
      if( currentBootLevel == currentLightLevel){
        if(currentLightLevel+1 <= powerSeqTotal){
          powerStick.setPixelColor(currentLightLevel+1, 0);
        }
        powerStick.setPixelColor(currentBootLevel, powerStick.Color(0, 0, 255));
        currentLightLevel = powerSeqTotal;
        currentBootLevel++;
      }else{
        if(currentLightLevel+1 <= powerSeqTotal){
          powerStick.setPixelColor(currentLightLevel+1, 0);
        }
        powerStick.setPixelColor(currentLightLevel, powerStick.Color(0, 0, 255));
        currentLightLevel--;
      }
      doUpdate = true;
    }else{
      powerBooted = true;
      currentBootLevel = powercellIndexOffset;
      currentLightLevel = powercellLedCount - powercellIndexOffset;
    }
    // END POWERCELL
  }

  // if we have changed an led
  if( doUpdate == true ){
    powerStick.show(); // commit all of the changes
  }
}

// idle/firing animation for the powercell/cyclotron
int cycOrder = 0;     // which cyclotron led will be lit next
int cycFading = -1;   // which cyclotron led is fading out for game style
void powerSequenceOne(int currentMillis, int anispeed, int cycspeed, int cycfadespeed) {
  bool doUpdate = false;  // keep track of if we changed something so we only update on changes

  // START CYCLOTRON 
  if( useGameCyclotronEffect == true ){ // if we are doing the video game style cyclotron
    if (currentMillis - prevCycMillis > cycspeed) {
      prevCycMillis = currentMillis;
      
      switch ( cycOrder ) {
        case 0:
          setCyclotronLightState(c1Start, c1End, 0);
          setCyclotronLightState(c2Start, c2End, 2);
          setCyclotronLightState(c3Start, c3End, 2);
          cycFading = 0;
          cyclotronRunningFadeOut = 255;
          cycOrder = 1;
          break;
        case 1:
          setCyclotronLightState(c2Start, c2End, 0);
          setCyclotronLightState(c3Start, c3End, 2);
          setCyclotronLightState(c4Start, c4End, 2);
          cycFading = 1;
          cyclotronRunningFadeOut = 255;
          cycOrder = 2;
          break;
        case 2:
          setCyclotronLightState(c1Start, c1End, 2);
          setCyclotronLightState(c3Start, c3End, 0);
          setCyclotronLightState(c4Start, c4End, 2);
          cycFading = 2;
          cyclotronRunningFadeOut = 255;
          cycOrder = 3;
          break;
        case 3:
          setCyclotronLightState(c1Start, c1End, 2);
          setCyclotronLightState(c2Start, c2End, 2);
          setCyclotronLightState(c4Start, c4End, 0);
          cycFading = 3;
          cyclotronRunningFadeOut = 255;
          cycOrder = 0;
          break;
      }
  
      doUpdate = true;
    }
  
    // now figure out the fading light
    if( currentMillis - prevFadeCycMillis > cycfadespeed ){
      prevFadeCycMillis = currentMillis;
      if( cycFading != -1 ){
        switch ( cycFading ) {
          case 0:
            setCyclotronLightState(c4Start, c4End, 3);
            break;
          case 1:
            setCyclotronLightState(c1Start, c1End, 3);
            break;
          case 2:
            setCyclotronLightState(c2Start, c2End, 3);
            break;
          case 3:
            setCyclotronLightState(c3Start, c3End, 3);
            break;
        }
        doUpdate = true;
      }
    }
  }else{ // otherwise this is the standard version
    if (currentMillis - prevCycMillis > cycspeed) {
      prevCycMillis = currentMillis;
      
      switch ( cycOrder ) {
        case 0:
          setCyclotronLightState(c4Start, c4End, 2);
          setCyclotronLightState(c1Start, c1End, 0);
          setCyclotronLightState(c2Start, c2End, 2);
          setCyclotronLightState(c3Start, c3End, 2);
          cycFading = 0;
          cyclotronRunningFadeOut = 255;
          cycOrder = 1;
          break;
        case 1:
          setCyclotronLightState(c1Start, c1End, 2);
          setCyclotronLightState(c2Start, c2End, 0);
          setCyclotronLightState(c3Start, c3End, 2);
          setCyclotronLightState(c4Start, c4End, 2);
          cycFading = 1;
          cyclotronRunningFadeOut = 255;
          cycOrder = 2;
          break;
        case 2:
          setCyclotronLightState(c1Start, c1End, 2);
          setCyclotronLightState(c2Start, c2End, 2);
          setCyclotronLightState(c3Start, c3End, 0);
          setCyclotronLightState(c4Start, c4End, 2);
          cycFading = 2;
          cyclotronRunningFadeOut = 255;
          cycOrder = 3;
          break;
        case 3:
          setCyclotronLightState(c1Start, c1End, 2);
          setCyclotronLightState(c2Start, c2End, 2);
          setCyclotronLightState(c3Start, c3End, 2);
          setCyclotronLightState(c4Start, c4End, 0);
          cycFading = 3;
          cyclotronRunningFadeOut = 255;
          cycOrder = 0;
          break;
      }
  
      doUpdate = true;
    }
  }
  // END CYCLOTRON

  // START POWERCELL
  if (currentMillis - prevPwrMillis > anispeed) {
    // save the last time you blinked the LED
    prevPwrMillis = currentMillis;

    for ( int i = powercellIndexOffset; i <= powerSeqTotal; i++) {
      if ( i <= powerSeqNum ) {
        powerStick.setPixelColor(i, powerStick.Color(0, 0, 150));
      } else {
        powerStick.setPixelColor(i, 0);
      }
    }
    
    if ( powerSeqNum <= powerSeqTotal) {
      powerSeqNum++;
    } else {
      powerSeqNum = powercellIndexOffset;
    }

    doUpdate = true;
  }
  // END POWERCELL

  // if we changed anything update
  if( doUpdate == true ){
    powerStick.show();
  }
}

// shutdown animation for the powercell/cyclotron
int cyclotronFadeOut = 255;
void powerSequenceShutdown(int currentMillis) {
  if (currentMillis - prevShtdMillis > pwr_shutdown_interval) {
    prevShtdMillis = currentMillis;

    // START CYCLOTRON
    for(int i=c1Start; i <= c4End; i++) {
      if( cyclotronFadeOut >= 0 ){
        powerStick.setPixelColor(i, 255 * cyclotronFadeOut/255, 0, 0);
        cyclotronFadeOut--;
      }else{
        powerStick.setPixelColor(i, 0);
      }
    }
    // END CYCLOTRON
    
    // START POWERCELL
    for ( int i = powerSeqTotal; i >= powercellIndexOffset; i--) {
      if ( i <= powerShutdownSeqNum ) {
        powerStick.setPixelColor(i, powerStick.Color(0, 0, 150));
      } else {
        powerStick.setPixelColor(i, 0);
      }
    }
    
    powerStick.show();
    
    if ( powerShutdownSeqNum >= powercellIndexOffset) {
      powerShutdownSeqNum--;
    } else {
      poweredDown = true;
      powerShutdownSeqNum = powercellLedCount - powercellIndexOffset;
      cyclotronFadeOut = 255;
    }
    // END POWERCELL
  }
}

/*************** Nose Jewel Firing Animations *********************/
unsigned long prevFireMillis = 0;
const int fire_interval = 50;     // interval at which to cycle lights (milliseconds).
int fireSeqNum = 0;
int fireSeqTotal = 5;

void clearFireStrobe() {
  for ( int i = 0; i < 7; i++) {
    noseJewel.setPixelColor(i, 0);
  }
  noseJewel.show();
  fireSeqNum = 0;
}

void fireStrobe(int currentMillis) {
  if (currentMillis - prevFireMillis > fire_interval) {
    prevFireMillis = currentMillis;
    
    switch ( fireSeqNum ) {
      case 0:
        noseJewel.setPixelColor(0, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(1, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(2, 0);
        noseJewel.setPixelColor(3, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(4, 0);
        noseJewel.setPixelColor(5, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(6, 0);
        break;
      case 1:
        noseJewel.setPixelColor(0, noseJewel.Color(0, 0, 255));
        noseJewel.setPixelColor(1, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(2, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(3, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(4, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(5, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(6, noseJewel.Color(255, 255, 255));
        break;
      case 2:
        noseJewel.setPixelColor(0, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(1, 0);
        noseJewel.setPixelColor(2, noseJewel.Color(0, 0, 255));
        noseJewel.setPixelColor(3, 0);
        noseJewel.setPixelColor(4, noseJewel.Color(0, 0, 255));
        noseJewel.setPixelColor(5, 0);
        noseJewel.setPixelColor(6, noseJewel.Color(255, 0, 0));
        break;
      case 3:
        noseJewel.setPixelColor(0, noseJewel.Color(0, 0, 255));
        noseJewel.setPixelColor(1, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(2, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(3, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(4, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(5, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(6, noseJewel.Color(255, 255, 255));
        break;
      case 4:
        noseJewel.setPixelColor(0, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(1, 0);
        noseJewel.setPixelColor(2, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(3, 0);
        noseJewel.setPixelColor(4, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(5, 0);
        noseJewel.setPixelColor(6, noseJewel.Color(255, 255, 255));
        break;
      case 5:
        noseJewel.setPixelColor(0, noseJewel.Color(255, 0, 255));
        noseJewel.setPixelColor(1, noseJewel.Color(0, 255, 0));
        noseJewel.setPixelColor(2, noseJewel.Color(255, 0, 0));
        noseJewel.setPixelColor(3, noseJewel.Color(0, 0, 255));
        noseJewel.setPixelColor(4, noseJewel.Color(255, 0, 255));
        noseJewel.setPixelColor(5, noseJewel.Color(255, 255, 255));
        noseJewel.setPixelColor(6, noseJewel.Color(0, 0, 255));
        break;
    }
  
    noseJewel.show();
  
    fireSeqNum++;
    if ( fireSeqNum > fireSeqTotal ) {
      fireSeqNum = 0;
    }
  }
}

/*************** Bar Graph Animations *********************/
void shutdown_leds() {
  // stub function for when I re-enable to bargraph
}
void barGraphSequenceOne(int currentMillis) {
  // stub function for when I re-enable to bargraph
}
void barGraphSequenceTwo(int currentMillis) {
  // stub function for when I re-enable to bargraph
}

