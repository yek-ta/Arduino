#include <avr/pgmspace.h>
#include <TMRpcm.h>
#include <SPI.h>
TMRpcm wav;
#include <SD.h>

#define GOMULU HIGH
#define GOMULUDEGIL LOW

#define buton1 2
#define buton2 3
#define buton3 4
#define buton4 5
#define buton5 6
#define buton6 7
#define SD_ChipSelectPin 8

void setup() {
  wav.speakerPin = 9;

  if (!SD.begin(SD_ChipSelectPin)) {
    return;
  }

  pinMode(buton1, INPUT);
  pinMode(buton2, INPUT);
  pinMode(buton3, INPUT);
  pinMode(buton4, INPUT);
  pinMode(buton5, INPUT);
  pinMode(buton6, INPUT);
  wav.setVolume(5);
}

void loop() {

  if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
      digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //A Harfi
    wav.play("A.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //B Harfi
    wav.play("B.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //C Harfi
    wav.play("C.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //Ç Harfi
    wav.play("CE.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //D Harfi
    wav.play("D.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //E Harfi
    wav.play("E.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //F Harfi
    wav.play("F.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //G Harfi
    wav.play("G.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //Ğ Harfi
    wav.play("GE.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //H Harfi
    wav.play("H.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //I Harfi
    wav.play("I.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //İ Harfi
    wav.play("EI.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //J Harfi
    wav.play("J.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //K Harfi
    wav.play("K.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //L Harfi
    wav.play("L.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //M Harfi
    wav.play("M.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //N Harfi
    wav.play("N.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //O Harfi
    wav.play("O.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //Ö Harfi
    wav.play("OE.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //P Harfi
    wav.play("P.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //Q Harfi
    wav.play("Q.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //R Harfi
    wav.play("R.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULU) {
    //S Harfi
    wav.play("S.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //Ş Harfi
    wav.play("SE.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULU) {
    //T Harfi
    wav.play("T.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //U Harfi
    wav.play("U.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULUDEGIL) {
    //Ü Harfi
    wav.play("UE.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //V Harfi
    wav.play("V.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULU && digitalRead(buton6) == GOMULUDEGIL) {
    //X Harfi
    wav.play("X.wav");
  }
  else if (digitalRead(buton1) == GOMULU && digitalRead(buton2) == GOMULUDEGIL && digitalRead(buton3) == GOMULU &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULUDEGIL) {
    //W Harfi
    wav.play("W.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULUDEGIL && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULUDEGIL) {
    //Y Harfi
    wav.play("Y.wav");
  }
  else if (digitalRead(buton1) == GOMULUDEGIL && digitalRead(buton2) == GOMULU && digitalRead(buton3) == GOMULUDEGIL &&
           digitalRead(buton4) == GOMULU && digitalRead(buton5) == GOMULUDEGIL && digitalRead(buton6) == GOMULUDEGIL) {
    //Z Harfi
    wav.play("Z.wav");
  }
  else {
    //Harf Bulunamadi
    wav.play("TA.wav");
  }
  delay(2000);
}

