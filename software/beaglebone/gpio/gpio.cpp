#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include "gpio.h"

// GPIO names
const std::string PinNames[] = {
	"P8_3", "P8_4", "P8_5", "P8_6", "P8_7", "P8_8", "P8_9", "P8_10", "P8_11", "P8_12", "P8_13", "P8_14", "P8_15", "P8_16", "P8_17", "P8_18", "P8_19", "P8_20", "P8_21", "P8_22", "P8_23", "P8_24", "P8_25", "P8_26", "P8_27", "P8_28", "P8_29", "P8_30", "P8_31", "P8_32", "P8_33", "P8_34", "P8_35", "P8_36", "P8_37", "P8_38", "P8_39", "P8_40", "P8_41", "P8_42", "P8_43", "P8_44", "P8_45", "P8_46", "P9_11", "P9_12", "P9_13", "P9_14", "P9_15", "P9_16", "P9_17", "P9_18", "P9_19", "P9_20", "P9_21", "P9_22", "P9_23", "P9_24", "P9_25", "P9_26", "P9_27", "P9_28", "P9_29", "P9_30", "P9_31"
};

// GPIO linux device number
const unsigned int PinNumbers[] = {
	38, 39, 34, 35, 66, 67, 69, 68, 45, 44, 23, 26, 47, 46, 27, 65, 22, 63, 62, 37, 36, 33, 32, 61, 86, 88, 87, 89, 10, 11, 9, 81, 8, 80, 78, 79, 76, 77, 74, 75, 72, 73, 70, 71, 30, 60, 31, 50, 48, 51, 5, 4, 13, 12, 3, 2, 49, 15, 117, 14, 115, 113, 111, 112, 110
};

// files
const std::string GPIO_PATH = "/sys/class/gpio/";	// path to gpio device files
const unsigned int MIN_BUTTON_TIME = 100000;		// debounce delay (in µs)


Gpio::Gpio(BB_PIN Pin, DIRECTION Dir) : m_Pin(Pin), m_Dir(Dir) {
	// TODO: throw exceptions, if failed
	enable();
	setDir();
	if(m_Dir == OUT) {
		setState(LOW);
	} else {
		m_State = UNKNOWN;
	}
}
Gpio::Gpio(BB_PIN Pin, DIRECTION Dir, Gpio::STATE State) : m_Pin(Pin), m_Dir(Dir) {
	// TODO: throw exceptions, if failed
	enable();
	setDir();
	setState(State);
}
Gpio::~Gpio() {
	disable();
}

// pin utils
std::string Gpio::pinName() const {
	return PinNames[m_Pin];
}
unsigned int Gpio::pinNumber() const {
	return PinNumbers[m_Pin];
}
BB_PIN Gpio::pin() const {
	return m_Pin;
}

// GPIO utils
bool Gpio::setState(Gpio::STATE State) {
	// check if GPIO is configured as input
	if(m_Dir == IN) {
		std::cout << "Error: this GPIO is configured as input" << std::endl;
		return false;
	}
	
	// open GPIO value file
	// TODO: implement with C++ file I/O
	int fd = open(getValueFileName().c_str(), O_WRONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO value file" << std::endl;
		return false;
	}
	
	// set value
	int res;
	if(State == HIGH) {
		res = write(fd, "1", 2);
	} else if (State == LOW) {
		res = write(fd, "0", 2);
	} else {
		std::cout << "Error: unknown state" << std::endl;
		return false;
	}
	if (res < 0) {
		std::cout << "Error: failed to set GPIO value" << std::endl;
		return false;
	}
	m_State = State;
	
	// close file
	close(fd);
	
	return true;
}

