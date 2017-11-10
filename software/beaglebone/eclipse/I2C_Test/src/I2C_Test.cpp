#include <iostream>

#include "i2c.h"

int main() {
	I2cDevice Mcu = I2cDevice(I2C_2, 0x44);
	bool Connected = Mcu.probe();
	std::cout << "Connected: " << Connected << std::endl;
	uint8_t Cmd = Mcu.readCommand();
	std::cout << "Receive command: " << static_cast<int>(Cmd) << std::endl;
	return 0;
}
