#include <iostream>
#include <unistd.h>

#include "I2cDevice.h"

int main() {
	I2cDevice Mcu1 = I2cDevice(I2C_2, 0x40);
	I2cDevice Mcu2 = I2cDevice(I2C_2, 0x44);
	//bool Connected = Mcu.probe();
	//std::cout << "Connected: " << Connected << std::endl;
	//uint8_t Cmd = Mcu.readCommand();
	//std::cout << "Receive command: " << static_cast<int>(Cmd) << std::endl;
	int i = 1000000;
	while(true) {
		Mcu1.sendCommand(0);
		Mcu2.sendCommand(1);
		usleep(i);
		Mcu1.sendCommand(1);
		Mcu2.sendCommand(0);
		usleep(i);
		i-=100000;
		if(i==0){
			i=1000000;
		}
	}
	return 0;
}
