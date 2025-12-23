#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Hoang Thuan";
const char* password = "12345678889";
// const char* server = "http://192.168.100.159/laymaunuoc";
// const char* server = "http://172.20.10.2/laymaunuoc";

const char* server = "http://10.221.66.163/laymaunuoc";

// ===== PIN =====
#define RELAY1 5
#define RELAY2 18
#define TRIG_PIN 3
#define ECHO_PIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== THAM SỐ =====
int max_level_threshold = 8; // Ngưỡng ngắt (cm)

// =================================================

void setup() {
  Serial.begin(115200);

  // Khởi tạo I2C cho LCD (SDA=6, SCL=7)
  Wire.begin(6, 7);

  lcd.init();
  lcd.backlight();
  lcd.print("Dang khoi dong");

  WiFi.begin(ssid, password);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Relay mặc định TẮT
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi OK");
  delay(1000);
}

// ===== ĐO KHOẢNG CÁCH =====
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  
  return (distance == 0) ? 999 : distance;
}

// ===== CẬP NHẬT LCD =====
void updateLCD(long distance) {
  lcd.setCursor(0, 0);
  lcd.print("Muc nuoc:      ");
  lcd.setCursor(10, 0);
  lcd.print(distance);
  lcd.print("cm ");

  lcd.setCursor(0, 1);
  lcd.print("B1:");
  lcd.print(digitalRead(RELAY1) ? "ON " : "OFF");
  lcd.print(" B2:");
  lcd.print(digitalRead(RELAY2) ? "ON " : "OFF");
}

// =================================================

void loop() {
  long currentDistance = getDistance();
  bool isAnyPumpOn = (digitalRead(RELAY1) == HIGH || digitalRead(RELAY2) == HIGH);

  // 1. NHẬN LỆNH TỪ WEB VÀ CẬP NHẬT TRẠNG THÁI
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "/get_command.php";

    // NẾU ĐÃ CHẠM NGƯỠNG MÀ BƠM VẪN ĐANG BẬT -> Gửi status=full để Server chuyển sang OFF
    if (currentDistance <= max_level_threshold && isAnyPumpOn) {
      url += "?status=full";
      
      // Tắt ngay lập tức tại ESP32
      digitalWrite(RELAY1, LOW);
      digitalWrite(RELAY2, LOW);
      Serial.println("NGAT AN TOAN: Da gui yeu cau OFF len Server");
    }

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      payload.trim();
      if (payload != "" && payload != "OFF") {
        processCommands(payload);
      }
    }
    http.end();
  }

  updateLCD(currentDistance);
  delay(300); 
}

// =================================================

void processCommands(String input) {
  int start = 0;
  int end = input.indexOf(',');

  while (end != -1) {
    executeSingleCommand(input.substring(start, end));
    start = end + 1;
    end = input.indexOf(',', start);
  }
  executeSingleCommand(input.substring(start));
}

void executeSingleCommand(String cmd) {
  cmd.trim();
  if (cmd == "") return;

  // Cập nhật ngưỡng MAX từ Web
  if (cmd.startsWith("MAX:")) {
    max_level_threshold = cmd.substring(4).toInt();
    Serial.print("NGUONG MOI: ");
    Serial.println(max_level_threshold);
  }
  
  // Bật/Tắt Bơm 1
  else if (cmd == "R1_ON") {
    // Chỉ cho phép bật nếu hiện tại đang an toàn (xa hơn ngưỡng ngắt)
    if (getDistance() > max_level_threshold) {
      digitalWrite(RELAY1, HIGH);
      Serial.println("R1 ON");
    }
  }
  else if (cmd == "R1_OFF") {
    digitalWrite(RELAY1, LOW);
    Serial.println("R1 OFF");
  }
  
  // Bật/Tắt Bơm 2
  else if (cmd == "R2_ON") {
    if (getDistance() > max_level_threshold) {
      digitalWrite(RELAY2, HIGH);
      Serial.println("R2 ON");
    }
  }
  else if (cmd == "R2_OFF") {
    digitalWrite(RELAY2, LOW);
    Serial.println("R2 OFF");
  }
}