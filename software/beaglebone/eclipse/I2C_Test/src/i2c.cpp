#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "i2c-dev.h"
#include <sys/ioctl.h>
#include <stdio.h>

#include "i2c.h"

const int INTERFACE_COUNT = 3;
const std::string DEVICE_FILE_NAMES[INTERFACE_COUNT] = { "/dev/i2c-0",
		"/dev/i2c-1", "/dev/i2c-2" };

I2cDevice::I2cDevice(I2C_INTERFACE Interface, int Address) :
		m_Interface(Interface), m_Address(Address) {

	init();
}
I2cDevice::I2cDevice(int Address) :
		m_Address(Address) {

	m_Interface = I2C_2;
	init();
}
I2cDevice::~I2cDevice() {
	close(m_DeviceFile);
}

bool I2cDevice::init() {
	m_DeviceFile = open(getDeviceFileName().c_str(), O_RDWR);
	if (m_DeviceFile < 0) {
		printf("Error: failed to open the I2C bus\n");
		return false;
	}
	if (ioctl(m_DeviceFile, I2C_SLAVE, m_Address) < 0) {
		printf("Error: I2C initialization failed\n");
		return false;
	}
	return true;
}

bool I2cDevice::probe() {
	uint8_t Id = readCommand();
	return (Id != 0xff);
}

bool I2cDevice::sendCommand(uint8_t cmd) {
	return (i2c_smbus_write_byte(m_DeviceFile, cmd) >= 0);
}

uint8_t I2cDevice::readCommand() {
	return i2c_smbus_read_byte(m_DeviceFile);
}

std::string I2cDevice::getDeviceFileName() {
	return DEVICE_FILE_NAMES[m_Interface];
}
