#include <stdint.h>
#include <string>

typedef enum {
	I2C_0 = 0,
	I2C_1 = 1,
	I2C_2 = 2,
} I2C_INTERFACE;

class I2cDevice {
	// types
	public:
		
	// public functions
	public:
		// con-/destructor
		I2cDevice(I2C_INTERFACE Interface, int Address);
		I2cDevice(int Address);
		~I2cDevice();
		
		bool probe();
		bool sendCommand(uint8_t cmd);
		uint8_t readCommand();
		// TODO: send Data
		
	// members
	private:
		int m_DeviceFile;
		I2C_INTERFACE m_Interface;
		int m_Address;
		
	// private functions
	private:
		bool init();
		std::string getDeviceFileName();
};
