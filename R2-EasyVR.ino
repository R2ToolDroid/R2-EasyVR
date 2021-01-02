#include "Arduino.h"
#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(__SAMD21G18A__)
  // Shield Jumper on HW (for Zero, use Programming Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_MONITOR
#elif defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due, use Native Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(12, 13);
  #define pcSerial SERIAL_PORT_MONITOR
#endif



#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


SoftwareSerial ComPort(10,11); //10 RX // 11 TX

#include "EasyVR.h"

EasyVR easyvr(port);

//Groups and Commands
enum Groups
{
  GROUP_0  = 0,
  GROUP_1  = 1,
  GROUP_2  = 2,
  GROUP_3  = 3,
  GROUP_4  = 4,
};

enum Group0 
{
  G0_R2 = 0,
};

enum Group1 
{
  G1_HILF_MIR = 0,
  G1_SETUP = 1,
  G1_MUSIK = 2,
  G1_WIE_GEHTS = 3,
  G1_DRIVE = 4,
  G1_STOP = 5,
  G1_R2 = 6,
};



char *group1[] = { 
  
  "Hilf Mir.Setup.Musik",
  "Wie gehts.Drive.Stop",
  "R2"
  };

 // "Testzeichensazluunge1"

enum Group2 
{
  G2_GIB_MIR_FEUER = 0,
  G2_DANKE = 1,
  G2_FESTHALTEN = 2,
  G2_FEST = 3,
  G2_LOS = 4,
  G2_SCHRAUBER = 5,
  G2_TOOL = 6,
  G2_POWER = 7,
  G2_STOP = 8,
  G2_R2 = 9,
};

char *group2[] = { 
  
   "Gib mir Feuer.Danke",
   "Festhalten.Fest.Los",
   "Schrauber.Tool.Power",
    "Stop.R2"};



// "Testzeichensazluunge1"



enum Group3 
{
  G3_AKKUSTAND = 0,
  G3_MODE = 1,
  G3_PANELS = 2,
  G3_SMIRK = 3,
  G3_R2 = 4,
};

char *group3[] = { 
  
   "Akkustand.Mode.Panels",
   "Smirk.R2"};



// "Testzeichensazluunge1"



enum Group4 
{
  G4_CANTINA = 0,
  G4_MANAMA = 1,
  G4_PLAY = 2,
  G4_STOP = 3,
  G4_R2 = 4,
};


char *group4[] = { 
  
   "Cantina.Manama.Play",
   "Stop.R2"};


//Grammars and Words
enum Wordsets
{
  SET_1  = -1,
  SET_2  = -2,
  SET_3  = -3,
};

enum Wordset1 
{
  S1_ACTION = 0,
  S1_MOVE = 1,
  S1_TURN = 2,
  S1_RUN = 3,
  S1_LOOK = 4,
  S1_ATTACK = 5,
  S1_STOP = 6,
  S1_HELLO = 7,
};


enum Wordset2 
{
  S2_LEFT = 0,
  S2_RIGHT = 1,
  S2_UP = 2,
  S2_DOWN = 3,
  S2_FORWARD = 4,
  S2_BACKWARD = 5,
};


enum Wordset3 
{
  S3_ZERO = 0,
  S3_ONE = 1,
  S3_TWO = 2,
  S3_THREE = 3,
  S3_FOUR = 4,
  S3_FIVE = 5,
  S3_SIX = 6,
  S3_SEVEN = 7,
  S3_EIGHT = 8,
  S3_NINE = 9,
  S3_TEN = 10,
};


// use negative group for wordsets
int8_t group, idx;

// store last group
int8_t last_group;



unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 10;

String setcom;
String CMD;
int cur=0;

int siz=0;

