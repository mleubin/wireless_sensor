#include <iostream>
#include <unistd.h>
#include <mutex>
#include <atomic>

#include "Gpio.h"
#include "GpioListener.h"

std::mutex Mutex;
//unsigned int InterruptCount = 0;
std::atomic<unsigned int> InterruptCount = 0;

void onGpioRise(Gpio& Input) {
	std::cout << "Input received from " << Input.pinName() << std::endl;
	//Mutex.lock();
	InterruptCount++;
	//Mutex.unlock();
}

int main() {
	/*Gpio pin1 = Gpio(P9_11, Gpio::IN);
	Gpio pin2 = Gpio(P9_13, Gpio::OUT);

	for (int i = 0; i < 100; i++) {
		pin1.waitChange(Gpio::BOTH, -1);
		pin2.toggle();
	}*/

	GpioListener Listener(P8_9, Gpio::RISE, &onGpioRise);
	Listener.start();

	//Mutex.lock();
	while(InterruptCount < 5) {
		//Mutex.unlock();
		usleep(100000);
		//Mutex.lock();
	}

	Listener.stop();

	return 0;
}
