/*
	Compile with 'g++ -o mt mt.cpp -std=c++11'
*/
#include <iostream>
#include <thread>
#include <mutex> 
#include <unistd.h>

#include "gpio_lib.h"

using namespace std;

// mutex
mutex mtx;

// interrupt variable
unsigned int GpioInterrupt = 0;

void waitGpio(unsigned int Header, unsigned int Pin) {
	
	// GPIO init
	unsigned int GpioNumber = get_gpio_number(Header, Pin);
	gpio_export(GpioNumber);
	gpio_dir(GpioNumber, IN);
	gpio_interrupt(GpioNumber, RISING);
	
	cout << "Set GPIO" << GpioNumber << " as interrupt pin\n";
	
	while(true) {
		gpio_wait_interrupt(GpioNumber, -1);
		cout << GpioInterrupt << endl;
		
		mtx.lock();
		GpioInterrupt++;
		mtx.unlock();
	}
	
	// TODO: should unexport GPIO
}

int main() {
	waitGpio(BB_HEADER_P9, 11);
	thread InterruptWaiter(waitGpio, BB_HEADER_P9, 11);
	
	unsigned int i = 0;
	while(i<10) {
		mtx.lock();
		if (GpioInterrupt > 0) {
			cout << "Interrupt!\n";
			GpioInterrupt--;
			i++;
		}
		mtx.unlock();
		sleep(3);
	}
	
	
	//InterruptWaiter.join();
	InterruptWaiter.detach();
	return 0;
}
