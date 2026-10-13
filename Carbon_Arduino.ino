
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Hardware pins
const int PHOTO_PIN = A0;
const int BUTTON_PIN = 7;
const int GREEN_LED = 8;
const int YELLOW_LED = 9;
const int RED_LED = 10;

// Sensor and serial timing
const unsigned long SENSOR_INTERVAL_MS = 250;
const unsigned long DEBOUNCE_MS = 35;

// Prototype states
const int STATE_LOW = 0;
const int STATE_MEDIUM = 1;
const int STATE_HIGH = 2;

int lightValue = 0;
int footprintState = STATE_LOW;
float demoIndex = 0.0;

unsigned long lastSensorTime = 0;
unsigned long lastDebounceTime = 0;

bool lastButtonReading = LOW;
bool stableButtonState = LOW;

char serialBuffer[64];
size_t serialIndex = 0;

void sendSensorReading() {
  lightValue = analogRead(PHOTO_PIN);

  Serial.print("LIGHT:");
  Serial.println(lightValue);
}

void updateLEDs(int state) {
  digitalWrite(GREEN_LED, state == STATE_LOW);
  digitalWrite(YELLOW_LED, state == STATE_MEDIUM);
  digitalWrite(RED_LED, state == STATE_HIGH);
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("Sensor level:   ");

  lcd.setCursor(0, 1);

  switch (footprintState) {
    case STATE_LOW:
      lcd.print("LOW   Index:");
      break;

    case STATE_MEDIUM:
      lcd.print("MED   Index:");
      break;

    case STATE_HIGH:
      lcd.print("HIGH  Index:");
      break;

    default:
      lcd.print("INVALID STATE  ");
      return;
  }

  // Print the prototype index, not actual CO2 emissions.
  lcd.print(demoIndex, 2);
  lcd.print(" ");
}

void handleSerialLine(char *line) {
  // Expected format: RESULT:<state>:<demoIndex>
  if (strncmp(line, "RESULT:", 7) != 0) {
    return;
  }

  char *stateText = line + 7;
  char *separator = strchr(stateText, ':');

  if (separator == NULL) {
    return;
  }

  *separator = '\0';

  char *endPointer;
  long parsedState = strtol(stateText, &endPointer, 10);

  if (*stateText == '\0' || *endPointer != '\0') {
    return;
  }

  char *indexText = separator + 1;
  float parsedIndex = strtof(indexText, &endPointer);

  if (*indexText == '\0' || *endPointer != '\0') {
    return;
  }

  if (parsedState < STATE_LOW ||
      parsedState > STATE_HIGH ||
      !isfinite(parsedIndex) ||
      parsedIndex < 0.0 ||
      parsedIndex > 10.0) {
    return;
  }

  footprintState = (int)parsedState;
  demoIndex = parsedIndex;

  updateLEDs(footprintState);
  updateDisplay();
}

void readSerialLines() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      serialBuffer[serialIndex] = '\0';

      if (serialIndex > 0) {
        handleSerialLine(serialBuffer);
      }

      serialIndex = 0;
    } else if (c != '\r') {
      if (serialIndex < sizeof(serialBuffer) - 1) {
        serialBuffer[serialIndex++] = c;
      } else {
        // Discard an overlong or malformed line.
        serialIndex = 0;
      }
    }
  }
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
    lastButtonReading = reading;
  }

  if (millis() - lastDebounceTime >= DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      if (stableButtonState == HIGH) {
        Serial.println("NEXT");
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  updateLEDs(STATE_LOW);

  lcd.setCursor(0, 0);
  lcd.print("Carbon Tracker");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  delay(1500);
  lcd.clear();

  Serial.println("READY");
}

void loop() {
  handleButton();
  readSerialLines();

  unsigned long now = millis();

  if (now - lastSensorTime >= SENSOR_INTERVAL_MS) {
    lastSensorTime = now;
    sendSensorReading();
  }
}