Gpio::STATE Gpio::getState() {
	// check if GPIO is configured as ouput
	if(m_Dir == OUT) {
		std::cout << "Warning: read state on GPIO configured as output" << std::endl;
	}
	
	// open GPIO value file
	// TODO: implement with C++ file I/O
	int fd = open(getValueFileName().c_str(), O_RDONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO value file" << std::endl;
		return UNKNOWN;
	}
	
	// read value
	char buf[2] = "";
	int res = read(fd, &buf, 1);
	if (res < 0) {
		std::cout << "Error: failed to read GPIO value" << std::endl;
		return UNKNOWN;
	}
	
	// close file
	close(fd);
	
	if(buf[0] == '0') {
		return HIGH;
	} else {
		return LOW;
	}
}
bool Gpio::toggle() {
	switch(m_State) {
		case LOW:
			return setState(HIGH);
		case HIGH:
			return setState(LOW);
		default:
			std::cout << "Warning: trying to toggle pin with unknown state" << std::endl;
			return false;
	}
}
bool Gpio::waitChange(Gpio::CHANGE Change, int TimeOut) {
	
	// open gpio value file
	int fd = open(getValueFileName.c_str(), O_RDONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO value file" << std::endl;
		return false;
	}
	
	// set up polling struct
	struct pollfd fds;
	fds.fd = fd;
	fds.events = POLLPRI;
	int nfds = 1;
	int ret;
	
	// dummy read (enables blocking polling)
	char buf[2] = "";
	int res = read(fds.fd, &buf, 1);
	if (res < 0) {
		std::cout << "Error: failed to wait on GPIO interrupt" << std::endl;
		return false;
	}
		
	// wait on gpio change
	ret = poll(&fds, nfds, timeout);
	if (ret < 0) {
	  std::cout << "Error: GPIO poll failed" << std::endl;
	  return false;
	}
	
	// wait for signal to settle
	usleep(MIN_BUTTON_TIME);
	
	// read value
	lseek(fds.fd, 0, SEEK_SET);
	res = read(fds.fd, &buf, 1);
	if (res < 0) {
		std::cout << "Error: failed to wait on GPIO interrupt" << std::endl;
		return false;
	}
	
	close(fd);
	
	return true;
}


bool Gpio::enable() {
	// open gpio value file
	int fd = open(getExportFileName().c_str(), O_WRONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO export file" << std::endl;
		return false;
	}
	
	// export gpio
	std::string Text = std::to_string(pinNumber());
	int res = write(fd, Text.c_str(), Text.length());
	if (res < 0) {
		std::cout << "Error: failed to enable GPIO" << std::endl;
		return false;
	}
	
	// close file
	close(fd);
	
	return true;
}
bool Gpio::disable() {
	// open gpio value file
	int fd = open(getUnexportFileName().c_str(), O_WRONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO unexport file" << std::endl;
		return false;
	}
	
	// export gpio
	std::string Text = std::to_string(pinNumber());
	int res = write(fd, Text.c_str(), Text.length());
	if (res < 0) {
		std::cout << "Error: failed to disable GPIO" << std::endl;
		return false;
	}
	
	// close file
	close(fd);
	
	return true;
}
bool Gpio::setDir() {
	// open gpio direction file
	int fd = open(getDirectionFileName().c_str(), O_WRONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO direction file" << std::endl;
		return false;
	}
	
	// set direction
	int res;
	if(m_Dir == OUT) {
		res = write(fd, "out", 4);
	} else {
		res = write(fd, "in", 3);
	}
	if (res < 0) {
		std::cout << "Error: failed set GPIO direction" << std::endl;
		return false;
	}

	// close file
	close(fd);
	
	return true;
}
bool Gpio::setInterrupt(Gpio::CHANGE Change) {
	// open gpio edge file
	int fd = open(getEdgeFileName().c_str(), O_WRONLY);
	if (fd < 0) {
		std::cout << "Error: failed to open GPIO edge file" << std::endl;
		return false;
	}
	
	// set edge
	int res;
	switch (Change) {
		case NONE:
			res = write(fd, "none", 5);
			break;
		case RISE:
			res = write(fd, "rising", 7);
			break;
		case FALL:
			res = write(fd, "falling", 8);
			break;
		case BOTH:
			res = write(fd, "both", 5);
			break;
		default:
			res = -1;
	}
	if (res < 0) {
		std::cout << "Error: failed to set GPIO interrupt edge direction" << std::endl;
		return false;
	}
	
	// close file
	close(fd);
	
	return true;
}

std::string Gpio::getValueFileName() {
	return GPIO_PATH + "gpio" + std::to_string(pinNumber()) + "/value";
}
std::string Gpio::getExportFileName() {
	return GPIO_PATH + "export";
}
std::string Gpio::getUnexportFileName() {
	return GPIO_PATH + "unexport";
}
std::string Gpio::getDirectionFileName() {
	return GPIO_PATH + "gpio" + std::to_string(pinNumber()) + "/direction";
}
std::string getEdgeFileName() {
	return GPIO_PATH + "gpio" + std::to_string(pinNumber()) + "/edge";
}