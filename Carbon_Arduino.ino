#include <LiquidCrystal.h>

// ---------------- LCD ----------------
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ---------------- Pins ----------------
const int buttonPin = 7;

const int greenLED = 8;
const int yellowLED = 9;
const int redLED = 10;

// Current footprint state
// 0 = LOW
// 1 = MEDIUM
// 2 = HIGH
int state = 0;

bool lastButtonState = LOW;

void setup()
{
  // Start Serial Communication
  Serial.begin(9600);

  // Start LCD
  lcd.begin(16, 2);

  // Set pin modes
  pinMode(buttonPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  // Welcome screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Carbon Foot");
  lcd.setCursor(0, 1);
  lcd.print("Calculator");

  delay(2000);
}

void loop()
{
  // ---------------- Read Button ----------------
  bool buttonState = digitalRead(buttonPin);

  // If button was just pressed...
  if (buttonState == HIGH && lastButtonState == LOW)
  {
    // Tell Python the button was pressed
    Serial.println("NEXT");

    delay(250);   // Debounce
  }

  lastButtonState = buttonState;

  // ---------------- Receive State from Python ----------------
  if (Serial.available())
  {
    state = Serial.parseInt();
  }

  // ---------------- Turn LEDs Off ----------------
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  // ---------------- Display Results ----------------
  lcd.clear();

  switch (state)
  {

    // ---------- LOW ----------
    case 0:

      digitalWrite(greenLED, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("Footprint:");

      lcd.setCursor(0, 1);
      lcd.print("LOW 0.8 kg");

      break;


    // ---------- MEDIUM ----------
    case 1:

      digitalWrite(yellowLED, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("Footprint:");

      lcd.setCursor(0, 1);
      lcd.print("MED 1.8 kg");

      break;


    // ---------- HIGH ----------
    case 2:

      digitalWrite(redLED, HIGH);

      lcd.setCursor(0, 0);
      lcd.print("Footprint:");

      lcd.setCursor(0, 1);
      lcd.print("HIGH 3.2 kg");

      break;

  }

  delay(100);
}