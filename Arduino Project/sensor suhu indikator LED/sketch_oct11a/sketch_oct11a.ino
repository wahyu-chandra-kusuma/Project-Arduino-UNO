#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LM35_PIN A0    // Pin analog yang terhubung ke LM35
#define LEDPIN 13      // Pin dimana LED terhubung

// Inisialisasi LCD I2C, alamat 0x27 bisa berbeda tergantung LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  pinMode(LEDPIN, OUTPUT);  // Atur pin LED sebagai output
  Serial.begin(9600);       // Memulai komunikasi serial
  lcd.init();               // Inisialisasi LCD
  lcd.backlight();          // Menghidupkan backlight LCD
  lcd.setCursor(0, 0);      // Atur posisi awal cursor
  lcd.print("Suhu:");       // Menampilkan teks awal
}

void loop() {
  // Membaca nilai dari sensor LM35 (sensor memberikan 10mV/°C)
  int sensorValue = analogRead(LM35_PIN);
  float voltage = sensorValue * (5.0 / 1023.0);  // Konversi nilai analog ke voltase
  float currentTemp = voltage * 100;  // Konversi voltase ke suhu dalam derajat Celcius

  // Menampilkan suhu pada LCD
  lcd.setCursor(6, 0);  // Set posisi setelah tulisan "Suhu:"
  lcd.print(currentTemp);  // Menampilkan suhu
  lcd.print(" C ");    // Menambahkan satuan suhu

  // Menyalakan LED jika suhu di atas 27 derajat Celcius
  if (currentTemp > 27) {
    digitalWrite(LEDPIN, HIGH);  // Menyalakan LED
  } else {
    digitalWrite(LEDPIN, LOW);   // Mematikan LED
  }

  // Mencetak suhu saat ini ke Serial Monitor
  Serial.print("Current Temperature: ");
  Serial.println(currentTemp);

  delay(2000);  // Menunggu 2 detik sebelum pembacaan berikutnya
}
