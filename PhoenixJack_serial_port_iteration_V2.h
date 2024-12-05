/*
Call directly through ports(number_of_ports);
ONLY designed for Arduino MEGAs or others with 4 HARDWARE Serial ports.
THEORETICALLY, the user could create a Software Serial port and assign a pointer to that virtual port to ports.serialPorts[portnum]
Reference by channel number.
No builtin portname aliasing. Do it yourself.
Check newdataregister and rcvtime[] array. get_char(portnum) DOES NOT set the newdataregister
DEFINES/CONSTANTS: CHAR_NL, CHAR_CR, NUM_PORTS
*/

#include "Arduino.h"
#include <stdint.h>
#ifndef CHAR_NL
#define CHAR_NL (char)10
#endif
#ifndef CHAR_CR
#define CHAR_CR (char)13
#endif
enum ports_error_level {    // list of possible error codes
  PORTS_NO_ERROR,           // no error, last operation executed with no issues
  PORTS_EXCEEDS_MAX_PORTS,  //
  PORTS_INVALID_PORT,       //
  PORTS_USER_IS_STUPID,     // how else do you want me to say it? i don't speak no other languages
  PORTS_ERRONEOUS_ERROR,    // because i feel like being a smartass
  PORTS_EROGENOUS_ERROR     // this error makes me horny. YEAH BABY!
};

const uint8_t MAX_PORTS = 6;
class ports {
public:
  explicit ports(uint8_t user_requested_ports);
  HardwareSerial* serialPorts[MAX_PORTS] = { &Serial, nullptr, nullptr, nullptr, nullptr, nullptr };
  void detectAvailablePorts();
  bool bootmessage[MAX_PORTS] = { true, false, false, false };
  void init(uint8_t index);
  void init(uint8_t index, unsigned long baud);
  bool newdata(uint8_t index);
  char end_of_msg = CHAR_CR;  // defaults to carriage return, but can be redefined
  void flush_buffer(uint8_t index);
  char get_char(uint8_t index);
  String getbuffer(uint8_t index);
  String getbufferCR(uint8_t index);
  String getbufferNL(uint8_t index);
  void print(uint8_t index, String* data);
  void print(uint8_t index, String data);
  uint8_t get_num_ports();
  uint8_t get_fault_code();
  uint8_t get_data_register();
  unsigned long get_rcv_time(uint8_t* index);
protected:
  unsigned long rcvtime[MAX_PORTS];      // will hold the millis() of the last received message
  uint8_t newdataregister = 0b00000000;  // will have the corresponding bit SET if new data was received
  uint8_t faultcode = PORTS_NO_ERROR;    // register to return a fault code
  uint8_t num_ports = 1;                 // trying to make the system scalable to multiple platforms, this will cap the max number of addressable ports
private:
  bool validindex(uint8_t* index);
  void dispbootmessage(uint8_t* index);
  void marktime(uint8_t* index);
  void flag_set(uint8_t* targetbit);
  void flag_clear(uint8_t* targetbit);
  const String error_out_of_bounds = "index out of bounds";
};

ports::ports(uint8_t user_requested_ports) {  // TODO: have detectAvailablePorts return a value and use the LOWER value: user input OR returned value
  if (user_requested_ports < 1) {
    num_ports = 1;                     // user did a stupid. fix it.
    faultcode = PORTS_USER_IS_STUPID;  // tell the stupid user that the user is stupid
  } else if (user_requested_ports > MAX_PORTS) {
    num_ports = MAX_PORTS;
    faultcode = PORTS_EXCEEDS_MAX_PORTS;
  } else {
    num_ports = user_requested_ports;
  }
  detectAvailablePorts();
};
void ports::detectAvailablePorts() {  // Assuming MAX_PORTS is 4
  serialPorts[0] = &Serial;
#ifdef Serial1
  serialPorts[1] = &Serial1;
#endif
#ifdef Serial2
  serialPorts[2] = &Serial2;
#endif
#ifdef Serial3
  serialPorts[3] = &Serial3;
#endif
}
bool ports::validindex(uint8_t* index) {
  if (*index < MAX_PORTS) {
    return true;
  } else {
    return false;
  }
}
void ports::init(uint8_t index) {  // defaults to 115200
  if (validindex(&index)) {
    serialPorts[index]->begin(115200);
    dispbootmessage(&index);
  }
};
void ports::init(uint8_t index, unsigned long baud) {
  if (validindex(&index)) {
    serialPorts[index]->begin(baud);
    dispbootmessage(&index);
  }
};
void ports::dispbootmessage(uint8_t* index) {  // will only act if boolean is true
  if (bootmessage[*index]) {
    serialPorts[*index]->println(F("\n\n\n\nBOOTED!"));
    serialPorts[*index]->print(F("COMPILED: "));
    serialPorts[*index]->print(__DATE__);
    serialPorts[*index]->print(F(" "));
    serialPorts[*index]->print(__TIME__);
  }
};
bool ports::newdata(uint8_t index) {
  if (validindex(&index)) {
    return serialPorts[index]->available() > 0;
  } else {
    return false;
  }
};
void ports::flush_buffer(uint8_t index) {
  serialPorts[index]->flush();
};
char ports::get_char(uint8_t index) {
  if (validindex(&index)) {
    return serialPorts[index]->read();
  }
  return CHAR_CR;
};
String ports::getbuffer(uint8_t index) {
  if (validindex(&index)) {
    return serialPorts[index]->readString();
  }
  return error_out_of_bounds;
};
String ports::getbufferCR(uint8_t index) {
  String returnval;
  if (validindex(&index)) {
    returnval = serialPorts[index]->readStringUntil(CHAR_CR);
    serialPorts[index]->flush();
  } else {
    returnval = error_out_of_bounds;
  }
  return returnval;
};
String ports::getbufferNL(uint8_t index) {
  String returnval;
  if (validindex(&index)) {
    returnval = serialPorts[index]->readStringUntil(CHAR_NL);
    serialPorts[index]->flush();
  } else {
    returnval = error_out_of_bounds;
  }
  return returnval;
};
void ports::print(uint8_t index, String* data) {
  if (validindex(&index)) {
    serialPorts[index]->print(*data);
  }
};
void ports::print(uint8_t index, String data) {
  if (validindex(&index)) {
    serialPorts[index]->print(data);
  }
};
void ports::marktime(uint8_t* index) {
  rcvtime[*index] = millis();
};
void ports::flag_set(uint8_t* targetbit) {
  bitSet(newdataregister, *targetbit);
};
void ports::flag_clear(uint8_t* targetbit) {
  bitClear(newdataregister, *targetbit);
};
uint8_t ports::get_num_ports() {
  return num_ports;
};
uint8_t ports::get_fault_code() {
  return faultcode;
};
uint8_t ports::get_data_register() {
  return newdataregister;
};
unsigned long ports::get_rcv_time(uint8_t* index) {
  if (validindex(index)) {
    return rcvtime[*index];
  } else {
    return 0;
  }
};