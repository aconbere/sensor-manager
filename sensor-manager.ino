#include <Wire.h>

#define ADDRESS 0x22

#define COMMAND_VOLTS            0x01
#define COMMAND_FIRMWARE_VERSION 0x97

#define RESPONSE_VOLTS            0x01
#define RESPONSE_ERROR            0x99
#define RESPONSE_FIRMWARE_VERSION 0x98

struct Log {
  uint8_t index;
  uint16_t entries[256];
};

uint8_t Log_Append(struct Log *log, long value) {
  uint8_t o_index = log->index;
  log->entries[o_index] = value;
  log->index = o_index + 1;
  return o_index;
}

int readResult(byte buf[3]) {
  int i = 0;

  while (Wire.available()) {
    buf[i] = Wire.read();
    i++;
  }

  if (i < 2) {
    return -1;
  }

  return 1;
}

struct Log VOLTS;
 
void setup() {
  Wire.begin();
 
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Sensor Manager");
}

void handleResponse(struct Log *log, byte buf[]) {
  uint16_t low;
  uint16_t high;
  uint16_t value;

  switch (buf[0]) {
  case RESPONSE_VOLTS:
    low = buf[1];
    high = buf[2];
    value = high << 8 | low;

    Serial.print("Volts: ");
    Serial.print(value, HEX);
    Serial.print("\n");

    Log_Append(log, value);
    break;
  case RESPONSE_ERROR:
    low = buf[1];
    high = buf[2];
    value = high << 8 | low;

    Serial.print("Received Error: (");
    Serial.print(buf[0], HEX);
    Serial.print(", ");
    Serial.print(buf[1], HEX);
    Serial.print(", ");
    Serial.print(buf[2], HEX);
    Serial.print(")\n");
    break;
  default:
    Serial.print("Received ???: (");
    Serial.print(buf[0], HEX);
    Serial.print(", ");
    Serial.print(buf[1], HEX);
    Serial.print(", ");
    Serial.print(buf[2], HEX);
    Serial.print(")\n");
    break;
  }

}

void loop() {
  byte buf[] = {0,0,0};
  int resp = -1;
  
  Wire.beginTransmission(ADDRESS);
  Wire.write(COMMAND_VOLTS);

  Wire.requestFrom(ADDRESS, 3);

  resp = readResult(buf);

  if (resp < 0) {
    Serial.println("Error reading: too few bytes");
    return;
  }

  handleResponse(&VOLTS, buf);
  Wire.endTransmission();
  delay(1000);
}
