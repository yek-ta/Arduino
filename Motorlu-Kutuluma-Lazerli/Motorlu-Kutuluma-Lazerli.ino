#include <AFMotor.h>

#define USTLDR A0  //pin
#define ALTLDR A1  //pin

AF_DCMotor ustmotor(1);
AF_DCMotor altmotor(2);

int kutuyakonulan;
boolean malzemegeciyor = false;

void setup() {
  Serial.begin(9600);
  ustmotor.setSpeed(255); //Hız 200 - 255
  altmotor.setSpeed(255); //Hız 200 - 255
  ustmotor.run(FORWARD);
  altmotor.run(RELEASE);
}

void loop() {
  int deger_ustldr = analogRead(USTLDR);
  if(deger_ustldr > 380){
    malzemegeciyor = false;
  }
  else {
    if(malzemegeciyor == false){
      kutuyakonulan++;
      Serial.println("Kutuya 1 atildi.");
      malzemegeciyor = true;
      if(kutuyakonulan >= 5){
        delay(500);
        ustmotor.run(RELEASE);
        altpanelicalistir();
        kutuyakonulan=0;
        ustmotor.run(FORWARD);
      }
    }
  }
  delay(100);
}

void altpanelicalistir(){
  int deger_altldr = analogRead(ALTLDR);
  
  altmotor.run(FORWARD);
  while(deger_altldr < 380){
    deger_altldr = analogRead(ALTLDR);
    delay(100);
  }
  Serial.println("Alt Paneldeki Kutu Gitti");
  
  while(deger_altldr > 380){
    deger_altldr = analogRead(ALTLDR);
    delay(100);
  }
  Serial.println("Alt Panele Yeni Kutu Geldi");
  altmotor.run(RELEASE);
}

