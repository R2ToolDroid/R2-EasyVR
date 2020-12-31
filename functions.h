#include "Arduino.h"

void sendcom(String cmd) {
   
    ComPort.println(cmd);
    ComPort.print("\r"); //Check if is R or N
  
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
      sendcom("$479"); //Sad Sound
      break;

      case -1:
      easyvr.playSound(3, EasyVR::VOL_FULL);
      sendcom("$16"); //Jawoll
      break;
     
      default:
      easyvr.playSound(0, EasyVR::VOL_FULL);
      sendcom("$12"); //TickTock
      break;
    
  }
  
}



void back(int8_t grp){


  pcSerial.print("last-grp: ");
  pcSerial.println(last_group);
  pcSerial.print("IDX: ");
  pcSerial.println(idx);
  
  
  group = last_group;

  
}
