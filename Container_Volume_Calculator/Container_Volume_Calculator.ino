#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define TRIG_PIN 23
#define ECHO_PIN 22

// ** Pilihan Bentuk: Tabung atau Balok **
// Pilih salah satu bentuk dengan mengaktifkan bagian yang sesuai,
// dan komentari bagian yang tidak digunakan.

// *** Untuk Tabung ***
const float diameter_tabung = 10.0; // Diameter tabung dalam cm
const float tinggi_tabung = 20.0;  // Tinggi tabung dalam cm

/*
  // *** Untuk Balok ***
  const float panjang_balok = 15.0; // Panjang balok dalam cm
  const float lebar_balok = 10.0;   // Lebar balok dalam cm
  const float tinggi_balok = 20.0;  // Tinggi balok dalam cm
*/

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Volume Meter");
  delay(2000);
}

void loop() {
  float distance = readUltrasonic();
  float volume_percent = 0;

  // *** Perhitungan untuk Tabung ***
  float tinggi_isi = tinggi_tabung - distance; 
  if (tinggi_isi < 0) tinggi_isi = 0;         
  float volume_total = 3.14159 * pow((diameter_tabung / 2), 2) * tinggi_tabung;
  float volume_isi = 3.14159 * pow((diameter_tabung / 2), 2) * tinggi_isi;
  volume_percent = (volume_isi / volume_total) * 100;

  /*
    // *** Perhitungan untuk Balok ***
    float tinggi_isi = tinggi_balok - distance; 
    if (tinggi_isi < 0) tinggi_isi = 0;         
    float volume_total = panjang_balok * lebar_balok * tinggi_balok;
    float volume_isi = panjang_balok * lebar_balok * tinggi_isi;
    volume_percent = (volume_isi / volume_total) * 100;
  */

  // Tampilkan hasil di Serial Monitor dan LCD
  Serial.print("Volume: ");
  Serial.print(volume_percent);
  Serial.println("%");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volume: ");
  lcd.print(volume_percent);
  lcd.print("%");

  delay(1000);
}

// Fungsi untuk membaca jarak dari sensor ultrasonik
float readUltrasonic() {
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;

  if (distance <= 0 || distance > 400) {
    Serial.println("Sensor Error!");
    return -1;
  }

  return distance;
}
