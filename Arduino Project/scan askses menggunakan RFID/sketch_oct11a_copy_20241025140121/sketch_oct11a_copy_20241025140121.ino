#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define LED 2

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C 0x27 bisa berbeda, cek dengan I2C scanner jika tidak berfungsi.

void setup() {
  // Inisialisasi LCD
  lcd.begin();
  lcd.backlight();

  // Inisialisasi komunikasi SPI untuk RFID
  SPI.begin();
  rfid.PCD_Init();

  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  Serial.println("Scan your card to check access...");

  // Menampilkan pesan selamat datang di LCD
  lcd.setCursor(0, 0);
  lcd.print("Scan your card!");
  delay(2000);
  lcd.clear();

  pinMode(LED,OUTPUT);

}

// Daftar UID kartu yang diizinkan (ubah sesuai dengan kartu Anda)
byte allowedCards[][7] = {
  {0x04, 0x20, 0x18, 0xEA, 0xF1, 0x6A, 0x80}
};
const int allowedCardsCount = sizeof(allowedCards) / sizeof(allowedCards[0]);

bool isCardAllowed(byte *uid) {
  for (int i = 0; i < allowedCardsCount; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (allowedCards[i][j] != uid[j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

void printUID(byte *uid, byte size) {
  for (byte i = 0; i < size; i++) {
    Serial.print(uid[i], HEX);
    if (i < size - 1) Serial.print(":");
  }
  Serial.println();
}

void loop() {
  // Periksa apakah kartu tersedia
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Pilih kartu
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Tampilkan UID kartu di LCD dan Serial Monitor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Card UID:");
  Serial.print("UID Kartu: ");
  
  // Baca UID kartu dan tampilkan di LCD dan Serial Monitor
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(rfid.uid.uidByte[i], HEX);
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) {
      lcd.print(":");
      Serial.print(":");
    }
  }
  Serial.println();

  // Cek apakah kartu diperbolehkan
  if (isCardAllowed(rfid.uid.uidByte)) {
    lcd.setCursor(0, 1);
    lcd.print("Access Granted");
    Serial.println("Akses Diperbolehkan");

  } else {
    lcd.setCursor(0, 1);
    lcd.print("Access Denied");
    Serial.println("Akses Ditolak");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
  
  }

  // Hentikan komunikasi dengan kartu
  rfid.PICC_HaltA();
  delay(3000); // Tunggu 3 detik sebelum kembali ke mode siap scan
  lcd.clear();
}
