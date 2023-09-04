#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  // Initialize the LCD with the corresponding pins

SoftwareSerial gsmSerial(0, 1);  // RX, TX pins for GSM module (D0, D1)

int pirPin = 8;  // PIR sensor connected to D8
int buzzerPin = 9;  // Buzzer connected to D9

void setup() {
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.clear();
  
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  gsmSerial.begin(9600);
}

void loop() {
  if (digitalRead(pirPin) == HIGH) {
    // PIR sensor detects motion (driver is drowsy)
    lcd.setCursor(0, 0);
    lcd.print("Drowsy Driver");
    lcd.setCursor(0, 1);
    lcd.print("Wake Up!");

    digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
    delay(1000);  // Buzzer sounds for 1 second
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer

    sendSMS();  // Send SMS to the office or agency
    makeCall();  // Make a call to the driver
  } else {
    // No motion detected (driver is awake)
    lcd.clear();
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
  }
  delay(100);  // Adjust the delay as needed
}

void sendSMS() {
  gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(100);
  gsmSerial.println("AT+CMGS=\"+1234567890\"");  // Replace with the office/agency's phone number
  delay(100);
  gsmSerial.println("Driver is drowsy!");  // SMS content
  gsmSerial.println((char)26);  // End SMS
  delay(1000);
}

void makeCall() {
  gsmSerial.println("ATD+1234567890;");  // Replace with the driver's phone number
  delay(1000);  // Call for 1 second
  gsmSerial.println("ATH");  // Hang up the call
  delay(1000);
}
