#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(11, 2, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, 3, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(54, 4, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(54, 5, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  delay(1000);
  Serial.begin(9600);
  /*strip2.begin();
  strip2.show();
  strip3.begin();
  strip3.show();
  strip4.begin();
  strip4.show();*/
}

void loop() {
  //playEffect(strip1);
  /*playEffect(strip2);
  playEffect(strip3);
  playEffect(strip4);*/
  Serial.println("Chase");
  theaterChase(strip1, strip1.Color(168, 79, 235), 50); // White
  delay(1000);
  Serial.println("Rainbow Chase");
  theaterChaseRainbow(strip1, 50);
  delay(1000);
  Serial.println("Green fill");
  fillStrip(strip1, strip1.Color(0, 255, 0));
  delay(1000);
  Serial.println("Red fill");
  fillStrip(strip1, strip1.Color(255, 0, 0));
  delay(1000);
  Serial.println("Off");
  fillStrip(strip1, strip1.Color(0, 0, 0));
  delay(1000);
}

void playEffect(Adafruit_NeoPixel strip) {
  // Some example procedures showing how to display to the pixels:
  Serial.println("Green wipe");
  colorWipe(strip, strip.Color(0, 255, 0), 50); // Green
  delay(1000);
  Serial.println("Blue wipe");
  colorWipe(strip, strip.Color(0, 0, 255), 50); // Blue
  delay(1000);
  Serial.println("Red wipe");
  colorWipe(strip, strip.Color(255, 0, 0), 50); // Red
  delay(1000);
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  Serial.println("White chase");
  theaterChase(strip, strip.Color(127, 127, 127), 50); // White
  Serial.println("Red chase");
  theaterChase(strip, strip.Color(127, 0, 0), 50); // Red
  Serial.println("Blue chase");
  theaterChase(strip, strip.Color(0, 0, 127), 50); // Blue

  Serial.println("Rainbow");
  rainbow(strip, 20);
  Serial.println("Rainbow cycle");
  rainbowCycle(strip, 20);
  Serial.println("Rainbow chase");
  theaterChaseRainbow(strip, 50);
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void fillStrip(Adafruit_NeoPixel strip, uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

void rainbow(Adafruit_NeoPixel strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel strip, uint32_t c, uint8_t wait) {
  for (int j=0; j<30; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel strip, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel(strip,  (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel strip, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
