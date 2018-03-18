/*
  Example sketch for controlling a neopixel strip via DMX. This will use up
  either 4x channels if not using any animations, or 5x channels if animations
  are used.
  DMX library is from here: https://sourceforge.net/projects/dmxlibraryforar/?source=typ_redirect
  NeoPixel library is here: https://github.com/adafruit/Adafruit_NeoPixel
  A lot more information about the NeoPixel library can be found here:
  https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use

  On the CQRobot shield, Jumper #2 needs to be set to Slave
  TX & RX pins need to be set to UART when the device is connected to the
  DMX512 network, or removed or placed on IO when uploading code to the Uno.

  Digital pin to use for the Neopixel Signal line is set with NEOPIXEL_PIN

  This sketch takes advantage of the fact that both DMX and the NeoPixel library
  use a "byte" of data, or a value from 0-255. This means that we can directly
  take the value that we get from the DMX network and write it to Red, Green,
  and blue. For more information about variables, see:
  https://www.arduino.cc/en/Reference/VariableDeclaration
*/

//----------------- WARNING FROM ADAFRUIT --------------------------------------
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

#include <Adafruit_NeoPixel.h>
#include <Conceptinetics.h>

#define NEOPIXEL_PIN 6
#define STRING_LENGTH 16

#define EL_PIN 7

//For now we're just controlling Red, Green, Blue, and Intensity so only need
//4x DMX channels. First channel is Red, second is Green, third is blue, and
//fourth is intensity.
// And now add a fifth for EL Wire!
#define DMX_SLAVE_CHANNELS 5

// Set here the DMX address of the "fixture". Can be anything 1-512
#define DMX_ADDRESS 1

//This is how we tell the Neopixel library information about the strip. For more
//information look at the Adafruit strandtest example.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRING_LENGTH, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

//Configure RS485 module to be in slave mode with previously defined number of
//channels used. For more information see the Conceptinetics example sketch
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// declare the Red, Green, Blue, and intensity bytes. If you want to initialize
// the strip to a color before DMX takes control, set it here, otherwise it will
// initialize to it's last known color/intensity.
byte red;
byte blue;
byte green;
byte intensity;
byte el_wire;

void setup() {
  // Enable DMX slave interface and start recording DMX data
  dmx_slave.enable ();

  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (DMX_ADDRESS);

  // Initialize the NeoPixels.
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initialize the pin for EL Wire Relay!
  pinMode(EL_PIN, OUTPUT);
  // Write the pin LOW to start with so we don't turn the wire on when we don't
  // want to. This assumes a NO relay.
  digitalWrite(EL_PIN, LOW);
}

void loop() {
  //since Red is our first channel, read the value of that and store it for
  //later.
  red = dmx_slave.getChannelValue (DMX_ADDRESS);
  //Green is our second, so take the start channel and add 1 to it. Same for
  //blue and intensity.
  green = dmx_slave.getChannelValue (DMX_ADDRESS + 1);
  blue = dmx_slave.getChannelValue (DMX_ADDRESS + 2);
  intensity = dmx_slave.getChannelValue (DMX_ADDRESS + 3);
  el_wire = dmx_slave.getChannelValue (DMX_ADDRESS + 4);

  // This combines all three colors into one 32-bit integer to pass to
  // setPixelColor later. It's a function built into the Adafruit library
  // that makes the next part easier.
  uint32_t color = strip.Color(red, green, blue);

  // Since each pixel is individually adressable, we have to tell each what they
  // need to be. This for loop essentially says "for every pixel from #0 all the
  // way to strip length, set the color to the value we calculated earlier".
  // NeoPixels don't live update their color when you call setPixelColor, so this
  // allows us to set the color of each pixel without seeing a stairstep effect
  // as they each update one by one (even if it would be stupid fast).
  for( int i = 0; i<STRING_LENGTH; i++){
    strip.setPixelColor(i, color);
  }


  // this configures intensity of the ENTIRE strip. It is generally not recommended
  // to call this anywhere but void setup() but we're trying to get the most
  // control possible. If the strip appears to flicker, comment this line out
  // and it can be called once during setup() if max intensity is too much.
  strip.setBrightness(intensity);

  // strip.show takes all of the previous commands we've sent to the library
  // and actually updates the color on the strip.
  strip.show();
  // If the console sends a comand greater than 128/50% to the fifth channel,
  // then turn our EL wire on. Otherwise, leave it off. This assumes a NO relay,
  // can be changed to NC but I wouldn't recommend it for this purpose.
  if (el_wire >= 125) {
    digitalWrite(EL_PIN, HIGH);
  }
  else {
    digitalWrite(EL_PIN, LOW);
  }
}
