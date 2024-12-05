#include <PhoenixJack_serial_port_iteration_V2.h>

ports io(2);

void setup() {
  io.bootmessage[0] = true;
  delay(500);
  for (uint8_t i = 0; i < io.get_num_ports(); i++) {
    io.init(i);
  }
  Serial.println("\n\nINIT COMPLETE!");
  delay(500);
  for (uint8_t i = 0; i < io.get_num_ports(); i++) {
    io.print(i, F("hello on port"));
    io.print(i, String(i, DEC));
    io.print(i, F("\n"));
  }
  Serial.println(F("\nBOOT MESSAGE ENABLED:"));
  for (uint8_t i = 0; i < io.get_num_ports(); i++) {
    Serial.print(F("\nport"));
    Serial.print(i);
    Serial.print(F(": "));
    Serial.print((io.bootmessage[i]) ? F("true") : F("false"));
    Serial.print(F("\tFAULT: "));
    Serial.print((io.get_fault_code()) ? F("true") : F("false"));
  }
  Serial.println(F("\n"));
}

void loop() {
}
