#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "i2c.h"

#define I2C_FILE "/dev/i2c-1"

int i2c_bus;

bool i2cInit(int i2cAddress) {
	
	i2c_bus = open(I2C_FILE, O_RDWR);
	if (i2c_bus < 0) {
		printf("Error: failed to open the I2C bus\nMake sure the DTO is loaded: echo BB-I2C1 > /sys/devices/platform/bone_capemgr/slots");
		return false;
	}
	
	return (ioctl(i2c_bus, I2C_SLAVE, i2cAddress) >= 0);
}

bool i2cSendCommand(uint8_t command) {
	return (i2c_smbus_write_byte(i2c_bus, command) >= 0);
}