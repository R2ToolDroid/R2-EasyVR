#include "Arduino.h"

void sendcom (String cmd){
  ComPort.println(cmd);
  ComPort.print(F("\r"));
  pcSerial.println(cmd);
}

void displayOut(String data){
  
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.drawRect(0, 0, 128, 11, WHITE); 
 
    switch(group)
    //switch(-3)
    {
      case GROUP_0:
      display.setCursor(50, 2); 
      display.print(F("Idle"));
      Line = "..wait";
      siz = 0;
      break;
      
      case GROUP_1:
      display.setCursor(30, 2);     // Start at top-left corner
      display.print(F("Action Menu"));
      Line = group1[cur];
      siz = 3;
      break;

      case GROUP_2:
      display.setCursor(28, 2);     // Start at top-left corner
      display.print(F("Service Menu"));
      Line = group2[cur];
      siz = 4;
      break;

      case GROUP_3:
      display.setCursor(34, 2);     // Start at top-left corner
      display.print(F("Setup Menu"));
      Line = group3[cur];
      siz = 2;
      break;

      case GROUP_4:
      display.setCursor(34, 2);     // Start at top-left corner
      display.print(F("Musik Menu"));
      Line = group4[cur];
      siz = 2;
      break; 

      //// NEW ///

      case SET_1:

      display.setCursor(35, 2);     // Start at top-left corner
      display.print(F("Action SET"));
      Line = set1[cur];
      siz= 2;
      ///    "Testzeichensazluunge1"
      break;

      case SET_2:

      display.setCursor(30, 2);     // Start at top-left corner
      display.print(F("Action Drive"));
      Line = set2[cur];
      siz= 2;
      ///    "Testzeichensazluunge1"
      break;

      case SET_3:

      display.setCursor(45, 2);     // Start at top-left corner
      display.print(F("Count"));
      Line = "one two three... ten";
      
      ///    "Testzeichensazluunge1"
      break;

      
      
      default:
      display.setCursor(45, 2);     // Start at top-left corner
      display.print(F("Error"));
      Line = "x";
      
      siz = 0;
      break;
        
    }
    
    //display.print(GP);
    //End of Group
 
    // Display Command recieved
    display.setCursor(0, 14);     // next row
    display.print(F("_ "));  
    display.print(data);

    //Display possible Command in Group
    display.setCursor(0, 24);
    
    
    
    //display.println("Testzeichensazluunge1mit unter sehr lang");
    //
    display.println(Line);

    

    cur++;
    if (cur >= siz)
    cur = 0;
    
    //pcSerial.print("siz ");
    //  pcSerial.println(siz);
    //  pcSerial.print("cur ");
    //  pcSerial.println(cur);
    
    display.display();
    
   //display.startscrollleft(0x03,0x07);
}





void beep(int8_t grp) {

  //easyvr.playSound(0, EasyVR::VOL_FULL);
  //pcSerial.println("grp: ");
  //pcSerial.println(grp);
  
  switch (grp){
      case 1:
      easyvr.playSound(1, EasyVR::VOL_FULL);
      sendcom("$12"); //TickTock
      break;

      case 2:
      easyvr.playSound(2, EasyVR::VOL_FULL);
      sendcom("$16"); //Jawoll
      break;

      case 3:
      easyvr.playSound(2, EasyVR::VOL_FULL);
      sendcom("$44"); //Jawoll
      break;

      case -1:
      easyvr.playSound(3, EasyVR::VOL_FULL);
      
      sendcom("$214"); //Sad Sound
      break;

      case -2:
      easyvr.playSound(3, EasyVR::VOL_FULL);
      
      sendcom("$33"); //Sad Sound
      break;
     
      default:
      easyvr.playSound(0, EasyVR::VOL_FULL);
      sendcom("$217"); //TickTock
      break;
    
  }
  
}



void back(int8_t grp){

  

  switch (Setcom){
     
     case TOOL:
     tosend = "tool";
     tosend.concat(idx);
     sendcom(tosend);
     break;

     case PLAY:
     tosend = "play";
     tosend.concat(idx);
     sendcom(tosend);
     break;

     case POWER:
     tosend = "power";
     tosend.concat(idx);
     sendcom(tosend);
     break;

     case PANELS:
     tosend = ":OP0";
     tosend.concat(idx);
     sendcom(tosend);
     break;
     
     case MODE:
     tosend = "mode";
     tosend.concat(idx);
     sendcom(tosend);
     break;

     default:
     
     break;
  
  }
   
  
  group = last_group;
  Setcom = RES;
  tosend="";

  
}
