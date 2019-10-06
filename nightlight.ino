#include <CapacitiveSensor.h>
#include <FastLED.h>
#include <EEPROMex.h>

#define DEBUG       false

#define LIGHT_PIN   6
#define NUM_LEDS    4
CRGB leds[NUM_LEDS];

#define NUM_SENSORS 3
CapacitiveSensor sensors[] = {
  CapacitiveSensor(5, 0),
  CapacitiveSensor(5, 1),
  CapacitiveSensor(5, 2),
  };

int colours[] = {0, 0, 0};
bool dir[] = {false, false, false};
bool writeToRom = false;

void setup()
{
  if (DEBUG) {
    Serial.begin(9600);
  }

  FastLED.addLeds<WS2812, LIGHT_PIN, GRB>(leds, NUM_LEDS);

  loadEEPROM();
}

void loop()
{
  writeToRom = false;
  
  // Read each sensor.  If sensor is over 100, increase channel and write pin.
  for(int i = 0; i < NUM_LEDS; i++) {
    long total = sensors[i].capacitiveSensor(30);

    if (total > 100) {
      if (! dir[i]) { // Increasing dir
        writeToRom = true;
        colours[i] += 5;
        if (colours[i] == 255) {
          dir[i] = true;
        }
      } else { // Decreasing dir
        writeToRom = true;
        colours[i] -= 5;
        if (colours[i] == 0) {
          dir[i] = false;
        }
      }
    }
  }
  
  /* Write colour to string to leds */
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(colours[0], colours[1], colours[2]); // Create colour from RGB
  }
  FastLED.show();

  /* Only write to rom if a change occurs */
  if (writeToRom) {
    updateEEPROM();
  }

  if (DEBUG) {
    char tbs[32];
    sprintf(tbs, "%d\t%d\t%d\t%d",colours[0],colours[1],colours[2],colours[3]);
    Serial.println(tbs);
  }
  delay(50);
}

/* Load data from EEPROM */
void loadEEPROM()
{
  byte temp_dir;
  int address = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    colours[i] = EEPROM.readInt(i);            // Read colour
    dir[i] = EEPROM.readBit(NUM_SENSORS, i);   // Read direction
  }
}

/* Write data to EEPROM */
void updateEEPROM()
{
  for (int i = 0; i < NUM_SENSORS; i++) {
    EEPROM.updateInt(i, colours[i]);            // Write colour
    EEPROM.updateBit(NUM_SENSORS, i, dir[i]);   // Write direction
  }
}
