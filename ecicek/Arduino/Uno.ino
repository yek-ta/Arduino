#include <Servo.h>
#include <Wire.h>
/* WeMos ile iletişim */
#define BU_ADRES 0x1 // I2C UNO kanal tanımlandırması.
#define DIGER_ADRES 0x2 //I2C WeMos kanal tanımlandırması.

/* Pinler */
#define KirmiziPin 12
#define YesilPin 11
#define MaviPin 10
#define LDRPin A0
#define ServoPin 6
#define TitresimPin A1

/* RGB Tanımlandırması ve Ayarları */
int RGBRenkleri[3];

/* Servo Tanımlandırması */
Servo CicekServo;
#define MaksimumServoDegeri 75 // Çiçeğin fazla açılıp kırılmaması için.
#define MinimumServoDegeri 10 // Çiçeğin fazla kapanıp kırılmaması için.

/* LDR Tanımlandırması, ayar değişkenleri */
int LDRDegeri, ServoDegeri;
unsigned long LDRicinEskiZaman, YeniZaman;
#define LDRSORGUSuresi 10 // Kaç saniyede bir LDR sorgusu yapması gerektiği.
#define LDRIsikYakmaDegeri 62 // MinimumServoDegeri ile MaksimumServoDegeri arasında olmalı.
int GelenMesajDegeri = 0; // WeMos tan gelen mesaj uno içerisinde hızlı çalıştığı için sadece gelen mesajın değer tanımlandırılması.
/* Diğer cihazlardan gelen verilerin okunması */
void DigerCihazdanGelenVeri() {
  String GelenMesaj = ""; // Mesajı sıfırlama ve tanımlandırma.
  while (Wire.available() > 0) { // Gelen veri bitene kadar.
    char c = Wire.read(); // Gelen veriyi karakter karakter al
    GelenMesaj = GelenMesaj + String(c); //Gelen Mesaja Ekle
  }
  Serial.println(GelenMesaj); // 9600 'e yazdır
  if (GelenMesaj.indexOf("yüzünü") >= 0 && GelenMesaj.indexOf("göster") >= 0) { // Yazı içeriği sorgusu
    GelenMesajDegeri = 1; //Sonraki çiçek işlemi için değer ekle
  }
  else if (GelenMesaj.indexOf("RKG") >= 0) { // Yazı içeriği sorgusu
    String RenkAnlama = GelenMesaj; //Gelen Mesaj RenkAnlama değişkenine yazdırılır.
    /* 
     *  Örnek olarak mesaj RKG: 255000255 olarak gelir.
     *  İlk 4 karakter silinir.
     *  Sonra 4. karakter ve sonrası silinir.
     *  Kalan yazı int'a çevirilerek Kırmızı rengine tanımlandırılır.
     */
    RenkAnlama.remove(0,4);
    RenkAnlama.remove(4);
    Serial.print(" Kir: "+RenkAnlama); // 9600 'e yazdır
    RGBRenkleri[0] = RenkAnlama.toInt();
    RenkAnlama = GelenMesaj; //Gelen Mesaj RenkAnlama değişkenine yazdırılır.
    /* 
     *  Örnek olarak mesaj RKG: 255000255 olarak gelir.
     *  İlk 8 karakter silinir.
     *  Sonra 4. karakter ve sonrası silinir.
     *  Kalan yazı int'a çevirilerek Kırmızı rengine tanımlandırılır.
     */
    RenkAnlama.remove(0,8);
    RenkAnlama.remove(3);
    Serial.print(" Yesil: "+RenkAnlama); // 9600 'e yazdır
    RGBRenkleri[1] = RenkAnlama.toInt();
    RenkAnlama = GelenMesaj; //Gelen Mesaj RenkAnlama değişkenine yazdırılır.
    /* 
     *  Örnek olarak mesaj RKG: 255000255 olarak gelir.
     *  İlk 11 karakter silinir.
     *  Sonra 4. karakter ve sonrası silinir.
     *  Kalan yazı int'a çevirilerek Kırmızı rengine tanımlandırılır.
     */
    RenkAnlama.remove(0,11);
    RenkAnlama.remove(4);
    Serial.println(" Mavi: "+RenkAnlama); // 9600 'e yazdır
    RGBRenkleri[2] = RenkAnlama.toInt();
  }
}

