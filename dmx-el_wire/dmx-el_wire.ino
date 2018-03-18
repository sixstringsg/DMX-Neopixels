#include <Conceptinetics.h>

// definitions for EL Wire Digital pin, number of DMX channels, and DMX start address

#define EL_PIN 7
#define DMX_SLAVE_CHANNELS 1
#define DMX_ADDRESS 1
//Configure RS485 module to be in slave mode with previously defined number of
//channels used. For more information see the Conceptinetics example sketch
DMX_Slave dmx_slave ( DMX_SLAVE_CHANNELS );

// declare that we're going to use the variable el_wire later on, and it's type
// is byte, or 0-255.
byte el_wire;

void setup() {
  // Enable DMX slave interface and start recording DMX data
  dmx_slave.enable ();

  // Set start address to 1, this is also the default setting
  // You can change this address at any time during the program
  dmx_slave.setStartAddress (DMX_ADDRESS);

  // Initialize the pin for EL Wire Relay!
  pinMode(EL_PIN, OUTPUT);
  // Write the pin LOW to start with so we don't turn the wire on when we don't
  // want to. This assumes a NO relay.
  digitalWrite(EL_PIN, LOW);
}

void loop () {
  // Read the value the console is sending us and store it as a byte named
  // el_wire
  el_wire = dmx_slave.getChannelValue (DMX_ADDRESS);

  // If the console sends a comand greater than 128/50% to the fifth channel,
  // then turn our EL wire on. Otherwise, leave it off. This assumes a NO relay,
  // can be changed to NC but I wouldn't recommend it for this purpose.
  if (el_wire >= 127) {
    digitalWrite(EL_PIN, HIGH);
  }
  else {
    digitalWrite(EL_PIN, LOW);
  }
}
