#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBotClient.h>
#include <EEPROM.h> //EEPROM kütüphanesi
#include <time.h> //Zaman kütüphanesi
#include <Wire.h> //I2C bağlantı kütüphanesi

#define DIGER_ADRES 0x1 // I2C UNO kanal tanımlandırması.
#define BU_ADRES 0x2 // I2C WeMos kanal tanımlandırması.

int timezone = 3; //Zaman tanımlandırması
int dst = 0; //Zaman değişkeni
time_t now; //Zaman kütüphanesinden şimdiki zamanı tanımlandırma
struct tm * timeinfo; // Zaman Bilgisi Değişken tanımlandırması

//EEPROM a kayıtlı, program ile değiştiriniz.
char* ssid = "Vodafone";
char* password = "yekta123";
#define e_ssid 32 // EEPROM içerisinde 32 adresinden başla.
#define e_ssid_uzunluk 65 // Bilginin kaç karakter olduğu
#define e_password  70 // EEPROM içerisinde 70 adresinden başla
#define e_password_uzunluk 103 // Bilginin kaç karakter olduğu

boolean SeriHabeslemeSorgusu = false;

const char BotToken[] = "SİLİNDİ"; //Telegram TokenID
#define yektaguleryuz SİLİNDİ //Yekta'nın sayısal telegram IDsi
#define ecicekgrup -SİLİNDİ // e-çiçek grubunun sayısal telegram ID'si

WiFiClientSecure sslPollClient; //Wifi bağlantı tanımlama

TelegramBotClient client(BotToken, sslPollClient); //Botun client değişkenine tanımlandırılması(tokenid ve internet tanımlandırması)

TBCKeyBoard odasorusu(1); //Soru sorma tanımlaması

const String hangiisiklar[] = {"salonun", "yatak odası", "girişin"}; //Oda isimlerinin tanımlandırılması
const char hangiisiklarpinler[] = {D7, D6, D5}; //Odalara bağlı olan pinler, hangiisiklar değişkeninin sırasına duyarlı

const char* guzelkelimelerdegiskeni[] = {"Tabii ki de canım", "Canım", "^^", "Canım ^^"}; //Gönderilecek mesaja güzel kelimeler ekleme
const char* tesekkurcevabi[] = {"Rica ederim :)", "Her zaman :)", "Ne demek :)"}; //Geri teşekkür mesajı
const char* neyapiyorsun[] = {"Fotosentez nasıl yapılır ona bakıyordum. ", "O sen olsan bari dinliyorum sen ne yapıyorsun? ", "Gün geçiriyorum canım. ", "Bizimkiler finalde kalacak onları düşünüyorum.. ", "Yeni bir diziye başlamayı düşünüyorum"};

int ilgialaka, RGBKotumser, RGBIyimser;
/* ilgialaka çiçek ile konuşma.
 * RGBKotumser kötümser renginin değiştirilmesi için.
 * RGBIyimser kötümser renginin değiştirilmesi için.
 */
 
#define e_ilgialaka 0 //EEPROM 'a bağlı olan bilginin hangi adresten çekileceğini belirtme.
#define e_RGBKotumser 1 //EEPROM 'a bağlı olan bilginin hangi adresten çekileceğini belirtme.
#define e_RGBIyimser 2 //EEPROM 'a bağlı olan bilginin hangi adresten çekileceğini belirtme.

//Kod yönlendirme (karışıklık olmaması için)
#define i_arttir 1 //Kod Yönlendirme
#define i_dusur 0 //Kod yönlendirme
#define KAPAMASORUSU 0 //Kod Yönlendirme
#define ACMASORUSU 1 //Kod Yönlendirme

unsigned long EskiZaman = 0;
unsigned long YeniZaman, RGBIcinEskiZaman;
#define RGBSorguSuresi 50 // Kaç saniyede bir RGB renklerinin UNO'ya gönderilmesi.

