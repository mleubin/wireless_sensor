#include <iostream>
#include <unistd.h>

#include "gpio.h"

int main() {
	Gpio pin1 = Gpio(P9_11, Gpio::IN);
	Gpio pin2 = Gpio(P9_13, Gpio::OUT);

	for (int i = 0; i < 100; i++) {
		pin1.waitChange(Gpio::BOTH, -1);
		pin2.toggle();
	}
	return 0;
}
