byte ledPins[] = {
  9, 8, 10, 7, 11, A0, A1, 12
};       // an array of pin numbers to which LEDs are attached

void setup() {
  Serial.begin(9600);
  byte thisPin;
  for (thisPin = 0; thisPin < 8; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
    digitalWrite(ledPins[thisPin], HIGH);
  }
  delay(750);
  for (thisPin = 0; thisPin < 8; thisPin++) {
    digitalWrite(ledPins[thisPin], LOW);
  }
}

void loop() {
  byte serialByte;
  int available = Serial.available();
  // this makes sure we get the last byte
  for (int curByte = 0; curByte < available; curByte++) {
    serialByte = Serial.read();
  }

  if (available > 0) {
    Serial.println();
    for (byte thisPin = 0; thisPin < 8; thisPin++) {
      if (serialByte >= 1<<(7-thisPin)) {
        digitalWrite(ledPins[thisPin], HIGH);
        /*Serial.print("Setting LED ");
        Serial.print(String(ledPins[thisPin]));
        Serial.println(" to HIGH");*/
        serialByte-=1<<(7-thisPin);
      } else {
        digitalWrite(ledPins[thisPin], LOW);
        /*Serial.print("Setting LED ");
        Serial.print(String(ledPins[thisPin]));
        Serial.println(" to LOW");*/
      }
    }
  }

  delay(10);
}
