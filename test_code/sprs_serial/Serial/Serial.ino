#include <HardwareSerial.h>


String pico_meessage;

void setup() {
  Serial.begin(1000000);
  Serial2.begin(31250);

}

void loop() {

  if (Serial2.available() > 0){
    String pico_message = Serial2.readStringUntil('\n'); 
    Serial.println(pico_message);
  }
  else{
    Serial.println("no print");
  }

  Serial2.println("Hello, pico");
  Serial2.flush();
  // Serial2.write("Hello, picoW\n");
  // Serial2.flush();
  // どっちでもOK
  Serial.println("end");
  Serial2.flush();
  delay(1000);
}
