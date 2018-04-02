#include <Conceptinetics.h>
#include <Adafruit_NeoPixel.h>
// definitions for EL Wire Digital pin, number of DMX channels, and DMX start address

#define NEOPIXEL_PIN 6
#define DMX_SLAVE_CHANNELS 1
#define DMX_ADDRESS 361
#define STRING_LENGTH 60
//Configure RS485 module to be in slave mode with previously defined number of
//channels used. For more information see the Conceptinetics example sketch
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// declare that we're going to use the variable el_wire later on, and it's type
// is byte, or 0-255.
byte el_wire;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRING_LENGTH, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // Enable DMX slave interface and start recording DMX data
  dmx_slave.enable ();

  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (DMX_ADDRESS);

  // Initialize the pin for EL Wire Relay!
  pinMode(NEOPIXEL_PIN, OUTPUT);
}

void loop () {
  // Read the value the console is sending us and store it as a byte named
  // el_wire
  el_wire = dmx_slave.getChannelValue (DMX_ADDRESS);

  // If the console sends a comand greater than 128/50% to the fifth channel,
  // then turn our EL wire on. Otherwise, leave it off. This assumes a NO relay,
  // can be changed to NC but I wouldn't recommend it for this purpose.
  if (el_wire >= 127) {
    rainbow(20);
  }
  else {
    rainbowCycle(20);
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
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
