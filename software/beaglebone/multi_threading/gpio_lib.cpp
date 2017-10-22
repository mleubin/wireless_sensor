#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <iostream>
#include <string>
 
#include "gpio_lib.h"

/// Minimal time a button needs to be pressed (in µs)
#define MIN_BUTTON_TIME 100000

/// Path to linux GPIO device files
#define GPIO_PATH "/sys/class/gpio/"

#define MAX_PATH_LENGTH 100

#define SUCCESS 1
#define FAILURE -1

// pin database
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


/**
 * Unexport (deactivate) a GPIO.
 * @param num Linux GPIO number.
 * @return {@link SUCCESS} in case of success, {@link FAILURE} otherwise.
 */
int gpio_unexport(int num) {
	// open gpio value file
	int fd = open(GPIO_PATH "unexport", O_WRONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO unexport file");
		return FAILURE;
	}
	
	// export gpio
	char buf[5] = "";
	int len = snprintf(buf, sizeof(buf),"%d",num);
	int res = write(fd, buf, len);
	if (res < 0) {
		printf("Error: could not unexport GPIO");
		return FAILURE;
	}
	
	// close file
	close(fd);
	
	return SUCCESS;
}


/**
 * Export (activate) a GPIO.
 * @param num Linux GPIO number.
 * @return {@link SUCCESS} in case of success, {@link FAILURE} otherwise.
 */
int gpio_export(int num) {
	// open gpio value file
	int fd = open(GPIO_PATH "export", O_WRONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO export file");
		return FAILURE;
	}
	
	// export gpio
	char buf[5] = "";
	int len = snprintf(buf, sizeof(buf),"%d",num);
	int res = write(fd, buf, len);
	if (res < 0) {
		printf("Error: could not export GPIO");
		return FAILURE;
	}
	
	// close file
	close(fd);
	
	return SUCCESS;
}

/**
 * Set direction of GPIO.
 * @param num Linux GPIO number.
 * @param dir Direction.
 * @return {@link SUCCESS} in case of success, {@link FAILURE} otherwise.
 */
int gpio_dir(int num, rl_direction dir) {
	
	// open gpio direction file
	char file_name[MAX_PATH_LENGTH];
	sprintf(file_name, GPIO_PATH "gpio%d/direction",num);
	int fd = open(file_name, O_WRONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO direction file");
		return FAILURE;
	}
	
	// set direction
	int res;
	if(dir == OUT) {
		res = write(fd, "out", 4);
	} else {
		res = write(fd, "in", 3);
	}
	if (res < 0) {
		printf("Error: could not set GPIO direction");
		return FAILURE;
	}

	// close file
	close(fd);
	
	return SUCCESS;
}


/**
 * Set direction of GPIO interrupt edge.
 * @param num Linux GPIO number.
 * @param edge Edge direction.
 * @return {@link SUCCESS} in case of success, {@link FAILURE} otherwise.
 */
int gpio_interrupt(int num, rl_edge edge) {
	
	// open gpio edge file
	char file_name[MAX_PATH_LENGTH];
	sprintf(file_name, GPIO_PATH "gpio%d/edge",num);
	int fd = open(file_name, O_WRONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO edge file");
		return FAILURE;
	}
	
	// set edge
	int res;
	switch (edge) {
		case NONE:
			res = write(fd, "none", 5);
			break;
		case RISING:
			res = write(fd, "rising", 7);
			break;
		case FALLING:
			res = write(fd, "falling", 8);
			break;
		case BOTH:
			res = write(fd, "both", 5);
			break;
		default:
			res = -1;
	}
	if (res < 0) {
		printf("Error: could not set GPIO interrupt edge direction");
		return FAILURE;
	}
	
	// close file
	close(fd);
	
	return SUCCESS;
}

/**
 * Set value of GPIO.
 * @param num Linux GPIO number.
 * @param val Value (0 or 1).
 * @return {@link SUCCESS} in case of success, {@link FAILURE} otherwise.
 */
int gpio_set_value(int num, int val) {
	
	// open gpio value file
	char file_name[MAX_PATH_LENGTH];
	sprintf(file_name, GPIO_PATH "gpio%d/value",num);
	int fd = open(file_name, O_WRONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO value file");
		return FAILURE;
	}
	
	// set value
	int res;
	if(val == 0) {
		res = write(fd, "0", 2);
	} else {
		res = write(fd, "1", 2);
	}
	if (res < 0) {
		printf("Error: could not set GPIO value");
		return FAILURE;
	}
	
	// close file
	close(fd);
	
	return SUCCESS;
}

/**
 * Get GPIO value.
 * @param num Linux GPIO number.
 * @return GPIO value, {@link FAILURE} on failure.
 */
int gpio_get_value(int num) {
	
	// open gpio value file
	char file_name[MAX_PATH_LENGTH];
	sprintf(file_name, GPIO_PATH "gpio%d/value",num);
	int fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO value file");
		return FAILURE;
	}
	
	// read value
	char buf[2] = "";
	int res = read(fd, &buf, 1);
	if (res < 0) {
		printf("Error: could not read GPIO value");
		return FAILURE;
	}
	
	// close file
	close(fd);
	
	return atoi(buf);
}

/**
 * Wait on GPIO interrupt.
 * @param num Linux GPIO number.
 * @param timeout Maximum waiting time (in ms). Set to 0 for infinite time out.
 * @return {@link SUCCESS} in case of interrupt, {@link FAILURE} otherwise.
 */
int gpio_wait_interrupt(int num, int timeout) {
	
	// open gpio value file
	char file_name[MAX_PATH_LENGTH];
	sprintf(file_name, GPIO_PATH "gpio%d/value",num);
	int fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		printf("Error: could not open GPIO value file");
		return FAILURE;
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
		printf("Error: could not wait on GPIO interrupt");
		return FAILURE;
	}
		
	// wait on gpio change
	ret = poll(&fds, nfds, timeout);
	if (ret < 0) {
	  printf("Error: GPIO poll failed");
	  return FAILURE;
	}
	
	// wait for signal to settle
	usleep(MIN_BUTTON_TIME);
	
	// read value
	lseek(fds.fd, 0, SEEK_SET);
	res = read(fds.fd, &buf, 1);
	if (res < 0) {
		printf("Error: could wait on GPIO interrupt");
		return FAILURE;
	}
	
	close(fd);
	
	return atoi(buf);
}
