#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9       // Pin RST dari modul RFID
#define SS_PIN 10       // Pin SDA dari modul RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Membuat objek MFRC522

// Inisialisasi LCD I2C (alamat I2C: 0x27, 16x2 karakter)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C, jumlah kolom dan baris

// UID KTP yang diizinkan (Ganti dengan UID yang sesuai)
byte allowedCard[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // Contoh UID KTP

void setup() {
  lcd.begin(16, 2);      // Inisialisasi LCD
  lcd.backlight();       // Menghidupkan backlight LCD
  Serial.begin(9600);    // Memulai komunikasi serial
  SPI.begin();           // Inisialisasi SPI bus
  rfid.PCD_Init();       // Inisialisasi RFID reader
  lcd.print("Scan KTP..."); // Menampilkan pesan di LCD
}

void loop() {
  // Mengecek apakah ada kartu yang terdeteksi
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    // Mencetak UID KTP yang terdeteksi
    Serial.print("UID KTP: ");
    for (byte i = 0; i < rfid.uid.size; i++) {
      Serial.print(rfid.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // Bandingkan UID kartu dengan UID yang diizinkan
    lcd.clear(); // Bersihkan LCD
    if (isAllowedCard()) {
      lcd.print("Status: ALLOW"); // Tampilkan "ALLOW"
    } else {
      lcd.print("Status: DENY"); // Tampilkan "DENY"
    }

    // Menunggu sebentar sebelum pengecekan berikutnya
    delay(3000); // Tampilkan hasil selama 3 detik
    lcd.clear();
    lcd.print("Scan KTP..."); // Kembali ke pesan awal

    // Menghentikan komunikasi dengan kartu
    rfid.PICC_HaltA();
  }

  delay(500); // Menunggu sebentar sebelum pengecekan berikutnya
}

// Fungsi untuk memeriksa apakah kartu yang terdeteksi adalah yang diizinkan
bool isAllowedCard() {
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != allowedCard[i]) {
      return false; // Kartu tidak diizinkan
    }
  }
  return true; // Kartu diizinkan
}
