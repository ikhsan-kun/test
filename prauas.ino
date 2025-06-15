#include "CTBot.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

CTBot myBot;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Atur alamat dan ukuran LCD (16x2)

String ssid  = "ikhsan";     // REPLACE mySSID WITH YOUR WIFI SSID
String pass  = "lombang123";      // REPLACE myPassword WITH YOUR WIFI PASSWORD, IF ANY
String token = "7317015072:AAF7EDEXNwaakN3o4mAdMwe3GxHevBZtQ7w"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
const int id = 1978767353;

const int buzzerPin = D8; // Pin untuk buzzer (sesuaikan dengan koneksi Anda)
const int mq2Pin = A0;    // Pin untuk sensor MQ-2
int mq2Value;

// Frekuensi nada
const int melody[] = {262, 294, 330, 349, 392, 440, 494}; // Nada: C, D, E, F, G, A, B
const int noteDuration = 500; // Durasi tiap nada dalam milidetik

// Ambang batas untuk deteksi asap (sesuaikan sesuai pengujian)
const int smokeThreshold = 400;

void setup() {
    // Initialize the Serial
    Serial.begin(9600);

    // Setup buzzer pin
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW); // Pastikan buzzer mati saat awal

    // Setup sensor MQ-2
    pinMode(mq2Pin, INPUT);

    Wire.begin(4, 5);
    // Initialize the LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Starting...");

    // Connect the ESP8266 to the desired access point
    myBot.wifiConnect(ssid, pass);

    // Set the Telegram bot token
    myBot.setTelegramToken(token);
    
    // Check if all things are OK
    if (myBot.testConnection()) {
        Serial.println("\ntestConnection OK");
        lcd.setCursor(0, 1);
        lcd.print("Bot Ready!");
    } else {
        Serial.println("\ntestConnection NOK");
        lcd.setCursor(0, 1);
        lcd.print("Bot Error!");
    }

    delay(2000);
    lcd.clear(); 
}

void playMelody() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Playing melody...");

    for (int i = 0; i < 7; i++) { 
        tone(buzzerPin, melody[i], noteDuration);
        delay(noteDuration + 100); 
    }

    noTone(buzzerPin); // Matikan buzzer
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Melody selesai!");
    lcd.clear();
}

void checkSmoke() {
    int mq2Value = analogRead(mq2Pin); // Membaca nilai dari sensor MQ-2
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MQ-2 Value: ");
    lcd.print(mq2Value);

    // Jika nilai lebih besar dari threshold, dianggap ada asap
    if (mq2Value > smokeThreshold) {
        lcd.setCursor(0, 1);
        lcd.print("Asap terdeteksi!");
        tone(buzzerPin, 1000); // Nyalakan buzzer
    } else {
        lcd.setCursor(0, 1);
        lcd.print("Asap Aman");
        tone(buzzerPin, 0); // Matikan buzzer
    }
}

void loop() {
    // TBMessage msg;
    // if (myBot.getNewMessage(msg)) {
    //     // Cetak pesan masuk di LCD
    //     lcd.clear(); // Bersihkan layar
    //     lcd.setCursor(0, 0);
    //     lcd.print("msg masuk : ");
    //     lcd.setCursor(0, 1);

    //     // Potong pesan jika lebih panjang dari 16 karakter
    //     String pesan = msg.text.length() > 16 ? msg.text.substring(0, 16) : msg.text;
    //     lcd.print(pesan);

    //     // Kirim balasan atau kontrol buzzer
    //     if (msg.text == "halo" || msg.text == "Halo") {
    //         myBot.sendMessage(id, "\nhalo juga ikhsan, saya botsmoke ");
    //     } else if (msg.text == "buz" || msg.text == "Buz") {
    //         myBot.sendMessage(id, "\nPlaying melody!");
    //         playMelody(); // Panggil fungsi untuk memainkan melodi
    //     } else if (msg.text == "cek" || msg.text == "Cek") {
    //         myBot.sendMessage(id, "Memeriksa kondisi asap...");                                                 
    //         // Kirim hasil ke Telegram
    //         mq2Value = analogRead(mq2Pin);
    //         if (mq2Value > smokeThreshold) {
    //             myBot.sendMessage(id, "Asap terdeteksi! Kondisi tidak aman. \n cepat periksa ruangan");
    //         } else {
    //             myBot.sendMessage(id, "Kondisi aman, tidak ada asap terdeteksi. \n aman bos, botsmoke ada disini ");
    //         }
    //     }
        
    // }
    // else{
      checkSmoke(); 
    // }
    mq2Value = analogRead(mq2Pin);
    if(mq2Value > smokeThreshold){
      myBot.sendMessage(id, "Asap terdeteksi! Kondisi tidak aman. \n cepat periksa ruangan");
    }
    delay(300);
}
