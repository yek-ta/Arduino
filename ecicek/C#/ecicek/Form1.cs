//C# kütüphaneleri
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports; //Seri haberleşme port kütüphanesi (USB)
using System.IO; //Seri haberleşme kütüphanesi

namespace ecicek
{
    public partial class Form1 : Form
    {
        int cicekinternetebagli; //İnternete bağlı olup olmadığını tutan değişken
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) //Program başladığında
        {
            comboBox1.DataSource = SerialPort.GetPortNames(); //Seriportların bütün hepsini combobox a yaz.
        }

        private void button1_Click(object sender, EventArgs e) //Bağlan butonu
        {
            if (!serialPort1.IsOpen) //Bağlantı yapılmadı ise (üst üste binip hata vermesin)
            {
                if (comboBox1.Text != "") //Combobox ta seçili var ise
                {
                    serialPort1.PortName = comboBox1.Text; //Port adını comboboxtan al
                    serialPort1.BaudRate = 115200; //Rate ayarı(WeMos için seri port 115200
                    serialPort1.Open(); //Seri haberleşmeyi başlat
                    label1.Text = "Bağlanıldı"; // combobox altındaki yere bilgi yaz
                    timer1.Start(); // Sürekli işlem yapmak için timer başlat
                    gonderilecekyazi = "Guncelleme"; //WeMos a gönderilecek komutun yazdırılması
                    serialPort1.WriteLine(gonderilecekyazi); //gonderilecekyazıyı WeMos a gönder.
                }
            }
            else
            {
                MessageBox.Show("Önce bağlantıyı kesmelisiniz");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen) //Bağlantı var ise işlem yaptır.
            {
                timer1.Stop(); //Sürekli işlemi bitir.
                serialPort1.Close(); //Seri haberleşme bağlantısını kapat.
                label1.Text = "Bağlantı Kesildi";
                label13.Text = "Çiçek bilgisayara bağlı değil"; //Bağlantı durumunu yazdır.
                cicekinternetebagli = 0; //İnternete bağlı olmadığını göster.
            }
        }
        string gonderilecekyazi; //Gönderilecek yazıyı tutan değişken.
        private void button3_Click(object sender, EventArgs e) //Yeni Wi-Fi gönderme butonu
        {
            if (serialPort1.IsOpen)
            {
                gonderilecekyazi = "Wifi: " + textBox1.Text; //Textboxtaki bilgiyi Wifi: ile birleştir gönder.
                serialPort1.WriteLine(gonderilecekyazi); //WriteLine ile en sona \n ekleniyor. Bu sayede mesajın bittiği bildiriliyor.
                label13.Text = "Çiçek internete bağlı değil";
                cicekinternetebagli = 0;
            }
        
        }

        private void button4_Click(object sender, EventArgs e) //Yeni Wi-Fi Şifresi gönderme butonu
        {
            if (serialPort1.IsOpen)
            {
                gonderilecekyazi = "Sifre: " + textBox2.Text;
                serialPort1.WriteLine(gonderilecekyazi);
                label13.Text = "Çiçek internete bağlı değil";
                cicekinternetebagli = 0;
            }
        }

        private void timer1_Tick(object sender, EventArgs e) //Sürekli çalışma
        {
            try
            {
                string sonuc = serialPort1.ReadExisting(); //Seri haberleşmen gelen bilgileri çekip sonuca yazdır.
                if (sonuc.IndexOf("Wi-Fi") >= 0 && sonuc.IndexOf("Baglanilan") >= 0) //Wi-Fi ve Baglanilan var ise sonuc değişkeninde içeri gir
                {
                    label4.Text = sonuc + ""; //Bağlanılan Wi-Fi bilgisini ekrana yazdır.
                }
                else if (sonuc.IndexOf("Wi-Fi") >= 0 && sonuc.IndexOf("Sifresi") >= 0)
                {
                    label5.Text = sonuc + ""; //Bağlanılan Wi-Fi Şifre bilgisini ekrana yazdır.
                }
                else if (sonuc.IndexOf("Wifi") >= 0 && sonuc.IndexOf("Bagli") >= 0)
                {
                    label13.Text = "Çiçek internete bağlı"; //Çiçek durumunu yazdır.
                    cicekinternetebagli = 1;
                }
                else if (sonuc.IndexOf("Wifi") >= 0 && sonuc.IndexOf("Degil") >= 0)
                {
                    label13.Text = "Çiçek internete bağlı değil";
                    cicekinternetebagli = 0;
                }
                label2.Text = sonuc + "";
            }
            catch (Exception ex)
            {
                timer1.Stop();
                if (serialPort1.IsOpen == true)
                {
                    serialPort1.Close();
                }
                MessageBox.Show(ex.Message);
                
                
                throw;
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e) //Program kapatıldı ise
        {
            if (serialPort1.IsOpen == true) //Bağlantı var ise bağlantıyı kes.
            {
                serialPort1.Close();
            }
        }

        private void button5_Click(object sender, EventArgs e) //Güncelle butonu
        {
            if (serialPort1.IsOpen) //Seri haberleşme var ise.
            {
                gonderilecekyazi = "Guncelleme"; //Guncelleme bilgisini WeMos a gönder.
                serialPort1.WriteLine(gonderilecekyazi);
            }
        }

        private void button6_Click(object sender, EventArgs e) //WeMos a kontrol komutu gönder.
        {
            if (serialPort1.IsOpen)
            {
                if (cicekinternetebagli == 1)
                {
                    gonderilecekyazi = "Kontrol";
                    serialPort1.WriteLine(gonderilecekyazi);
                }
                else //İnternete bağlı değil ise çiçek işlem yapmayacaktır. Bunu anlamak için yapılan sorgu.
                {
                    MessageBox.Show("Bu komutun çalışabilmesi için çiçeğin internete bağlı olması gerekmektedir.");
                }
            }
        }

    }
}