/* Bir kere çalıştırılacak tanımlandırmalar */
void setup() {
  Serial.begin(9600); //Seri haberleşme, rate: 9600

  while ( (MaksimumServoDegeri < LDRIsikYakmaDegeri) ||  (MinimumServoDegeri > LDRIsikYakmaDegeri)) {
    // Eğer ki RGB ledin yanma değeri servo motor değerleri arasında değil ise rgb led yanmayacaktır.
     // bunu engellemek için koşul oluşturuldu ve çiçeğin başlangıcına engel olundu.
    Serial.println("LDRIsikYakmaDegeri uygun değerde değil. "); // 9600 'e yazdır
    delay(1000);
  }
  while ( LDRSORGUSuresi <= 0) {
    // Işık için sorgu süresi 0'ın altına indiğinde mikrodenetleyiciye yük olduğu için engelleme konuldu.
    Serial.println("LDRSORGUSuresi 0'ın altında olamaz."); // 9600 'e yazdır
    delay(1000);
  }
  //RGB ledlerin başlangıçtaki değerleri(çalıştır komutu göndermek için).
  RGBRenkleri[0] = 255;
  RGBRenkleri[1] = 255;
  RGBRenkleri[2] = 255;
  pinMode(KirmiziPin, OUTPUT); //RGB Kırmızı Bacak
  pinMode(YesilPin, OUTPUT); // RGB Yeşil Bacak
  pinMode(MaviPin, OUTPUT); // RGB Mavi Bacak
  Wire.begin(BU_ADRES); // Diğer cihazlar ile haberleşir iken hangi kanalda olduğu
  Wire.onReceive(DigerCihazdanGelenVeri); // Diğer cihazlardan gelen verinin yönlendirilmesi
  CicekAcmaKapamaIslemi(); // İlk çalıştırma
}


