#include <N64Controller.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8
#define CTRL_PIN 5

struct RfDataStruct
{
  bool btA = false;
  bool btB = false;
};

typedef struct RfDataStruct rfDataStruct;
rfDataStruct rfData;

const uint8_t pipes[][6] = {
  "oi"
};

RF24 radio(CE_PIN, CSN_PIN);
N64Controller ctrl (CTRL_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  
  ctrl.begin();
  
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(rfData));
  radio.openWritingPipe(pipes[0]);
  radio.stopListening();

  printf_begin();             // needed only once for printing details
  radio.printDetails();       // (smaller) function that prints raw register values
  radio.printPrettyDetails(); // (larger) function that prints human readable data
  delay(5000);
}

void loop() {
  ctrl.update();
  rfData.btA = ctrl.A();
  rfData.btB = ctrl.B();

  Serial.print("BT_A = ");
  Serial.print(rfData.btA);
  Serial.print(", BT_B = ");
  Serial.print(rfData.btB);
  Serial.println(" ");

  radio.write(&rfData, sizeof(rfData));

  delay(1000);
}
