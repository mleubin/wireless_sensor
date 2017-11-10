#include <iostream>

#include "i2c.h"

using namespace std;

int main() {
	bool status = i2cInit(5);
	if(status) {
		cout << "Init successfull\n";
	} else {
		cout << "Init failed\n";
	}
	
	status = i2cSendCommand(5);
	
	return 0;
}