void setup()
{
  // setup PC serial port
  pcSerial.begin(9600);
  ComPort.begin(9600);

// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  
bridge:
  // bridge mode?
  int mode = easyvr.bridgeRequested(pcSerial);
  switch (mode)
  {
  case EasyVR::BRIDGE_NONE:
    // setup EasyVR serial port
    port.begin(9600);
    // run normally
    pcSerial.println(F("Bridge not requested, run normally"));
    pcSerial.println(F("---"));
    break;
    
  case EasyVR::BRIDGE_NORMAL:
    // setup EasyVR serial port (low speed)
    port.begin(9600);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("Bridge connection aborted"));
    pcSerial.println(F("---"));
    break;
    
  case EasyVR::BRIDGE_BOOT:
    // setup EasyVR serial port (high speed)
    port.begin(115200);
    pcSerial.end();
    pcSerial.begin(115200);
    // soft-connect the two serial ports (PC and EasyVR)
    easyvr.bridgeLoop(pcSerial);
    // resume normally if aborted
    pcSerial.println(F("Bridge connection aborted"));
    pcSerial.println(F("---"));
    break;
  }

  // initialize EasyVR  
  while (!easyvr.detect())
  {
    pcSerial.println(F("EasyVR not detected!"));
    for (int i = 0; i < 10; ++i)
    {
      if (pcSerial.read() == '?')
        goto bridge;
      delay(100);
    }
  }

  pcSerial.print(F("EasyVR detected, version "));
  pcSerial.print(easyvr.getID());

  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // Shield 2.0 LED off

  if (easyvr.getID() < EasyVR::EASYVR)
    pcSerial.print(F(" = VRbot module"));
  else if (easyvr.getID() < EasyVR::EASYVR2)
    pcSerial.print(F(" = EasyVR module"));
  else if (easyvr.getID() < EasyVR::EASYVR3)
    pcSerial.print(F(" = EasyVR 2 module"));
  else
    pcSerial.print(F(" = EasyVR 3 module"));
  pcSerial.print(F(", FW Rev."));
  pcSerial.println(easyvr.getID() & 7);

  easyvr.setDelay(0); // speed-up replies

  easyvr.setTimeout(5);
  easyvr.setLanguage(0); //<-- same language set on EasyVR Commander when code was generated

  group = EasyVR::TRIGGER; //<-- start group (customize)
}

#include "functions.h"

void loop()
{
  
  unsigned long currentMillis = millis();
  
  //delay(2000);
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    // displayOut(CMD);
    }
  displayOut(CMD);
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)

  if (group < 0) // SI wordset/grammar
  {
    pcSerial.print("Say a word in Wordset ");
    pcSerial.println(-group);
    easyvr.recognizeWord(-group);
  }
  else // SD group
  {
    pcSerial.print("Say a command in Group ");
    pcSerial.println(group);
    easyvr.recognizeCommand(group);
    
    
  }

  do
  {
    // allows Commander to request bridge on Zero (may interfere with user protocol)
    if (pcSerial.read() == '?')
    {
      setup();
      return;
    }
    // <<-- can do some processing here, while the module is busy
  }
  while (!easyvr.hasFinished());
  
  if (easyvr.getID() < EasyVR::EASYVR3)
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  idx = easyvr.getWord();
  if (idx == 0 && group == EasyVR::TRIGGER)
  {
    // beep
    
    //easyvr.playSound(0, EasyVR::VOL_FULL);
    beep(group);
    
    // print debug message
    pcSerial.println("Word: ROBOT");
    // write your action code here
    // group = GROUP_X\SET_X; <-- jump to another group or wordset
    return;
  }
  else if (idx >= 0)
  {
    // beep
    beep(group);
    //easyvr.playSound(0, EasyVR::VOL_FULL);



    
    // print debug message
    uint8_t flags = 0, num = 0;
    char name[32];
    pcSerial.print("Word: ");
    pcSerial.print(idx);
    if (easyvr.dumpGrammar(-group, flags, num))
    {
      for (uint8_t pos = 0; pos < num; ++pos)
      {
        if (!easyvr.getNextWordLabel(name))
          break;
        if (pos != idx)
          continue;
        pcSerial.print(F(" = "));
        pcSerial.println(name);
         CMD = name;
        break;
      }
    }
    // perform some action
    action();
    return;
  }
  idx = easyvr.getCommand();
  if (idx >= 0)
  {
    // beep
    //easyvr.playSound(0, EasyVR::VOL_FULL);

    beep(group);
    // print debug message
    uint8_t train = 0;
    char name[32];
    pcSerial.print("Command: ");
    pcSerial.print(idx);
    if (easyvr.dumpCommand(group, idx, name, train))
    {
      pcSerial.print(" = ");
      pcSerial.println(name);
      CMD = name;
    }
    else
      pcSerial.println();
    // perform some action
    action();
  }
  else // errors or timeout
  {
    if (easyvr.isTimeout())
      pcSerial.println("Timed out, try again...");
    int16_t err = easyvr.getError();
    if (err >= 0)
    {
      pcSerial.print("Error ");
      pcSerial.println(err, HEX);
    }
  }
}

