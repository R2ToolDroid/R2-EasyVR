#include "Arduino.h"

void sendcom(String cmd) {
   
    ComPort.println(cmd);
    ComPort.print("\r"); //Check if is R or N
   pcSerial.println(cmd);
}


void displayOut(String data){

    String GP;
    String Line;
    
  
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(0, 1);     // Start at top-left corner
    display.print("# ");
      // "Testzeichensazluunge1"
    switch(group)
    {
      case GROUP_0:
      GP = "Idle...";
      Line = "..wait";
      siz = 0;
      break;
      
      case GROUP_1:
      GP = "  Action Menu    #";
      Line = group1[cur];
      siz = 3;
      break;

      case GROUP_2:
      GP = "  Service Menu   #";
      Line = group2[cur];
      siz = 4;
      break;

      case GROUP_3:
      GP = "  Setup   Menu   #";
      Line = group3[cur];
      siz = 2;
      break;

      case GROUP_4:
      GP = "  Musik Menu   #";
      Line = group4[cur];
      siz = 2;
      break; 
      
      default:
      siz = 0;
      break;
    
    
    }

    display.print(GP);
    //End of Group

    
    // Display Command recieved
    display.setCursor(0, 10);     // Start at top-left corner
    display.print("_ ");  
    display.print(data);

    //Display possible Command in Group
    display.setCursor(0, 24);
    
    
    
    //display.print("Testzeichensazluunge1");
    display.print(Line);

    cur++;
    if (cur >= siz)
    cur = 0;
    
    
    display.display();

    
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

      case -1:
      easyvr.playSound(3, EasyVR::VOL_FULL);
      
      sendcom("$479"); //Sad Sound
      break;
     
      default:
      easyvr.playSound(0, EasyVR::VOL_FULL);
      sendcom("$12"); //TickTock
      break;
    
  }
  
}



void back(int8_t grp){

  String tosend;

  pcSerial.print("last-grp: ");
  pcSerial.println(last_group);
  pcSerial.print("IDX: ");
  pcSerial.println(idx);

  if (setcom == "tool"){
    pcSerial.print("tool");
    pcSerial.println(idx);
    
    tosend = "tool";
    tosend.concat(idx);
    sendcom(tosend);
  }

  if (setcom == "play"){
    pcSerial.print("play");
    pcSerial.println(idx);
    
    tosend = "play";
    tosend.concat(idx);
    sendcom(tosend);
  }

  if (setcom == "power"){
    pcSerial.print("power");
    pcSerial.println(idx);
    
    tosend = "power";
    tosend.concat(idx);
    sendcom(tosend);
  }

  if (setcom == "panels"){
    pcSerial.print("panels");
    pcSerial.println(idx);
    
    tosend = ":OP0";
    tosend.concat(idx);
    sendcom(tosend);
  }

  if (setcom == "mode"){
    pcSerial.print("mode");
    pcSerial.println(idx);
    
    tosend = "mode";
    tosend.concat(idx);
    sendcom(tosend);
  }
  
  
  
  group = last_group;
  setcom="";
  tosend="";

  
}
