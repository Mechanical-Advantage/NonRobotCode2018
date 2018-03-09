#define SET_BITS 2
#define DATA_MASK 0x3f // Should have data bits set, others 0, must match SET_BITS

byte ledPins[] = {
  2, 3, 4, 6, 5, 7, 8, 9, 10, 11, 12, A5, A4, A3, A2, A1, A0
};       // an array of pin numbers to which LEDs are attached

void setup() {
  Serial.begin(9600);
  byte thisPin;
  for (thisPin = 0; thisPin < sizeof(ledPins); thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
    digitalWrite(ledPins[thisPin], HIGH);
  }
  delay(750);
  for (thisPin = 0; thisPin < sizeof(ledPins); thisPin++) {
    digitalWrite(ledPins[thisPin], LOW);
  }
}

void loop() {
  byte serialByte;
  int available = Serial.available();
  serialByte = Serial.read();

  if (available > 0) {
    //Serial.println();
    // Get the set index
    byte setIndex = serialByte>>(8-SET_BITS);
    // Clear set index from byte
    serialByte &= DATA_MASK;
    for (byte thisPin = 0; thisPin < 8-SET_BITS; thisPin++) {
      byte ledIndex = thisPin+((8-SET_BITS)*setIndex);
      if (ledIndex < sizeof(ledPins)) {
        if (serialByte >= 1<<(7-SET_BITS-thisPin)) {
          digitalWrite(ledPins[ledIndex], HIGH);
          /*Serial.print("Setting LED ");
          Serial.print(String(ledPins[ledIndex]));
          Serial.println(" to HIGH");*/
          serialByte-=1<<(7-SET_BITS-thisPin);
        } else {
          digitalWrite(ledPins[ledIndex], LOW);
          /*Serial.print("Setting LED ");
          Serial.print(String(ledPins[ledIndex]));
          Serial.println(" to LOW");*/
        }
      }
    }
  }

  delay(10);
}
