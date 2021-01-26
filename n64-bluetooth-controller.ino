#include <N64Controller.h>

N64Controller player1 (12);

void setup() {
    player1.begin();
    Serial.begin(115200);
}

void loop() {
    delay(5000);
    player1.update();
    Serial.println("##########");
    player1.print_N64_status();
}
