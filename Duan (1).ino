#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int fanPin = 11;    

// Xác định địa chỉ I2C và đăng ký địa chỉ
const int i2cAddress = 0x48; // Thay đổi địa chỉ này phù hợp với cảm biến nhiệt độ của bạn
const int tempRegister = 0; // Đây là thanh ghi lưu trữ dữ liệu nhiệt độ (xem datasheet của cảm biến)

const float desiredTemperature = 20.0; // Nhiệt độ cần thiết là 20 độ C

void setup() {
  pinMode(fanPin, OUTPUT);       
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Tu lanh ");
  lcd.setCursor(0, 1);
  lcd.print("Nhiet do: "); 
  Wire.begin();
} 

float readTemperature() {
  Wire.beginTransmission(i2cAddress);
  Wire.write(tempRegister); // Gửi địa chỉ thanh ghi
  Wire.endTransmission(false); // Chưa kết thúc truyền

  Wire.requestFrom(i2cAddress, 2); // Yêu cầu 2 byte dữ liệu (giá trị nhiệt độ)
  int rawTemperature = Wire.read() << 8 | Wire.read(); // Kết hợp hai byte thành một giá trị 16 bit
  float temperature = rawTemperature / 16.0; // Chuyển đổi giá trị thô sang độ C

  return temperature;
}

void loop() 
{
  float temperature = readTemperature();

  // Hiển thị nhiệt độ trên LCD
  lcd.setCursor(0, 1);
  lcd.print("Nhiet do: ");
  lcd.setCursor(10, 1);
  lcd.print(temperature);
  lcd.setCursor(14, 1);
  lcd.print("C");

  Serial.print(temperature);
  Serial.println(" C");

  if (temperature >= desiredTemperature) {
      digitalWrite(fanPin, HIGH);
    } 
  else {
      digitalWrite(fanPin, LOW);
    }
  delay(1000); // Đợi 1 giây trước khi đọc lại nhiệt độ
}