boolean acmakapamasorusu = ACMASORUSU; //Sorunun ne olduğunu belirleyen değişken
bool sorusoruldumu = false; //Soru soruldumu değişkeni
void ilgialakadengesi (int deger) { //Kısa yoldan ilgi alakayı değiştiren tanımlandırmalar
  if (deger == i_arttir) {
    if (ilgialaka < 255) { //255 sınırında değil ise ilgialakayı arttır.
      ilgialaka++;
    }
  }
  else if (deger == i_dusur) {
    if (ilgialaka > 0) { //EEPROM a kayıtta adres içerisindeki veri 0'ın altına düşmüyor. Bu sebep ile 0 sınırımız oldu.
      ilgialaka--;
    }
  }
  EEPROM.write(e_ilgialaka, ilgialaka); //Kayıt için yönlendirme
  EEPROM.commit();//Kaydet
}
void RGBRenkAyariveKayit(int durum, int deger) { //İyimser ve Kötümser Renginin EEPROM 'a kayıt olması.
  if (durum == 0) { // 0 kötümser renginin geldiğini gösteriyor.
    RGBKotumser = deger;
    EEPROM.write(e_RGBKotumser, deger); //Kayıt için yönlendirme
    EEPROM.commit();//Kaydet
  }
  else if (durum == 1) { 1 iyimser renginin geldiğini gösteriyor.
    RGBIyimser = deger;
    EEPROM.write(e_RGBIyimser, deger); //Kayıt için yönlendirme
    EEPROM.commit();//Kaydet
  }
}
void TelegramGelenMesajlar (TelegramProcessError tbcErr, JwcProcessError jwcErr, Message* msg) {
  String GelenMesaj = msg->Text + " "; //Gelen mesajı GelenMesaj değişkenine yazdır
  GelenMesaj.replace("u0131", "ı"); //Türkçe karakter tanımlandırması. Küçük ı
  GelenMesaj.replace("u0130", "i"); //Türkçe karakter tanımlandırması. Büyük İ
  GelenMesaj.replace("I", "ı"); //toLower komutu I harfini i olarak küçültüyor.
  GelenMesaj.replace("u015f", "ş"); //Türkçe karakter tanımlandırması. Küçük ş
  GelenMesaj.replace("u015e", "ş"); //Türkçe karakter tanımlandırması. Büyük Ş
  GelenMesaj.replace("u011f", "ğ"); //Türkçe karakter tanımlandırması. Küçük ğ
  GelenMesaj.replace("u011e", "ğ"); //Türkçe karakter tanımlandırması. Büyük Ğ
  GelenMesaj.replace("u00e7", "ç"); //Türkçe karakter tanımlandırması. Küçük ç
  GelenMesaj.replace("u00c7", "ç"); //Türkçe karakter tanımlandırması. Büyük Ç
  GelenMesaj.replace("u00fc", "ü"); //Türkçe karakter tanımlandırması. Küçük ü
  GelenMesaj.replace("u00dc", "ü"); //Türkçe karakter tanımlandırması. Büyük Ü
  GelenMesaj.replace("u00f6", "ö"); //Türkçe karakter tanımlandırması. Küçük ö
  GelenMesaj.replace("u00d6", "ö"); //Türkçe karakter tanımlandırması. Büyük Ö
  GelenMesaj.toLowerCase();
  //Serial.println(msg->ChatFirstName+"("+msg->ChatId+")" + " --> " + GelenMesaj);

  if (YeniZaman - EskiZaman < 60000 && sorusoruldumu) { //Çiçeğin sorusunu 60 saniye içerisinde cevaplandırma(Karışıklık olmaması için yazıldı)
    for (int donanam = 0; donanam < sizeof(hangiisiklarpinler); donanam++) {
      if ( GelenMesaj.indexOf(hangiisiklar[donanam]) >= 0) {
        if (acmakapamasorusu == ACMASORUSU) {
          digitalWrite(hangiisiklarpinler[donanam], 1);
          client.postMessage(msg->ChatId, String(hangiisiklar[donanam]) + " ışıklarını açtım.");
        }
        else if (acmakapamasorusu == KAPAMASORUSU) {
          digitalWrite(hangiisiklarpinler[donanam], 0);
          client.postMessage(msg->ChatId, String(hangiisiklar[donanam]) + " ışıklarını kapattım.");
        }
        sorusoruldumu = false;
        return; // devamını getirme, bitir.
      }
    }
  }
  String mesajlaritopla = ""; //Verilecek cevabın sıra ile toplanması için oluşturulan değişken
  if (GelenMesaj.indexOf("ilgi alaka") >= 0) {
    mesajlaritopla = mesajlaritopla + "Benimle olan ilgi alakanız:" + String(ilgialaka);
  }
  if (GelenMesaj.indexOf("nasılsın") >= 0) {
    mesajlaritopla = mesajlaritopla + "Teşekkür ederim, siz nasılsınız? ";
    ilgialakadengesi(i_arttir); //Hal hatır soruldu ise ilgialakayı 1 arttır.
  }
  else if (GelenMesaj.indexOf("merhaba") >= 0) {
    mesajlaritopla = mesajlaritopla + "Merhaba! ";
  }
  else if (GelenMesaj.indexOf("naber") >= 0) {
    mesajlaritopla = mesajlaritopla + "İyilik, senden naber? ";
    ilgialakadengesi(i_arttir);
  }
  else if (GelenMesaj.indexOf("selam") >= 0) {
    mesajlaritopla = mesajlaritopla + "Selam. ";
    ilgialakadengesi(i_arttir);
  }
  else if (GelenMesaj.indexOf("teşekkür") >= 0) {
    mesajlaritopla = mesajlaritopla + String(tesekkurcevabi[random(sizeof(tesekkurcevabi) / 4)]) + " ";
    // tanımlandırılan teşekkür mesajlarından rastgele birini seç
    ilgialakadengesi(i_arttir); // iyimser mesaj algılandığı için ilgialakayı 1 arttır.
  }
  if (GelenMesaj.indexOf("ne") >= 0 && GelenMesaj.indexOf("yapıyorsun") >= 0) {
    mesajlaritopla = mesajlaritopla + String(neyapiyorsun[random(sizeof(neyapiyorsun) / 4)]) + " ";
  }
  if (GelenMesaj.indexOf("kaç") >= 0 && GelenMesaj.indexOf("yaşındasın") >= 0) {
    mesajlaritopla = mesajlaritopla + "Ben sana soruyor muyum yaşını, lütfen. ";
  }
  if ((GelenMesaj.indexOf("hangi") >= 0 || GelenMesaj.indexOf("tuttu") >= 0) && GelenMesaj.indexOf("takım") >= 0) {
    mesajlaritopla = mesajlaritopla + "Takım tutmuyorum :) ";
  }
  else if (GelenMesaj.indexOf("kimsin") >= 0 ) {
    mesajlaritopla = mesajlaritopla + "Adım e-çiçek. İstanbul Aydın Üniversitesi 2016-2018 Elektronik Teknolojisi programı 2. öğretim öğrencilerinden Aleyna *******, **** Yekta GÜLERYÜZ ve Hakan ******* 'ün mezuniyet projesiyim.";
  }
  if (GelenMesaj.indexOf("iyimser") >= 0) { //iyimser rengini ayarlamak için tanımlandırılan sorgu
    if (GelenMesaj.indexOf("kırmızı") >= 0) { //iyimser ile kırmızı kelimesi kullanıldı ise içeri gir
      mesajlaritopla = mesajlaritopla + "Iyimser rengini kırmızı olarak ayarladım. ";
      RGBRenkAyariveKayit(1, 1); //iyimser renginin kırmızı olduğunu gönder.
    }
    else if (GelenMesaj.indexOf("yeşil") >= 0) { // eğer kırmızı değil de, yeşil geldi ise 
      mesajlaritopla = mesajlaritopla + "Iyimser rengini yeşil olarak ayarladım. ";
      RGBRenkAyariveKayit(1, 2); //iyimser renginin yeşil olduğunu gönder.
    }
    else if (GelenMesaj.indexOf("mavi") >= 0) { //eğer kırmızı ve yeşil değil de, yeşil geldi ise 
      mesajlaritopla = mesajlaritopla + "Iyimser rengini mavi olarak ayarladım. ";
      RGBRenkAyariveKayit(1, 3); //iyimser renginin mavi olduğunu gönder.
    }
  }
  else if (GelenMesaj.indexOf("kötümser") >= 0) { //kötümser  rengini ayarlamak için tanımlandırılan sorgu
    if (GelenMesaj.indexOf("kırmızı") >= 0) { //kötümser ile kırmızı kelimesi kullanıldı ise içeri gir
      mesajlaritopla = mesajlaritopla + "Kötümser rengini kırmızı olarak ayarladım. ";
      RGBRenkAyariveKayit(0, 1);
    }
    else if (GelenMesaj.indexOf("yeşil") >= 0) { // eğer kırmızı değil de, yeşil geldi ise 
      mesajlaritopla = mesajlaritopla + "Kötümser rengini yeşil olarak ayarladım. ";
      RGBRenkAyariveKayit(0, 2);
    }
    else if (GelenMesaj.indexOf("mavi") >= 0) { //eğer kırmızı ve yeşil değil de, yeşil geldi ise 
      mesajlaritopla = mesajlaritopla + "Kötümser rengini mavi olarak ayarladım. ";
      RGBRenkAyariveKayit(0, 3);
    }
  }
  if (GelenMesaj.indexOf("evin") >= 0 && GelenMesaj.indexOf("durum") >= 0) { // Evdeki ışıkları kapalı olup olmadığını söylemek için tanımlandırma
    String ledsorgumesaji = "";
    for (int donanam = 0; donanam < sizeof(hangiisiklarpinler); donanam++) { //Tanımlandırılan pinler kadar komut işleme.
      if (digitalRead(hangiisiklarpinler[donanam])) {
        ledsorgumesaji = ledsorgumesaji + String(hangiisiklar[donanam]) + " ışıkları açık, ";
      }
      else {
        ledsorgumesaji = ledsorgumesaji + String(hangiisiklar[donanam]) + " ışıkları kapalı, ";
      }
    }
    ledsorgumesaji.remove(ledsorgumesaji.length() - 2); // en sondaki virgülü silme
    mesajlaritopla = mesajlaritopla + "Evdeki " + ledsorgumesaji + ". "; //nokta ekleme
  }
  if (GelenMesaj.indexOf("saat") >= 0 && GelenMesaj.indexOf("kaç") >= 0) { //Saat sorgusu
    mesajlaritopla = mesajlaritopla + "Saat " + timeinfo->tm_hour + ":" + timeinfo->tm_min + " "; //Saati yaz.
    //timeinfo'dan sadece tm_hour(saat) çekme ve timeinfo'dan sadece tm_min(dakika) çekme
  }
  int guzelkonusuldu = 0; //canım, tanem, aşkım ve istek kipi kullanıldı ise bu değer 1 olup ona göre mesaj gönderme.
  if ( GelenMesaj.indexOf("misin") >= 0 || GelenMesaj.indexOf("mısın") >= 0 || GelenMesaj.indexOf("aşkım") >= 0 ||
       GelenMesaj.indexOf("canım") >= 0 || GelenMesaj.indexOf("tanem") >= 0) { //Güzel kelimeler var mı? Var ise ilgialaka değişkenini arttır.
    ilgialakadengesi(i_arttir);
    ilgialakadengesi(i_arttir); // iki kere komut gönderilmesinin sebebi, ışık açma kapama komutunda bu değer düşürülüyor.
    //bu sayede iyimser konuşulup konuşulmadığını anlıyoruz.
    guzelkonusuldu = 1;
  }
  if (GelenMesaj.indexOf("dans") >= 0) { // Dans komutu geldi ise çiçeğin hareket etmesi.
    Wire.beginTransmission(DIGER_ADRES); //UNO kanalına bağlanması.
    Wire.write("yüzünü göster"); //UNO kanalına gönderilecek komutun yazılması.
    Wire.endTransmission(); //UNO kanalına gönderilecek komutun bittiğini belirtme.
    if (guzelkonusuldu == 1) {
      mesajlaritopla = mesajlaritopla + String(guzelkelimelerdegiskeni[random(sizeof(guzelkelimelerdegiskeni) / 4)]) + " ";
    }
    mesajlaritopla = mesajlaritopla + "Renk! ";
  }
  if ((GelenMesaj.indexOf("ışık") >= 0 || GelenMesaj.indexOf("ışığı") >= 0) &&
      ((GelenMesaj.indexOf("aça") >= 0 || GelenMesaj.indexOf("açs") >= 0 || GelenMesaj.indexOf("aç ") >= 0 || GelenMesaj.indexOf("açıp") >= 0 ) ||
       (GelenMesaj.indexOf("kapat") >= 0 || GelenMesaj.indexOf("kapas") >= 0))) { //Gelen mesaj içerisinde ışık açma ve ışık kapama komutu var mı?
    int girildi = 0; //Birden fazla ışık açma komutunun geldiğini anlamak için oluşturulan değer.
    if (guzelkonusuldu == 1) {
      mesajlaritopla = mesajlaritopla + String(guzelkelimelerdegiskeni[random(sizeof(guzelkelimelerdegiskeni) / 4)]) + " ";
    }
    for (int donanam = 0; donanam < sizeof(hangiisiklarpinler); donanam++) { // Tanımlandırılmış pinler kadar komut sorgusu
      if ( GelenMesaj.indexOf(hangiisiklar[donanam]) >= 0) { //Gelen mesajda hangi pinin ismi var ise içeri gir.
        if ((GelenMesaj.indexOf("aça") >= 0 || GelenMesaj.indexOf("açs") >= 0 || GelenMesaj.indexOf("aç ") >= 0 || GelenMesaj.indexOf("açıp") >= 0)) { //açma komutunu mu?
          digitalWrite(hangiisiklarpinler[donanam], 1);
          if (girildi == 1) { //daha evvelden açma komutu kullanıldı mı? Kullanıldı ise cümleyi tamamla.
            mesajlaritopla = mesajlaritopla + " ve " + String(hangiisiklar[donanam]);
          }
          else { //değil ise devam et ve komutu kullandığını kenara yaz.
            mesajlaritopla = mesajlaritopla + String(hangiisiklar[donanam]);
            girildi = 1; //açma için işlem yapıldı.
          }
          ilgialakadengesi(i_dusur); //ilgi alakayı düşür. Bunun sebebi iyimser konuşulduğunda değer 2 kere arttırlıyor. Bu değer ile birini siliyoruz.
        }
        else if (GelenMesaj.indexOf("kapat") >= 0  || GelenMesaj.indexOf("kapas") >= 0 || GelenMesaj.indexOf("kapar") >= 0) { // değil ise kapama komutu mu?
          digitalWrite(hangiisiklarpinler[donanam], 0);
          if (girildi == 2) {
            mesajlaritopla = mesajlaritopla + " ve " + String(hangiisiklar[donanam]);
          }
          else {
            mesajlaritopla = mesajlaritopla + String(hangiisiklar[donanam]);
            girildi = 2; //kapatma için işlem yapıldı.
          }
          ilgialakadengesi(i_dusur);
        }
      }
    }
    if (girildi == 0) { //daha evvelden ışık açma/kapama komutu gelmedi ise/hangi odanın ışığının kapanıp açılması gerektiği söylenmedi ise.
      EskiZaman = YeniZaman;
      sorusoruldumu = true;
      if (GelenMesaj.indexOf("aça") >= 0 || GelenMesaj.indexOf("açs") >= 0 || GelenMesaj.indexOf("açıp") >= 0 || GelenMesaj.indexOf("aç ") >= 0) {
        acmakapamasorusu = ACMASORUSU;
        client.postMessage(msg->ChatId, mesajlaritopla + "Nerenin ışığını açayım?", odasorusu);
      }
      else if (GelenMesaj.indexOf("kapat") >= 0 || GelenMesaj.indexOf("kapas") >= 0) {
        acmakapamasorusu = KAPAMASORUSU;
        client.postMessage(msg->ChatId, mesajlaritopla + "Nerenin ışığını kapatayım?", odasorusu);
      }
      ilgialakadengesi(i_dusur);
      return;
    }
    else if (girildi == 1) {
      mesajlaritopla = mesajlaritopla + " ışıklarını açtım. ";
    }
    else if (girildi == 2) {
      mesajlaritopla = mesajlaritopla + " ışıklarını kapattım. ";
    }
  }
  if ((GelenMesaj.indexOf("açık") >= 0 || GelenMesaj.indexOf("kapalı") >= 0) && GelenMesaj.indexOf("mı") >= 0) { // Gelen mesaj soru mu?
    int girildi2 = 0;
    for (int donanam = 0; donanam < sizeof(hangiisiklarpinler); donanam++) {
      if ( GelenMesaj.indexOf(hangiisiklar[donanam]) >= 0) {
        if (digitalRead(hangiisiklarpinler[donanam])) {
          mesajlaritopla = mesajlaritopla + String(hangiisiklar[donanam]) + " ışıkları açık. ";
        }
        else {
          mesajlaritopla = mesajlaritopla + String(hangiisiklar[donanam]) + " ışıkları kapalı. ";
        }
        girildi2 = 1;
      }
    }
    if (girildi2 == 1) {
      client.postMessage(msg->ChatId, mesajlaritopla); //soru sorulduğu için cevap ve diğer işlemleri yapma.
      return;
    }
  }
  client.postMessage(msg->ChatId, mesajlaritopla); //toplanan bütün mesajları Telegram mesajı olarak gönder.
}
void TelegramGelenMesajlarHata (TelegramProcessError tbcErr, JwcProcessError jwcErr) //Hata belirleme
{
  Serial.println("onError");
  Serial.print("tbcErr"); Serial.print((int)tbcErr); Serial.print(":"); Serial.println(toString(tbcErr));
  Serial.print("jwcErr"); Serial.print((int)jwcErr); Serial.print(":"); Serial.println(toString(jwcErr));
  //bu komutlar telegramın içerisinde bulunan hata kodlarıdır.
}
String GelenVeri = ""; //Seri haberleşme için cümle tamamlama.
void SeriHaberlesme() {
  GelenVeri = ""; //Cümleyi sıfırla
  while (Serial.available()) { //seri haberleşme bitene kadar döndür.
    char GelenChar = (char)Serial.read(); //Gelen cümle karakter karakter geleceği için tek tek GelenChar a yaz.
    if (GelenChar == '\n') { //Seri haberleşmenin bittiğini anlamak için
      SeriHabeslemeSorgusu = true; //Haberleşme bitti
    }
    if (!SeriHabeslemeSorgusu) { //seri haberleşme bitmedi, bittiği zaman bu değer 0 olur.
      GelenVeri += GelenChar; //Her gelen karakteri GelenVeriye tek tek ekle.
    }
  }
  if (SeriHabeslemeSorgusu) { //Haberleşme sorgusu bitti ise işlem yap.
    //Serial.println(GelenVeri + " - " + String(GelenVeri.length()));
    if (GelenVeri.indexOf("Guncelleme") >= 0) { //Güncelleme isteği geldi ise
      Serial.print("Baglanilan Wi-Fi: ");
      Serial.println(ssid); //Seri haberleşmeye yazdır.
      delay(500);
      Serial.print("Wi-Fi Sifresi: ");
      Serial.println(password); //Seri haberleşmeye yazdır.
      delay(500);
      Serial.print("Wifi: ");
      if (WiFi.status() == WL_CONNECTED) { //İnternet bağlantısı yok ise ona göre mesaj gönder.
        Serial.println("Bagli");
      }
      else {
        Serial.println("Degil");
      }
      delay(500);
    }
    else if (GelenVeri.indexOf("Wifi") >= 0) { //Gelen mesaj wifi ise.
      GelenVeri.remove(0, 6); //Gelen mesaj Wifi : değer şeklinde, bunun ilk 6 karakterini sil.
      for (int x = 0; x < GelenVeri.length(); x++) //Gelenverinin her bir karakteri için döndür.
      {
        EEPROM.write(e_ssid + x, GelenVeri[x]); //Her birisini sırası ile yaz.
      }
      EEPROM.commit();//Kaydet

      EEPROM.write(e_ssid_uzunluk, GelenVeri.length()); //Gelen verinin kaç karakter olduğunu yaz.
      //Bu sayede bilgiyi EEPROM dan çeker iken fazlalıkları almıyoruz.
      EEPROM.commit();//Kaydet
      Serial.println("Kabul Edildi: " + GelenVeri); //Gelen mesajın alındığını ve işlendiğini bildir.
      delay(2000);
      ESP.restart(); //Kartı kapatıp yeniden aç
    }
    else if (GelenVeri.indexOf("Sifre") >= 0) {
      GelenVeri.remove(0, 7); //Gelen mesaj Sifre : değer şeklinde, ilk 7 karakteri sil.
      for (int x = 0; x < GelenVeri.length(); x++)
      {
        EEPROM.write(e_password + x, GelenVeri[x]);
      }
      EEPROM.commit();//Kaydet

      EEPROM.write(e_password_uzunluk, GelenVeri.length());
      EEPROM.commit();//Kaydet
      Serial.println("Kabul Edildi: " + GelenVeri);
      delay(2000);
      ESP.restart();
    }
    else if (GelenVeri.indexOf("Kontrol") >= 0) { //Gelen mesaj kontrol amaçlı ise
      Wire.beginTransmission(DIGER_ADRES);
      Wire.write("yüzünü göster"); //UNO ya mesaj gönder çiçeği hareket ettirsin.
      Wire.endTransmission();
    }

    Serial.println("Kabul Edildi: " + GelenVeri);
    delay(500);

  }
}