void action()
{
  switch (group)
  {
  case GROUP_0:
    switch (idx)
    {
    case G0_R2:
      // write your action code here
      group = 1; //GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case GROUP_1:
    switch (idx)
    {
    case G1_HILF_MIR:
      // write your action code here
      sendcom("mode3");
      group = 2;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_SETUP:
      // write your action code here
      group = 3;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_MUSIK:
      // write your action code here
      group = 4;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_WIE_GEHTS:
      // write your action code here
      sendcom("$51");
      group = -1 ;//GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_DRIVE:
      // write your action code here
      last_group = GROUP_1; //Set Last Group
      group = -2;// 1_STOP:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G1_R2:
      // write your action code here
      sendcom(":SE00");
      group = 0; // GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case GROUP_2:
    switch (idx)
    {
    case G2_GIB_MIR_FEUER:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#FION");
      break;
    case G2_DANKE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#FIOFF");
      break;
    case G2_FESTHALTEN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#GAON");
      break;
    case G2_FEST:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#CC");
      break;
    case G2_LOS:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#OC");
      break;
    case G2_SCHRAUBER:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("#SAON");
      break;
    case G2_TOOL:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      setcom = "tool";
      last_group = GROUP_2; //Set Last Group
      group = SET_3;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G2_POWER:
      setcom ="power";
      last_group = GROUP_2;
      // write your action code here
      group = SET_3; // <-- or jump to another group or wordset for composite commands
      break;
    case G2_STOP:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("CBD1");
      break;
    case G2_R2:
      // write your action code here
      group = 1; // GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case GROUP_3:
    switch (idx)
    {
    case G3_AKKUSTAND:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G3_MODE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G3_PANELS:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      setcom ="panels";
      last_group = GROUP_3; //Set Last Group
      group = SET_3;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G3_SMIRK:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom(":SE02");
      break;
    case G3_R2:
      // write your action code here
      group = 1; // GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case GROUP_4:
    switch (idx)
    {
    case G4_CANTINA:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom(":SE07");
      break;
    case G4_MANAMA:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("CB7");
      break;
    case G4_PLAY:
      // write your action code here
      last_group = GROUP_4; //Set Last Group
      setcom ="play";
      group = SET_3;// GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case G4_STOP:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      sendcom("$O");
      break;
    case G4_R2:
      // write your action code here
      group = GROUP_1;//SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case SET_1:
    switch (idx)
    {
    case S1_ACTION:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_MOVE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_TURN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_RUN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_LOOK:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_ATTACK:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_STOP:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      group = GROUP_1;//\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    case S1_HELLO:
      // write your action code here
      group = GROUP_1;//\SET_X; <-- or jump to another group or wordset for composite commands
      break;
    }
    break;
  case SET_2:
    switch (idx)
    {
    case S2_LEFT:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S2_RIGHT:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S2_UP:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S2_DOWN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S2_FORWARD:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S2_BACKWARD:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    }
    break;
  case SET_3:
    switch (idx)
    {
    case S3_ZERO:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_ONE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_TWO:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_THREE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      
      
      break;
    case S3_FOUR:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_FIVE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_SIX:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_SEVEN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_EIGHT:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_NINE:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    case S3_TEN:
      // write your action code here
      // group = GROUP_X\SET_X; <-- or jump to another group or wordset for composite commands
      back(group);
      break;
    }
    break;
  }

}
