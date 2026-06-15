#include <serial_port_iteration.h>

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
  for (uint8_t i = 0; i < io.get_num_ports(); i++) {
    if (io.newdata(i)) {
      char c = io.get_char(i);

      Serial.print(F("PORT "));
      Serial.print(i);
      Serial.print(F(" RX: "));

      if (c == CHAR_CR) {
        Serial.println(F("<CR>"));
      } else if (c == CHAR_NL) {
        Serial.println(F("<NL>"));
      } else {
        Serial.println(c);
      }
    }
  }
}
