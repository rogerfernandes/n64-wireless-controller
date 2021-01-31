#include <N64Controller.h>
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 7
#define CSN_PIN 8
#define CTRL_PIN 5
#define pipeSizeArray 5

struct RfDataStruct {
  bool dUp = false;
  bool dDown = false;
  bool dLeft = false;
  bool dRight = false;
  bool start = false;
  bool a = false;
  bool b = false;
  bool z = false;
  bool l = false;
  bool r = false;
  bool cUp = false;
  bool cDown = false;
  bool cLeft = false;
  bool cRight = false;
  int axisX = 0;
  int axisY = 0;
};

typedef struct RfDataStruct rfDataStruct;
rfDataStruct rfData;
rfDataStruct prevRfData;

const uint64_t pipes[pipeSizeArray] = {
  0xE14BC8F482,
  0xE8E8F0F0E1,
  0xE8E8F0F0E2,
  0xF0F0F0F0E1,
  0xF0F0F0F0D2,
};

bool DEBUG = true;

RF24 radio(CE_PIN, CSN_PIN);
N64Controller controller (CTRL_PIN);

void setup() {
  serialSetup();
  controllerSetup();
  radioSetup();
  debug();
}

void loop() {
  populateRfData();

  if(isValuesChanged()){
    printRfData();
    radioSendData();
  }
  delay(10);
}

void serialSetup(){
  if(DEBUG){
    Serial.begin(115200);
    while (!Serial) {}
  }
}

void controllerSetup(){
  controller.begin();
}

void radioSetup(){
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!!"));
    while (1) {}
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(sizeof(rfData));
  radio.openWritingPipe(pipes[0]);
}

void debug(){
  if(DEBUG){
    printf_begin();
    radio.printPrettyDetails();
    Serial.println("---------------------------------------------");
    delay(5000);
  }
}

void populateRfData(){
  controller.update();
  rfData.dUp = controller.D_up();
  rfData.dDown = controller.D_down();
  rfData.dLeft = controller.D_left();
  rfData.dRight = controller.D_right();
  rfData.start = controller.Start();
  rfData.a = controller.A();
  rfData.b = controller.B();
  rfData.z = controller.Z();
  rfData.l = controller.L();
  rfData.r = controller.R();
  rfData.cUp = controller.C_up();
  rfData.cDown = controller.C_down();
  rfData.cLeft = controller.C_left();
  rfData.cRight = controller.C_right();
  rfData.axisX = controller.axis_x();
  rfData.axisY = controller.axis_y();
}

bool isValuesChanged(){
  bool changed = true;
  if(  prevRfData.dUp == rfData.dUp
    && prevRfData.dDown == rfData.dDown
    && prevRfData.dLeft == rfData.dLeft
    && prevRfData.dRight == rfData.dRight
    && prevRfData.start == rfData.start
    && prevRfData.a == rfData.a
    && prevRfData.b == rfData.b
    && prevRfData.z == rfData.z
    && prevRfData.l == rfData.l
    && prevRfData.r == rfData.r
    && prevRfData.cUp == rfData.cUp
    && prevRfData.cDown == rfData.cDown
    && prevRfData.cLeft == rfData.cLeft
    && prevRfData.cRight == rfData.cRight
    && prevRfData.axisX == rfData.axisX
    && prevRfData.axisY == rfData.axisY
    ){
      changed = false;
  }

  if(changed){
    prevRfData = rfData;
  }

  return changed;
}

void printRfData(){
  if(DEBUG){
    Serial.print("dUp:    ");
    Serial.println(rfData.dUp);
    Serial.print("dDown:  ");
    Serial.println(rfData.dDown);
    Serial.print("dLeft:  ");
    Serial.println(rfData.dLeft);
    Serial.print("dRight: ");
    Serial.println(rfData.dRight);
    Serial.print("start:  ");
    Serial.println(rfData.start);
    Serial.print("a:      ");
    Serial.println(rfData.a);
    Serial.print("b:      ");
    Serial.println(rfData.b);
    Serial.print("z:      ");
    Serial.println(rfData.z);
    Serial.print("l:      ");
    Serial.println(rfData.l);
    Serial.print("r:      ");
    Serial.println(rfData.r);
    Serial.print("cUp:    ");
    Serial.println(rfData.cUp);
    Serial.print("cDown:  ");
    Serial.println(rfData.cDown);
    Serial.print("cLeft:  ");
    Serial.println(rfData.cLeft);
    Serial.print("cRight: ");
    Serial.println(rfData.cRight);
    Serial.print("axisX:  ");
    Serial.println(rfData.axisX);
    Serial.print("axisY:  ");
    Serial.println(rfData.axisY);
    Serial.println("------------------");
  }  
}

void radioSendData(){
  radio.write(&rfData, sizeof(rfData));
}
