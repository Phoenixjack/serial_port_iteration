# serial_port_iteration

An Arduino helper/example for initializing and iterating across multiple `HardwareSerial` ports.

This project provides a small wrapper around multiple hardware serial ports so code can reference serial channels by index instead of manually repeating setup and print/read logic for `Serial`, `Serial1`, `Serial2`, and `Serial3`.

It was written for Arduino Mega-style boards or other Arduino-compatible boards with multiple hardware UARTs.

## Project Status

Prototype / utility code.

This repo is intended as a lightweight helper and example for multi-port serial projects. It is not a full serial protocol framework.

## Target Hardware

This project is primarily intended for:

* Arduino Mega-style boards
* boards with multiple `HardwareSerial` ports
* projects that need to initialize, read from, or write to multiple UARTs

The helper attempts to detect common hardware serial objects:

```cpp
Serial
Serial1
Serial2
Serial3
```

Availability depends on the selected Arduino board/core.

## What It Does

The helper can:

* store pointers to multiple hardware serial ports
* initialize serial ports by index
* initialize ports at a default or user-specified baud rate
* print to a selected serial port by index
* check whether a selected port has available data
* read a character from a selected port
* read strings or strings terminated by carriage return / newline
* optionally display boot messages per port
* expose simple fault/status information

## Basic Use

Include the helper:

```cpp
#include "serial_port_iteration.h"
```

Create a port manager:

```cpp
ports io(2);
```

Initialize each configured port:

```cpp
void setup() {
  for (uint8_t i = 0; i < io.get_num_ports(); i++) {
    io.init(i, 115200);
  }
}
```

Print to each configured port:

```cpp
for (uint8_t i = 0; i < io.get_num_ports(); i++) {
  io.print(i, "hello on port ");
  io.print(i, String(i, DEC));
  io.print(i, "\n");
}
```

Check for incoming data:

```cpp
for (uint8_t i = 0; i < io.get_num_ports(); i++) {
  if (io.newdata(i)) {
    char c = io.get_char(i);
    // process character
  }
}
```

## Example Sketch

The included sketch:

1. creates a `ports` object
2. initializes the configured ports
3. prints a startup message
4. sends a hello message to each configured port
5. prints boot-message and fault status over `Serial`

The example loop polls each configured serial port once per pass and reads at most one character from any port before moving on. This keeps the sketch from blocking or spending too long on one busy UART.

## Relationship to `serial_buffer`

This repo is related to, but different from, [`serial_buffer`](https://github.com/Phoenixjack/serial_buffer).

* `serial_port_iteration` focuses on selecting and iterating across multiple serial ports.
* `serial_buffer` focuses on buffering incoming serial data until a complete message is ready to process.

They could be combined in a future project, but they are currently separate helpers.

## Suggested Arduino IDE Layout

The Arduino IDE expects the sketch folder and main `.ino` file to share the same name.

Recommended layout:

```text
serial_port_iteration/
├── serial_port_iteration.ino
├── serial_port_iteration.h
├── README.md
└── LICENSE
```

## Known Limitations

* Prototype utility code
* Best suited for boards with multiple hardware UARTs
* Not all boards support `Serial1`, `Serial2`, or `Serial3`
* Uses Arduino `String`
* No complete message buffering by itself
* `loop()` example is currently empty
* Does not provide port name aliases
* No formal protocol handling
* No warranty or support commitment

## Possible Future Improvements

Possible future improvements:

* add clearer examples for reading from multiple ports
* add examples combining this helper with `serial_buffer`
* add port alias support
* add configurable per-port baud rates during construction
* add safer bounds checking for unavailable port pointers
* add optional SoftwareSerial support
* add non-blocking multi-port polling example
* add callback-based receive handling
* split implementation into `.h` and `.cpp` if the helper grows

## License

This project is released under the MIT License.

You are free to use, modify, and adapt it for your own projects. No warranty is provided, and no ongoing support or maintenance is implied.
