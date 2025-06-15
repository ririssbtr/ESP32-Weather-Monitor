#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>          // Ganti dengan ESP8266WiFi.h jika menggunakan ESP8266
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";       // Ganti dengan SSID Wi-Fi kamu
const char* password = "";              // Ganti dengan password Wi-Fi kamu

String apiKey = "20ca0ff523294dcdeb424dfc5802e21b";       // API Key dari OpenWeatherMap
String city = "Malang";              
String units = "metric";              // "metric" untuk Celsius
String server = "https://api.openweathermap.org/data/2.5/weather?q=Malang&units=metric&appid=20ca0ff523294dcdeb424dfc5802e21b";

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Alamat I2C dan ukuran LCD

void setup() {
  Serial.begin(115200);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Weather Info:");

  // Koneksi Wi-Fi
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected!");
  delay(2000);
  lcd.clear();
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    
    HTTPClient http;
    http.begin(server);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      
      String payload = http.getString();
      Serial.println(payload);

      // Parsing suhu (temperature)
      int tempIndex = payload.indexOf("\"temp\":");
      int tempEndIndex = payload.indexOf(",", tempIndex);
      String temp = payload.substring(tempIndex + 7, tempEndIndex);

      // Parsing humidity
      int humIndex = payload.indexOf("\"humidity\":");
      int humEndIndex = payload.indexOf(",", humIndex);
      String humidity = payload.substring(humIndex + 10, humEndIndex);

      // Parsing deskripsi cuaca
      int descIndex = payload.indexOf("description");
      String desc = payload.substring(descIndex + 14, payload.indexOf("\"", descIndex + 14));

      // Menampilkan ke LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("T:");
      lcd.print(temp);
      lcd.print((char)223); // simbol derajat
      lcd.print("C H");
      lcd.print(humidity);
      lcd.print("%");

      lcd.setCursor(0, 1);
      lcd.print(desc);

    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
  
  delay(60000);  // Update setiap 1 menit
}