/* Sonsuz Döngü */
void loop() {
  YeniZaman = millis(); // Milisaniye olarak yenizamana şuanki zamanı tanımlandır
  if (YeniZaman - LDRicinEskiZaman > LDRSORGUSuresi * 1000) { // LDR Sorgusu yaparken bekletme

    CicekAcmaKapamaIslemi();
    
    LDRicinEskiZaman = YeniZaman;
  }
  hareketalgilama();
  GelenMesajSorgulari();
  
}
void GelenMesajSorgulari() {
  if (GelenMesajDegeri == 1) {
    Serial.println("Islem yapiliyor.. 1"); // 9600 'e yazdır
    CicekServo.attach(ServoPin); // Servoyu tanımlandır
    int kopkop; //RGB ledin sıra ile yanması için.
    for (int dongu = 0; dongu < 5; dongu++){ //Çiçeğin 5 kere kapanıp açılması için for döngüsü kullanıldı.
      for (int pos = MinimumServoDegeri; pos <= MaksimumServoDegeri; pos += 1) { //Çiçeğin açılması.
        CicekServo.write(pos);              
        delay(10);
        kopkop++;
        if(kopkop == 3){
          RGBRenkAyari(0, 255, 255); // Açık Mavi                     
        }
        else if(kopkop == 6){
          RGBRenkAyari(255, 255, 0); // Sarı
        }
        else if(kopkop == 9){
          RGBRenkAyari(0, 0, 255); // Mavi
        }
        else if(kopkop == 12){
          RGBRenkAyari(255, 0, 0); // Kırmızı
        }
        else if(kopkop == 15){
          RGBRenkAyari(0, 255, 0); // Yeşil
        }
        else if(kopkop == 18){
          RGBRenkAyari(80, 0, 80); // Mor
          kopkop = 0;
        } 
      }
      for (int pos = MaksimumServoDegeri; pos >= MinimumServoDegeri; pos -= 1) { //Çiçeğin kapanması
        CicekServo.write(pos);              
        delay(10);  
        kopkop++;
        if(kopkop == 3){
          RGBRenkAyari(0, 255, 255); // Açık Mavi                     
        }
        else if(kopkop == 6){
          RGBRenkAyari(255, 255, 0); // Sarı
        }
        else if(kopkop == 9){
          RGBRenkAyari(0, 0, 255); // Mavi
        }
        else if(kopkop == 12){
          RGBRenkAyari(255, 0, 0); // Kırmızı
        }
        else if(kopkop == 15){
          RGBRenkAyari(0, 255, 0); // Yeşil
        }
        else if(kopkop == 18){
          RGBRenkAyari(80, 0, 80); // Mor
          kopkop = 0;
        }                    
      }
    }
    
    RGBRenkAyari(255, 255, 255); //Beyaz
    CicekServo.write(25); // Servoya konum ver, çiçeği aç
    delay(1000); // 1 Saniye bekle
    CicekServo.detach(); Servoyu durdur.
    
    delay(10000); // 10 Saniye bekle
    CicekServo.attach(ServoPin); // Servoyu tanımlandır
    CicekServo.write(ServoDegeri); // Servo konumunu komuttan önceki değerine geri al
    delay(1000); // 1 Saniye bekle
    CicekServo.detach(); // Servoyu durdur.
    GelenMesajDegeri = 0;
  }
}
void hareketalgilama(){
  int hareketDegeri = analogRead(TitresimPin); // Analog olarak Titreşimi algıla (0-1024)
  Serial.print("Hareket Değeri: "); // Servo değerini 9600 'e yazdır
  if(hareketDegeri > 1000){ //Titreşim ya 0 32'dir yada 1024.
    CicekServo.detach();
     CicekServo.attach(ServoPin); // Servoyu tanımlandır
    CicekServo.write(MaksimumServoDegeri); // Servoya konum ver
    delay(1000);
    CicekServo.detach(); // Servoyu iptal et. (Bunun sebebi, eğer çiçeği hareket ettirir iken son hamleyi yapamıyor ise ses çıkartıyor, bu sesi engelleme)
    delay(5000);
    CicekServo.attach(ServoPin); // Servoyu tanımlandır
    CicekServo.write(ServoDegeri);
    delay(1000); // 1 Saniye bekle
    CicekServo.detach();
  }
  Serial.println(hareketDegeri);
  delay(260);
}
void CicekAcmaKapamaIslemi() {
  int LDRDegeri = analogRead(LDRPin); // Analog olan LDR Değerini değişkene yazdır.
  LDRDegeri = map(LDRDegeri, 0, 1023, MaksimumServoDegeri, MinimumServoDegeri); // LDRDeğeri 0-1024 arasıdır, bunu servo için uygun gördüğümüz minimum ve maksimum değere orantıla.
  if (LDRDegeri != ServoDegeri) { // LDR Değeri Servo Değerine eşit değil ise (Döngüde bir önceki değer var ise işlem yapmaya gerek yok)
    ServoDegeri = LDRDegeri; // ServoDegeri değişkenini LDRDegerine eşitle
    CicekServo.attach(ServoPin); // Servoyu tanımlandır
    CicekServo.write(ServoDegeri); // Servoya konum ver
    Serial.print("Servo Değeri: "); // Servo değerini 9600 'e yazdır
    Serial.println(ServoDegeri);
    delay(1000); // 1 Saniye bekle
    CicekServo.detach(); // Servoyu iptal et. (Bunun sebebi, eğer çiçeği hareket ettirir iken son hamleyi yapamıyor ise ses çıkartıyor, bu sesi engelleme)
  }
  else { //LDR Değeri Servo değerine eşit ise
    Serial.print("Servo Değeri Ayni: ");
    Serial.println(LDRDegeri);
  }

  if (LDRDegeri >= LDRIsikYakmaDegeri ) { //LDR Değeri verilen değerden büyük ise ışığı yak
    RGBRenkAyari(RGBRenkleri[0], RGBRenkleri[1], RGBRenkleri[2]);
  }
  else {
    RGBRenkAyari(0, 0, 0);
  }
  delay(1000);
}
void RGBRenkAyari(int Kirmizi, int Yesil, int Mavi)
{
  Kirmizi = 255 - Kirmizi;
  Yesil = 255 - Yesil;
  Mavi = 255 - Mavi;
  analogWrite(KirmiziPin, Kirmizi);
  analogWrite(YesilPin, Yesil);
  analogWrite(MaviPin, Mavi);
}