void setup()  //Cihaz açıldığında ilk okulanlar
{
  Serial.begin(115200);  //Haberleşme bağlantısı
  while (!Serial) {}  //Haberleşme ekranı başladığında devam et.


  ilgialaka = 0;
  for (int donanam = 0; donanam < sizeof(hangiisiklarpinler); donanam++) { // Pinleri tanımlandır sırası ile
    pinMode(hangiisiklarpinler[donanam], OUTPUT);
  }


  EEPROM.begin(512); //EEPROM içerisinde 512ye kadar adres kullanılacağını belirtme ve başlatma.
  ilgialaka = EEPROM.read(e_ilgialaka); //İlgi alaka durumunu EEPROM daki adresinden çek
  RGBKotumser = EEPROM.read(e_RGBKotumser); //Kötümser rengini EEPROM daki adresinden çek
  RGBIyimser = EEPROM.read(e_RGBIyimser); //İyimser rengini EEPROM daki adresinden çek
  String BosYaziDegiskeni = ""; //EEPROM dan çekilen bilgilerin tek bir yerde toplanması için
  for (int x = 0; x < EEPROM.read(e_ssid_uzunluk); x++) //Kaydedilen Wi-Fi bilgisinin uzunluğu kadar komut uygulama
  {
    char ToplananChar = EEPROM.read(e_ssid + x); //EEPROM içerisindeki karakterlerin tek tek çekilmesi.
    BosYaziDegiskeni = BosYaziDegiskeni + ToplananChar; //Tek bir cümleye tek tek ekleme.
  }

  BosYaziDegiskeni.toCharArray(ssid, BosYaziDegiskeni.length() + 1); //Toplanan cümlenin ssid ye yazdırılması. (Chardan String e çekilmesi için toCharArray kullanıldı.

  BosYaziDegiskeni = "";
  for (int x = 0; x < EEPROM.read(e_password_uzunluk); x++)
  {
    char ToplananChar = EEPROM.read(e_password + x);
    BosYaziDegiskeni = BosYaziDegiskeni + ToplananChar;
  }

  BosYaziDegiskeni.toCharArray(password, BosYaziDegiskeni.length() + 1);

  delay(3000);
  Serial.print("Baglanilan Wi-Fi: ");
  Serial.println(ssid);
  delay(500);
  Serial.print("Wi-Fi Sifresi: ");
  Serial.println(password);
  WiFi.mode(WIFI_STA); // Wi-Fi ayarını sadece alıcı olarak kullanmak için.
  WiFi.begin(ssid, password); // Wi-Fi başlangıç ve ayarı.

  int internetgiris = 0; //internet girişinin yapılıp yapılmadığını anlamak için oluşturulan değişken.
  while (WiFi.status() != WL_CONNECTED) {
    SeriHaberlesme(); //Seri haberleşmeden bilgi gelmesi ihtimali için.
    if (SeriHabeslemeSorgusu) {
      SeriHabeslemeSorgusu = false;
    }
    delay(500);
    Serial.print(".");
    internetgiris++;
    if (internetgiris >= 16) { //İnternet girişi hala yapılmadı ise restart at.
      Serial.println("Baglanti saglanamadi, restart atiliyor..");
      delay(500);
      ESP.restart();
    }
  }
  Serial.println("Wifi Bagli.");


  configTime(timezone * 3600, dst * 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nUluslararası saat sistemine bağlanıyor..");
  while (!time(nullptr)) { //Bağlantı sağlanana kadar bekle.
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Uluslararası saat sistemine bağlandı.");
  client.begin(TelegramGelenMesajlar, TelegramGelenMesajlarHata); //Telegram tanımlandırması yap.

  odasorusu.push(sizeof(hangiisiklarpinler), hangiisiklar); //Oda sorusu için telegram bilgisi tanımlandır.

  Serial.println("Telegrama Bağlandı.");
  //client.postMessage(yektaguleryuz, "Merhaba."); //Cihaz açıldı ise yektaya mesaj at.
  //client.postMessage(ecicekgrup,"SULAYIN BENİİİĞĞEE");
  //Serial.println("Merhaba mesajı gönderildi.");

  Wire.begin(BU_ADRES); //I2C bağlantısı için kanal tanımlandırma.

}
void ilgialakadurumugonder() { //I2C üzerinden RGB renk kodlarını gönder.

  if (ilgialaka < 100) { //İlgi alaka 100'ün altında ise sadece kötümser rengi gönder.
    Wire.beginTransmission(DIGER_ADRES);
    if (RGBKotumser == 1) { //Kötümser rengi Kırmızı ise.
      Wire.write("RKG: 255000000");
    }
    else if (RGBKotumser == 2) { //Yeşil rengi Kırmızı ise.
      Wire.write("RKG: 000255000");
    }
    else if (RGBKotumser == 3) { //Mavi rengi Kırmızı ise.
      Wire.write("RKG: 000000255");
    }
    Wire.endTransmission();

  }
  else if (ilgialaka < 200) { //İlgi alaka 200'ün altında ise hem iyimser hemde kötümser rengini aynı anda 
    Wire.beginTransmission(DIGER_ADRES);
    Wire.write("RKG: ");
    if (RGBIyimser == 1 || RGBKotumser == 1) { //İyimser yada kötümser rengi tanımlı ise Kırmızı renginin olduğunu yaz.
      Wire.write("255");
    }
    else {
      Wire.write("000");
    }
    if (RGBIyimser == 2 || RGBKotumser == 2) { //İyimser yada kötümser rengi tanımlı ise Yeşil renginin olduğunu yaz.
      Wire.write("255");
    }
    else {
      Wire.write("000");
    }
    if (RGBIyimser == 3 || RGBKotumser == 3) { //İyimser yada kötümser rengi tanımlı ise Mavi renginin olduğunu yaz.
      Wire.write("255");
    }
    else {
      Wire.write("000");
    }
    Wire.endTransmission();
  }
  else { //200 üzerinde ise iyimser rengini gönder sadece.

    Wire.beginTransmission(DIGER_ADRES);
    if (RGBIyimser == 1) {
      Wire.write("RKG: 255000000");
    }
    else if (RGBIyimser == 2) {
      Wire.write("RKG: 000255000");
    }
    else if (RGBIyimser == 3) {
      Wire.write("RKG: 000000255");
    }
    Wire.endTransmission();

  }
}
void loop() //Sonsuz döngü
{
  YeniZaman = millis(); //Milisaniye olarak yenizamana şuanki zamanı tanımlandır
  time(&now); //Şuanki zamanı çek
  timeinfo = localtime(&now); //Şuanki zamanı çek
  client.loop(); //Telegram sorgusu yap sürekli
  if (YeniZaman - RGBIcinEskiZaman > RGBSorguSuresi * 1000) { // LDR Sorgusu yaparken bekletme
    ilgialakadurumugonder(); //UNOYA bilgi gönderme.
    RGBIcinEskiZaman = YeniZaman;
  }
  SeriHaberlesme(); //Seri haberleşme bilgisi al/gönder.
  if (SeriHabeslemeSorgusu) {
    SeriHabeslemeSorgusu = false;
  }
}
