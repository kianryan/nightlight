#include <CapacitiveSensor.h>
#include <EEPROM.h>
 
int light_pins[] = {8, 7, 6, 5};
CapacitiveSensor sensors[] = {
  CapacitiveSensor(A4, A0), 
  CapacitiveSensor(A4, A1), 
  CapacitiveSensor(A4, A2), 
  CapacitiveSensor(A4, A3) };

int colours[] = {0, 0, 0, 0};
bool dir[] = {false, false, false, false};

void setup()                    
{
  //Serial.begin(9600);

  loadEEPROM();

  // Init LED lights
  for (int i=0; i < 5; i++) {
    pinMode(light_pins[i], OUTPUT);
    analogWrite(light_pins[i], colours[i]);
  }
}

void loop()                    
{
  // Read each sensor.  If sensor is over 100, increase channel and write pin.
  for(int i = 0; i < 4; i++) {
    long total = sensors[i].capacitiveSensor(30);
    if (total > 100) {
      if (! dir[i]) { // Increasing dir
        colours[i] += 5;
        if (colours[i] == 255) {
          dir[i] = true;  
        }  
      } else { // Decreasing dir
        colours[i] -= 5;
        if (colours[i] == 0) {
          dir[i] = false;  
        } 
      }
      
      analogWrite(light_pins[i], colours[i]); 
    }
  }

  updateEEPROM();

  // char tbs[32];
  // sprintf(tbs, "%d\t%d\t%d\t%d",colours[0],colours[1],colours[2],colours[3]);
  //Serial.println(tbs);
  delay(50);
}

/* Load data from EEPROM */
void loadEEPROM()
{
  int address = 0;
  for (int i = 0; i < 4; i++) {
    colours[i] = (int)EEPROM.read(i);
    dir[i] = (bool)EEPROM.read(i+4); 
  }
}

/* Write data to EEPROM */
void updateEEPROM()
{
  for (int i = 0; i < 4; i++) {
    EEPROM.update(i, colours[i]);
    EEPROM.update(i+4, dir[i]);  
  }
}

