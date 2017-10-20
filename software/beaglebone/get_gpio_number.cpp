/*
 Compile with 'g++ -std=c++11 get_gpio_number.cpp'
*/

#include <stdint.h>
#include <iostream>
#include <string>

#define BB_HEADER_P8 8
#define BB_HEADER_P9 9

unsigned int gpio_numbers_p8[] = {38,39,34,35,66,67,69,68,45,44,23,26,47,46,27,65,22,63,62,37,36,33,32,61,86,88,87,89,10,11,9,81,8,80,78,79,76,77,74,75,72,73,70,71};
unsigned int gpio_numbers_p9[] = {30,60,31,50,48,51,5,4,13,12,3,2,49,15,117,14,115,113,111,112,110};


unsigned int get_gpio_number(unsigned int header,unsigned int pin) {
	switch(header) {
		case BB_HEADER_P8:
			if (pin < 3 || pin > 46) {
				std::cout << "\nError: P8." << pin << " is not a GPIO pin\n";
				return 0;
			} else {
				return gpio_numbers_p8[pin-3];
			}
			break;
		case BB_HEADER_P9:
			if (pin < 11 || pin > 31) {
				std::cout << "\nError: P9." << pin << " is not a GPIO pin\n";
				return 0;
			} else {
				return gpio_numbers_p9[pin-11];
			}
			break;
		default:
			std::cout << "\nError: P" << header << " is not a header. Select {8, 9}\n";
			return 0;
	}
}

int main() {
	std::string header;
	std::string pin;
	std::cout << "Enter Header {8, 9}:\n";
	std::getline(std::cin, header);
	std::cout << "Enter Pin Number:\n";
	std::getline(std::cin, pin);

	std::cout << "Pin P" << (unsigned int) std::stoul(header) << "." << (unsigned int) std::stoul(pin) << " selected";
	
	unsigned int gpio_number = get_gpio_number((unsigned int) std::stoul(header), (unsigned int) std::stoul(pin));
	if (gpio_number != 0) {
		std::cout << " -> GPIO number: " << gpio_number << std::endl;
	}

	return 0;
}
